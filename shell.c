#include "shell.h"

int is_handled(char_ptr command, List_ptr aliases, List_ptr vars, char_ptr *args, int *exit_code)
{
  if (strcmp(*args, "exit") == 0)
  {
    exit(0);
  }
  if (strcmp(*args, "") == 0)
  {
    return 1;
  }
  if (strcmp(*args, "cd") == 0 || strcmp(*args, "chdir") == 0)
  {
    *exit_code = chdir(args[1]);
    if (*exit_code == -1)
    {
      perror("cd");
    }
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

void executeCommand(char_ptr command, List_ptr aliases, List_ptr vars, int *exit_code, int *pipes, int *fd_set)
{
  command = trim(command);
  char_ptr *args = split(command, ' ');
  interpolate_variables(args, vars);
  interpolate_aliases(args, aliases);
  if (is_handled(command, aliases, vars, args, exit_code))
  {
    return;
  }
  int pid = fork();
  if (pid == 0)
  {
    signal(SIGINT, NULL);
    fd_set[0] && close(pipes[0]);
    fd_set[1] && dup2(pipes[1], 1);
    fd_set[2] && close(pipes[1]);
    fd_set[3] && dup2(pipes[0], 0);
    if (handle_redirection(args) == -1)
    {
      exit(1);
    }
    execvp(*args, args);
    printf("sh: command not found: %s\n", *args);
    exit(127);
  }
  else if (pid == -1)
  {
    printf("\nFailed forking child..\n");
  }
  else
  {
    waitpid(pid, exit_code, 0);
  }
}

void execute(char_ptr command, List_ptr aliases, List_ptr vars, int *exit_code)
{
  int pipes[2];
  int fd_set[4] = {0, 0, 0, 0};
  pipe(pipes);
  if (!includes(command, '|'))
  {
    executeCommand(command, aliases, vars, exit_code, pipes, fd_set);
    return;
  }
  char_ptr *pipeCommands = split(command, '|');
  fd_set[0] = 1;
  fd_set[1] = 1;
  executeCommand(pipeCommands[0], aliases, vars, exit_code, pipes, fd_set);
  close(pipes[1]);
  fd_set[0] = 0;
  fd_set[1] = 0;
  fd_set[2] = 1;
  fd_set[3] = 1;
  executeCommand(pipeCommands[1], aliases, vars, exit_code, pipes, fd_set);
  close(pipes[0]);
}

int main(void)
{
  signal(SIGINT, SIG_IGN);
  char command[MAX_SIZE];
  char pwd[MAX_SIZE];
  int exit_code = 0;
  List_ptr aliases = load_aliases();
  List_ptr vars = load_variables();
  load_sh_rc(aliases, vars, &exit_code);
  while (1)
  {
    getcwd(pwd, sizeof(pwd));
    printf(CYAN "%s " RESET, pwd);
    printf(exit_code ? RED : GREEN);
    printf("$ " RESET);
    gets(command);
    execute(command, aliases, vars, &exit_code);
  }

  return 0;
}

// do i know about the command
// parse the arguments
// variable interpolation
// set any environment variable
// parse language if it allows
