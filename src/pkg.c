#include <stdio.h>

#include "pkg.h"

/* global config variable */
config_t *config = NULL;

config_t *config_new(void)
{
    config_t *newconfig = calloc(1, sizeof(config_t));
    if(!newconfig) {
        fprintf(stderr, "malloc failure: could not allocate %zu bytes\n", sizeof(config_t));
        return NULL;
    }

    return newconfig;
}

void config_repo_free(config_repo_t *repo)
{
    if(repo == NULL) {
        return;
    }
    free(repo->name);
    FREELIST(repo->servers);
    free(repo);
}

int config_free(config_t *oldconfig)
{
    if(oldconfig == NULL) {
        return -1;
    }

    alpm_list_free_inner(config->repos, (alpm_list_fn_free) config_repo_free);
    alpm_list_free(config->repos);

    free(oldconfig->rootdir);
    free(oldconfig->dbpath);
    free(oldconfig);

    return 0;
}

static int _add_mirror(alpm_db_t *db, char *value)
{
    if(alpm_db_add_server(db, value) != 0) {
        /* pm_errno is set by alpm_db_setserver */
        fprintf(stderr, "could not add server URL to database '%s': %s (%s)\n",
                  alpm_db_get_name(db), value, alpm_strerror(alpm_errno(config->handle)));
        return 1;
    }

    return 0;
}

static int register_repo(config_repo_t *repo)
{
    alpm_list_t *i;
    alpm_db_t *db;

    db = alpm_register_syncdb(config->handle, repo->name, repo->siglevel);
    if(db == NULL) {
        fprintf(stderr, "could not register '%s' database (%s)\n",
                  repo->name, alpm_strerror(alpm_errno(config->handle)));
        return 1;
    }

    /*
    fprintf(stderr, "setting usage of %d for %s repository\n",
              repo->usage, repo->name);
    alpm_db_set_usage(db, repo->usage);
    */

    for(i = repo->servers; i; i = alpm_list_next(i)) {
        if(_add_mirror(db, i->data) != 0) {
            return 1;
        }
    }

    return 0;
}

/** Sets up libalpm global stuff in one go. Called after the command line
 * and initial config file parsing. Once this is complete, we can see if any
 * paths were defined. If a rootdir was defined and nothing else, we want all
 * of our paths to live under the rootdir that was specified. Safe to call
 * multiple times (will only do anything the first time).
 */
static int setup_libalpm(void)
{
    alpm_errno_t err;
    alpm_handle_t *handle;
    alpm_list_t *i;

    /* initialize library */
    handle = alpm_initialize(config->rootdir, config->dbpath, &err);
    if(!handle) {
        fprintf(stderr, "failed to initialize alpm library:\n(root: %s, dbpath: %s)\n%s\n",
                config->rootdir, config->dbpath, alpm_strerror(err));
        if(err == ALPM_ERR_DB_VERSION) {
            fprintf(stderr, "The localdb is in a newer/older format than libalpm expects, "
                            "try running pacman-db-upgrade\n\n");
        }
        return -1;
    }
    config->handle = handle;

    for(i = config->repos; i; i = alpm_list_next(i)) {
        register_repo(i->data);
    }

    return 0;
}

/**
 * Init libalpm functions.
 * @return 0 on success, non-zero on error
 */
int init(void)
{
    int ret;
    if((ret = setup_libalpm())) {
        return ret;
    }
    alpm_list_free_inner(config->repos, (alpm_list_fn_free) config_repo_free);
    alpm_list_free(config->repos);
    config->repos = NULL;
    return ret;
}

int check_syncdbs(size_t need_repos, int check_valid)
{
    int ret = 0;
    alpm_list_t *i;
    alpm_list_t *sync_dbs = alpm_get_syncdbs(handle);

    if (need_repos && sync_dbs == NULL) {
        fprintf(stderr, "no usable package repositories configured.\n");
        return 1;
    }

    if (check_valid) {
        for (i = sync_dbs; i; i = alpm_list_next(i)) {
            alpm_db_t *db = i->data;
            if (alpm_db_get_valid(db)) {
                fprintf(stderr, "database '%s' is not valid (%s)\n",
                        alpm_db_get_name(db), alpm_strerror(alpm_errno(handle)));
                ret = 1;
            }
        }
    }
    return ret;
}


static unsigned short get_locality(alpm_pkg_t *pkg)
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

int pkgs_num(unsigned short locality)
{
    int packages = 0;
    int ret = 0;
    int match = 0;

    alpm_list_t *i;
    alpm_pkg_t *pkg = NULL;
    alpm_db_t *local_db;

    if (check_syncdbs(1, 1)) {
        return 1;
    }

    local_db = alpm_get_localdb(handle);

    for (i = alpm_db_get_pkgcache(local_db); i; i = alpm_list_next(i)) {
        pkg = i->data;

        if (locality == get_locality(pkg)) {
            match = 1;
            packages++;
        }

        if (!match) {
            ret = 1;
        }
        return ret;

    }

    return packages;
}
