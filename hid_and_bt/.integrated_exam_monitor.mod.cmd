savedcmd_integrated_exam_monitor.mod := printf '%s\n'   integrated_exam_monitor.o | awk '!x[$$0]++ { print("./"$$0) }' > integrated_exam_monitor.mod
