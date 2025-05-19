#include <sys/socket.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>

char *socket_path = "\0socket";

int main(int argc, char** argv){
   int client_socket, bytes_sent, bytes_received;
   struct sockaddr_un addr;
   char response[100];
   char buf[100];
   
   if((client_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
      perror("socket error");
      exit(-1);
   }

   memset(&addr, 0, sizeof(addr));
   addr.sun_family = AF_UNIX;
   if(*socket_path == '\0'){
      *addr.sun_path = '\0';
      strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
   } else {
      strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
   }

   if(connect(client_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1){
      perror("connect error");
      exit(-1);
   }

   while((bytes_sent = read(STDIN_FILENO, buf, sizeof(buf))) > 0){
      if(write(client_socket, buf, bytes_sent) != bytes_sent){
         if(bytes_sent > 0) fprintf(stderr,"partial write");
         else {
            perror("write error");
            exit(-1);
         }
      }

      // Aguarda a resposta do servidor
      bytes_received = read(client_socket, response, sizeof(response)-1);
      if(bytes_received > 0){
         response[bytes_received] = '\0';  // Termina a string
         printf("Traduzido: %s\n", response);
      } else if (bytes_received == 0){
         printf("Servidor fechou a conexão.\n");
         break;
      } else {
         perror("read error");
         exit(-1);
      }
   }

   return 0;
}
