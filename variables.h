#ifndef __VARIABLES_H__
#define __VARIABLES_H__

#include "list.h"
#include "file_utils.h"

int add_variable(List_ptr vars, char_ptr var);
void interpolate_variables(char_ptr *args, List_ptr vars);
char_ptr get_variable_value(List_ptr vars, char_ptr key);

#endif
