#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/notifier.h>

#define TARGET_VENDOR_ID  0x03f0   // HP
#define TARGET_PRODUCT_ID 0x1985   // HP Flash Drive

#define DEVICE_NAME "usb_pendrive"
#define CLASS_NAME  "usbdrv"


static int major;
static struct class *usb_class;
static struct cdev usb_cdev;

static int device_connected;
static int connection_count;
static u16 vendor_id;
static u16 product_id;

//Fops
static int usb_open(struct inode *inode, struct file *file)
{
    pr_info("usbdrv: device opened\n");
    return 0;
}

static int usb_release(struct inode *inode, struct file *file)
{
    pr_info("usbdrv: device closed\n");
    return 0;
}

static ssize_t usb_read(struct file *file,
                        char __user *buf,
                        size_t count,
                        loff_t *ppos)
{
    char kbuf[256];
    int len;

    if (*ppos > 0)
        return 0;

    len = snprintf(kbuf, sizeof(kbuf),
        "USB Pendrive Driver Status\n"
        "--------------------------\n"
        "Connection State : %s\n"
        "Vendor ID        : 0x%04x\n"
        "Product ID       : 0x%04x\n"
        "Total Connects   : %d\n",
        device_connected ? "Connected" : "Disconnected",
        vendor_id,
        product_id,
        connection_count
    );

    if (copy_to_user(buf, kbuf, len))
        return -EFAULT;

    *ppos = len;
    return len;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = usb_open,
    .read    = usb_read,
    .release = usb_release,
};
//Usb  notifier
static int usb_event(struct notifier_block *nb,
                     unsigned long action,
                     void *data)
{
    struct usb_device *udev = data;

    if (!udev)
        return NOTIFY_OK;

    if (le16_to_cpu(udev->descriptor.idVendor) != TARGET_VENDOR_ID ||
        le16_to_cpu(udev->descriptor.idProduct) != TARGET_PRODUCT_ID)
        return NOTIFY_OK;

    switch (action) {
    case USB_DEVICE_ADD:
        vendor_id = le16_to_cpu(udev->descriptor.idVendor);
        product_id = le16_to_cpu(udev->descriptor.idProduct);
        device_connected = 1;
        connection_count++;
        pr_info("usbdrv: HP Pendrive connected\n");
        break;

    case USB_DEVICE_REMOVE:
        device_connected = 0;
        pr_info("usbdrv: HP Pendrive disconnected\n");
        break;
    }

    return NOTIFY_OK;
}

static struct notifier_block usb_nb = {
    .notifier_call = usb_event,
};

static int __init usb_init(void)
{
    dev_t dev;
    int ret;

    ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    major = MAJOR(dev);

    cdev_init(&usb_cdev, &fops);
    cdev_add(&usb_cdev, dev, 1);

    usb_class = class_create(CLASS_NAME);
    device_create(usb_class, NULL, dev, NULL, DEVICE_NAME);

    usb_register_notify(&usb_nb);

    pr_info("usbdrv: module loaded successfully\n");
    return 0;
}

static void __exit usb_exit(void)
{
    dev_t dev = MKDEV(major, 0);

    usb_unregister_notify(&usb_nb);
    device_destroy(usb_class, dev);
    class_destroy(usb_class);
    cdev_del(&usb_cdev);
    unregister_chrdev_region(dev, 1);

    pr_info("usbdrv: module unloaded\n");
}

module_init(usb_init);
module_exit(usb_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ameya Katdare");
MODULE_DESCRIPTION("Safe USB Pendrive Monitor using USB Notifier + Char Driver");

