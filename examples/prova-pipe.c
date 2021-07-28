#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BSIZE 6 // => 99 999 (10^5-1) packages in total

int main(void) {
  
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
  buffer[strlen(buffer)-1] = '\0';

  printf("Output: %s\n", buffer);
  
  close(fd_2[0]);

  waitpid(pid_2, NULL, 0);

  return 0;
}
