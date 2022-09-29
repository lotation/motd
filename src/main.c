#include "motd.h"
#include "ip.h"
#include "pkg.h"

int main(void)
{
    greeting();
    systeminfo();
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


void systeminfo(void)
{
    struct utsname info;
    uname(&info);

    char *kernel = info.release;
    char *distro = get_distro();

    // System load
    // CPU
    // MEM
    // ...

    // IP address + hostname
    char *ip_addr;
    char *public = get_public_ip();
    net_info *local = get_local_ip();

    if (strncmp(local->ip, "127", 3 * sizeof(char)) == 0) {
        ip_addr = strdup(public);
    } else {
        ip_addr = (char *) calloc(IP_STR, sizeof(char));
        MCHECK(ip_addr);

        snprintf(ip_addr, IP_STR, "%s  (%s)", local->ip, public);
    }

    // Badly written attempt to mimic pacman
    int pkgs     = get_pkg_num(NATIVE);
    int aur_pkgs = get_pkg_num(FOREIGN);

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

    printf(COLOR_MAGENTA "Distro"     COLOR_RESET ":\t \t %s\n", distro);
    printf(COLOR_MAGENTA "Kernel"     COLOR_RESET ":\t \t %s\n", kernel);
    printf(COLOR_MAGENTA "Hostname  " COLOR_RESET ":\t %s\n", local->host);
    printf(COLOR_MAGENTA "IP Address" COLOR_RESET ":\t %s\n", ip_addr);
    printf(COLOR_MAGENTA "Packages  " COLOR_RESET ":\t %d pacman  (%d AUR)\n", pkgs, aur_pkgs);
    printf("\n");

    free(distro);
    free(public);
    free(local);
    free(ip_addr);
}


void fsuse(void)
{
    // Grep info on the filesystems
    char **fs_path = get_fs_mountpoint();

    printf(COLOR_MAGENTA "Filesystem Usage" COLOR_RESET ":\n");
    for (size_t i = 0; fs_path[i] != NULL; i++) {
        fsinfo_t fsinfo = get_fs_info(fs_path[i]);
        printfs(fs_path[i], fsinfo);
        putchar(10);
    }
}

void goodbye(void)
{
    printf("\n\n");

    //char *docs = get_documentation();
    printf("Remember the bible: " COLOR_MAGENTA "https://wiki.archlinux.org/" COLOR_RESET "\n\n");

    // TODO
    // ssh-like last login
}
