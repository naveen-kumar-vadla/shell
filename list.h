#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string_utils.h"

typedef struct node
{
  char_ptr key;
  char_ptr value;
  struct node *next;
} Node;

typedef Node *Node_ptr;

typedef struct
{
  Node_ptr first;
  Node_ptr last;
} List;

typedef List *List_ptr;

List_ptr create_list(void);
Node_ptr create_node(char_ptr key, char_ptr value);

Node_ptr get_node_of(List_ptr list, char_ptr key);
char_ptr get_value_of(List_ptr list, char_ptr key);
void add_to_list(List_ptr list, char_ptr key, char_ptr value);
void print_list(List_ptr list);
void print_matching_key(List_ptr list, char_ptr key);

#endif
