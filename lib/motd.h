#ifndef MOTD_H
#define MOTD_H

#define _DEFAULT_SOURCE

#include <stdio.h>         // basic I/O
#include <stdlib.h>        // malloc, free, exit
#include <string.h>        // strings utils
#include <time.h>          // time utilities

#include <unistd.h>        // POSIX API
#include <sys/sysinfo.h>   // system uptime
#include <sys/statvfs.h>   // filesystem stats
#include <sys/utsname.h>   // system relese, version, etc
#include <sys/wait.h>      // wait, waitpid

#define STR_SIZE  86  // => length of time_string
#define DISTRO    64  // => max line length of /etc/os-release
#define LINE     192  // => max line length of /proc/mounts
#define MAX_NAME   8  // => max name length

#define COLOR_MAGENTA "\033[38;5;197m"
#define COLOR_RESET   "\033[0m" //"\x1b[0m"

#define MCHECK(ptr)  if (ptr == NULL) { \
                        fprintf(stderr, "Could not allocate memory.\n"); \
                        exit(EXIT_FAILURE); \
                    }


typedef struct __fsinfo_t {
    double total;
    double free;
    double used;
    double free_percent;
    double used_percent;
} fsinfo_t;

typedef struct __dirlist_t {
    size_t index;
    size_t size;
    char **field;
} dirlist_t;

char *get_datetime(void);
char *get_distro(void);

char *pipe_of(const char op1[], const char op2[], int buffer_size);
//static void strsplit(char *str, char *str_arr[]);

fsinfo_t get_fs_info(const char *path);
void* printfs(const char* name, fsinfo_t fs);
char **get_fs_mountpoint(void);
char *get_uptime(void);


#endif
