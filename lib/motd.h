/**
 * @file motd.h
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
 * This file has motd functions declaration
 * 
 */
#ifndef MOTD_H
#define MOTD_H

#include <stdio.h>         /// basic I/O
#include <stdlib.h>        /// malloc, free
#include <stdbool.h>       /// true, false, bool type
#include <string.h>        /// strings utils
#include <time.h>          /// time utilities
#include <unistd.h>        /// POSIX api
#include <sys/statvfs.h>   /// filesystem stats
#include <sys/utsname.h>   /// system relese, version, etc
#include <sys/wait.h>      /// wait, waitpid
#include <errno.h>         /// error numbers

#define STR_SIZE  80  ///< length of time_string
#define BSIZE      6  ///< 99 999 (10^5-1) packages in total
#define DISTRO    40  ///< max line length of /etc/lsb-release
#define LINE     192  ///< max line length of /proc/mounts

#define COLOR_MAGENTA "\033[38;5;197m"      ///< Color Magenta
#define COLOR_RESET   "\033[0m" //"\x1b[0m" ///< Reset Color

#define MCHECK(ptr)  if (ptr == NULL) { \
                        fprintf(stderr, "Could not allocate memory.\n"); \
                        exit(EXIT_FAILURE); \
                    } \


typedef struct fsinfo {
    double total;
    double free;
    double used;
    double free_percent;
    double used_percent;
} fsinfo;

// Main functions
void greeting(void);
void sysinfo(void);
void fsuse(void);
void goodbye(void);

// Motd functions
void get_datetime(char *time_string);
char *get_distro(void);
fsinfo get_fs_info(const char *path);
void printfs(const char *name, fsinfo fs);
char **get_fs_mountpoint(void);

// Pipe functions
char *pipe_of(const char op1[], const char op2[], int buffer_size);
void strsplit(char *str, char *str_arr[]);

// ALPM Functions
int pkgs_num(void);
int aur_pkgs_num(void);


#endif
