#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/timekeeping.h>
#include <linux/time.h>

#define PROC_FILENAME "monitor_status"
static struct proc_dir_entry *proc_entry;
static int monitor_event_count = 0;

static ssize_t monitor_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
    char kbuf[32];
    struct timespec64 ts;
    struct tm tm;

    if (count > sizeof(kbuf) - 1)
        return -EINVAL;

    if (copy_from_user(kbuf, buffer, count))
        return -EFAULT;

    kbuf[count] = '\0';

    ktime_get_real_ts64(&ts);
    time64_to_tm(ts.tv_sec, 19800, &tm);  // IST (UTC+5:30)

    monitor_event_count++;

    printk(KERN_INFO "[MonitorDetector] Status: %s | Count: %d | Time: %02d-%02d-%ld %02d:%02d:%02d IST\n",
           kbuf,
           monitor_event_count,
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
           tm.tm_hour, tm.tm_min, tm.tm_sec);

    return count;
}

static const struct proc_ops proc_fops = {
    .proc_write = monitor_write,
};

static int __init monitor_init(void) {
    proc_entry = proc_create(PROC_FILENAME, 0666, NULL, &proc_fops);
    if (!proc_entry) {
        pr_err("MonitorDetector: Failed to create /proc entry.\n");
        return -ENOMEM;
    }
    pr_info("MonitorDetector: Module loaded. Use /proc/%s\n", PROC_FILENAME);
    return 0;
}

static void __exit monitor_exit(void) {
    proc_remove(proc_entry);
    pr_info("MonitorDetector: Module unloaded.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Barathraj");
MODULE_DESCRIPTION("Monitor connection tracker with Indian timestamp");

module_init(monitor_init);
module_exit(monitor_exit);
