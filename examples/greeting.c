#include <stdio.h>         // printf
#include <stdlib.h>        // malloc, free 
#include <string.h>        // 
#include <time.h>          // time utilities
#include <unistd.h>        // POSIX api
#include <sys/statvfs.h>   // filesystem stats
#include <sys/utsname.h>   // system relese, version, etc...

#include "../lib/motd.h"

#define STR_SIZE 76

int main(void) {

    greeting();
    
    return 0;
}

void greeting(void) {
    char *time_string = (char *) calloc(STR_SIZE, sizeof(char));
    char *username;

    get_datetime(time_string);
    username = getlogin();

    printf("%s\n\n", time_string);
    printf("Welcome back, %s\n\n", username);

    free(time_string);
}