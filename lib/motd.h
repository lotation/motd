#ifndef MOTD_H
#define MOTD_H

#include <stdio.h>         // basic I/O
#include <stdlib.h>        // malloc, free
#include <stdbool.h>       // true, false, bool type
#include <string.h>        // strings utils
#include <time.h>          // time utilities
#include <unistd.h>        // POSIX api
#include <sys/statvfs.h>   // filesystem stats
#include <sys/utsname.h>   // system relese, version, etc
#include <sys/wait.h>      // wait, waitpid

#define STR_SIZE  80  // => length of time_string
#define BSIZE      6  // => 99 999 (10^5-1) packages in total
#define DISTRO    40  // => max line length of /etc/lsb-release
#define LINE     128  // => max line length of /proc/mounts

#define COLOR_MAGENTA "\033[38;5;197m"
#define COLOR_RESET   "\033[0m" //"\x1b[0m"

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

void greeting(void);
void sysinfo(void);
void fsuse(void);
void goodbye(void);

bool is_in_string(const char *target, const char *string);
void get_datetime(char *time_string);
char *get_distro(void);

char *pipe_of(const char op1[], const char op2[], int buffer_size);
void strsplit(char *str, char *str_arr[]);

fsinfo get_fs_info(const char *path);
void printfs(const char *name, fsinfo fs);
char **get_fs_mountpoint(void);


#endif