savedcmd_eth_alert.mod := printf '%s\n'   eth_alert.o | awk '!x[$$0]++ { print("./"$$0) }' > eth_alert.mod
