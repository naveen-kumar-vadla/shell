#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "alias.h"
#include "string_utils.h"
#include "variables.h"
#include "file_utils.h"

#define CYAN "\x1B[36m"
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"

#define MAX_SIZE 255

int is_handled(char_ptr command, List_ptr aliases, List_ptr vars, char_ptr *args, int *exit_code);
void executeCommand(char_ptr command, List_ptr aliases, List_ptr vars, int *exit_code);

#endif