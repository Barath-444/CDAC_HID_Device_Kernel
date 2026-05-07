#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xd272d446, "kernel_power_off" },
	{ 0x47886e07, "usb_unregister_notify" },
	{ 0xfd285498, "fb_unregister_client" },
	{ 0x7a8e92c6, "unregister_kprobe" },
	{ 0x14c2bc4e, "kthread_create_on_node" },
	{ 0x54b29f7e, "wake_up_process" },
	{ 0x40a621c5, "snprintf" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xe54e0a6b, "__fortify_panic" },
	{ 0x680628e7, "ktime_get_real_ts64" },
	{ 0x7fd36f2e, "time64_to_tm" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0x47886e07, "usb_register_notify" },
	{ 0xfd285498, "fb_register_client" },
	{ 0x1c489eb6, "register_kprobe" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x67628f51, "msleep" },
	{ 0xc773217c, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xd272d446,
	0x47886e07,
	0xfd285498,
	0x7a8e92c6,
	0x14c2bc4e,
	0x54b29f7e,
	0x40a621c5,
	0x90a48d82,
	0xd272d446,
	0xe54e0a6b,
	0x680628e7,
	0x7fd36f2e,
	0xd272d446,
	0xe8213e80,
	0x47886e07,
	0xfd285498,
	0x1c489eb6,
	0xd272d446,
	0x67628f51,
	0xc773217c,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"kernel_power_off\0"
	"usb_unregister_notify\0"
	"fb_unregister_client\0"
	"unregister_kprobe\0"
	"kthread_create_on_node\0"
	"wake_up_process\0"
	"snprintf\0"
	"__ubsan_handle_out_of_bounds\0"
	"__stack_chk_fail\0"
	"__fortify_panic\0"
	"ktime_get_real_ts64\0"
	"time64_to_tm\0"
	"__fentry__\0"
	"_printk\0"
	"usb_register_notify\0"
	"fb_register_client\0"
	"register_kprobe\0"
	"__x86_return_thunk\0"
	"msleep\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "984BB3926274B578CC302A0");
