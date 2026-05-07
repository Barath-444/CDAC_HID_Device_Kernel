savedcmd_monitor_detect.mod := printf '%s\n'   monitor_detect.o | awk '!x[$$0]++ { print("./"$$0) }' > monitor_detect.mod
