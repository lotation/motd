#include "../lib/motd.h"

void str_alloc(char *str) {
    str = (char *) malloc(STR_SIZE * sizeof(char));
    if (str == NULL) {
        (void) fprintf(stderr, "Failed to allocate memory for datetime.\n");
        exit(EXIT_FAILURE);
    }
}

long get_available_space(const char *path) {
    // file's absolute path
    struct statvfs stat;

    if (statvfs(path, &stat) != 0) {
        (void) fprintf(stderr, "Failed to obtain filesystem usage.\n");
        exit(EXIT_FAILURE);
    }

    return (stat.f_bsize * stat.f_bavail);
}

void get_datetime(char *time_string) {
    
    time_t current_time = time(NULL);
    if (current_time == ((time_t) - 1)) {
        (void) fprintf(stderr, "Failed to obtain current time.\n");
        exit(EXIT_FAILURE);
    }
    
    struct tm *time_info;    
    time_info = localtime(&current_time);
    
    // " DAY NUM MON 	                       -                        	HOUR:MIN "
    strftime(time_string, STR_SIZE, "[ %a %d %b  %t                       -                        %t %R ]", time_info);
}

void get_distro_name(char *name) {
    char index;
    FILE *fp;

    fp = fopen("/etc/os-release", "r");
    if (fp == NULL) {
        (void) fprintf(stderr, "Failed to open /etc/os-release file.\n");
        exit(EXIT_FAILURE);
    }

    while ((index = fgetc(fp))  != EOF) {
        fscanf(fp, "NAME=\"%s\"", name);
    }

    fclose(fp);
}