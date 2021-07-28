#include <stdio.h>

#include "../lib/motd.h"

void get_datetime(char *time_string) {
    time_t current_time = time(NULL);
    if (current_time == ((time_t) - 1)) {
        (void) fprintf(stderr, "Failed to obtain current time.\n");
        exit(EXIT_FAILURE);
    }
    
    struct tm *time_info;    
    time_info = localtime(&current_time);
    
    // " DAY NUM MON 	                       -                        	HOUR:MIN "
    strftime(time_string, STR_SIZE, "[ %a %d %b  %t                       -                        %t %R ]", time_info);
}
