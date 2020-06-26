#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "string_utils.h"

int get_count_of_words(char *string, char splitBy)
{
  int word_count = 1;
  FOR_EACH(0, strlen(string))
  {
    if (string[i] == splitBy)
    {
      word_count++;
    }
  }
  return word_count;
}

void copy_string(String *destination_string, char *source_string, int length)
{
  *destination_string = malloc(sizeof(char) * length);
  strncpy(*destination_string, source_string, length);
}

String *split_string(char *string, char splitBy, int word_count)
{
  String *array_of_strings = malloc(sizeof(String) * word_count);
  int strings_count = 0;
  int pointer_to_string = 0;
  FOR_EACH(0, strlen(string))
  {
    if (string[i] == splitBy)
    {
      copy_string(&array_of_strings[strings_count], &string[pointer_to_string], i - pointer_to_string);
      pointer_to_string = i + 1;
      strings_count++;
    }
  }
  copy_string(&array_of_strings[strings_count], &string[pointer_to_string], strlen(string) - pointer_to_string);
  array_of_strings[strings_count + 1] = NULL;
  return array_of_strings;
}

String *split(char *string, char splitBy)
{
  int wordCount = get_count_of_words(string, splitBy);
  return split_string(string, splitBy, wordCount + 1);
}

BOOL includes(char *text, char delimiter)
{
  BOOL is_found = FALSE;
  for (int i = 0; i < strlen(text); i++)
  {
    if (text[i] == delimiter)
    {
      is_found = TRUE;
    }
  }
  return is_found;
}
