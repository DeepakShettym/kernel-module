#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mychardev"
#define BUF_SIZE 100

static dev_t dev_num;          // major + minor number
static struct cdev my_cdev;    // character device structure
static char buffer[BUF_SIZE];  // device buffer

// open
static int my_open(struct inode *inode, struct file *file)
{
    pr_info("mychardev: device opened\n");
    return 0;
}

// read
static ssize_t my_read(struct file *file, char __user *user_buf,
                       size_t count, loff_t *ppos)
{
    return simple_read_from_buffer(user_buf, count, ppos, buffer, strlen(buffer));
}

// write
static ssize_t my_write(struct file *file, const char __user *user_buf,
                        size_t count, loff_t *ppos)
{
    if (count > BUF_SIZE)
        count = BUF_SIZE;

    if (copy_from_user(buffer, user_buf, count))
        return -EFAULT;

    buffer[count] = '\0';
    pr_info("mychardev: received %s\n", buffer);
    return count;
}

// release (close)
static int my_release(struct inode *inode, struct file *file)
{
    pr_info("mychardev: device closed\n");
    return 0;
}

// file operations
static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .read    = my_read,
    .write   = my_write,
    .release = my_release,
};

// init
static int __init my_init(void)
{
    // allocate major + minor numbers
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);

    pr_info("mychardev: major=%d minor=%d\n", MAJOR(dev_num), MINOR(dev_num));

    // initialize cdev
    cdev_init(&my_cdev, &fops);
    cdev_add(&my_cdev, dev_num, 1);

    return 0;
}

// exit
static void __exit my_exit(void)
{
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("mychardev: unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

