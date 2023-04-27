#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <sys/sysinfo.h>
#include <sys/statvfs.h>

#include "motd.h"

char *get_datetime(void)
{
    char *time_string = MANY(char, STR_SIZE);

    time_t current_time = time(NULL);
    if (current_time == ((time_t) - 1)) {
        fprintf(stderr, "Failed to obtain current time.\n");
        ABORT(current_time);
    }

    struct tm *time_info;
    time_info = localtime(&current_time);

    //" DAY NUM MON 	                       -                        	HOUR:MIN "
    strftime(time_string, STR_SIZE, "[ %a %d %b  %t                             -                              %t %R ]", time_info);

    return time_string;
}

char *get_distro(void)
{
    char *distro = NULL;
    char *id = NULL;
    char *buffer = MANY(char, DISTRO);

    FILE *fp = FOPEN("/etc/os-release");

    char *tmp, *token;

    while (fgets(buffer, DISTRO, fp)) {
        buffer[strlen(buffer) - 1] = '\0';

        if (strstr(buffer, "PRETTY_NAME") ||
                strstr(buffer, "NAME"))
        {
            tmp = strdup(buffer);
            token = strtok(tmp, "=");
            token = strtok(NULL, "\"");

            distro = strdup(token);
            MCHECK(distro);

            free(tmp);
        }

        if (strstr(buffer, "BUILD_ID") ||
                strstr(buffer, "VERSION"))
        {
            tmp = strdup(buffer);
            token = strtok(tmp, "=");
            token = strtok(NULL, "\"");

            id = strdup(token);
            MCHECK(id);

            free(tmp);
        }
    }

    if (strlen(distro) == 0) {
        distro = strdup("Unknown");
    }

    snprintf(buffer, DISTRO, "%s (%s)", distro, id);

    fclose(fp);

    return buffer;
}

void printfs(const char *name, struct fsinfo_s fs)
{
    char c = '=';
    char spacing[MAX_NAME];
    spacing[0] = ' '; spacing[1] = '\t';

    if (strlen(name) == MAX_NAME) {
        strcat(spacing, " \t \t");
    } else if (strlen(name) < MAX_NAME) {
        strcat(spacing, " \t \t \t");
    }

    int used_percent = fs.used_percent - ((int)fs.used_percent) >= 0.5 ? ((int)fs.used_percent) + 1 : ((int)fs.used_percent);

    printf(" %s%s ", name, spacing);

    for (size_t i = 1; i <= 50; i++) {
        if (i >= (fs.used_percent / 2)) {
            printf("%c", c);
        } else {
            printf(COLOR_ON "%c" RESET, c);
        }
    }
    printf(" %s%d%%", used_percent <= 9 ? " " : "", used_percent);
}

struct fsinfo_s get_fs_info(const char *path)
{
    struct statvfs stat;
    struct fsinfo_s fsinfo;

    if (statvfs(path, &stat) != 0) {
        fprintf(stderr, "Failed to obtain filesystem usage.\n");
        ABORT(statvfs);
    }

    //double gib = 1024 * 1024 * 1024;
    double gb = 1000000000;
    fsinfo.total = (double)(stat.f_blocks * stat.f_frsize) / gb;
    fsinfo.free  = (double)(stat.f_bfree * stat.f_frsize) / gb;
    fsinfo.used = fsinfo.total - fsinfo.free;
    fsinfo.free_percent = fsinfo.free * 100.00 / fsinfo.total;
    fsinfo.used_percent = fsinfo.used * 100.00 / fsinfo.total;

    return fsinfo;
}

char **get_fs_mountpoint(void)
{
    size_t size = 1;
    // TODO
    char **sysfs = (char **) calloc(size, sizeof(char *));
    char *buffer = MANY(char, MOUNTS);

    FILE *fp = FOPEN("/proc/mounts");

    int i = 0;
    while (fgets(buffer, MOUNTS, fp)) {
        buffer[strlen(buffer) - 1] = '\0';

        if (strstr(buffer, "/dev/sd") || strstr(buffer, "/dev/nvme") ||
                strstr(buffer, "/dev/mmc") || strstr(buffer, "/dev/vd"))
        {
            char *tmp = strdup(buffer);
            char *token = strtok(tmp, " ");
            token = strtok(NULL, " ");

            // add a new string
            size += 1;
            sysfs = (char **) realloc(sysfs, size * sizeof(char *));
            MCHECK(sysfs);
            sysfs[i] = (char *) calloc(strlen(token) + 1, sizeof(char));
            MCHECK(sysfs[i]);
            strcpy(sysfs[i], token);

            i++;
            free(tmp);
        }
    }
    // use NULL as terminator
    sysfs[i] = NULL;

    free(buffer);
    fclose(fp);

    return sysfs;
}

char *get_uptime(void)
{
    long ut;
    long sec, min, hour, days;
    char *uptime = MANY(char, STR_SIZE);

    struct sysinfo info;
    sysinfo(&info);
    ut = info.uptime;

    if (ut >= 0 && ut < 60) {
        sec = ut;
        snprintf(uptime, STR_SIZE, "%ld sec\n", sec);
    }
    else if (ut >= 60 && ut < 3600) {
        min = ut / 60;
        sec = ut - min*60;
        snprintf(uptime, STR_SIZE, "%ld min, %ld sec\n", min, sec);
    }
    else if (ut >= 3600 && ut < 86400) {
        hour = ut / 3600;
        min = (ut - hour*3600) / 60;
        ut -= hour*3600;
        sec = ut - min*60;
        snprintf( uptime, STR_SIZE, "%ld hours, %ld min, %ld sec\n", hour, min, sec);
    }
    else if (ut >= 86400) {
        days = ut / 86400;
        hour = (ut - days*86400) / 3600;
        ut -= days*86400;
        min = (ut - hour*3600) / 60;
        ut -= hour*3600;
        sec = ut - min*60;
        snprintf(uptime, STR_SIZE, "%ld days, %ld hours, %ld min, %ld sec\n", days, hour, min, sec);
    }
    else {
        fprintf(stderr, "unknown system uptime.\n");
        ABORT(sysinfo);
    }

    uptime = REALLOC(uptime, char, strlen(uptime) + 1);

    return uptime;
}

/*
int get_sysload(void) {
    int load = getloadavg();
}
*/
