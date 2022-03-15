/**
 * @file motd.c
 * @author lotation
 * @brief 
 * @version 0.2
 * @date 2022-03-15
 * 
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This file has motd function definition
 * 
 */

#include "motd.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Get system datetime
 * 
 * @param time_string formatted string with datetime to display in the first row
 */
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

/**
 * @brief Get distribution name
 * 
 * @return char* name of the distro
 */
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

/**
 * @brief Helps printing filesystem usage
 * 
 * @param name of the partition (it's typically the mountpoint)
 * @param fs struct with filesystem informations
 */
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

/**
 * @brief Get filesystem informations 
 * 
 * @param path 
 * @return fsinfo 
 */
fsinfo get_fs_info(const char *path) {
    struct statvfs stat;
    struct fsinfo fs_info;

    if (statvfs(path, &stat) != 0) {
        //fprintf(stderr, "Failed to obtain filesystem usage.\n");
        perror("Failed to obtain filesystem usage.\n");
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

/**
 * @brief Get the filesystems mountpoint 
 * 
 * @return char** strings array with mountpoints
 */
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

        if (strstr(buffer, "/dev/sd") || strstr(buffer, "/dev/nvme") || strstr(buffer, "/dev/mmc")) {
        //if(strstr(buffer, "/dev/nvme")) {
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
