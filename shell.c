#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "alias.h"
#include "string_utils.h"
#include "variables.h"

#define CYAN "\x1B[36m"
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"

void display_chdir_errors(int *exit_code, char_ptr *args);
void display_chdir_errors(int *exit_code, char_ptr *args)
{
  if (*exit_code == -1)
  {
    char_ptr message = "no such file or directory";
    if (includes(args[1], '.'))
    {
      message = "not a directory";
    }
    printf("cd: %s: %s\n", message, args[1]);
  }
}

int is_handled(char_ptr command, List_ptr aliases, List_ptr vars, char_ptr *args, int *exit_code);
int is_handled(char_ptr command, List_ptr aliases, List_ptr vars, char_ptr *args, int *exit_code)
{
  if (strcmp(*args, "exit") == 0)
  {
    exit(0);
  }
  if (strcmp(*args, "cd") == 0 || strcmp(*args, "chdir") == 0)
  {
    *exit_code = chdir(args[1]);
    display_chdir_errors(exit_code, args);
    return 1;
  }
  if (strcmp(*args, "alias") == 0)
  {
    handle_alias(args, aliases);
    *exit_code = 0;
    return 1;
  }
  if (includes(*args, '='))
  {
    *exit_code = add_variable(vars, *args);
    return 1;
  }
  return 0;
}

void executeCommand(char_ptr command, List_ptr aliases, List_ptr vars, int *exit_code);
void executeCommand(char_ptr command, List_ptr aliases, List_ptr vars, int *exit_code)
{
  char_ptr *args = split(command, ' ');
  interpolate_variables(args, vars);
  char_ptr actual = get_actual(aliases, args[0]);
  args[0] = actual;
  if (is_handled(command, aliases, vars, args, exit_code))
  {
    return;
  }
  int pid = fork();
  if (pid == 0)
  {
    signal(SIGINT, NULL);
    if (execvp(*args, args) == -1)
    {
      printf("sh: command not found: %s\n", *args);
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
  List_ptr aliases = create_list();
  List_ptr vars = create_list();
  while (1)
  {
    getcwd(pwd, sizeof(pwd));
    printf(CYAN "%s " RESET, pwd);
    printf(exit_code ? RED : GREEN);
    printf("$ " RESET);
    gets(command);
    executeCommand(command, aliases, vars, &exit_code);
  }

  return 0;
}

// do i know about the command
// parse the arguments
// variable interpolation
// set any environment variable
// parse language if it allows
