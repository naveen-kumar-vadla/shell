#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define CYAN "\x1B[36m"
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"

void executeCommand(char *command);
void executeCommand(char *command)
{
  int pid = fork();
  int status = 0;
  if (pid == 0)
  {
    signal(SIGINT, NULL);
    status = execlp(command, command, NULL);
  }
  else
  {
    wait(&pid);
  }
  if (status == -1)
  {
    printf(RED "\nCommand Not Found\n" RESET);
    exit(0);
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
    if (strcmp(command, "exit") == 0)
    {
      exit(0);
    }
    else
    {
      executeCommand(command);
    }
  }

  return 0;
}

// do i know about the command
// parse the arguments
// variable interpolation
// set any environment variable
// parse language if it allows