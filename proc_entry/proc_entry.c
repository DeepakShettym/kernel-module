#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "myproc"
#define BUF_SIZE 10

static char msg[BUF_SIZE];

struct proc_dir_entry* rootDir;
struct proc_dir_entry* infoDir;

struct proc_dir_entry* versionEntry;
struct proc_dir_entry* statusEntry;
struct proc_dir_entry* configEntry;

static ssize_t myDriver_read(struct file *file, char __user *user_buf,
                           size_t count, loff_t *ppos){
	char driver_msg[] = "get a life bro!";

	if(*ppos > 0){
		return 0;
	}

	int len = strlen(msg);


	if(copy_to_user(user_buf ,driver_msg , len)){
		return -EFAULT;
	}

	*ppos = len;

	return len;
}

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

static const struct proc_ops myDriver_ops = {
    .proc_read = myDriver_read
};
static int __init myproc_init(void)
{   /*func to create proc entry */
    proc_create(PROC_NAME, 0666, NULL, &myproc_ops); // name , permission , parent , file operation
    rootDir = proc_mkdir("myDriver" , NULL);
    infoDir = proc_mkdir("DriverInfo",rootDir);

    versionEntry = proc_create("version", 0666 , infoDir , &myDriver_ops);
    statusEntry = proc_create("status" , 0666 , infoDir , &myDriver_ops);
    configEntry = proc_create("config" , 0666 , infoDir , &myDriver_ops);
    
    pr_info("myproc  and myDriver craeted\n");
    return 0;
}

static void __exit myproc_exit(void)
{   /*func to remove proc entry*/
    remove_proc_entry(PROC_NAME, NULL); // older methord

    if (versionEntry) proc_remove(versionEntry);
    if (statusEntry)  proc_remove(statusEntry);
    if(configEntry)   proc_remove(configEntry);

    // remove infoDir folder
    if (infoDir) proc_remove(infoDir);

    // remove rootDir folder
    if (rootDir) proc_remove(rootDir);

    pr_info("proc directories removed\n");
    pr_info("myproc removed\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Deepak");
MODULE_DESCRIPTION("Simple proc entry example");
module_init(myproc_init);
module_exit(myproc_exit);

