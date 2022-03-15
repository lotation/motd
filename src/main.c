/**
 * @file main.c
 * @author lotation
 * @brief 
 * @version 0.2
 * @date 2022-03-15
 * 
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Main file
 * 
 */

#include "motd.h"

/**
 * @brief Call sub-functions and exit
 * 
 * @return int 
 */
int main(void) {

    greeting();
    sysinfo();
    fsuse();
    goodbye();
    
    return 0;
}

/**
 * @brief prints datetime and greeting message
 * 
 */
void greeting(void) {
    char *username;
    char *time_string = (char *) calloc(STR_SIZE, sizeof(char));  MCHECK(time_string)

    get_datetime(time_string);
    username = getlogin();

    printf("%s\n\n", time_string);
    printf("Welcome back, " COLOR_MAGENTA "%s" COLOR_RESET "\n\n", username);

    free(time_string);
}

/**
 * @brief Print system informations: kernel, distro, packages number
 * 
 */
void sysinfo(void) {
    struct utsname info;
    uname(&info);

    char *kernel = info.release;
    char *distro = get_distro();

    // Badly written attempt to mimic the bash pipe
    int pkgs     = pkgs_num(); //atoi(pipe_of("pacman -Q", "wc -l", BSIZE));
    int aur_pkgs = aur_pkgs_num(); //atoi(pipe_of("pacman -Qm", "wc -l", BSIZE));
    //int upgrades     = atoi(pipe_of("checkupdates", "wc -l", BSIZE));
    //int aur_upgrades = atoi(pipe_of("checkupdates-aur", "wc -l", BSIZE));


    printf(COLOR_MAGENTA "Distro" COLOR_RESET ":\t \t %s\n", distro);
    printf(COLOR_MAGENTA "Kernel" COLOR_RESET ":\t \t %s\n", kernel);
    
    printf(COLOR_MAGENTA "Packages" COLOR_RESET":\t %d\tpacman\n", pkgs);
    printf(                                   "\t\t %d\tAUR\n", aur_pkgs);

    /*
    if (upgrades > 0) {
        printf(COLOR_MAGENTA "Packages" COLOR_RESET":\t %d\t(" COLOR_MAGENTA "%d" COLOR_RESET " upgradable)\tpacman\n", pkgs, upgrades);
    } else {
        printf(COLOR_MAGENTA "Packages" COLOR_RESET":\t %d pacman\n", pkgs);
    }
    if (aur_upgrades > 0) {
        printf(                                   "\t\t %d\t(" COLOR_MAGENTA "%d" COLOR_RESET " upgradable)\tAUR\n", aur_pkgs, aur_upgrades);
    } else {
        printf(                                   "\t\t %d AUR\n", aur_pkgs);
    }
    */

    printf("\n");

    free(distro);
}

/**
 * @brief Get and print filesystem usage
 * 
 */
void fsuse(void) {
    // Grep info on the filesystems
    char **system_fs = get_fs_mountpoint();
    
    for (int i = 0; system_fs[i] != NULL; i++) {
        struct fsinfo fs_info = get_fs_info(system_fs[i]);

        printfs(system_fs[i], fs_info);
    }
}

/**
 * @brief Goodbye message: wiki remainder and last login
 * 
 */
void goodbye(void) {
    printf("\n\n\n");
    printf("Remember the bible: " COLOR_MAGENTA "https://wiki.archlinux.org/" COLOR_RESET "\n\n");
    // TODO
    // ssh-like last login info
}
