#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static int value = 10;
module_param(value, int, 0644); // variable , type , permission
MODULE_PARM_DESC(value, "An integer parameter");

static char *name = "deepak";
module_param(name, charp, 0644);
MODULE_PARM_DESC(name, "A string parameter");

static int __init param_init(void)
{
    printk(KERN_INFO "param module loaded\n");
    printk(KERN_INFO "value = %d\n", value);
    printk(KERN_INFO "name = %s\n", name);
    return 0;
}

static void __exit param_exit(void)
{
    printk(KERN_INFO "param module unloaded\n");
}

module_init(param_init);
module_exit(param_exit);

