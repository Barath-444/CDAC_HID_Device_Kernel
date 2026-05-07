savedcmd_exam_alert.mod := printf '%s\n'   exam_alert.o | awk '!x[$$0]++ { print("./"$$0) }' > exam_alert.mod
