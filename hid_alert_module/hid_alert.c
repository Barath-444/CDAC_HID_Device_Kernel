#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/notifier.h>
#include <linux/timekeeping.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/reboot.h>

#define HID_CLASS 0x03
#define IST_OFFSET 19800  // +5:30 in seconds

// Global counters
static int mouse_count = 0;
static int keyboard_count = 0;
static int other_hid_count = 0;

// Shutdown kernel thread
static int shutdown_thread(void *data) {
    ssleep(60);  // wait 60 seconds
    printk(KERN_ALERT "[EXAM ALERT] Triggering system shutdown due to HID security violation\n");

    char *envp[] = { "HOME=/", NULL };
    char *argv[] = { "/sbin/shutdown", "-h", "now", NULL };
    return call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
}

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
                const char *vendor  = udev->manufacturer ? udev->manufacturer : "UnknownVendor";
                const char *product = udev->product ? udev->product : "UnknownProduct";

                // Get current time (with IST adjustment)
                struct timespec64 ts;
                struct tm tm;
                char time_str[64];

                ktime_get_real_ts64(&ts);
                time64_to_tm(ts.tv_sec + IST_OFFSET, 0, &tm);
                snprintf(time_str, sizeof(time_str), "%04ld-%02d-%02d %02d:%02d:%02d",
                         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                         tm.tm_hour, tm.tm_min, tm.tm_sec);

                if (iface_desc->bInterfaceProtocol == 0x01) {
                    keyboard_count++;
                    printk(KERN_ALERT "[EXAM ALERT] [%s] Keyboard #%d: %s - %s\n", time_str, keyboard_count, vendor, product);
                }
                else if (iface_desc->bInterfaceProtocol == 0x02) {
                    mouse_count++;
                    printk(KERN_ALERT "[EXAM ALERT] [%s] Mouse #%d: %s - %s\n", time_str, mouse_count, vendor, product);
                }
                else {
                    other_hid_count++;
                    printk(KERN_ALERT "[EXAM ALERT] [%s] Other HID #%d: %s - %s\n", time_str, other_hid_count, vendor, product);
                }

                // Trigger shutdown on 2nd HID device
                if ((mouse_count + keyboard_count + other_hid_count) == 2) {
                    printk(KERN_ALERT "[EXAM ALERT] 2nd HID detected! Starting 60-second shutdown timer\n");
                    kthread_run(shutdown_thread, NULL, "hid_shutdown_thread");
                }
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
MODULE_AUTHOR("s0s Team");
MODULE_DESCRIPTION("Detects and classifies HID devices with auto shutdown after second device");
