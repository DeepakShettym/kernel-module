#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "ioctl_deriver.h"

int main()
{
    int fd;
    int32_t val = 567;

    fd = open("/dev/avng_device", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    ioctl(fd, WR_VALUE, &val);

    val = 0;
    ioctl(fd, RD_VALUE, &val);
    printf("Value from kernel = %d\n", val);

    close(fd);
    return 0;
}

