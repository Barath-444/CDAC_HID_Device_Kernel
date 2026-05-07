savedcmd_bt_kprobe.mod := printf '%s\n'   bt_kprobe.o | awk '!x[$$0]++ { print("./"$$0) }' > bt_kprobe.mod
