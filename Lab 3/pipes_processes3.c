#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

int main (int argc, char * argv[]){
  if (argc != 2){
    fprintf(stderr, "Usage: %s <grep_argument>\n", argv[0]);
    return 1;
  }

  int fd1[2];
  int fd2[2];

  if (pipe(fd1) == -1 || pipe(fd2) == -1){
    perror("Pipe failed to create");
    return 1;
  }
  pid_t p1 = fork();
  if (p1 < 0) {
    perror("Fork failed");
    return 1;
  }

  if (p1 == 0) {
    close(fd1[1]);
    dup2(fd1[0], STDIN_FILENO);
    close(fd1[0]);

    close(fd2[0]);
    dup2(fd2[1], STDOUT_FILENO);
    close(fd2[1]);

    execlp("grep", "grep", argv[1], NULL);
    perror("execlp failed");
    exit(1);

  }else{
    pid_t p2 = fork();
    if (p2<0){
      perror("Fork failed");
      return 1;

    }

    if (p2 == 0){
      close(fd1[0]);
      close(fd1[1]);
      close(fd2[1]);
      dup2(fd2[0], STDIN_FILENO);
      close(fd2[0]);

      execlp("sort", "sort", NULL);
      perror("execlp failed");
      exit(1);

    }else{
      close(fd1[0]);
      dup2(fd1[1],STDOUT_FILENO);
      close(fd1[1]);

      close(fd2[0]);
      close(fd2[1]);

      execlp("cat", "cat", "scores", NULL);
      perror("execlp failed");
      exit(1);

    }
  }
  return 0;
}