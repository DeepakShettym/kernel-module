savedcmd_ioctl_deriver.mod := printf '%s\n'   ioctl_deriver.o | awk '!x[$$0]++ { print("./"$$0) }' > ioctl_deriver.mod
