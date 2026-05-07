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
	{ 0x54b29f7e, "wake_up_process" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xac4b86a6, "filp_open" },
	{ 0x00cbc0d1, "kernel_read" },
	{ 0x2a11f418, "filp_close" },
	{ 0x17545440, "strstr" },
	{ 0x67628f51, "msleep" },
	{ 0x5e505530, "kthread_should_stop" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xd272d446, "kernel_power_off" },
	{ 0x806b982e, "kthread_stop" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0x14c2bc4e, "kthread_create_on_node" },
	{ 0xc773217c, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x54b29f7e,
	0xd272d446,
	0xac4b86a6,
	0x00cbc0d1,
	0x2a11f418,
	0x17545440,
	0x67628f51,
	0x5e505530,
	0xd272d446,
	0xd272d446,
	0x806b982e,
	0xd272d446,
	0xe8213e80,
	0x14c2bc4e,
	0xc773217c,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"wake_up_process\0"
	"__x86_return_thunk\0"
	"filp_open\0"
	"kernel_read\0"
	"filp_close\0"
	"strstr\0"
	"msleep\0"
	"kthread_should_stop\0"
	"__stack_chk_fail\0"
	"kernel_power_off\0"
	"kthread_stop\0"
	"__fentry__\0"
	"_printk\0"
	"kthread_create_on_node\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D7D30BE3D4509DB31BC0BAF");
