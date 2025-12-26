📌 USB Pendrive Monitor Driver (Linux)
🔹 Project Overview

This project implements a safe hybrid Linux device driver that monitors USB pendrive connection and disconnection events.
The driver registers with the Linux USB subsystem to detect pendrive events and exposes the device status to user space through a character device (/dev/usb_pendrive).
Actual data transfer and mounting are handled by the existing usb-storage driver, while this driver focuses on monitoring and reporting.

🔧 Features

Detects USB pendrive connect / disconnect

Displays Vendor ID, Product ID

Maintains connection count

User-space access via standard file operations (cat)

⚙️ Build & Run Instructions
1️⃣ Install required tools
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r) git

2️⃣ Clone the repository
git clone https://github.com/Ameya32/Usb-pendrive-monitor-driver.git
cd Usb-pendrive-monitor-driver

3️⃣ Build the kernel module
make

4️⃣ Insert the driver
sudo insmod usb_pendrive_enhanced.ko

5️⃣ Read driver output
sudo cat /dev/usb_pendrive

6️⃣ Check kernel logs (optional)
dmesg | tail

7️⃣ Remove the driver
sudo rmmod usb_pendrive_enhanced

📄 Sample Output
USB Pendrive Driver Status
--------------------------
Connection State : Connected
Vendor ID        : 0x03f0
Product ID       : 0x1985
Total Connects   : 1


Author

Ameya Katdare
