#include "motd.h"
#include "ip.h"
#include "packages.h"


int main(void)
{
    greeting();
    sysinfo();
    fsuse();
    goodbye();
    
    return 0;
}

void greeting(void)
{
    /* char *username = getlogin(); */
    char *username = getenv("USER");
    char *time_string = get_datetime();

    printf("%s\n\n", time_string);
    printf("Welcome back, " COLOR_MAGENTA "%s" COLOR_RESET "\n\n", username);

    free(time_string);
}


void sysinfo(void)
{
    struct utsname info;
    uname(&info);

    char *kernel = info.release;
    char *distro = get_distro();

    // IP address + hostname
    char *public = get_public_ip();
    net_info *local = get_local_ip();

    // Badly written attempt to mimic pacman
    int pkgs     = pkgs_num(NATIVE);
    int aur_pkgs = pkgs_num(FOREIGN);

    /*
    int upgrades     = atoi(pipe_of("checkupdates", "wc -l", BSIZE));
    int aur_upgrades = atoi(pipe_of("checkupdates-aur", "wc -l", BSIZE));

    if (upgrades > 0) {
        printf(COLOR_MAGENTA "Packages" COLOR_RESET":\t %d\t(" COLOR_MAGENTA "%d" COLOR_RESET " upgradable)\tpacman\n", pkgs, upgrades);
    } else {
        printf(COLOR_MAGENTA "Packages" COLOR_RESET":\t %d pacman\n", pkgs);
    }
    if (aur_upgrades > 0) {
        printf("\t\t\t %d\t(" COLOR_MAGENTA "%d" COLOR_RESET " upgradable)\tAUR\n", aur_pkgs, aur_upgrades);
    } else {
        printf("\t\t\t %d AUR\n", aur_pkgs);
    }
    */

    printf(COLOR_MAGENTA "Distro" COLOR_RESET ":\t \t %s\n", distro);
    printf(COLOR_MAGENTA "Kernel" COLOR_RESET ":\t \t %s\n", kernel);
    printf(COLOR_MAGENTA "Hostname" COLOR_RESET ":\t\t%s\n", local->host);
    printf(COLOR_MAGENTA "IP Address" COLOR_RESET ":\t\t%s\t(%s)\n", local->ip, public);
    printf(COLOR_MAGENTA "Packages" COLOR_RESET":\t %hu pacman\n", pkgs);
    printf("\t\t\t %hu AUR\n", aur_pkgs);
    printf("\n");

    free(distro);
    free(public);
    free(local);
}


void fsuse(void)
{
    // Grep info on the filesystems
    char **system_fs = get_fs_mountpoint();

    printf(COLOR_MAGENTA "Filesystem Usage" COLOR_RESET ":\n");
    for (int i = 0; system_fs[i] != NULL; i++) {
        fs_info fsinfo = get_fs_info(system_fs[i]);

        printf("%s\n", print_fs(system_fs[i], fsinfo));
    }
}

void goodbye(void)
{
    printf("\n\n");

    printf("Remember the bible: " COLOR_MAGENTA "https://wiki.archlinux.org/" COLOR_RESET "\n\n");

    // TODO
    // ssh-like last login
}
