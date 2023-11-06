#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chall3"
#define CLASS_NAME "chall3"

MODULE_AUTHOR("Dvorhack");
MODULE_DESCRIPTION("Challenge 3");
MODULE_LICENSE("GPL");

int major;
static struct class*  my_class  = NULL;
static struct device* my_device = NULL;

unsigned long arg = 0;
unsigned long (*function)(unsigned long) = NULL;

static ssize_t chall_3_ioctl(struct file *filp, unsigned int code, unsigned long data) {
    printk(KERN_ALERT "[chall_3] chall_3_ioctl");
    // unsigned long (*p)(unsigned long) = NULL;
    unsigned long ret = 0;
    unsigned long ret2 = 0;

   
    switch(code){
        case 0xb00b:
            ret = copy_from_user(&arg, (void *)data, 8);
        break;
        case 0xb0b0:
            ret = copy_from_user(&function, (void *)data, 8);
        break;
        case 0xbeef:
            printk(KERN_ALERT "[chall_3] run %p %lx", function, arg);
            ret2 = function(arg);
            ret = copy_to_user((void *)data, &ret2, 8);
        break;
        default:
            printk(KERN_ALERT "[chall_3] unknown ioctl");
    }


    return ret;
}

static struct file_operations fops = {
    .unlocked_ioctl = chall_3_ioctl,
};

static int __init chall_3_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if ( major < 0 )
        printk(KERN_ALERT "[chall_3] Error assigning Major Number!");
    
    // Register device class
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[chall_3] Failed to register device class\n");
    }

    // Register the device driver
    my_device = device_create(my_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[chall_3] Failed to create the device\n");
    }

    arg = 0;

    return 0;
}

static void __exit chall_3_exit(void) {
    device_destroy(my_class, MKDEV(major, 0));              // remove the device
    class_unregister(my_class);                             // unregister the device class
    class_destroy(my_class);                                // remove the device class
    unregister_chrdev(major, DEVICE_NAME);                  // unregister the major number
    printk(KERN_INFO "[chall_3] Closing!\n");
}

module_init(chall_3_init);
module_exit(chall_3_exit);
