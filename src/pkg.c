#include "pkg.h"


pu_config_t *config = NULL;
alpm_handle_t *handle = NULL;

static int get_locality(alpm_pkg_t *pkg)
{
    const char *pkgname = alpm_pkg_get_name(pkg);
    alpm_list_t *i;
    alpm_list_t *sync_dbs = alpm_get_syncdbs(handle);

    for (i = sync_dbs; i; i = alpm_list_next(i)) {
        if (alpm_db_get_pkg(i->data, pkgname) != NULL) {
            return NATIVE;
        }
    }
    return FOREIGN;
}

int get_pkg_num(int locality)
{
    int packages = 0;

    config = pu_ui_config_load(NULL, "/etc/pacman.conf");
    handle = pu_initialize_handle_from_config(config);
    pu_register_syncdbs(handle, config->repos);

    alpm_list_t *i;
    alpm_pkg_t *pkg = NULL;
    alpm_db_t *local_db;

    local_db = alpm_get_localdb(handle);

    for (i = alpm_db_get_pkgcache(local_db); i; i = alpm_list_next(i)) {
        pkg = i->data;

        if (locality == get_locality(pkg)) {
            packages++;
        }
    }

    alpm_release(handle);
    pu_config_free(config);

    return packages;
}