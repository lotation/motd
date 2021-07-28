#ifndef MOTD_H
#define MOTD_H

#include <stdio.h>         // basic I/O
#include <stdlib.h>        // malloc, free 
#include <string.h>        // strings utils
#include <time.h>          // time utilities
#include <unistd.h>        // POSIX api
#include <sys/statvfs.h>   // filesystem stats
#include <sys/utsname.h>   // system relese, version, etc
#include <sys/wait.h>      // wait, waitpid

#define STR_SIZE 76  // => length of time_string
#define BSIZE     6  // => 99 999 (10^5-1) packages in total

#define MCHECK(ptr)  if (ptr == NULL) { \
                        fprintf(stderr, "Could not allocate memory.\n"); \
                        exit(EXIT_FAILURE); \
                    } \


void greeting(void);
void sysinfo(void);
void fsuse(void);
void goodbye(void);

void get_datetime(char *time_string);
char *get_distro(void);

char *pipe_of(char *op1, char *op2);
void strsplit(char *str, char *str_arr[]);


#endif