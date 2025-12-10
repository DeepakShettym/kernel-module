savedcmd_param.mod := printf '%s\n'   param.o | awk '!x[$$0]++ { print("./"$$0) }' > param.mod
