#include "variables.h"

int add_variable(List_ptr vars, char_ptr var)
{
  char_ptr *split_vars = split(var, '=');
  add_to_list(vars, split_vars[0], split_vars[1]);
  return split_vars[2] ? 1 : 0;
}

char_ptr get_variable_value(List_ptr vars, char_ptr key)
{
  char key_without_dollar[strlen(key) - 1];
  memcpy(key_without_dollar, key + 1, strlen(key) - 1);
  char_ptr value = get_value_of(vars, key_without_dollar);
  return value ? value : "";
}

void interpolate_variables(char_ptr *args, List_ptr vars)
{
  int i = 0;
  while (args[i])
  {
    if (args[i][0] == '$')
    {
      args[i] = get_variable_value(vars, args[i]);
    }
    i++;
  }
}
