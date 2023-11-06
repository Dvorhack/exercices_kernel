#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chall2"
#define CLASS_NAME "chall2"
#define MY_BLOCK_SIZE 0x1000

MODULE_AUTHOR("Dvorhack");
MODULE_DESCRIPTION("Challenge 2");
MODULE_LICENSE("GPL");

int major;
static struct class*  my_class  = NULL;
static struct device* my_device = NULL;
struct to_read_s {
    unsigned long addr;
    char content[MY_BLOCK_SIZE];
};

char *secret_flag;

static ssize_t chall_2_read(struct file *filp, char __user *buffer, size_t len, loff_t *off) {
    printk(KERN_ALERT "[chall_2] in chall_2_read");
    int ret = copy_to_user(buffer, "Cette fonction ne te sert à rien <3", 36);
    return ret;
}

static ssize_t chall_2_ioctl(struct file *filp, unsigned int code, unsigned long data) {
    printk(KERN_ALERT "[chall_2] in chall_2_ioctl");

    struct to_read_s *to_read = (struct to_read_s *)data;
    char *addr_to_read = 0;
    int ret = 0;

    switch(code){
        case 0xc0fe:
            ret = copy_from_user(&addr_to_read, &to_read->addr, 8);
            ret = copy_to_user(to_read->content, addr_to_read, MY_BLOCK_SIZE);
            break;
        default:
            printk(KERN_ALERT "[chall_2] unknown ioctl");
    }

    return ret;
}

static struct file_operations fops = {
    .read = chall_2_read,
    .unlocked_ioctl = chall_2_ioctl,
};

static int __init chall_2_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if ( major < 0 )
        printk(KERN_ALERT "[chall_2] Error assigning Major Number!");
    
    // Register device class
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[chall_2] Failed to register device class\n");
    }

    // Register the device driver
    my_device = device_create(my_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[chall_2] Failed to create the device\n");
    }

    secret_flag = kmalloc(100, GFP_KERNEL);
    strcpy(secret_flag, "HackUTT{fake_flag}");

    return 0;
}

static void __exit chall_2_exit(void) {
    device_destroy(my_class, MKDEV(major, 0));              // remove the device
    class_unregister(my_class);                             // unregister the device class
    class_destroy(my_class);                                // remove the device class
    unregister_chrdev(major, DEVICE_NAME);                  // unregister the major number
    printk(KERN_INFO "[chall_2] Closing!\n");
}

module_init(chall_2_init);
module_exit(chall_2_exit);
