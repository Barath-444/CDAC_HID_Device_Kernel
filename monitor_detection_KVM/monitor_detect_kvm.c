#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/reboot.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/dirent.h>
#include <asm/segment.h>  
#define CHECK_INTERVAL (5 * HZ)

static struct task_struct *monitor_thread;
static atomic_t shutdown_triggered = ATOMIC_INIT(0);
static int last_connected_count = 0;

static int count_connected_monitors(void) {
    struct file *dir, *fp;
    struct linux_dirent64 *dirent;
    mm_segment_t old_fs;
    char *kbuf = kzalloc(PAGE_SIZE, GFP_KERNEL);
    char path[256];
    char buf[32] = {0};
    loff_t pos;
    int count = 0, ret, bpos = 0;

    dir = filp_open("/sys/class/drm", O_RDONLY | O_DIRECTORY, 0);
    if (IS_ERR(dir)) {
        printk(KERN_ERR "[KVM_MON] Could not open /sys/class/drm\n");
        kfree(kbuf);
        return 0;
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    ret = kernel_read(dir, kbuf, PAGE_SIZE - 1, &dir->f_pos);
    set_fs(old_fs);

    if (ret > 0) {
        while (bpos < ret) {
            dirent = (struct linux_dirent64 *)(kbuf + bpos);

            // Skip internal laptop screen (eDP)
            if (strstr(dirent->d_name, "-") &&
                !strstr(dirent->d_name, "eDP") &&
                (strstr(dirent->d_name, "HDMI") || strstr(dirent->d_name, "VGA") || strstr(dirent->d_name, "DP"))) {

                snprintf(path, sizeof(path), "/sys/class/drm/%s/status", dirent->d_name);
                fp = filp_open(path, O_RDONLY, 0);
                if (!IS_ERR(fp)) {
                    pos = 0;
                    memset(buf, 0, sizeof(buf));
                    kernel_read(fp, buf, sizeof(buf) - 1, &pos);
                    filp_close(fp, NULL);
                    if (strstr(buf, "connected")) {
                        printk(KERN_INFO "[KVM_MON] Detected connected display: %s\n", path);
                        count++;
                    }
                }
            }
            bpos += dirent->d_reclen;
        }
    }

    filp_close(dir, NULL);
    kfree(kbuf);
    return count;
}

static int monitor_thread_fn(void *data) {
    last_connected_count = count_connected_monitors();
    printk(KERN_INFO "[KVM_MON] Initial external monitors: %d\n", last_connected_count);

    while (!kthread_should_stop()) {
        int current_count = count_connected_monitors();

        if (current_count > last_connected_count) {
            printk(KERN_ALERT "[KVM_MON] New external monitor connected! Shutting down in 60 seconds.\n");
            atomic_set(&shutdown_triggered, 1);
            ssleep(60);
            kernel_power_off();
        }

        ssleep(5);
    }
    return 0;
}

static int __init monitor_detect_init(void) {
    printk(KERN_ALERT "[KVM_MON] Universal external monitor detection module loaded.\n");
    monitor_thread = kthread_run(monitor_thread_fn, NULL, "monitor_kvm_thread");
    return 0;
}

static void __exit monitor_detect_exit(void) {
    if (monitor_thread)
        kthread_stop(monitor_thread);
    printk(KERN_ALERT "[KVM_MON] Monitor detection module unloaded.\n");
}

module_init(monitor_detect_init);
module_exit(monitor_detect_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Barathraj");
MODULE_DESCRIPTION("Universal external monitor detection (HDMI/VGA/DP, excluding eDP)");
