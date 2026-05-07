#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/reboot.h>
#include <linux/delay.h>

#define CHECK_INTERVAL (5 * HZ) // Check every 5 seconds

// Your confirmed HDMI path
static const char *hdmi_status_paths[] = {
    "/sys/class/drm/card2-HDMI-A-1/status"
};

static struct task_struct *monitor_task;

// Read HDMI status: returns 1 if "connected"
static int is_hdmi_connected(const char *path) {
    struct file *file;
    char buf[32] = {0};
    loff_t pos = 0;
    int ret = 0;

    file = filp_open(path, O_RDONLY, 0);
    if (IS_ERR(file))
        return 0;

    kernel_read(file, buf, sizeof(buf) - 1, &pos);
    filp_close(file, NULL);

    if (strstr(buf, "connected")) {
        printk(KERN_ALERT "[HDMI_MON] Monitor connected at %s\n", path);
        ret = 1;
    }

    return ret;
}

// Thread function to monitor HDMI state
static int monitor_thread_fn(void *data) {
    while (!kthread_should_stop()) {
        if (is_hdmi_connected(hdmi_status_paths[0])) {
            printk(KERN_ALERT "[HDMI_MON] HDMI connected — shutting down in 60 seconds\n");
            ssleep(60);
            kernel_power_off();  // 🔥 System shutdown here
        }
        ssleep(5);
    }
    return 0;
}

static int __init monitor_init(void) {
    printk(KERN_INFO "[HDMI_MON] HDMI monitor detection module loaded.\n");
    monitor_task = kthread_run(monitor_thread_fn, NULL, "monitor_thread");
    return IS_ERR(monitor_task) ? PTR_ERR(monitor_task) : 0;
}

static void __exit monitor_exit(void) {
    if (monitor_task)
        kthread_stop(monitor_task);
    printk(KERN_INFO "[HDMI_MON] HDMI monitor detection module unloaded.\n");
}

module_init(monitor_init);
module_exit(monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Barathraj R");
MODULE_DESCRIPTION("Kernel module for HDMI monitor detection and auto shutdown.");
