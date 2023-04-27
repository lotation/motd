#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <curl/curl.h>

#include "ip.h"

static size_t write_mem_callback(void *contents, size_t size, size_t nmemb, void *userp);

static size_t write_mem_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct mem_struct_s *mem = (struct mem_struct_s *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    MCHECK(ptr);

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

struct net_info_s *get_local_ip(void)
{
    struct net_info_s *netinfo = NEW(struct net_info_s);
    char host_buffer[MAX_HOST];
    struct hostent *host_entry;
    int hostname;

    hostname = gethostname(host_buffer, MAX_HOST);
    if (hostname == -1) {
        ABORT(gethostname);
    }

    netinfo->host = strdup(host_buffer);

    host_entry = gethostbyname(netinfo->host);
    if (host_entry == NULL) {
        ABORT(gethostbyname);
    }

    // Convert an Internet network address to ASCII string
    netinfo->ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
    if (netinfo->ip == NULL) {
        ABORT(inet_ntoa);
    }

    return netinfo;
}

char *get_public_ip(void)
{
    char *ret = NULL;

    const char *url = "ident.me";
    CURL *curl_handle;
    CURLcode res;

    struct mem_struct_s chunk;

    chunk.memory = NEW(char);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    if (curl_handle != NULL) {
        // specify URL to get
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        // send all data to this function
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_mem_callback);
        // ass 'chunk' struct to the callback function
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl_handle);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
        } else {
            ret = strdup(chunk.memory);
        }

        curl_easy_cleanup(curl_handle);

        free(chunk.memory);

        curl_global_cleanup();
    }

    return ret;
}
