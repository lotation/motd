#include "ip.h"


static size_t write_mem_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    mem_struct *mem = (mem_struct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
        exit(EXIT_FAILURE);
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

net_info *get_local_ip(void)
{
    net_info *netinfo = (net_info *) calloc(1, sizeof(net_info));
    if (netinfo == NULL) {
        fprintf(stderr, "Could not allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    char host_buffer[MAX_HOST];
    struct hostent *host_entry;
    int hostname;

    /* Retrieve hostname */
    hostname = gethostname(host_buffer, MAX_HOST);
    if (hostname == -1) {
        perror("gethostname ");
        exit(EXIT_FAILURE);
    }

    netinfo->host = strdup(host_buffer);
    if (netinfo->host == NULL) {
        perror("strdup ");
        exit(EXIT_FAILURE);
    }

    /* Retrieve host information */
    host_entry = gethostbyname(netinfo->host);
    if (host_entry == NULL) {
        perror("gethostbyname ");
        exit(EXIT_FAILURE);
    }

    /* Convert an Internet network ddress to ASCII string */
    netinfo->ip = inet_ntoa(*((struct in_addr*)
                           host_entry->h_addr_list[0]));
    if (netinfo->ip == NULL) {
        perror("inet_ntoa ");
        exit(EXIT_FAILURE);
    }

    return netinfo;
}

char *get_public_ip(void)
{
    char *ret = NULL;

    const char *url = "ident.me";
    CURL *curl_handle;
    CURLcode res;

    mem_struct chunk;
    /* will be grown as needed by the realloc above */
    chunk.memory = (char *) malloc(sizeof(char));
    /* no data at this point */
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    /* init the curl session */
    curl_handle = curl_easy_init();

    if (curl_handle != NULL)
    {
        /* specify URL to get */
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        /* send all data to this function  */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_mem_callback);
        /* ass 'chunk' struct to the callback function */
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