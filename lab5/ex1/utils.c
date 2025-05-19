#include <utils.h>

typedef struct {
   int size;
   int consonants;
   int vowels;
   int spaces;
} msg_info;

int is_vowel(char chr){
   char vowels[10] = "AaEeIiOoUu";
   for(int i = 0; i < 10; i++){
      if(chr == vowels[i])
      return 1;
   }
   return 0;
}

msg_info get_string_info(char* string){
   int size = strlen(string);
   int consonants = 0;
   int vowels = 0;
   int spaces = 0;
   char chr = string[0];
   for(int i = 0; chr != '\0'; i++){
      chr = string[i];
      if(chr == 32){
         spaces++;
      } else if((chr > 64 && chr < 91) || (chr > 96 && chr < 123)){
         if(is_vowel(chr))
            vowels++;
         else consonants++;
      }
   }
   
   msg_info info = {
      size, 
      consonants, 
      vowels, 
      spaces
   };

   return info;
}