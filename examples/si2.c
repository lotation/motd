#include <stdio.h>         // printf
#include <stdlib.h>

#include "../lib/motd.h"

int main(void) {

    sysinfo();

    return 0;
}

void sysinfo(void) {
    unsigned short int packages, upgrades;
    struct utsname info;
    uname(&info);

    char *kernel = info.release;
    char *distro = get_distro();

    //packages = atoi(pipe_of("pacman -Q", "wc -l"));
    //upgrades = atoi(pipe_of("checkupdates 2>/dev/null", "wc -l"));

    printf("Distro:       %s\n", distro);
    printf("Kernel:       %s\n", kernel);
    printf("Packages:     %hu  (%hu upgradable)\n", packages, upgrades);
    printf("\n");

}

char *get_distro(void) {
    char *name = (char *) calloc(STR_SIZE, sizeof(char));
    MCHECK(name)
    char index;
    FILE *fp;

    fp = fopen("/etc/os-release", "r");
    if (fp == NULL) {
        (void) fprintf(stderr, "Failed to open /etc/os-release file.\n");
        exit(EXIT_FAILURE);
    }

    while ((index = fgetc(fp))  != EOF) {
        fscanf(fp, "NAME=\"%s\"", name);
    }

    fclose(fp);

    return name;
}