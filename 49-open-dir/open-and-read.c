#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/*
 *
 * O_DIRECTORY : If this flag is used, open will fail if path is not directory
 *		 So just a sanity check
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_or_directory>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("open was sucessfull now do read (getdents:read_dir, read:read_file\n");

    close(fd);
    return 0;
}

