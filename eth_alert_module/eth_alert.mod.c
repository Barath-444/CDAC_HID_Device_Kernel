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
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x680628e7, "ktime_get_real_ts64" },
	{ 0x7fd36f2e, "time64_to_tm" },
	{ 0x40a621c5, "snprintf" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xfd285498, "unregister_netdevice_notifier" },
	{ 0xd272d446, "__fentry__" },
	{ 0xfd285498, "register_netdevice_notifier" },
	{ 0xe8213e80, "_printk" },
	{ 0xc773217c, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xd272d446,
	0x680628e7,
	0x7fd36f2e,
	0x40a621c5,
	0xd272d446,
	0xfd285498,
	0xd272d446,
	0xfd285498,
	0xe8213e80,
	0xc773217c,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"__x86_return_thunk\0"
	"ktime_get_real_ts64\0"
	"time64_to_tm\0"
	"snprintf\0"
	"__stack_chk_fail\0"
	"unregister_netdevice_notifier\0"
	"__fentry__\0"
	"register_netdevice_notifier\0"
	"_printk\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C0239A65264B5C3094E0D54");
