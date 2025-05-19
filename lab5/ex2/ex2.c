// - Descrição:............ Um programa que manipula arquivos e, ao receber 
//                          o sinal 2 (SIGINT) ou o sinal 15 (SIGTERM), realiza 
//                          uma finalização limpa, armazenando as informações 
//                          pendentes e fechando o arquivo.
// - Autor:................ Henry Meneguini Farias
// // - Data de Criação:...... 17/05/2025
// - Data de Atualização:.. 18/05/2025


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define FILENAME "data.txt"


volatile sig_atomic_t stop = 0;

//Gerencia o sinal.
void handle_signal(int sig){
   stop = 1;
   printf("\nSinal %d recebido, finalizando...\n", sig);
}

//Função que inicializa o gerenciador de sinal.
void setup_signal_handler(){
   struct sigaction sa;
   sa.sa_handler = handle_signal;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   sigaction(SIGINT, &sa, NULL);
   sigaction(SIGTERM, &sa, NULL);
}

int main(){
   char buffer[256];
   int counter = -1;
   //Abre o arquivo no modo append.
   FILE* file = fopen(FILENAME, "a");
   if(file == NULL){
      perror("Erro ao abrir o arquivo");
      exit(EXIT_FAILURE);
   }

   printf("Digite textos para serem gravados em '%s'. Pressione Ctrl+C para sair.\n", FILENAME);
   //Enquanto não houver sinal, continua.
   while (!stop) {
      printf("Entrada %d: ", counter);
      if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
         if (stop) break; // se o sinal ocorreu durante o input
         perror("Erro na leitura");
         continue;
      }

      // Remove newline se presente.
      buffer[strcspn(buffer, "\n")] = '\0';

      // Escreve no arquivo
      fprintf(file, "Linha %d: %s\n", counter, buffer);
      fflush(file); // Garante que os dados sejam gravados imediatamente.
      counter++;
   }

   // Finalização limpa.
   if (file != NULL) {
      printf("[INFO] Fechando o arquivo e encerrando...\n");
      fclose(file);
   }

   return 0;
}
