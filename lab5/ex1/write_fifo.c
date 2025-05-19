// - Descrição:............ Codigo que escreve mensagens fifo digitadas pelo usuário
// - Autor:................ André Felipe Baretta
// - Data de Criação:...... 17/05/2025
// - Data de Atualização:.. 18/05/2025

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define SERVER_FIFO "/tmp/serverfifo"

int main(int argc, char** argv){
   int fd_server;
   char* buffer = malloc(512*sizeof(char));

   if((fd_server = open(SERVER_FIFO, O_WRONLY)) == -1){
      perror("open error: server fifo");
      return 1;
   }

   while(1){
      if (fgets(buffer, 512, stdin) == NULL) break;
      buffer[strcspn(buffer, "\n")] = 0;
      write(fd_server, buffer, strlen(buffer));
   }

   if(close(fd_server) == -1){
      perror("close error: server fifo");
      return 2;
   }

   return 0;
}