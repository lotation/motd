#include <stdio.h>

#include "../lib/motd.h"


int main(void) {

    greeting();
    //sysinfo();
    //fsuse();
    //goodbye();
    
    return 0;
}

void greeting(void) {
    char *username;
    char *time_string = (char *) calloc(STR_SIZE, sizeof(char));
    MCHECK(time_string)

    get_datetime(time_string);
    username = getlogin();

    printf("%s\n\n", time_string);
    printf("Welcome back, %s\n\n", username);

    free(time_string);
}

/*
void sysinfo(void) {
    char *distro;
    char *kernel;
    //int packages;
    struct utsname info;

    str_alloc(distro);
    str_alloc(kernel);

    uname(&info);
    kernel = info.release;

    get_distro_name(distro);
}

void fsuse(void) {
    long int root_fs_info;
        
    root_fs_info = get_available_space("/home/lotation");

    printf("FS Usage:\n%li\n", root_fs_info);
}

void goodbye(void) {
    printf("Goodbye.\n");
}
*/