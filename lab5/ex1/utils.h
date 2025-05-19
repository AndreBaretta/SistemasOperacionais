#include <string.h>

typedef struct {
   int size;
   int consonants;
   int vowels;
   int spaces;
} msg_info;

int is_vowel(char chr);

msg_info get_string_info(char* string);
