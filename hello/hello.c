#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Deepak");
MODULE_DESCRIPTION("Simple Hello World Module");

static int __init hello_init(void)
{
    pr_info("Hello Deepak, kernel module loaded!\n");
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("Goodbye Deepak, kernel module unloaded!\n");
}

module_init(hello_init);
module_exit(hello_exit);

