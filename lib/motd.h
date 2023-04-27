#ifndef MOTD_H
#define MOTD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 128       // length of time_string = 92
#define DISTRO    64       // max line length of /etc/os-release
#define MOUNTS   192       // max line length of /proc/mounts
#define MAX_NAME   9       // max name length

#define COLOR_ON    "\033[38;5;197m"
#define ERROR_ON    "\033[31m"
#define BOLD_ON     "\033[1m"
#define RESET       "\033[0m"

//#define SWAP(A,B,T) do { T tmp = A; A = B; B = tmp; } while(0);
//#define UNUSED(A) (void)(A)
#define STRINGIFY(A) #A

#define ERROR(FUNC) fprintf(stderr, BOLD_ON "%s:%d:" RESET " %s: "  ERROR_ON "error:" RESET " %s: %s\n", __FILE__, __LINE__, __func__, STRINGIFY(FUNC), strerror(errno));
#define ABORT(FUNC) { ERROR(FUNC); exit(EXIT_FAILURE); }
#define MCHECK(PTR) if (PTR == NULL) { ERROR("malloc"); exit(EXIT_FAILURE); }
#define FOPEN(FILENAME) (FILE*)({ errno = 0; FILE *FP = fopen(FILENAME, "r"); if (FP  == NULL) { fprintf(stderr, "Failed to open file '%s'.\n", FILENAME); ERROR(FILENAME); exit(EXIT_FAILURE); }; FP; })
#define MANY(TYPE,COUNT) (TYPE*)({ errno = 0; TYPE *RET = calloc((COUNT), sizeof(TYPE)); MCHECK(RET); RET; })
#define REALLOC(PTR,TYPE,COUNT) (TYPE*)({ errno = 0; TYPE *RET = reallocarray(PTR, (COUNT), sizeof(TYPE)); MCHECK(RET); RET; })
#define NEW(TYPE) MANY(TYPE,1)

struct fsinfo_s {
    double total;
    double free;
    double used;
    double free_percent;
    double used_percent;
};

struct dirlist_s {
    size_t index;
    size_t size;
    char **field;
};


char *get_datetime(void);
char *get_distro(void);
char *get_uptime(void);
char **get_fs_mountpoint(void);
struct fsinfo_s get_fs_info(const char *path);
void printfs(const char* name, struct fsinfo_s fs);
//int get_sysload(void)


#endif
