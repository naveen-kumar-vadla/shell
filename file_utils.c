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