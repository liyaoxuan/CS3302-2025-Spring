#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

static ssize_t proc_read(struct file *fp, char __user *ubuf, size_t len, loff_t *pos)
{
	return 0;
}

static ssize_t proc_write(struct file *fp, const char __user *ubuf, size_t len, loff_t *pos)
{
	return 0;
}

static const struct proc_ops proc_ops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

static int __init proc_init(void)
{
    printk(KERN_INFO "hello from kernel!\n");
    return 0;
}

static void __exit proc_exit(void)
{
    printk(KERN_INFO "module unloaded\n");
}

module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
