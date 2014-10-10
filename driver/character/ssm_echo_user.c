#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define MY_MAGIC 'G'
#define READ_IOCTL  _IOR(MY_MAGIC, 0, int)
#define WRITE_IOCTL _IOW(MY_MAGIC, 1, int)
#define DEVICE_PATH "/dev/ssm_echo"

int main (void)
{
  char buf[BUFSIZ];
  int fd = -1;
  if ((fd = open (DEVICE_PATH, O_RDWR)) < 0)
    {
      perror ("open");
      return -1;
    }

  if (ioctl (fd, WRITE_IOCTL, "Hello, world!") < 0)
    perror ("ioctl");
  if (ioctl (fd, READ_IOCTL, buf) < 0)
    perror ("ioctl");

  printf ("Message: %s\n", buf);

  return 0;
}
