#include "utils.h"

int get_count_of_words(char_ptr string, char splitBy)
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

void copy_string(char_ptr *destination_string, char_ptr source_string, int length)
{
  *destination_string = malloc(sizeof(char) * length);
  strncpy(*destination_string, source_string, length);
}

char_ptr *split_string(char_ptr string, char splitBy, int word_count)
{
  char_ptr *array_of_strings = malloc(sizeof(char_ptr) * word_count);
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

char_ptr *split(char_ptr string, char splitBy)
{
  int wordCount = get_count_of_words(string, splitBy);
  return split_string(string, splitBy, wordCount + 1);
}

BOOL includes(char_ptr text, char delimiter)
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

BOOL includes_array(char_ptr *text, char delimiter)
{
  BOOL is_found = FALSE;
  int index = -1;
  int i = 0;
  while (!is_found && (text[i]))
  {
    is_found = includes(text[i], delimiter);
    if (is_found)
    {
      index = i;
    }
    i++;
  }

  return index;
}
