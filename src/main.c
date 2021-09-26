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
    int pkgs     = atoi(pipe_of("pacman -Q", "wc -l", BSIZE));
    int aur_pkgs = atoi(pipe_of("pacman -Qm", "wc -l", BSIZE));
    int upgrades     = atoi(pipe_of("checkupdates", "wc -l", BSIZE));
    int aur_upgrades = atoi(pipe_of("checkupdates-aur", "wc -l", BSIZE));


    printf(COLOR_MAGENTA "Distro" COLOR_RESET ":\t \t %s\n", distro);
    printf(COLOR_MAGENTA "Kernel" COLOR_RESET ":\t \t %s\n", kernel);
    
    if (upgrades > 0) {
        printf(COLOR_MAGENTA "Packages" COLOR_RESET":\t %d\t(" COLOR_MAGENTA "%d" COLOR_RESET " upgradable)\tpacman\n", pkgs, upgrades);
    } else {
        printf(COLOR_MAGENTA "Packages" COLOR_RESET":\t %d pacman\n", pkgs);
    }
    if (aur_upgrades > 2) {
        printf(                                   "\t\t %d\t(" COLOR_MAGENTA "%d" COLOR_RESET " upgradable)\tAUR\n", aur_pkgs, aur_upgrades);
    } else {
        printf(                                   "\t\t %d AUR\n", aur_pkgs);
    }
    printf("\n");

    free(distro);
}


void fsuse(void) {
    // Grep info on the filesystems
    char **system_fs = get_fs_mountpoint();
    
    for (int i = 0; system_fs[i] != NULL; i++) {
        struct fsinfo fs_info = get_fs_info(system_fs[i]);

        printfs(system_fs[i], fs_info);
    }
}

void goodbye(void) {
    printf("\n\n\n");
    printf("Remember the bible: " COLOR_MAGENTA "https://wiki.archlinux.org/" COLOR_RESET "\n\n");
    printf("%s\n\n", pipe_of("echo", "fortune -n 72 -sae", STR_SIZE));
}
