#include "list.h"

List_ptr create_list(void)
{
  List_ptr list = malloc(sizeof(List));
  list->first = NULL;
  list->last = NULL;
  return list;
}

Node_ptr create_node(char_ptr key, char_ptr value)
{
  Node_ptr node = malloc(sizeof(Node));
  node->key = key;
  node->value = value;
  node->next = NULL;
  return node;
}

void add_to_list(List_ptr list, char_ptr key, char_ptr value)
{
  Node_ptr new_node = create_node(key, value);
  Node_ptr *ptr_to_set = &list->first;
  if (new_node == NULL)
  {
    return;
  }
  if (list->first != NULL)
  {
    ptr_to_set = &list->last->next;
  }
  (*ptr_to_set) = new_node;
  list->last = new_node;
  list->length++;
}

char_ptr get_value_of(List_ptr list, char_ptr key)
{
  Node_ptr p_walk = list->first;
  char_ptr value = NULL;
  BOOL is_found = FALSE;
  while (p_walk != NULL && !is_found)
  {
    if (strcmp(p_walk->key, key) == 0)
    {
      value = malloc(sizeof(char) * strlen(p_walk->value));
      value = p_walk->value;
      is_found = TRUE;
    }
    p_walk = p_walk->next;
  }

  return value;
}

void print_list(List_ptr list)
{
  Node_ptr p_walk = list->first;
  while (p_walk != NULL)
  {
    printf("%s=%s ", p_walk->key, p_walk->value);
    p_walk = p_walk->next;
  }
  printf("\n");
}

void print_matching_key(List_ptr list, char_ptr key)
{
  if (!key)
  {
    print_list(list);
    return;
  }
  Node_ptr p_walk = list->first;
  while (p_walk != NULL)
  {
    if (strcmp(p_walk->key, key) == 0)
    {
      printf("%s=%s ", p_walk->key, p_walk->value);
    }
    p_walk = p_walk->next;
  }
  printf("\n");
}
