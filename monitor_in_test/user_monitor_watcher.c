#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

#define STATUS_PATH "/sys/class/drm/"
#define PROC_FILE "/proc/monitor_status"

int is_monitor_connected() {
    DIR *dir;
    struct dirent *entry;
    char path[512], status[32];
    FILE *fp;

    dir = opendir(STATUS_PATH);
    if (!dir) {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip non-display entries and internal display (eDP)
        if (strstr(entry->d_name, "-") && !strstr(entry->d_name, "eDP")) {
            snprintf(path, sizeof(path), "%s%s/status", STATUS_PATH, entry->d_name);
            fp = fopen(path, "r");
            if (fp) {
                if (fgets(status, sizeof(status), fp)) {
                    fclose(fp);
                    closedir(dir);
                    return (strncmp(status, "connected", 9) == 0);
                }
                fclose(fp);
            }
        }
    }
    closedir(dir);
    return 0;
}

int main() {
    int prev = -1;

    while (1) {
        int connected = is_monitor_connected();
        if (connected != prev) {
            FILE *f = fopen(PROC_FILE, "w");
            if (f) {
                fprintf(f, "%s\n", connected ? "connected" : "disconnected");
                fclose(f);
            } else {
                perror("fopen /proc/monitor_status");
            }
            prev = connected;
        }
        sleep(2);
    }

    return 0;
}
