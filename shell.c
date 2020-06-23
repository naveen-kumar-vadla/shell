#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void);
int main(void)
{
  while (1)
  {
    char command[255];
    printf("my-shell $ ");
    gets(command);
    int pid = fork();
    if (pid == 0)
    {
      if (strcmp(command, "ls") == 0)
      {
        execl("/bin/ls", "/bin/ls", NULL);
      }
      else
      {
        printf("Command not found\n");
        exit(-1);
      }
    }
    else
    {
      wait(&pid);
    }
  }

  return 0;
}

// do i know about the command
// parse the arguments
// variable interpolation
// set any environment variable
// parse language if it allows