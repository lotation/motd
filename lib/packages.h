#ifndef PACKAGES_H
#define PACKAGES_H

#include <alpm.h>

#define NATIVE  (1 << 0) // 1
#define FOREIGN (1 << 1) // 2

int pkgs_num(int locality);
int get_locality(alpm_pkg_t *pkg);

#endif
