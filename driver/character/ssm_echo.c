#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define BUFSIZ 4096
#define DEVICE_NAME "ssm_echo"
#define MY_MAGIC 'G'
#define READ_IOCTL _IOR (MY_MAGIC, 0, int)
#define WRITE_IOCTL _IOW (MY_MAGIC, 1, int)

static char buf[BUFSIZ];

static long echo_file_ioctl (struct file *fp, unsigned int cmd, unsigned long arg)
{
  int len = BUFSIZ;
  switch (cmd)
    {
    case READ_IOCTL:
      printk (KERN_ALERT "Send to user: %s\n", buf);
      copy_to_user ((char *)arg, buf, BUFSIZ);
      break;
    case WRITE_IOCTL:
      copy_from_user (buf, (char *)arg, len);
      printk (KERN_ALERT "Recevie from user: %s\n", buf);
      break;
    default:
      return -ENOTTY;
    }

    return len;
}

struct file_operations fops = {
  // .ioctl = echo_file_ioctl // Previous verison kernel 2.6
  .compat_ioctl = echo_file_ioctl,
  .unlocked_ioctl = echo_file_ioctl,
};

static int major;

static int __init echo_module_init (void)
{
  printk (KERN_ALERT "Hello, ssm_echo character device driver!\n");

	major = register_chrdev(0, DEVICE_NAME, &fops);

	if (major < 0)
		{
			printk (KERN_ALERT "Registering ssm_echo character device failure. return value: %d.\n", major);
			return major;
		}

  printk (KERN_ALERT "Registering ssm_echo characcter device success, major number: %d\n", major);

  return 0;
}

static void __exit echo_module_exit (void)
{
  printk (KERN_ALERT "Good bye, echo character device driver.\n");

	unregister_chrdev (major, DEVICE_NAME);
}

module_init (echo_module_init);
module_exit (echo_module_exit);

MODULE_LICENSE("GPL");
