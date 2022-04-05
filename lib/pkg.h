#ifndef PKG_H
#define PKG_H

#include <alpm.h>
#include <alpm_list.h>

#define NATIVE  (unsigned short) 0
#define FOREIGN (unsigned short) 1

/** Frees a list and its contents */
#define FREELIST(p) do { alpm_list_free_inner(p, free); alpm_list_free(p); p = NULL; } while(0)

typedef struct __config_repo_t {
    char *name;
    alpm_list_t *servers;
    int usage;
    int siglevel;
    int siglevel_mask;
} config_repo_t;

typedef struct __config_t {
    char *rootdir;
    char *dbpath;

    unsigned short locality;

    /* connection to libalpm */
    alpm_handle_t *handle;
    alpm_list_t *repos;
} config_t;

/** libalpm conf */
config_t *config_new(void);
void config_repo_free(config_repo_t *repo);
int config_free(config_t *oldconfig);
static int _add_mirror(alpm_db_t *db, char *value);
static int register_repo(config_repo_t *repo);
static int setup_libalpm(void);
int init(void);

/** query paclages */
static unsigned short get_locality(alpm_pkg_t *pkg);
int pkgs_num(unsigned short locality);

#endif
