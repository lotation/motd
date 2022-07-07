#include "motd.h"


char *get_datetime(void)
{
    char *time_string = (char *) calloc(STR_SIZE, sizeof(char));
    MCHECK(time_string);

    time_t current_time = time(NULL);
    if (current_time == ((time_t) - 1)) {
        fprintf(stderr, "Failed to obtain current time.\n");
        exit(EXIT_FAILURE);
    }

    struct tm *time_info;
    time_info = localtime(&current_time);

    //                              " DAY NUM MON 	                       -                        	HOUR:MIN "
    strftime(time_string, STR_SIZE, "[ %a %d %b  %t                       -                        %t %R ]", time_info);

    return time_string;
}

char *get_distro(void)
{
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

        if (strstr(buffer, "DESCRIPTION")) {
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

char *print_fs(const char *name, fsinfo_t fs)
{
    char *res = (char *) calloc(STR_SIZE, sizeof(char));
    MCHECK(res);
    char spacing[MAX_NAME];
    spacing[0] = '\t';

    if (strcmp(name, "/") == 0) {
        strcat(spacing, "\t\t");
    } else if (strlen(name) < MAX_NAME) {
        strcat(spacing, "\t");
    }

    snprintf(res, STR_SIZE, "\t\t\t%s%s[", name, spacing);

    for (int i = 1; i <= 50; i++) {
        if (i >= (fs.used_percent/2)) {
            strcat(res, "=");
        } else {
            snprintf(res, STR_SIZE, "%c" COLOR_RESET, '=');
        }
    }
    strcat(res, "]");

    return (char *) realloc(res, (strlen(res) + 1) * sizeof(char));
}

fsinfo_t get_fs_info(const char *path)
{
    struct statvfs stat;
    fsinfo_t fsinfo;

    if (statvfs(path, &stat) != 0) {
        fprintf(stderr, "Failed to obtain filesystem usage.\n");
        exit(EXIT_FAILURE);
    }

    double gb = 1024 * 1024 * 1024;
    fsinfo.total = (double)(stat.f_blocks * stat.f_frsize) / gb;
    fsinfo.free  = (double)(stat.f_bfree * stat.f_frsize) / gb;
    fsinfo.used = fsinfo.total - fsinfo.free;
    fsinfo.free_percent = fsinfo.free * 100.00 / fsinfo.total;
    fsinfo.used_percent = fsinfo.used * 100.00 / fsinfo.total;

    return fsinfo;
}

char **get_fs_mountpoint(void)
{
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

        if (strstr(buffer, "/dev/sd") || strstr(buffer, "/dev/nvme") || strstr(buffer, "/dev/mmc")) {
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
    // Use NULL as terminator
    sysfs[i] = NULL;

    fclose(fp);

    return sysfs;
}

void strsplit(char *str, char *str_arr[]) {
    char *token = strtok(str, " ");
    int i;

    /* For each word in str_arr (as is using space as delimiter)
     * creates a new value in the string array and copies token in it */
    for (i = 0; token != NULL; i++) {
        str_arr = (char **) realloc(str_arr, (sizeof(*str_arr) + 1) * sizeof(char *));
        MCHECK(str_arr);
        str_arr[i] = (char *) calloc(strlen(token) + 1, sizeof(char));
        MCHECK(str_arr[i]);
        strcpy(str_arr[i], token);
        token = strtok(NULL, " ");
    }

    // Set the last element of the array as NULL
    str_arr = (char **) realloc(str_arr, (sizeof(*str_arr) + 1) * sizeof(char *));
    MCHECK(str_arr);
    str_arr[i] = NULL;
}
