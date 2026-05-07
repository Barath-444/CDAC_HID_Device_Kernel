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
	{ 0xd272d446, "kernel_power_off" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0xa5c7582d, "strsep" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0x680628e7, "ktime_get_real_ts64" },
	{ 0x2ecc6b55, "rtc_time64_to_tm" },
	{ 0x058c185a, "jiffies" },
	{ 0x32feeafc, "mod_timer" },
	{ 0x2352b148, "timer_delete_sync" },
	{ 0x80db3428, "proc_remove" },
	{ 0xd272d446, "__fentry__" },
	{ 0xa9cd46c3, "proc_create" },
	{ 0xe8213e80, "_printk" },
	{ 0x02f9bbf0, "init_timer_key" },
	{ 0xc773217c, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xd272d446,
	0xd272d446,
	0xa61fd7aa,
	0x092a35a2,
	0xa5c7582d,
	0x90a48d82,
	0xd272d446,
	0x680628e7,
	0x2ecc6b55,
	0x058c185a,
	0x32feeafc,
	0x2352b148,
	0x80db3428,
	0xd272d446,
	0xa9cd46c3,
	0xe8213e80,
	0x02f9bbf0,
	0xc773217c,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"__x86_return_thunk\0"
	"kernel_power_off\0"
	"__check_object_size\0"
	"_copy_from_user\0"
	"strsep\0"
	"__ubsan_handle_out_of_bounds\0"
	"__stack_chk_fail\0"
	"ktime_get_real_ts64\0"
	"rtc_time64_to_tm\0"
	"jiffies\0"
	"mod_timer\0"
	"timer_delete_sync\0"
	"proc_remove\0"
	"__fentry__\0"
	"proc_create\0"
	"_printk\0"
	"init_timer_key\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "E784567857DCBF5EA9E7CBC");
