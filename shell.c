#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "split.h"

#define CYAN "\x1B[36m"
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"

int is_handled(char *command, char **argv);
int is_handled(char *command, char **argv)
{
  if (strcmp(*argv, "exit") == 0)
  {
    exit(0);
  }
  else if (strcmp(*argv, "cd") == 0 || strcmp(*argv, "chdir") == 0)
  {
    chdir(argv[1]);
    return 1;
  }
  return 0;
}

void executeCommand(char *command);
void executeCommand(char *command)
{
  char **argv = split(command, ' ');
  if (is_handled(command, argv))
  {
    return;
  }
  int pid = fork();
  if (pid == 0)
  {
    signal(SIGINT, NULL);
    if (execvp(*argv, argv) == -1)
    {
      printf("zsh: command not found: %s\n", *argv);
      exit(0);
    }
  }
  else
  {
    wait(&pid);
  }
}

int main(void);
int main(void)
{
  signal(SIGINT, SIG_IGN);
  char command[255];
  char pwd[255];
  while (1)
  {
    getcwd(pwd, sizeof(pwd));
    printf(CYAN "%s " RESET, pwd);
    printf(GREEN "$ " RESET);
    gets(command);
    executeCommand(command);
  }

  return 0;
}

// do i know about the command
// parse the arguments
// variable interpolation
// set any environment variable
// parse language if it allows
