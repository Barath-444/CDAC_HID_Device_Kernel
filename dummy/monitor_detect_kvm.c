#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/reboot.h>
#include <linux/fs.h>
#include <linux/fb.h>
#include <linux/notifier.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/atomic.h>
#include <linux/init.h>
#include <linux/types.h>

#define CHECK_INTERVAL (5 * HZ)

static struct task_struct *monitor_thread;
static struct notifier_block fb_nb;
static atomic_t shutdown_triggered = ATOMIC_INIT(0);

// Check DRM connector status files for "connected"
static int is_hdmi_connected_drm(void) {
    struct file *fp;
    char path[128];
    char buf[32];
    loff_t pos = 0;
    int i;

    for (i = 0; i < 10; i++) {
        snprintf(path, sizeof(path), "/sys/class/drm/card%d-HDMI-A-1/status", i);
        fp = filp_open(path, O_RDONLY, 0);
        if (IS_ERR(fp))
            continue;

        memset(buf, 0, sizeof(buf));
        kernel_read(fp, buf, sizeof(buf) - 1, &pos);
        filp_close(fp, NULL);

        if (strstr(buf, "connected")) {
            printk(KERN_ALERT "[KVM_MON] DRM HDMI connected at %s\n", path);
            return 1;
        }
    }

    return 0;
}

// fb_register_client callback
static int fb_event_notify(struct notifier_block *self, unsigned long event, void *data) {
    if (event == FB_EVENT_MODE_CHANGE || event == FB_EVENT_BLANK) {
        if (atomic_cmpxchg(&shutdown_triggered, 0, 1) == 0) {
            printk(KERN_ALERT "[KVM_MON] Framebuffer event detected. Shutting down in 60 seconds.\n");
            ssleep(60);
            kernel_power_off();
        }
    }
    return NOTIFY_OK;
}

// Thread for polling DRM status
static int monitor_thread_fn(void *data) {
    while (!kthread_should_stop()) {
        if (atomic_read(&shutdown_triggered))
            break;

        if (is_hdmi_connected_drm()) {
            if (atomic_cmpxchg(&shutdown_triggered, 0, 1) == 0) {
                printk(KERN_ALERT "[KVM_MON] HDMI detected via DRM. Shutting down in 60 seconds.\n");
                ssleep(60);
                kernel_power_off();
            }
        }

        ssleep(5);
    }
    return 0;
}

static int __init monitor_detect_init(void) {
    printk(KERN_INFO "[KVM_MON] Universal monitor detection module loaded.\n");

    fb_nb.notifier_call = fb_event_notify;
    fb_register_client(&fb_nb);

    monitor_thread = kthread_run(monitor_thread_fn, NULL, "monitor_kvm_thread");
    return 0;
}

static void __exit monitor_detect_exit(void) {
    fb_unregister_client(&fb_nb);
    if (monitor_thread)
        kthread_stop(monitor_thread);

    printk(KERN_INFO "[KVM_MON] Monitor detection module unloaded.\n");
}

module_init(monitor_detect_init);
module_exit(monitor_detect_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Barathraj R");
MODULE_DESCRIPTION("Universal HDMI/VGA/KVM monitor detection with framebuffer and DRM support");
