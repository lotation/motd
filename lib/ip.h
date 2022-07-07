#ifndef IP_H
#define IP_H

#include <stdio.h>         // basic I/O
#include <stdlib.h>        // malloc, free, exit
#include <string.h>        // strings utils

#include <unistd.h>        // POSIX API
#include <netdb.h>         // gethostname
/* #include <netinet/in.h>    */
#include <arpa/inet.h>     // IP address

#include <curl/curl.h>     // CURL HTTP GET

#define MAX_HOST 128
#define IP_STR    64 /* ((3 char * 4 campi) + 3 dots) * 2 ip_addr + 4 spazi + 2 parentesi + 1 '\0'  = 37 */

typedef struct mem_struct_ {
    char *memory;
    size_t size;
} mem_struct;

typedef struct net_info_ {
    char *host;
    char *ip;
} net_info;

size_t write_mem_callback(void *contents, size_t size, size_t nmemb, void *userp);
net_info *get_local_ip(void);
char *get_public_ip(void);

#endif