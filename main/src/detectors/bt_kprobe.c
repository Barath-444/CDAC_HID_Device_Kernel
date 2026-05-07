#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/types.h>
#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>

#include "../../header/bt_kprobe.h"


#define MAX_BT_DEVICES 1000

static int bt_count = 0;
static bdaddr_t known_addrs[MAX_BT_DEVICES];

static bool already_logged(bdaddr_t *addr)
{
    int i;
    for (i = 0; i < bt_count; i++) {
        if (!bacmp(addr, &known_addrs[i]))
            return true;
    }
    return false;
}

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    struct hci_conn *conn = (struct hci_conn *)regs->di;

    if (!conn)
        return 0;

    if (already_logged(&conn->dst))
        return 0;

    if (bt_count >= MAX_BT_DEVICES)
        return 0;

    known_addrs[bt_count++] = conn->dst;

    char addr[18];
    snprintf(addr, sizeof(addr), "%pMR", &conn->dst);

    printk(KERN_ALERT "[BT KPROBE] Bluetooth #%d connected: %s\n", bt_count, addr);
    return 0;
}

static struct kprobe kp = {
    .symbol_name = "hci_connect_acl",
    .pre_handler = handler_pre,
};

int bt_kprobe_init(void)
{
    int ret = register_kprobe(&kp);
    if (ret < 0) {
        printk(KERN_ERR "s0s bt: Failed to register kprobe (error %d)\n", ret);
        return ret;
    }

    printk(KERN_INFO "s0s bt: Kprobe registered on hci_connect_acl\n");
    return 0;
}

void bt_kprobe_exit(void)
{
    unregister_kprobe(&kp);
    printk(KERN_INFO "s0s bt: Kprobe unregistered\n");
}
