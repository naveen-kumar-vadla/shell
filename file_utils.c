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
    executeCommand(command, aliases, vars, exit_code);
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