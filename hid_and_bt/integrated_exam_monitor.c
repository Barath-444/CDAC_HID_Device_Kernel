// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/types.h>
#include <linux/usb.h>
#include <linux/notifier.h>
#include <linux/timekeeping.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/reboot.h>
#include <linux/fb.h>
#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>

#define MAX_BT_DEVICES 1000
#define HID_CLASS       0x03
#define MOUSE_SUBCLASS  0x01
#define KEYBOARD_SUBCLASS 0x01
#define IST_OFFSET 19800

static int mouse_count = 0;
static int keyboard_count = 0;
static int hdmi_connected = 0;
static int bt_count = 0;

static bdaddr_t known_addrs[MAX_BT_DEVICES];
static struct task_struct *shutdown_task;

// --- Time formatting ---
static void get_time_string(char *buf, size_t size) {
    struct timespec64 ts;
    struct tm tm;
    ktime_get_real_ts64(&ts);
    time64_to_tm(ts.tv_sec + IST_OFFSET, 0, &tm);
    snprintf(buf, size, "%04ld-%02d-%02d %02d:%02d:%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
}

// --- Shutdown logic ---
static int shutdown_thread(void *data) {
    ssleep(60);
    printk(KERN_ALERT "[EXAM ALERT] Triggering system shutdown due to HID/HDMI/BT violation\n");
     kernel_power_off();
    return 0;
}

static void trigger_shutdown(void) {
    if (!shutdown_task)
        shutdown_task = kthread_run(shutdown_thread, NULL, "shutdown_thread");
}

// --- HID Detection ---
static int usb_notifier_callback(struct notifier_block *nb, unsigned long action, void *data) {
    struct usb_interface *interface = data;
    struct usb_host_interface *host;
    int class, subclass;
    char timebuf[32];

    if (action != USB_DEVICE_ADD || !interface)
        return NOTIFY_OK;

    host = interface->cur_altsetting;
    class = host->desc.bInterfaceClass;
    subclass = host->desc.bInterfaceSubClass;

    if (class == HID_CLASS) {
        get_time_string(timebuf, sizeof(timebuf));

        if (subclass == 1 && mouse_count == 0) {
            mouse_count++;
            printk(KERN_ALERT "[EXAM ALERT] Mouse connected at %s\n", timebuf);
        } else if (subclass == 0 && keyboard_count == 0) {
            keyboard_count++;
            printk(KERN_ALERT "[EXAM ALERT] Keyboard connected at %s\n", timebuf);
        } else {
            printk(KERN_ALERT "[EXAM ALERT] Additional HID device at %s\n", timebuf);
            trigger_shutdown();
        }
    }

    return NOTIFY_OK;
}

static struct notifier_block usb_nb = {
    .notifier_call = usb_notifier_callback
};

// --- HDMI Detection ---
static int fb_notifier_callback(struct notifier_block *nb, unsigned long action, void *data) {
    struct fb_event *event = data;
    char timebuf[32];

    if (action == FB_EVENT_BLANK && event->data) {
        get_time_string(timebuf, sizeof(timebuf));
        printk(KERN_ALERT "[EXAM ALERT] HDMI display connected at %s\n", timebuf);

        if (++hdmi_connected >= 2) {
            trigger_shutdown();
        }
    }

    return NOTIFY_OK;
}

static struct notifier_block fb_nb = {
    .notifier_call = fb_notifier_callback
};

// --- Bluetooth Detection ---
static bool already_logged(bdaddr_t *addr) {
    int i;
    for (i = 0; i < bt_count; i++) {
        if (!bacmp(addr, &known_addrs[i]))
            return true;
    }
    return false;
}

static int handler_pre(struct kprobe *p, struct pt_regs *regs) {
    struct hci_conn *conn = (struct hci_conn *)regs->di;
    char addr_str[18];

    if (!conn || already_logged(&conn->dst) || bt_count >= MAX_BT_DEVICES)
        return 0;

    known_addrs[bt_count++] = conn->dst;
    snprintf(addr_str, sizeof(addr_str), "%pMR", &conn->dst);
    printk(KERN_ALERT "[BT ALERT] Bluetooth #%d connected: %s\n", bt_count, addr_str);

    if (bt_count > 1)
        trigger_shutdown();

    return 0;
}

static struct kprobe kp = {
    .symbol_name = "hci_connect",
    .pre_handler = handler_pre,
};

// --- Module Init/Exit ---
static int __init exam_monitor_init(void) {
    printk(KERN_INFO "[EXAM MONITOR] Integrated monitoring module initialized\n");

    usb_register_notify(&usb_nb);
    fb_register_client(&fb_nb);
    register_kprobe(&kp);

    return 0;
}

static void __exit exam_monitor_exit(void) {
    printk(KERN_INFO "[EXAM MONITOR] Exiting exam monitoring module\n");

    usb_unregister_notify(&usb_nb);
    fb_unregister_client(&fb_nb);
    unregister_kprobe(&kp);
}

module_init(exam_monitor_init);
module_exit(exam_monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Barathraj - Integrated by ChatGPT");
MODULE_DESCRIPTION("Kernel module for detecting unauthorized HID, HDMI, and Bluetooth devices");

