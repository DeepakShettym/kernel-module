#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include "ioctl_deriver.h"
static int32_t value;

static dev_t dev;
static struct cdev avng_cdev;
static struct class *dev_class;

/* Prototypes */
static int avng_open(struct inode *, struct file *);
static int avng_release(struct inode *, struct file *);
static long avng_ioctl(struct file *, unsigned int, unsigned long);

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = avng_open,
    .release        = avng_release,
    .unlocked_ioctl = avng_ioctl,
};

static int avng_open(struct inode *inode, struct file *file)
{
    pr_info("Device opened\n");
    return 0;
}

static int avng_release(struct inode *inode, struct file *file)
{
    pr_info("Device closed\n");
    return 0;
}

static long avng_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {

    case WR_VALUE:
        if (copy_from_user(&value,
                           (int32_t __user *)arg,
                           sizeof(value)))
            return -EFAULT;

        pr_info("Value written = %d\n", value);
        break;

    case RD_VALUE:
        if (copy_to_user((int32_t __user *)arg,
                         &value,
                         sizeof(value)))
            return -EFAULT;
        break;

    default:
        return -ENOTTY;
    }
    return 0;
}

static int __init avng_driver_init(void)
{
    if (alloc_chrdev_region(&dev, 0, 1, "avng_dev") < 0)
        return -EINVAL;

    cdev_init(&avng_cdev, &fops);

    if (cdev_add(&avng_cdev, dev, 1))
        goto err_unregister;

    dev_class = class_create("avng_class");
    if (IS_ERR(dev_class))
        goto err_cdev;

    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "avng_device")))
        goto err_class;

    pr_info("AVNG driver loaded\n");
    return 0;

err_class:
    class_destroy(dev_class);
err_cdev:
    cdev_del(&avng_cdev);
err_unregister:
    unregister_chrdev_region(dev, 1);
    return -EINVAL;
}

static void __exit avng_driver_exit(void)
{
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&avng_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("AVNG driver unloaded\n");
}

module_init(avng_driver_init);
module_exit(avng_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Deepak M");
MODULE_DESCRIPTION("Correct IOCTL Linux Driver");
MODULE_VERSION("1.5");

