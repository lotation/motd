#include "packages.h"

alpm_errno_t err;
alpm_handle_t *handle;
alpm_db_t *local_db;

int get_locality(alpm_pkg_t *pkg)
{
    const char *pkgname = alpm_pkg_get_name(pkg);
    alpm_list_t *i;
    alpm_list_t *sync_dbs = alpm_get_syncdbs(handle);

    for (i = sync_dbs; i != NULL; i = alpm_list_next(i)) {
        if (alpm_db_get_pkg(i->data, pkgname)) {
            return NATIVE;
        }
    }
    return FOREIGN;
}

int pkgs_num(int locality)
{
    handle = alpm_initialize("/", "/var/lib/pacman", &err);
    local_db = alpm_get_localdb(handle);

    int packages = 0;
    alpm_list_t *i;
    alpm_pkg_t *pkg = NULL;

    for(i = alpm_db_get_pkgcache(local_db); i != NULL; i = alpm_list_next(i)) {
        pkg = i->data;

        /* check if packages is in local_db */
        if(locality != get_locality(pkg))
            packages++;
    }

    alpm_release(handle);

    return packages;
}
