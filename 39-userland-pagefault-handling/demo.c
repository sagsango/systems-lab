#define _GNU_SOURCE
#include <pthread.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/userfaultfd.h>
#include <fcntl.h>
#include <errno.h>

#define PAGE_SIZE 4096
#define REGION_SIZE (PAGE_SIZE * 2) // Two pages

void *region; // memory region to be monitored

int uffd; // userfaultfd file descriptor

void *pagefault_handler_thread(void *arg) {
    struct uffd_msg msg;
    struct pollfd pollfd;
    char *page = malloc(PAGE_SIZE);
    if (!page) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    memset(&pollfd, 0, sizeof(pollfd));
    pollfd.fd = uffd;
    pollfd.events = POLLIN;

    while (1) {
        int nready = poll(&pollfd, 1, -1);
        if (nready == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if (pollfd.revents & POLLIN) {
            ssize_t nread = read(uffd, &msg, sizeof(msg));
            if (nread == 0) {
                printf("EOF on userfaultfd\n");
                break;
            } else if (nread == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            if (msg.event != UFFD_EVENT_PAGEFAULT) {
                fprintf(stderr, "Unexpected event on userfaultfd\n");
                exit(EXIT_FAILURE);
            }

            // Fill the page with some pattern (e.g., 'A')
            memset(page, 'A', PAGE_SIZE);

            struct uffdio_copy uffdio_copy;
            uffdio_copy.src = (unsigned long) page;
            uffdio_copy.dst = msg.arg.pagefault.address & ~(PAGE_SIZE - 1);
            uffdio_copy.len = PAGE_SIZE;
            uffdio_copy.mode = 0;
            uffdio_copy.copy = 0;

            if (ioctl(uffd, UFFDIO_COPY, &uffdio_copy) == -1) {
                perror("ioctl-UFFDIO_COPY");
                exit(EXIT_FAILURE);
            }

            printf("Handled page fault at address: %p\n", (void*)msg.arg.pagefault.address);
        }
    }

    free(page);
    return NULL;
}

int main() {
    uffd = syscall(SYS_userfaultfd, O_CLOEXEC | O_NONBLOCK);
    if (uffd == -1) {
        perror("userfaultfd");
        exit(EXIT_FAILURE);
    }

    struct uffdio_api uffdio_api = {
        .api = UFFD_API,
        .features = 0,
    };

    if (ioctl(uffd, UFFDIO_API, &uffdio_api) == -1) {
        perror("ioctl-UFFDIO_API");
        exit(EXIT_FAILURE);
    }

    region = mmap(NULL, REGION_SIZE, PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (region == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("Mapped region at %p\n", region);

    struct uffdio_register uffdio_register = {
        .range = {
            .start = (unsigned long) region,
            .len = REGION_SIZE
        },
        .mode = UFFDIO_REGISTER_MODE_MISSING
    };

    if (ioctl(uffd, UFFDIO_REGISTER, &uffdio_register) == -1) {
        perror("ioctl-UFFDIO_REGISTER");
        exit(EXIT_FAILURE);
    }

    pthread_t thr;
    if (pthread_create(&thr, NULL, pagefault_handler_thread, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // Trigger a page fault by accessing the region
    printf("Triggering page fault...\n");
    printf("First byte: %c\n", ((char*)region)[0]);

    // Clean up
    pthread_join(thr, NULL);
    munmap(region, REGION_SIZE);
    close(uffd);

    return 0;
}

