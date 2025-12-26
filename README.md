USB Pendrive Monitor Driver (Linux)  
🔹 Project Overview

This project implements a safe hybrid Linux device driver that monitors USB pendrive connection and disconnection events.
The driver registers with the Linux USB subsystem to detect pendrive events and exposes the device status to user space through a character device (/dev/usb_pendrive).
Actual data transfer and mounting are handled by the existing usb-storage driver, while this driver focuses on monitoring and reporting.

🔹Features

Detects USB pendrive connect / disconnect

Displays Vendor ID, Product ID

Maintains connection count

User-space access via standard file operations (cat)

🔹 Build & Run Instructions  
 1.Install required tools  
`sudo apt update`  
`sudo apt install build-essential linux-headers-$(uname -r) git`

 2.Clone the repository  
git clone https://github.com/Ameya32/Usb-pendrive-monitor-driver.git  
`cd Usb-pendrive-monitor-driver`

 3.Build the kernel module  
`make`

 4.Insert the driver  
`sudo insmod usb_pendrive_enhanced.ko`

 5.Read driver output  
`sudo cat /dev/usb_pendrive`

 6.Check kernel logs  
`dmesg | tail`

 7.Remove the driver  
`sudo rmmod usb_pendrive_enhanced`

Author

Ameya Katdare
