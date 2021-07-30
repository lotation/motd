#include "../lib/motd.h"

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
    char *name = (char *) calloc(STR_SIZE, sizeof(char));  MCHECK(name)
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
        (void) fprintf(stderr, "Failed to obtain filesystem usage.\n");
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