#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "test"
#define CLASS_NAME "test"

MODULE_AUTHOR("Dvorhack");
MODULE_DESCRIPTION("Test");
MODULE_LICENSE("GPL");

int major;
static struct class*  my_class  = NULL;
static struct device* my_device = NULL;

static ssize_t test_read(struct file *filp, char __user *buffer, size_t len, loff_t *off) {
    printk(KERN_ALERT "[test] in test_read");

    int ret = copy_to_user(buffer, "Bonjour à vous, je suis un module kernel de test", 49);

    return ret;
}

static ssize_t test_write(struct file *filp, const char __user *buffer, size_t len, loff_t *off) {
    printk(KERN_ALERT "[test] in test_write");

    char buf[100];
    int ret = copy_from_user(buf, buffer, len);
    printk(KERN_ALERT "[test] Got: %s", buf);
    
    return ret;
}

static ssize_t test_ioctl(struct file *filp, unsigned int code, unsigned long data) {
    printk(KERN_ALERT "[test] in test_ioctl");
    int param, ret;

    switch(code){
        case 0x1337:
            ret = copy_from_user(&param, (void *)data, 4);
            printk(KERN_ALERT "[test] Got %d", param);
            break;
        default:
            printk(KERN_ALERT "[test] unknown ioctl");
    }

    return ret;
}

static struct file_operations fops = {
    .read = test_read,
    .write = test_write,
    .unlocked_ioctl = test_ioctl,
};

static int __init test_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if ( major < 0 )
        printk(KERN_ALERT "[test] Error assigning Major Number!");
    
    // Register device class
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[test] Failed to register device class\n");
    }

    // Register the device driver
    my_device = device_create(my_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[test] Failed to create the device\n");
    }
    printk(KERN_ALERT "[test] Je suis bien chargé :)");
    return 0;
}

static void __exit test_exit(void) {
    device_destroy(my_class, MKDEV(major, 0));              // remove the device
    class_unregister(my_class);                             // unregister the device class
    class_destroy(my_class);                                // remove the device class
    unregister_chrdev(major, DEVICE_NAME);                  // unregister the major number
    printk(KERN_INFO "[test] Closing!\n");
}

module_init(test_init);
module_exit(test_exit);
