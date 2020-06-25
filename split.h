#ifndef __SPLIT_H_
#define __SPLIT_H_

#define FOR_EACH(startAt, endAt) for (int i = startAt; i < endAt; i++)

typedef char *String;

String *split(char *string, char splitBy);
String *split_string(char *string, char splitBy, int word_count);
int get_count_of_words(char *string, char splitBy);
void copy_string(String *destination_string, char *source_string, int length);

#endif