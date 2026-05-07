#ifndef BT_KPROBE_H
#define BT_KPROBE_H

#include <net/bluetooth/bluetooth.h>
#include <linux/kprobes.h>                 // For struct kprobe
#include <linux/ptrace.h>   

 //extern bdaddr_t known_addrs[MAX_BT_DEVICES];
static bool already_logged(bdaddr_t *addr);
static int handler_pre(struct kprobe *p, struct pt_regs *regs);
int bt_kprobe_init(void);
 void bt_kprobe_exit(void);

//extern struct kprobe kp;


#endif