#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "string_utils.h"

#define CYAN "\x1B[36m"
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"

int is_handled(char *command, char **argv, int *exit_code);
int is_handled(char *command, char **argv, int *exit_code)
{
  if (strcmp(*argv, "exit") == 0)
  {
    exit(0);
  }
  else if (strcmp(*argv, "cd") == 0 || strcmp(*argv, "chdir") == 0)
  {
    *exit_code = chdir(argv[1]);
    if (*exit_code == -1)
    {
      char *message = "no such file or directory";
      if (includes(argv[1], '.'))
      {
        message = "not a directory";
      }
      printf("cd: %s: %s\n", message, argv[1]);
    }
    return 1;
  }
  return 0;
}

void executeCommand(char *command, int *exit_code);
void executeCommand(char *command, int *exit_code)
{
  char **argv = split(command, ' ');
  if (is_handled(command, argv, exit_code))
  {
    return;
  }
  int pid = fork();
  if (pid == 0)
  {
    signal(SIGINT, NULL);
    if (execvp(*argv, argv) == -1)
    {
      printf("sh: command not found: %s\n", *argv);
      exit(127);
    }
  }
  else if (pid == -1)
  {
    printf("\nFailed forking child..");
    return;
  }
  else
  {
    waitpid(pid, exit_code, 0);
  }
}

int main(void);
int main(void)
{
  signal(SIGINT, SIG_IGN);
  char command[255];
  char pwd[255];
  int exit_code = 0;
  while (1)
  {
    getcwd(pwd, sizeof(pwd));
    printf(CYAN "%s " RESET, pwd);
    printf(exit_code ? RED : GREEN);
    printf("$ " RESET);
    gets(command);
    executeCommand(command, &exit_code);
  }

  return 0;
}

// do i know about the command
// parse the arguments
// variable interpolation
// set any environment variable
// parse language if it allows
