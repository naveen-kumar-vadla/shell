#ifndef __STRING_UTILS_H_
#define __STRING_UTILS_H_

#define FOR_EACH(startAt, endAt) for (int i = startAt; i < endAt; i++)

typedef enum
{
  FALSE,
  TRUE
} BOOL;

typedef char *char_ptr;

char_ptr *split(char_ptr string, char splitBy);
char_ptr *split_string(char_ptr string, char splitBy, int word_count);
int get_count_of_words(char_ptr string, char splitBy);
void copy_string(char_ptr *destination_string, char_ptr source_string, int length);
BOOL includes(char_ptr text, char delimiter);

#endif