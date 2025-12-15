#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>        // alloc_chrdev_region
#include <linux/cdev.h>      // cdev
#include <linux/uaccess.h>   // copy_to_user, copy_from_user
#include <linux/device.h>    // class_create, device_create

#define DEVICE_NAME "Iron_Man"
#define CLASS_NAME  "Avengers"
#define BUFF_SIZE   100

static dev_t dev_num;                  // major + minor
static struct cdev assemble_cdev;      // cdev structure
static struct class *assemble_class;   // sysfs class
static struct device *assemble_device; // device

static char buffer[BUFF_SIZE];

/* ---------------- file operations ---------------- */

static int assemble_open(struct inode *inode, struct file *file)
{
    pr_info("Iron man ready to fight\n");
    return 0;
}

static ssize_t assemble_read(struct file *file, char __user *user_buf,
                             size_t count, loff_t *ppos)
{   int ret ;
    ret =  simple_read_from_buffer(
        user_buf, count, ppos, buffer, strlen(buffer)
    );

    pr_info("iron man read the message\n");
    return ret;
}

static ssize_t assemble_write(struct file *file,
                              const char __user *user_buf,
                              size_t count, loff_t *ppos)
{
    if (count > BUFF_SIZE)
        count = BUFF_SIZE;

    if (copy_from_user(buffer, user_buf, count))
        return -EFAULT;

    buffer[count] = '\0';
    pr_info("Message to Tony Stark received: %s\n", buffer);

    return count;
}

static int assemble_release(struct inode *inode, struct file *file)
{
    pr_info("Iron Man saved the world\n");
    return 0;
}

/* file operations table */
static struct file_operations fops = {
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

    /* Allocate major + minor */
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    pr_info("Tony Stark: major=%d minor=%d\n",
            MAJOR(dev_num), MINOR(dev_num));

    /* Init and add cdev */

    //why cdev ? so its a kernel side registration object that maps device number -> driver functions
 
    cdev_init(&assemble_cdev, &fops);
    ret = cdev_add(&assemble_cdev, dev_num, 1); // it takes the dev_num and map it to driver function 
    if (ret)
        goto err_cdev;

    /* Create class */
    assemble_class = class_create(CLASS_NAME);
    if (IS_ERR(assemble_class)) {
        ret = PTR_ERR(assemble_class);
        goto err_class;
    }

    /* Create device (auto creates /dev/Iron_Man) */
    assemble_device = device_create(
        assemble_class, NULL, dev_num, NULL, DEVICE_NAME
    );
    if (IS_ERR(assemble_device)) {
        ret = PTR_ERR(assemble_device);
        goto err_device;
    }

    pr_info("Avengers assembled successfully\n");
    return 0;

err_device:
    class_destroy(assemble_class);
err_class:
    cdev_del(&assemble_cdev);
err_cdev:
    unregister_chrdev_region(dev_num, 1);
    return ret;
}

/* ---------------- module exit ---------------- */

static void __exit avengers_disassemble(void)
{
    device_destroy(assemble_class, dev_num);
    class_destroy(assemble_class);

    cdev_del(&assemble_cdev);
    unregister_chrdev_region(dev_num, 1);

    pr_info("Avengers disassembled\n");
}

module_init(avengers_assemble);
module_exit(avengers_disassemble);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Deepak");
MODULE_DESCRIPTION("Avengers character device driver with sysfs");

