#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chall1"
#define CLASS_NAME "chall1"
#define NB_CHAPITRE 10

MODULE_AUTHOR("Dvorhack");
MODULE_DESCRIPTION("Challenge 1");
MODULE_LICENSE("GPL");

int major;
static struct class*  my_class  = NULL;
static struct device* my_device = NULL;
struct chapitre {
    unsigned int size;
    char content[10*NB_CHAPITRE];
};
struct chapitre livre[NB_CHAPITRE];
unsigned long chapitre_en_cours;

static ssize_t chall_1_read(struct file *filp, char __user *buffer, size_t len, loff_t *off) {
    printk(KERN_ALERT "[chall_1] in chall_1_read");

    char to_send[50];
    strcpy(to_send, livre[chapitre_en_cours].content);
    int ret = copy_to_user(buffer, to_send, livre[chapitre_en_cours].size);

    return ret;
}

static ssize_t chall_1_write(struct file *filp, const char __user *buffer, size_t len, loff_t *off) {
    printk(KERN_ALERT "[chall_1] in chall_1_write");
    int ret = 0;
    if (len >= livre[chapitre_en_cours].size){
        printk(KERN_ALERT "[chall_1] size too big");
        return -1;
    }else{
        ret = copy_from_user(livre[chapitre_en_cours].content, buffer, len);
    }
    return ret;
}

static ssize_t chall_1_ioctl(struct file *filp, unsigned int code, unsigned long data) {
    printk(KERN_ALERT "[chall_1] in chall_1_ioctl");

    switch(code){
        case 0x1337:
            if (data >= NB_CHAPITRE){
                printk(KERN_ALERT "[chall_1] chapter too big");
                return -1;
            }else{
                chapitre_en_cours = data;
            }
            break;
        default:
            printk(KERN_ALERT "[chall_1] unknown ioctl");
    }

    return 0;
}

static struct file_operations fops = {
    .read = chall_1_read,
    .write = chall_1_write,
    .unlocked_ioctl = chall_1_ioctl,
};

static int __init chall_1_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if ( major < 0 )
        printk(KERN_ALERT "[chall_1] Error assigning Major Number!");
    
    // Register device class
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[chall_1] Failed to register device class\n");
    }

    // Register the device driver
    my_device = device_create(my_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[chall_1] Failed to create the device\n");
    }

    // Initialisation du livre
    for(int i=1; i<=NB_CHAPITRE; i++){
        livre[i-1].size = i*10;
        memset(livre[i-1].content, 0, livre[i-1].size);
    }
    chapitre_en_cours = 0;

    return 0;
}

static void __exit chall_1_exit(void) {
    device_destroy(my_class, MKDEV(major, 0));              // remove the device
    class_unregister(my_class);                             // unregister the device class
    class_destroy(my_class);                                // remove the device class
    unregister_chrdev(major, DEVICE_NAME);                  // unregister the major number
    printk(KERN_INFO "[chall_1] Closing!\n");
}

module_init(chall_1_init);
module_exit(chall_1_exit);
