#ifndef PKG_H
#define PKG_H

#include <pacutils.h>

#define NATIVE  0
#define FOREIGN 1

int get_locality(alpm_pkg_t *pkg);
int get_pkg_num(int locality);

#endif
