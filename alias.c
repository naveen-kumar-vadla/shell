#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "alias.h"

void add_alias(List_ptr aliases, char_ptr alias)
{
  char_ptr *split_alias = split(alias, '=');
  add_to_list(aliases, split_alias[0], split_alias[1]);
}

void handle_alias(char_ptr *args, List_ptr aliases)
{
  if (args[1] && args[1][0] != '=' && includes(args[1], '='))
  {
    add_alias(aliases, args[1]);
    return;
  }
  print_matching_key(aliases, args[1]);
}

char_ptr get_actual(List_ptr aliases, char_ptr aka)
{
  char_ptr actual = get_value_of(aliases, aka);
  return actual ? actual : aka;
}
