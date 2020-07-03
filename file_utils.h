#ifndef __FILE_UTILS_H__
#define __FILE_UTILS_H__

#include "alias.h"
#include "variables.h"
#include "shell.h"

List_ptr load_aliases();
List_ptr load_variables();
void store_List(List_ptr list, char *filename);
void load_sh_rc(List_ptr aliases, List_ptr vars, int *exit_code);

#endif