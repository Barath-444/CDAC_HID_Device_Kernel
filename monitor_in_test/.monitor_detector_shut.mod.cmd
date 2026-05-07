savedcmd_monitor_detector_shut.mod := printf '%s\n'   monitor_detector_shut.o | awk '!x[$$0]++ { print("./"$$0) }' > monitor_detector_shut.mod
