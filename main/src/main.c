#include <linux/module.h>
#include <linux/init.h>

#include "../header/bt_kprobe.h"
#include "../header/exam_alert.h"

static int __init my_module_init(void)
{   
    exam_alert_init();
    bt_kprobe_init();
    printk(KERN_INFO "Full Exam Alert Module Loaded - s0s Team\n");
    return 0;
}

static void __exit my_module_exit(void)
{
    exam_alert_exit();
    bt_kprobe_exit();
    printk(KERN_INFO "Full Exam Alert Module Unloaded - s0s Team\n");
}


module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SOS");
MODULE_DESCRIPTION("Modular kernel detection system");
