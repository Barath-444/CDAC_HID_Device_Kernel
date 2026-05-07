#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/slab.h>

#define HID_CLASS 0x03

// Global counters for each device type
static int mouse_count = 0;
static int keyboard_count = 0;
static int other_hid_count = 0;

static int usb_notify(struct notifier_block *nb, unsigned long action, void *data)
{
    struct usb_device *udev = data;
    int i, j;

    if (!udev || udev->descriptor.bDeviceClass == USB_CLASS_HUB)
        return NOTIFY_DONE;

    if (action != USB_DEVICE_ADD)
        return NOTIFY_DONE;

    for (i = 0; i < udev->descriptor.bNumConfigurations; i++) {
        struct usb_host_config *config = &udev->config[i];

        for (j = 0; j < config->desc.bNumInterfaces; j++) {
            struct usb_interface_descriptor *iface_desc = &config->interface[j]->altsetting[0].desc;

            if (iface_desc->bInterfaceClass == HID_CLASS) {
                const char *vendor = udev->manufacturer ? udev->manufacturer : "UnknownVendor";
                const char *product = udev->product ? udev->product : "UnknownProduct";

                char count_str[64], tag_str[64], device_str[128];

                snprintf(device_str, sizeof(device_str), "DEVICE_NAME=%s_%s", vendor, product);

                if (iface_desc->bInterfaceProtocol == 0x01) { // Keyboard
                    keyboard_count++;
                    snprintf(count_str, sizeof(count_str), "KEYBOARD_COUNT=%d", keyboard_count);
                    snprintf(tag_str, sizeof(tag_str), "TYPE=Keyboard");

                    printk(KERN_ALERT "[EXAM ALERT] Keyboard #%d: %s - %s\n", keyboard_count, vendor, product);
                }
                else if (iface_desc->bInterfaceProtocol == 0x02) { // Mouse
                    mouse_count++;
                    snprintf(count_str, sizeof(count_str), "MOUSE_COUNT=%d", mouse_count);
                    snprintf(tag_str, sizeof(tag_str), "TYPE=Mouse");

                    printk(KERN_ALERT "[EXAM ALERT] Mouse #%d: %s - %s\n", mouse_count, vendor, product);
                }
                else {
                    other_hid_count++;
                    snprintf(count_str, sizeof(count_str), "OTHER_HID_COUNT=%d", other_hid_count);
                    snprintf(tag_str, sizeof(tag_str), "TYPE=Other");

                    printk(KERN_ALERT "[EXAM ALERT] Other HID #%d: %s - %s\n", other_hid_count, vendor, product);
                }

                // Send uevent
                char *envp[] = {
                    "ACTION=add",
                    "EVENT_TYPE=HID_ALERT",
                    "SUBSYSTEM=usb",
                    count_str,
                    tag_str,
                    device_str,
                    NULL
                };

                kobject_uevent_env(&udev->dev.kobj, KOBJ_CHANGE, envp);
            }
        }
    }

    return NOTIFY_OK;
}

static struct notifier_block usb_nb = {
    .notifier_call = usb_notify,
};

static int __init hid_alert_init(void)
{
    usb_register_notify(&usb_nb);
    printk(KERN_INFO "HID Alert Module Loaded - s0s Team\n");
    return 0;
}

static void __exit hid_alert_exit(void)
{
    usb_unregister_notify(&usb_nb);
    printk(KERN_INFO "HID Alert Module Unloaded - s0s Team\n");
}

module_init(hid_alert_init);
module_exit(hid_alert_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SOS-Team");
MODULE_DESCRIPTION("Detects and classifies HID devices with individual counters");
