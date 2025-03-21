#include <linux/kernel.h>
#include <linux/module.h>

static int __init hello_init(void)
{
    printk(KERN_INFO "hello from kernel!\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "goodbye from kernel!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TEST");
MODULE_AUTHOR("CS3302");
