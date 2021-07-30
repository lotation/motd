#include "../lib/motd.h"


int main(void) {

    greeting();
    sysinfo();
    fsuse();
    goodbye();
    
    return 0;
}

void greeting(void) {
    char *username;
    char *time_string = (char *) calloc(STR_SIZE, sizeof(char));  MCHECK(time_string)

    get_datetime(time_string);
    username = getlogin();

    printf("%s\n\n", time_string);
    printf("Welcome back, " COLOR_MAGENTA "%s" COLOR_RESET "\n\n", username);

    free(time_string);
}


void sysinfo(void) {
    struct utsname info;
    uname(&info);

    char *kernel = info.release;
    char *distro = get_distro();

    // Badly written attempt to mimic the bash pipe
    int upgrades = atoi(pipe_of("checkupdates 2>/dev/null", "wc -l", 6));
    int pkgs = atoi(pipe_of("pacman -Q", "wc -l", 6));
    int aur_pkgs = atoi(pipe_of("pacman -Qm", "wc -l", 6));


    printf(COLOR_MAGENTA "Distro" COLOR_RESET ":\t \t %s Linux\n", distro);
    printf(COLOR_MAGENTA "Kernel" COLOR_RESET ":\t \t %s\n", kernel);
    printf(COLOR_MAGENTA "Packages" COLOR_RESET":\t %d  (" COLOR_MAGENTA "%d" COLOR_RESET " upgradable)\n", pkgs + aur_pkgs, upgrades);
    printf("\n");

}


void fsuse(void) {
    // Grep info on the filesystems
    char root[] = "/";
    char home[] = "/home";
    //char data[] = "/media/DATA";

    struct fsinfo fs_root = get_fs_info(root);
    struct fsinfo fs_home = get_fs_info(home);
    //struct fsinfo fs_data = get_fs_info(data);

    printfs(root, fs_root);
    printfs(home, fs_home);
    //printfs(data, fs_data);
    
}

void goodbye(void) {
    printf("\n\n\n");
    printf("Remember the bible: " COLOR_MAGENTA "https://wiki.archlinux.org/" COLOR_RESET "\n\n");
    printf("%s\n\n", pipe_of("echo", "fortune -n 72 -sae", STR_SIZE));
}