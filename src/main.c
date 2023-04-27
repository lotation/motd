#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/utsname.h>

#include "motd.h"
#include "ip.h"
#include "pkg.h"


static void greeting(void);
static void systeminfo(void);
static void fsuse(void);
static void goodbye(void);

int main(void)
{
    errno = 0;

    greeting();
    systeminfo();
    fsuse();
    goodbye();
    
    return 0;
}

static void greeting(void)
{
    char *username = getenv("USER");
    char *time_string = get_datetime();

    printf("%s\n\n", time_string);
    printf("Welcome back, " COLOR_ON "%s" RESET "\n\n", username);

    free(time_string);
}


static void systeminfo(void)
{
    struct utsname info;
    uname(&info);

    char *kernel = info.release;
    char *distro = get_distro();

    // TODO
    // System load
    // CPU
    // MEM

    // IP address + hostname
    char *ip_addr;
    char *public = get_public_ip();
    struct net_info_s *local = get_local_ip();

    if (strncmp(local->ip, "127", 3 * sizeof(char)) == 0) {
        ip_addr = strdup(public);
    } else {
        ip_addr = MANY(char, IP_STR);

        snprintf(ip_addr, IP_STR, "%s  (%s)", local->ip, public);
    }

    int pkgs     = get_pkg_num(NATIVE);
    int aur_pkgs = get_pkg_num(FOREIGN);

    /*
    int upgrades     = atoi(pipe_of("checkupdates", "wc -l", BSIZE));
    int aur_upgrades = atoi(pipe_of("checkupdates-aur", "wc -l", BSIZE));

    if (upgrades > 0) {
        printf(COLOR_ON "Packages" RESET":\t %d\t(" COLOR_ON "%d" RESET " upgradable)\tpacman\n", pkgs, upgrades);
    } else {
        printf(COLOR_ON "Packages" RESET":\t %d pacman\n", pkgs);
    }
    if (aur_upgrades > 0) {
        printf("\t\t\t %d\t(" COLOR_ON "%d" RESET " upgradable)\tAUR\n", aur_pkgs, aur_upgrades);
    } else {
        printf("\t\t\t %d AUR\n", aur_pkgs);
    }
    */

    printf(COLOR_ON "Distro"     RESET ":\t \t %s\n", distro);
    printf(COLOR_ON "Kernel"     RESET ":\t \t %s\n", kernel);
    printf(COLOR_ON "Hostname  " RESET ":\t %s\n", local->host);
    printf(COLOR_ON "IP Address" RESET ":\t %s\n", ip_addr);
    printf(COLOR_ON "Packages  " RESET ":\t %d pacman  (%d AUR)\n", pkgs, aur_pkgs);
    printf("\n");

    free(distro);
    free(public);
    free(local);
    free(ip_addr);
}


static void fsuse(void)
{
    char **fs_path = get_fs_mountpoint();

    printf(COLOR_ON "Filesystem Usage" RESET ":\n");
    for (size_t i = 0; fs_path[i] != NULL; i++) {
        struct fsinfo_s fsinfo = get_fs_info(fs_path[i]);
        printfs(fs_path[i], fsinfo);
        putchar(10);
    }
}

static void goodbye(void)
{
    printf("\n\n");

    //char *docs = get_documentation();
    printf("Remember the bible: " COLOR_ON "https://wiki.archlinux.org/" RESET "\n\n");

    // TODO
    // ssh-like last login
}
