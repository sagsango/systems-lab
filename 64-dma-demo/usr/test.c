#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/dma_read_demo", O_RDONLY);
    char buf[64];

    read(fd, buf, sizeof(buf));

    write(1, buf, sizeof(buf)); /* print to stdout */
    write(1, "\n", 1);

    close(fd);
    return 0;
}

