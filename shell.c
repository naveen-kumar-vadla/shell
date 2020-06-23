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
      execl("/bin/echo", "/bin/echo", "hello world", NULL);
    }
    else
    {
      wait(&pid);
    }
  }

  return 0;
}