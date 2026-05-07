savedcmd_hid_alert.mod := printf '%s\n'   hid_alert.o | awk '!x[$$0]++ { print("./"$$0) }' > hid_alert.mod
