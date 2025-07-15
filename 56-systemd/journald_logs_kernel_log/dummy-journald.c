// dummy-journald.c: A minimal program to collect kernel logs from /dev/kmsg and send them to systemd-journald
#include <systemd/sd-journal.h> // For journald logging API
#include <fcntl.h>             // For open(), O_RDONLY
#include <unistd.h>            // For read(), close()
#include <string.h>            // For strlen(), strstr()
#include <stdio.h>             // For perror()
#include <errno.h>             // For errno

// Buffer size for reading /dev/kmsg messages (sufficient for most kernel messages)
#define BUFFER_SIZE 8192

int main(int argc, char *argv[]) {
    // File descriptor for /dev/kmsg
    int kmsg_fd;
    // Buffer to store kernel messages
    char buffer[BUFFER_SIZE];
    // Number of bytes read
    ssize_t n;

    // Log startup message to journald
    sd_journal_print(LOG_INFO, "dummy-journald started: Collecting kernel logs from /dev/kmsg");

    /*
     * XXX:
     *	Here all the magic happens;
     *	kernel logs are written into ring buffer
     *	read end is available through the device /dev/kmsg
     */

    // Open /dev/kmsg for reading kernel messages
    // O_RDONLY ensures we only read from the device
    kmsg_fd = open("/dev/kmsg", O_RDONLY | O_NONBLOCK);
    if (kmsg_fd < 0) {
        sd_journal_print(LOG_ERR, "Failed to open /dev/kmsg: %s", strerror(errno));
        return 1;
    }

    // Main loop to read kernel messages
    while (1) {
        // Read a message from /dev/kmsg
        n = read(kmsg_fd, buffer, BUFFER_SIZE - 1);
        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No data available, wait briefly and retry
                usleep(100000); // Sleep 100ms
                continue;
            }
            sd_journal_print(LOG_ERR, "Failed to read /dev/kmsg: %s", strerror(errno));
            close(kmsg_fd);
            return 1;
        } else if (n == 0) {
            // No data, wait and retry
            usleep(100000);
            continue;
        }

        // Null-terminate the buffer for string operations
        buffer[n] = '\0';

        // Parse the message to extract priority and content
        // /dev/kmsg format: <priority>message (e.g., "<6>kernel: Hello")
        int priority = LOG_INFO; // Default priority
        char *msg_start = buffer;
        if (buffer[0] == '<') {
            // Extract priority (e.g., "<6>")
            char *end = strstr(buffer, ">");
            if (end) {
                char level = buffer[1];
                if (level >= '0' && level <= '7') {
                    priority = level - '0'; // Convert ASCII to integer (0=LOG_EMERG, 7=LOG_DEBUG)
                }
                msg_start = end + 1; // Skip past "<6>"
            }
        }

        // Send the message to journald with kernel-specific metadata
        sd_journal_send("MESSAGE=%s", msg_start,
                        "PRIORITY=%i", priority,
                        "_TRANSPORT=kernel",
                        "CUSTOM_FIELD=dummy-journald",
                        NULL);

        // Log to journald that we processed a message (for debugging)
        sd_journal_print(LOG_DEBUG, "Processed kernel message: %s", msg_start);
    }

    // Cleanup (unreachable due to infinite loop, but included for completeness)
    sd_journal_print(LOG_INFO, "dummy-journald stopped");
    close(kmsg_fd);
    return 0;
}
