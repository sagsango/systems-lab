#include <systemd/sd-journal.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Log messages with different priorities
    sd_journal_print(LOG_INFO, "MyService started");
    sd_journal_print(LOG_DEBUG, "Debug: Initializing service...");

    // Log with structured data
    sd_journal_send("MESSAGE=Service is running",
                    "PRIORITY=%i", LOG_INFO,
                    "CUSTOM_FIELD=%s", "example_value",
                    NULL);

    // Main loop
    while (1) {
        sd_journal_print(LOG_INFO, "Service is still alive");
        sleep(5); // Log every 5 seconds
    }

    sd_journal_print(LOG_INFO, "MyService stopped");
    return 0;
}
