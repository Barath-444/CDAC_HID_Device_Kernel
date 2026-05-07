#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/timekeeping.h>
#include <linux/if_arp.h>


static int eth_count = 0;

// Ethernet netdevice event handler
static int netdev_event(struct notifier_block *nb, unsigned long event, void *ptr) {
    struct net_device *dev = netdev_notifier_info_to_dev(ptr);

    if (event == NETDEV_UP && dev && dev->type == ARPHRD_ETHER) {
        eth_count++;

        struct timespec64 ts;
        struct tm tm;
        char time_str[64];

        // Get current time and manually apply IST offset (+5:30)
        ktime_get_real_ts64(&ts);
        ts.tv_sec += (5 * 3600) + (30 * 60);  // Add 19800 seconds
        time64_to_tm(ts.tv_sec, 0, &tm);

        snprintf(time_str, sizeof(time_str), "%04ld-%02d-%02d %02d:%02d:%02d",
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                 tm.tm_hour, tm.tm_min, tm.tm_sec);

        printk(KERN_ALERT "[EXAM ALERT] [%s] Ethernet #%d: Interface %s connected\n",
               time_str, eth_count, dev->name);
    }

    return NOTIFY_DONE;
}

static struct notifier_block nb_netdev = {
    .notifier_call = netdev_event,
};

static int __init eth_alert_init(void) {
    register_netdevice_notifier(&nb_netdev);
    printk(KERN_INFO "Ethernet Alert Module Loaded - s0s Team\n");
    return 0;
}

static void __exit eth_alert_exit(void) {
    unregister_netdevice_notifier(&nb_netdev);
    printk(KERN_INFO "Ethernet Alert Module Unloaded - s0s Team\n");
}

module_init(eth_alert_init);
module_exit(eth_alert_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s0s Team");
MODULE_DESCRIPTION("Detects Ethernet interfaces with individual counters and correct IST timestamp logging");
