#ifndef MOTD_H
#define MOTD_H

#include <stdio.h>         // basic I/O
#include <stdlib.h>        // malloc, free, exit
#include <stdbool.h>       // true, false, bool type
#include <string.h>        // strings utils
#include <time.h>          // time utilities

#include <unistd.h>        // POSIX API
#include <sys/statvfs.h>   // filesystem stats
#include <sys/utsname.h>   // system relese, version, etc
#include <sys/wait.h>      // wait, waitpid

#define STR_SIZE  80  // => length of time_string
#define DISTRO    40  // => max line length of /etc/lsb-release
#define LINE     192  // => max line length of /proc/mounts
#define MAX_NAME   8  // => max name length

#define COLOR_MAGENTA "\033[38;5;197m"
#define COLOR_RESET   "\033[0m" //"\x1b[0m"

#define MCHECK(ptr)  if (ptr == NULL) { \
                        fprintf(stderr, "Could not allocate memory.\n"); \
                        exit(EXIT_FAILURE); \
                    }


typedef struct fsinfo_ {
    double total;
    double free;
    double used;
    double free_percent;
    double used_percent;
} fs_info;

void greeting(void);
void sysinfo(void);
void fsuse(void);
void goodbye(void);

char *get_datetime(void);
char *get_distro(void);

char *pipe_of(const char op1[], const char op2[], int buffer_size);
void strsplit(char *str, char *str_arr[]);

fs_info get_fs_info(const char *path);
char *print_fs(const char *name, fs_info fs);
char **get_fs_mountpoint(void);


#endif
