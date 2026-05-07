#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/reboot.h>

#define PROC_NAME "monitor_status"
#define BUF_LEN 64
#define SHUTDOWN_DELAY_SECONDS 60

static struct proc_dir_entry *proc_entry;
static int monitor_event_count = 0;
static struct timer_list shutdown_timer;

static void shutdown_system(struct timer_list *t)
{
    printk(KERN_INFO "[MonitorDetector] Initiating system shutdown...\n");
    kernel_power_off();  // Clean shutdown
}

static ssize_t monitor_write(struct file *file, const char __user *ubuf, size_t len, loff_t *off)
{
    char kbuf[BUF_LEN];
    char *cmd;
    char *status = NULL;
    struct timespec64 ts;
    struct rtc_time tm;

    if (len > BUF_LEN - 1)
        return -EINVAL;

    if (copy_from_user(kbuf, ubuf, len))
        return -EFAULT;

    kbuf[len] = '\0';

    cmd = kbuf;
    status = strsep(&cmd, "\n");

    if (status == NULL)
        return -EINVAL;

    ktime_get_real_ts64(&ts);
    rtc_time64_to_tm(ts.tv_sec, &tm);

    monitor_event_count++;

    printk(KERN_INFO "[MonitorDetector] Status: %s | Count: %d | Time: %02d-%02d-%ld %02d:%02d:%02d IST\n",
           status, monitor_event_count,
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
           tm.tm_hour, tm.tm_min, tm.tm_sec);

    if (monitor_event_count >= 2) {
        mod_timer(&shutdown_timer, jiffies + SHUTDOWN_DELAY_SECONDS * HZ);
    }

    return len;
}

static const struct proc_ops monitor_ops = {
    .proc_write = monitor_write,
};

static int __init monitor_init(void)
{
    proc_entry = proc_create(PROC_NAME, 0666, NULL, &monitor_ops);
    if (!proc_entry) {
        printk(KERN_ERR "MonitorDetector: Failed to create /proc entry.\n");
        return -ENOMEM;
    }

    timer_setup(&shutdown_timer, shutdown_system, 0);

    printk(KERN_INFO "MonitorDetector: Module loaded. Use /proc/%s\n", PROC_NAME);
    return 0;
}

static void __exit monitor_exit(void)
{
    del_timer_sync(&shutdown_timer);
    proc_remove(proc_entry);
    printk(KERN_INFO "MonitorDetector: Module unloaded.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Barathraj");
MODULE_DESCRIPTION("Monitor Detection and Shutdown on Double Connection");
MODULE_VERSION("1.0");

module_init(monitor_init);
module_exit(monitor_exit);

