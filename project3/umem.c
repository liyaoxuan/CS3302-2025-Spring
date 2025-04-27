#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "umem.h"

MODULE_LICENSE("GPL");

// page managed by umem
struct umem_pool_page_info
{
    struct page *page;
    struct list_head list;
};

// pool of pages managed by umem
static struct umem_pool_info
{
    struct list_head page_list[UMEM_NUM_POOL];
} umem_pool_info;


// page managed by umem for a process
struct umem_proc_addr_page_info
{
    struct page *page;
    struct list_head list;
};

// mem info and pages of a process
struct umem_proc_addr_info
{
    struct umem_info kern_umem_info;
    struct list_head page_list;
    struct list_head list;
};

// list of mem info and pages of a process
struct umem_proc_info
{
    struct list_head addr_list;
};

/*
                          ┌────────────────┐                 
                          │ umem_proc_info │                 
                          │                │                 
┌─────────────────────────┼───addr_list◄───┼────────────────┐
│                         └────────────────┘                │
│ ┌───────────────────┐       ┌───────────────────┐         │
│ │umem_proc_addr_info│       │umem_proc_addr_info│         │
│ │                   │       │                   │         │
│ │  kern_umem_info   │       │  kern_umem_info   │         │
│ │┌──────page_list◄──┼─┐     │       page_list   │         │
└─┼┼──────────►list───┼─┼─────┼───────────►list───┼─► ... ──┘
  └┼──────────────────┘ │     └───────────────────┘          
   │                    └──────────────────┐                 
   │ ┌──────────────────────────┐          │                 
   │ │ umem_proc_addr_page_info │          │                 
   │ │                          │          │                 
   │ │           page           │          │                 
   └─┼──────────►list───────────┼──► ... ──┘                 
     └──────────────────────────┘                            
 */
static int umem_open(struct inode *inode, struct file *filp)
{
    // TODO: Initialize umem_proc_info in filp 
    pr_info("todo\n");
    return 0;
}

static int umem_release(struct inode *inode, struct file *filp)
{
    // TODO: release all pages to pool and delete related data 
    pr_info("todo\n");
    return 0;
}

static long umem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    long err;
    struct umem_info kern_umem_info;

    pr_info("umem_ioctl cmd: %u\n", cmd);

    switch (cmd)
    {
    case UMEM_IOC_MALLOC:
        err = get_user(kern_umem_info.umem_size, &((struct umem_info __user *)arg)->umem_size);
        if (err)
        {
            pr_info("umem_ioctl cmd malloc: failed to get umem_size\n");
            return err;
        }
        err = get_user(kern_umem_info.umem_pool, &((struct umem_info __user *)arg)->umem_pool);
        if (err)
        {
            pr_info("umem_ioctl cmd malloc: failed to get umem_pool\n");
            return err;
        }
        pr_info("umem_ioctl cmd malloc: %llu %llu\n", kern_umem_info.umem_size, kern_umem_info.umem_pool);
        if (kern_umem_info.umem_pool >= UMEM_NUM_POOL)
        {
            pr_info("umem_ioctl cmd malloc: invalid pool\n");
            return -EINVAL;
        }
        // TODO: 
        // allocate vm using vm_mmap()
        // update umem_addr in args using put_user
        // update proc_addr_info
        pr_info("todo\n");
        return -EINVAL;
        return 0;

    case UMEM_IOC_FREE:
        err = get_user(kern_umem_info.umem_addr, &((struct umem_info __user *)arg)->umem_addr);
        if (err)
        {
            pr_info("umem_ioctl cmd free: failed to get umem_addr\n");
            return err;
        }
        pr_info("umem_ioctl cmd free: %px\n", (void *)kern_umem_info.umem_addr);
        // TODO
        // find the corresponding umem_proc_addr_info
        // check if the addr is valid
        // munmap the vm using vm_munmap()
        // free the pages by returning the page to pool
        // remove umem_proc_addr_info from list and release it
        pr_info("todo\n");
        return -EINVAL;
        return 0;
    case UMEM_IOC_PAGE_FAULT:
        err = get_user(kern_umem_info.umem_addr, &((struct umem_info __user *)arg)->umem_addr);
        if (err)
        {
            pr_info("umem_ioctl cmd page_fault: failed to get umem_addr\n");
            return err;
        }
        pr_info("umem_ioctl cmd page_fault: %px\n", (void *)kern_umem_info.umem_addr);
        // TODO
        // check if the address is a valid address by traversaling the addr_list
        // check if the pool is not empty
        // get the first page from the pool
        // add the page info in the umem_proc_addr_info
        // find the vma using find_vma()
        // remap the page using remap_pfn_range()

        pr_info("todo\n");
        return -EINVAL;
        return 0;
    }

    return -EINVAL;
}

static dev_t devt;
static struct cdev cdev;
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = umem_open,
    .release = umem_release,
    .unlocked_ioctl = umem_ioctl,
    .compat_ioctl = umem_ioctl,
};
static struct class *cls;
static struct device *dev;

static void umem_pool_init(void)
{
    // TODO
    // initialize the page list for each pool
    // allocate UMEM_POOL_SIZE pages to each pool using alloc_page()
    // and add the page info in the list
    pr_info("todo\n");
}

static void umem_pool_destroy(void)
{
    // TODO
    // release all pages in the pool using __free_page()
    // and free the umem_pool_page_info
    pr_info("todo\n");
}

static int __init umem_init(void)
{
    int err;

    err = alloc_chrdev_region(&devt, 0, 1, UMEM_NAME);
    if (err)
    {
        goto err_alloc_chrdev_region;
    }

    cdev_init(&cdev, &fops);
    err = cdev_add(&cdev, devt, 1);
    if (err)
    {
        goto err_cdev_add;
    }

    cls = class_create(UMEM_NAME);
    if (IS_ERR(cls))
    {
        err = PTR_ERR(cls);
        goto err_class_create;
    }
    dev = device_create(cls, NULL, devt, NULL, UMEM_NAME);
    if (IS_ERR(dev))
    {
        err = PTR_ERR(dev);
        goto err_device_create;
    }
    pr_info("umem installed\n");
    umem_pool_init();
    return 0;
err_device_create:
    class_destroy(cls);
err_class_create:
err_cdev_add:
    cdev_del(&cdev);
    unregister_chrdev_region(devt, 1);
err_alloc_chrdev_region:
    return err;
}

static void __exit umem_exit(void)
{
    umem_pool_destroy();
    device_destroy(cls, devt);
    class_destroy(cls);
    cdev_del(&cdev);
    unregister_chrdev_region(devt, 1);
    pr_info("umem removed\n");
}

module_init(umem_init);
module_exit(umem_exit);