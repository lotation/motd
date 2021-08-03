#include "../lib/motd.h"

bool is_in_string(const char *target, const char *string) {
    size_t i, j, foo_i;

    for (i = 0; i < strlen(target); i++) {
        j = 0; foo_i = i;
        while (target[foo_i] == string[j] && j < strlen(string)) {      
            if (j == (strlen(string) - 1))
                return true;

            j++;
            foo_i++;
        }
    }

    return false;
}

void get_datetime(char *time_string) {
    time_t current_time = time(NULL);
    if (current_time == ((time_t) - 1)) {
        (void) fprintf(stderr, "Failed to obtain current time.\n");
        exit(EXIT_FAILURE);
    }
    
    struct tm *time_info;    
    time_info = localtime(&current_time);
    
    //                              " DAY NUM MON 	                       -                        	HOUR:MIN "
    strftime(time_string, STR_SIZE, "[ %a %d %b  %t                       -                        %t %R ]", time_info);
}

char *get_distro(void) {
    char filename[] = "/etc/lsb-release";
    char *name = (char *) calloc(1, sizeof(char));
    char *buffer = (char *) calloc(DISTRO, sizeof(char));

    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, DISTRO, fp)) {
        // Remove the new line
        buffer[strlen(buffer) - 1] = '\0';

        if (is_in_string(buffer, "DESCRIPTION")) {
            char *tmp = strdup(buffer);
            char *token = strtok(tmp, "=");

            token = strtok(NULL, "\"");

            // Save distro name
            name = (char *) realloc(name, (strlen(token) + 1) * sizeof(char));
            strcpy(name, token);

            free(tmp);
        }
    }

    free(buffer);
    fclose(fp);

    return name;
}

void printfs(const char *name, fsinfo fs) {
    if (strcmp(name, "/") == 0) {
        printf(COLOR_MAGENTA "FS Usage" COLOR_RESET ":       ");
    } else {
        printf("\t \t");
    }

    printf("%s %s", name, (strcmp(name, "/media/DATA") == 0) ? "" : "\t    ");
    printf("[");
    for (int i = 1; i <= 50; i++) {
       if (i >= (fs.used_percent/2)) {
           printf("%c", '=');
       } else {
           printf(COLOR_MAGENTA "%c" COLOR_RESET, '=');
       } 
    }
    printf("]\n");
}

fsinfo get_fs_info(const char *path) {
    struct statvfs stat;
    struct fsinfo fs_info;

    if (statvfs(path, &stat) != 0) {
        fprintf(stderr, "Failed to obtain filesystem usage.\n");
        exit(EXIT_FAILURE);
    }

    double gb = 1024 * 1024 * 1024;
    fs_info.total = (double)(stat.f_blocks * stat.f_frsize) / gb; 
    fs_info.free  = (double)(stat.f_bfree  * stat.f_frsize) / gb;
    fs_info.used = fs_info.total - fs_info.free;
    fs_info.free_percent = fs_info.free * 100.00 / fs_info.total;
    fs_info.used_percent = fs_info.used * 100.00 / fs_info.total;

    return fs_info;
}

char **get_fs_mountpoint(void) {
    char **sysfs = (char **) calloc(1, sizeof(char *));
    char filename[] = "/proc/mounts";
    char *buffer = (char *) calloc(LINE, sizeof(char));

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not open %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (fgets(buffer, LINE, fp)) {
        buffer[strlen(buffer)-1] = '\0';

        if (is_in_string(buffer, "/dev/sd") || is_in_string(buffer, "/dev/nvme")) {
            char *tmp = strdup(buffer);
            char *token = strtok(tmp, " ");

            // Skip the first token since here it's not needed
            token = strtok(NULL, " ");

            // Add a new string
            sysfs = (char **) realloc(sysfs, (sizeof(*sysfs) + 1) * sizeof(char *));
            sysfs[i] = (char *) calloc(strlen(token) + 1, sizeof(char));
            strcpy(sysfs[i], token);

            i++;
            free(tmp);
        }
    }

    fclose(fp);

    return sysfs;
}