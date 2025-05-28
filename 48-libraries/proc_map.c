#include <stdio.h>
#include <stdlib.h>

#define MAPS_FILE "/proc/self/maps"

int main() {
    FILE *file = fopen(MAPS_FILE, "r");
    if (!file) {
        perror("Error opening " MAPS_FILE);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    if (ferror(file)) {
        perror("Error reading " MAPS_FILE);
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}
