/**
 * @file pipe.c
 * @author lotation (xlapsiu@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-02
 * 
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 * 
 * This file emulates UNIX pipe
 * 
 */
#include "motd.h"

/**
 * @brief split the given string into substrings (using space as delimiter)
 * 
 * @param str phrase string
 * @param str_arr result strings array
 */
void strsplit(char *str, char *str_arr[]) {
    char *token = strtok(str, " ");
    int i;

    /* For each word in str_arr (as is using space as delimiter) 
     * creates a new value in the string array and copies token in it */
    for (i = 0; token != NULL; i++) {
        str_arr = (char **) realloc(str_arr, (sizeof(*str_arr) + 1) * sizeof(char *));  MCHECK(str_arr)
        str_arr[i] = (char *) calloc(strlen(token) + 1, sizeof(char));  MCHECK(str_arr[i])
        strcpy(str_arr[i], token);
        token = strtok(NULL, " ");
  	}
    
    // Set the last element of the array as NULL 
    str_arr = (char **) realloc(str_arr, (sizeof(*str_arr) + 1) * sizeof(char *));  MCHECK(str_arr)
    str_arr[i] = NULL;
}

/**
 * @brief emulates the UNIX pipe redirecting the op1 stdout to op2 stdin
 * 
 * @param op1 first command
 * @param op2 second command
 * @param buffer_size size of the output buffer
 * @return char* buffer with op2 output
 */
char *pipe_of(const char op1[], const char op2[], int buffer_size) {

    // Creates new strings to contain the operations
    char *string_1 = (char *) calloc(strlen(op1) + 1, sizeof(char));  MCHECK(string_1)
    strcpy(string_1, op1);
    char *string_2 = (char *) calloc(strlen(op2) + 1, sizeof(char));  MCHECK(string_2)
    strcpy(string_2, op2);
    
    // Declaring the two arguments array to use in exec later
    char **arg_1 = (char **) calloc(1, sizeof(char *));  MCHECK(arg_1)
    strsplit(string_1, arg_1);
    char **arg_2 = (char **) calloc(1, sizeof(char *));  MCHECK(arg_2)
    strsplit(string_2, arg_2);


    // Open the first pipe from the first child to the second
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
        // duplicate the write end of the pipe, fd_1
        dup2(fd_1[1], STDOUT_FILENO);
        close(fd_1[0]);
        close(fd_1[1]);

        execvp(arg_1[0], arg_1);

        // If the program is here, then the exec failed
        fprintf(stderr, "Could not execute the command %s.\n", arg_1[0]);
        exit(EXIT_FAILURE);
    }

    // Open the other pipe from the second child to the parent
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
        // duplicate both file descriptors (read 1 and write 2)
        dup2(fd_1[0], STDIN_FILENO);
        dup2(fd_2[1], STDOUT_FILENO);
        close(fd_1[0]);
        close(fd_1[1]);
        close(fd_2[0]);
        close(fd_2[1]);

        execvp(arg_2[0], arg_2);

        // If the program is here, then the exec failed
        fprintf(stderr, "Could not execute the command %s.\n", arg_2[0]);
        exit(EXIT_FAILURE);
    }
    
    close(fd_1[0]);
    close(fd_1[1]);
    close(fd_2[1]);
    waitpid(pid_1, NULL, 0);

    // Creates a buffer to store the result of the pipe
    char *buffer = (char *) calloc(buffer_size, sizeof(char));
    if (read(fd_2[0], buffer, sizeof(char) * buffer_size) == -1) {
        fprintf(stderr, "Could not get output from the second child process.\n");
        exit(EXIT_FAILURE);
    }
    // Replace the last char with the terminator
    buffer[strlen(buffer) - 1] = '\0';

    close(fd_2[0]);
    
    free(string_1);
    free(string_2);
    free(arg_1);
    free(arg_2);

    waitpid(pid_2, NULL, 0);


    return buffer;
}