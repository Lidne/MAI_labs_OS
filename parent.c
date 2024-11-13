#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
   int pipe1[2], pipe2[2], pipe_add[2];

   if (pipe(pipe1) == -1 || pipe(pipe2) == -1 || pipe(pipe_add) == -1) {
      perror("pipe");
      exit(1);
   }

   pid_t pid1 = fork();
   if (pid1 == -1) {
      perror("fork");
      exit(1);
   }

   if (pid1 == 0) {

      close(pipe1[1]);
      close(pipe_add[0]);
      close(pipe2[0]);
      close(pipe2[1]);

      dup2(pipe1[0], STDIN_FILENO);
      dup2(pipe_add[1], STDOUT_FILENO);

      close(pipe1[0]);
      close(pipe_add[1]);

      execl("./childUpper.out", "./childUpper.out", NULL);
      perror("execl");
      exit(1);
   }

   pid_t pid2 = fork();
   if (pid2 == -1) {
      perror("fork");
      exit(1);
   }

   if (pid2 == 0) {

      close(pipe1[0]);
      close(pipe1[1]);
      close(pipe2[0]);
      close(pipe_add[1]);

      dup2(pipe_add[0], STDIN_FILENO);
      dup2(pipe2[1], STDOUT_FILENO);

      close(pipe2[1]);
      close(pipe_add[0]);

      execl("./childSpaces.out", "./childSpaces.out", NULL);
      perror("execl");
      exit(1);
   }

   close(pipe1[0]);
   close(pipe2[1]);
   close(pipe_add[0]);
   close(pipe_add[1]);

   char input[1024];
   printf("Input string: ");

   fgets(input, 1024, stdin);
   write(pipe1[1], input, strlen(input));
   close(pipe1[1]);

   char output[1024];
   read(pipe2[0], output, 1024);
   close(pipe2[0]);

   printf("Result: %s", output);
   fflush(stdout);

   return 0;
}
