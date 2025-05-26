#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>
#include <sys/wait.h>

char *piped_cmds[] = {"/bin/cat", "/bin/grep", "/bin/wc"};
char *(piped_args[])[5] = {{"/bin/cat", "file.txt", NULL}, {"/bin/grep", "hello", NULL}, {"/bin/wc", "-l", NULL}};
int main() {
  int N = sizeof(piped_cmds) / sizeof(char *);
  int pipes[N - 1][2];
  pid_t child;
  for (int i = 0; i < N-1; ++i) {
    pipe(pipes[i]);
  }
  for (int i = 0; i < N; ++i) {
    int first = i == 0;
    int last = i == N - 1;
    switch (child = fork()) {
    case -1: {
      perror("Cant fork");
      exit(EXIT_FAILURE);
      break;
    }
    case 0: {
      if (!first) {
        dup2(pipes[i-1][0], STDIN_FILENO);
      }
      if (!last) {
         dup2(pipes[i][1], STDOUT_FILENO);
      }
      //close(pipes[i - 1][0]);
      //close(pipes[i][1]);
      for (int i = 2; i < 1024; ++i) {
        close(i);
	  }
      execv(piped_cmds[i], &piped_args[i]);
      perror("Exec failed");
    }
    default: {
      // If we close here then next child will not be able to get the read end of the pipe.
      //close(pipes[i][0]);
      //close(pipes[i][1]);
	}
    }
  }
  // for (int i = 0; i + 1 < N; ++i) {
  //   close(pipes[i][0]);
  //   close(pipes[i][1]);
  // }
  for (int i = 2; i < 1024; ++i) {
    close(i);
  }
  for (int i = 0; i < N; ++i) {
    wait(NULL);
  }
}
