#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "myproc"
#define BUF_SIZE 10

static char msg[BUF_SIZE];

// Read handler
static ssize_t myproc_read(struct file *file, char __user *user_buf,
                           size_t count, loff_t *ppos)
{
    return simple_read_from_buffer(user_buf, count, ppos,
                                   msg, strlen(msg));
}

// Write handler
static ssize_t myproc_write(struct file *file, const char __user *user_buf,
                            size_t count, loff_t *ppos)
{
    if (count > BUF_SIZE)
        count = BUF_SIZE;

    if (copy_from_user(msg, user_buf, count)) // safely copy the content to buffer
        return -EFAULT;

    msg[count] = '\0';
    pr_info("myproc: received: %s\n", msg);
    return count;
}

// File ops
static const struct proc_ops myproc_ops = {
    .proc_read  = myproc_read,
    .proc_write = myproc_write,
}; // needed to map the file opreration performed by the user to the function to be called 

static int __init myproc_init(void)
{   /*func to create proc entry */
    proc_create(PROC_NAME, 0666, NULL, &myproc_ops); // name , permission , parent , file ops
    pr_info("myproc created\n");
    return 0;
}

static void __exit myproc_exit(void)
{   /*func to remove proc entry*/
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("myproc removed\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Deepak");
MODULE_DESCRIPTION("Simple proc entry example");
module_init(myproc_init);
module_exit(myproc_exit);

