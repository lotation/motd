#ifndef IP_H
#define IP_H

#include "motd.h"

#define MAX_HOST  128
// old 64
#define IP_STR     37 /* ((3 char * 4 fields) + 3 dots) * 2 ip_addr + 4 spaces + 2 parenthesis + 1 '\0'  = 37 */

struct mem_struct_s {
    char *memory;
    size_t size;
};

struct net_info_s {
    char *host;
    char *ip;
};

struct net_info_s *get_local_ip(void);
char *get_public_ip(void);

#endif
