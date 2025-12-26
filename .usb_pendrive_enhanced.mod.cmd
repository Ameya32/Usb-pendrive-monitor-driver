savedcmd_usb_pendrive_enhanced.mod := printf '%s\n'   usb_pendrive_enhanced.o | awk '!x[$$0]++ { print("./"$$0) }' > usb_pendrive_enhanced.mod
