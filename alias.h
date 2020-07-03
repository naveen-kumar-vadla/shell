#ifndef __ALIAS_H__
#define __ALIAS_H__

#include "list.h"
#include "file_utils.h"

void add_alias(List_ptr aliases, char_ptr alias);
void handle_alias(char_ptr *args, List_ptr aliases);
char_ptr get_actual(List_ptr aliases, char_ptr key);
void interpolate_aliases(char_ptr *args, List_ptr aliases);

#endif