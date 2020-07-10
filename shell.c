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

void executeCommand(char_ptr command, List_ptr aliases, List_ptr vars, int *exit_code, int read_fd, int write_fd, int *fd_set)
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
    fd_set[0] && dup2(read_fd, 0);
    fd_set[1] && dup2(write_fd, 1);
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
  int fd_set[] = {0, 0};
  int pipes_count = get_count_of_words(command, '|');
  int pipes[2 * (pipes_count - 1)];
  if (!includes(command, '|'))
  {
    executeCommand(command, aliases, vars, exit_code, pipes[0], pipes[1], fd_set);
    return;
  }
  for (int i = 0; i < sizeof(pipes) / sizeof(int); i += 2)
  {
    pipe(pipes + i);
  }

  char_ptr *pipeCommands = split(command, '|');
  int read_fd_pos = 0;
  int write_fd_pos = 3;

  fd_set[1] = 1;
  executeCommand(pipeCommands[0], aliases, vars, exit_code, 0, pipes[1], fd_set);
  close(pipes[1]);
  for (int i = 1; i < pipes_count - 1; i++)
  {
    fd_set[0] = 1;
    fd_set[1] = 1;
    executeCommand(pipeCommands[i], aliases, vars, exit_code, pipes[read_fd_pos], pipes[write_fd_pos], fd_set);
    close(pipes[read_fd_pos]);
    close(pipes[write_fd_pos]);
    read_fd_pos += 2;
    write_fd_pos += 2;
  }
  fd_set[0] = 1;
  fd_set[1] = 0;
  executeCommand(pipeCommands[pipes_count - 1], aliases, vars, exit_code, pipes[read_fd_pos], 1, fd_set);
  close(pipes[read_fd_pos]);
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
