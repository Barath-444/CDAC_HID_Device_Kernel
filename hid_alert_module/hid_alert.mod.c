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
	{ 0x67628f51, "msleep" },
	{ 0x88cd17e1, "call_usermodehelper" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0x47886e07, "usb_unregister_notify" },
	{ 0x680628e7, "ktime_get_real_ts64" },
	{ 0x7fd36f2e, "time64_to_tm" },
	{ 0x40a621c5, "snprintf" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0x14c2bc4e, "kthread_create_on_node" },
	{ 0x54b29f7e, "wake_up_process" },
	{ 0xd272d446, "__fentry__" },
	{ 0x47886e07, "usb_register_notify" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xc773217c, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x67628f51,
	0x88cd17e1,
	0xd272d446,
	0x47886e07,
	0x680628e7,
	0x7fd36f2e,
	0x40a621c5,
	0x90a48d82,
	0x14c2bc4e,
	0x54b29f7e,
	0xd272d446,
	0x47886e07,
	0xe8213e80,
	0xd272d446,
	0xc773217c,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"msleep\0"
	"call_usermodehelper\0"
	"__stack_chk_fail\0"
	"usb_unregister_notify\0"
	"ktime_get_real_ts64\0"
	"time64_to_tm\0"
	"snprintf\0"
	"__ubsan_handle_out_of_bounds\0"
	"kthread_create_on_node\0"
	"wake_up_process\0"
	"__fentry__\0"
	"usb_register_notify\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "6E5071EA0D9047B27DFA045");
