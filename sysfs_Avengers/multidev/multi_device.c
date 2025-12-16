#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME   "ASSEMBLE"
#define CLASS_NAME    "Avengers"
#define BUFF_SIZE     100
#define DEVICE_COUNT  2

/* device numbers */
static dev_t dev_num;  // use dev_num + 0 , 1 .. etc

/* cdevs */
static struct cdev ironman_cdev;
static struct cdev spiderman_cdev;

/* sysfs */

/*class same for both*/
static struct class  *avengers_class;

/*create 2 devices */
static struct device *ironman_device;
static struct device *spiderman_device;

/*used diff buff per device*/
static char ironman_buffer[BUFF_SIZE];
static char spiderman_buffer[BUFF_SIZE];

/* ---------------- file operations ---------------- */

static int assemble_open(struct inode *inode, struct file *file)
{
    int minor = iminor(inode);

    if (minor == 0)
        pr_info("Iron Man ready to fight\n");
    else
        pr_info("Spider Man ready to fight\n");

    return 0;
}

static ssize_t assemble_read(struct file *file,
                             char __user *user_buf,
                             size_t count,
                             loff_t *ppos)
{
    int minor = iminor(file_inode(file));
    char *buf;
    ssize_t ret;

    buf = (minor == 0) ? ironman_buffer : spiderman_buffer;

    ret = simple_read_from_buffer(
        user_buf, count, ppos, buf, strlen(buf)
    );

    if (minor == 0)
        pr_info("Iron Man read the message\n");
    else
        pr_info("Spider Man read the message\n");

    return ret;
}

static ssize_t assemble_write(struct file *file,
                              const char __user *user_buf,
                              size_t count,
                              loff_t *ppos)
{
    int minor = iminor(file_inode(file));
    char *buf;

    if (count > BUFF_SIZE)
        count = BUFF_SIZE;

    buf = (minor == 0) ? ironman_buffer : spiderman_buffer;

    if (copy_from_user(buf, user_buf, count))
        return -EFAULT;

    buf[count] = '\0';

    if (minor == 0)
        pr_info("Message to Tony Stark received: %s\n", buf);
    else
        pr_info("Message to Spider Man received: %s\n", buf);

    return count;
}

static int assemble_release(struct inode *inode, struct file *file)
{
    int minor = iminor(inode);

    if (minor == 0)
        pr_info("Tony Stark saved the world\n");
    else
        pr_info("Spider Man saved the world\n");

    return 0;
}

/* file operations table */
static struct file_operations assemble_fops = {
    .owner   = THIS_MODULE,
    .open    = assemble_open,
    .read    = assemble_read,
    .write   = assemble_write,
    .release = assemble_release,
};

/* ---------------- module init ---------------- */

static int __init avengers_assemble(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, DEVICE_COUNT, DEVICE_NAME);
    if (ret)
        return ret;

    pr_info("AVENGERS ASSEMBLE: major=%d\n", MAJOR(dev_num));

    /* Iron Man (minor 0) */
    cdev_init(&ironman_cdev, &assemble_fops);
    ret = cdev_add(&ironman_cdev, dev_num + 0, 1);
    if (ret)
        goto err_cdev1;

    /* Spider Man (minor 1) */
    cdev_init(&spiderman_cdev, &assemble_fops);
    ret = cdev_add(&spiderman_cdev, dev_num + 1, 1);
    if (ret)
        goto err_cdev2;

    avengers_class = class_create(CLASS_NAME);
    if (IS_ERR(avengers_class)) {
        ret = PTR_ERR(avengers_class);
        goto err_class;
    }

    ironman_device = device_create(
        avengers_class, NULL, dev_num + 0, NULL, "IronMan"
    );
    if (IS_ERR(ironman_device)) {
        ret = PTR_ERR(ironman_device);
        goto err_device1;
    }

    spiderman_device = device_create(
        avengers_class, NULL, dev_num + 1, NULL, "SpiderMan"
    );
    if (IS_ERR(spiderman_device)) {
        ret = PTR_ERR(spiderman_device);
        goto err_device2;
    }

    pr_info("Avengers assembled successfully\n");
    return 0;

err_device2:
    device_destroy(avengers_class, dev_num + 0);
err_device1:
    class_destroy(avengers_class);
err_class:
    cdev_del(&spiderman_cdev);
err_cdev2:
    cdev_del(&ironman_cdev);
err_cdev1:
    unregister_chrdev_region(dev_num, DEVICE_COUNT);
    return ret;
}

/* ---------------- module exit ---------------- */

static void __exit avengers_disassemble(void)
{
    device_destroy(avengers_class, dev_num + 1);
    device_destroy(avengers_class, dev_num + 0);

    class_destroy(avengers_class);

    cdev_del(&spiderman_cdev);
    cdev_del(&ironman_cdev);

    unregister_chrdev_region(dev_num, DEVICE_COUNT);

    pr_info("Avengers disassembled\n");
}

module_init(avengers_assemble);
module_exit(avengers_disassemble);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Deepak");
MODULE_DESCRIPTION("Avengers multi-device char driver (IronMan + SpiderMan)");

