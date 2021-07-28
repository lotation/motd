#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/motd.h"

int main(void) {
    printf("Output: %s\n", pipe_of("pacman -Q", "wc -l"));

    return 0;
}

void strsplit(char *str, char *str_arr[]) {
    char *token = strtok(str, " ");

    for (int i = 0; token != NULL; i++) {
        str_arr[i] = (char *) calloc(strlen(token) + 1, sizeof(char));
        strcpy(str_arr[i], token);
        token = strtok(NULL, " ");
  	}
}

char *pipe_of(char *op1, char *op2) {
  
  /* 
   * TO-DO:
   * 1- Split both strings in multiple ones 
   * 2- Store them in an array of strings
   * 3- Use this array as argument of execv
  */

    char **first_arg, **second_arg;
    strsplit(op1, first_arg);
    strsplit(op2, second_arg);

    int fd_1[2];
    if (pipe(fd_1) == -1) {
        fprintf(stderr, "Could not open the first pipe.\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid_1 = fork();
    if (pid_1 < 0) {
        fprintf(stderr, "Could not fork the process.\n");
        exit(EXIT_FAILURE);
    }
    else if (pid_1 == 0) {
        // First Child process
        // duplicate the write fd
        dup2(fd_1[1], STDOUT_FILENO);
        close(fd_1[0]);
        close(fd_1[1]);

        execlp("pacman", "pacman", "-Q", NULL);

        // If the program is here, then the exec failed
        fprintf(stderr, "Could not execute the command.\n");
        exit(EXIT_FAILURE);
    }

    int fd_2[2];
    if (pipe(fd_2) == -1) {
        fprintf(stderr, "Could not open the second pipe.\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid_2 = fork();
    if (pid_2 < 0) {
        fprintf(stderr, "Could not fork the process.\n");
        exit(EXIT_FAILURE);
    }
    else if (pid_2 == 0) {
        // Second Child process
        // duplicate both fds
        dup2(fd_1[0], STDIN_FILENO);
        dup2(fd_2[1], STDOUT_FILENO);
        close(fd_1[0]);
        close(fd_1[1]);
        close(fd_2[0]);
        close(fd_2[1]);

        execlp("wc", "wc", "-l", NULL);

        // If the program is here, then the exec failed
        fprintf(stderr, "Could not execute the command.\n");
        exit(EXIT_FAILURE);
    }
    
    close(fd_1[0]);
    close(fd_1[1]);
    close(fd_2[1]);
    waitpid(pid_1, NULL, 0);

    // Creates a buffer to store the result of the pipe
    char buffer[BSIZE];
    if (read(fd_2[0], buffer, sizeof(char) * BSIZE) == -1) {
        fprintf(stderr, "Could not get output from the second child process.\n");
        exit(EXIT_FAILURE);
    }
    // Replace the last char with the terminator
    buffer[strlen(buffer) - 1] = '\0';

    printf("Output: %s\n", buffer);
    
    close(fd_2[0]);

    waitpid(pid_2, NULL, 0);

    return 0;
}