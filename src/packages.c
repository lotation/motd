/**
 * @file packages.c
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
 * This file implements some pacman functionalities using alpm library 
 */
#include <alpm.h>

#include "motd.h"

/**
 * @brief Get the number of installed pacman packages
 * 
 * @return int packages number
 */
int pkgs_num(void) {

    alpm_errno_t err;
    alpm_handle_t *handle = alpm_initialize("/", "/var/lib/pacman", &err);

    alpm_db_t *db = alpm_get_localdb(handle);

    alpm_list_t *pkgs = alpm_db_get_pkgcache(db);

    size_t n_pkgs = alpm_list_count(pkgs);

    alpm_release(handle);
    
    return (int) n_pkgs;
}

/**
 * @brief Get foreign packages installed using the AUR or manually
 * 
 * @return int packages number
 */
int aur_pkgs_num(void) {
    // TODO
}
