// - Descrição:............ Codigo que lê mensagens de um FIFO e exibe na tela junto com
//                               o número de vogais, consoantes e espaços
// - Autor:................ André Felipe Baretta
// - Data de Criação:...... 17/05/2025
// - Data de Atualização:.. 18/05/2025

#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include "./utils.h"

#define SERVER_FIFO "/tmp/serverfifo"
#define BUFFER_SIZE 512

int main(int argc, char **argv){
   msg_info string_info;
   int fd_server, num_bytes_read;
   char* buf = malloc(BUFFER_SIZE*sizeof(char));

   if((mkfifo(SERVER_FIFO, 0664) == -1) && (errno != EEXIST)){
      perror("mkfifo");
      exit(1);
   }

   if((fd_server = open(SERVER_FIFO, O_RDONLY)) == -1)
      perror("open");

   while(1){
      memset(buf, '\0', BUFFER_SIZE);
      num_bytes_read = read(fd_server, buf, BUFFER_SIZE);
      switch(num_bytes_read){
         case -1:
            perror("-- read error"); 
            break;
         case 0:
            printf("-- None data... close and reopen fifo --\n");
            close(fd_server);
            fd_server = open(SERVER_FIFO, O_RDONLY);
            break;
         default:
            string_info = get_string_info(buf);
            printf("String: %s\nTamanho: %d\nVogais: %d\nConsoantes: %d\nEspaços: %d\n", buf, 
               string_info.size,string_info.vowels,string_info.consonants, string_info.spaces);
      }
   }

   if(close(fd_server) == -1)
      perror("close");
}


