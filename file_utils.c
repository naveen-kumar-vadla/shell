#include "file_utils.h"

void remove_newline_ch(char_ptr line)
{
  int new_line = strlen(line) - 1;
  if (line[new_line] == '\n')
    line[new_line] = '\0';
}

List_ptr load_aliases()
{
  List_ptr aliases = create_list();
  char command[MAX_SIZE];
  FILE *fptr = fopen("./bin/.aliases", "r");
  if (fptr == NULL)
    return aliases;
  while (fgets(command, sizeof(command), fptr) != NULL)
  {
    remove_newline_ch(command);
    add_alias(aliases, command);
  }
  fclose(fptr);
  return aliases;
}

List_ptr load_variables()
{
  List_ptr variables = create_list();
  char command[MAX_SIZE];
  FILE *fptr = fopen("./bin/.variables", "r");
  if (fptr == NULL)
    return variables;
  while (fgets(command, sizeof(command), fptr) != NULL)
  {
    remove_newline_ch(command);
    add_variable(variables, command);
  }
  fclose(fptr);
  return variables;
}

void store_List(List_ptr list, char *filename)
{
  FILE *fptr = fopen(filename, "w");
  if (fptr == NULL)
    return;
  Node_ptr p_walk = list->first;
  while (p_walk)
  {
    fprintf(fptr, "%s=%s\n", p_walk->key, p_walk->value);
    p_walk = p_walk->next;
  }
  fclose(fptr);
}

void load_sh_rc(List_ptr aliases, List_ptr vars, int *exit_code)
{
  char command[MAX_SIZE];
  FILE *fptr = fopen("./bin/.shrc", "r");
  if (fptr == NULL)
    return;
  while (fgets(command, sizeof(command), fptr) != NULL)
  {
    remove_newline_ch(command);
    execute(command, aliases, vars, exit_code);
  }
  fclose(fptr);
}

int open_file_and_append(char_ptr *args, int index, int fd, mode_t mode)
{
  int new_fd = open(args[index + 1], mode, 0644);
  if (new_fd == -1)
  {
    perror("sh");
    return new_fd;
  }
  dup2(new_fd, fd);
  args[index] = NULL;
  return new_fd;
}

int open_file_and_overwrite(char_ptr *args, int index, int fd)
{
  int new_fd = creat(args[index + 1], 0644);
  if (new_fd == -1)
  {
    perror("sh");
    return new_fd;
  }
  dup2(new_fd, fd);
  args[index] = NULL;
  return new_fd;
}

int handle_redirection(char_ptr *args)
{
  int index = includes_array(args, '>');
  if (index > 0 && strcmp(args[index], ">>") == 0)
  {
    return open_file_and_append(args, index, 1, O_WRONLY | O_CREAT | O_APPEND);
  }
  if (index > 0 && strcmp(args[index], "2>>") == 0)
  {
    return open_file_and_append(args, index, 2, O_WRONLY | O_CREAT | O_APPEND);
  }
  if (index > 0 && strcmp(args[index], "2>") == 0)
  {
    return open_file_and_overwrite(args, index, 2);
  }
  if (index > 0)
  {
    return open_file_and_overwrite(args, index, 1);
  }
  index = includes_array(args, '<');
  if (index > 0)
  {
    return open_file_and_append(args, index, 0, O_RDONLY);
  }
  return 0;
}


void handle_piping(char_ptr command, List_ptr aliases, List_ptr vars, int *exit_code, int pipes_count, int *pipes, int *fd_set)
{
  for (int i = 0; i < 2 * (pipes_count - 1); i += 2)
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
