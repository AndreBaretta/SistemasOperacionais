// - Descrição:............ Funções auxiliares para o programa do ex1
// - Autor:................ André Felipe Baretta
// - Data de Criação:...... 17/05/2025
// - Data de Atualização:.. 18/05/2025

#include <string.h>

typedef struct {
   int size;
   int consonants;
   int vowels;
   int spaces;
} msg_info;

// Checa se a letra é uma vogal
int is_vowel(char chr);

// Corre pela string e calcula tamanho e numero de: vogais, consoantes e espaços
msg_info get_string_info(char* string);
