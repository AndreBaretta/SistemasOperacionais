#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdio.h>

char *socket_path = "\0socket";

typedef struct {
   char* portugues;
   char* english;
   char* deutsche;
} word;

word words[] = {
   {"cachorro", "dog", "hund"},
   {"peixe", "fish", "fisch"},
   {"lagoa", "pond", "teich"},
   {"palavra", "word", "wort"},
   {"ombro", "shoulder", "schulter"}
};

int dictionary_size = sizeof(words) / sizeof(words[0]);

char* translate(char* input);

int main(int argc, char** argv){
   int server_socket,
       client_socket,
       received_bytes;
   struct sockaddr_un addr;
   char buf[100];

   unlink(socket_path);

   if((server_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
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
      unlink(socket_path);
   }

   if(bind(server_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1){
      perror("bind error");
      exit(-1);
   }

   if(listen(server_socket, 5) == -1){
      perror("listen error");
      exit(-1);
   }

   printf("servidor escutando...\n");

   while(1){
      if((client_socket = accept(server_socket, NULL, NULL)) == -1){
         perror("accept error");
         continue;
      }

      if(received_bytes == -1){
         perror("read");
         exit(-1);
      } else if(received_bytes == 0){
         printf("EOF\n");
         close(client_socket);
      }
      
      while((received_bytes = read(client_socket, buf, sizeof(buf))) > 0){
         pid_t pid = fork();
         if (pid == 0) { // processo filho
            // lê uma mensagem e responde
            buf[received_bytes] = '\0';
            if (received_bytes > 0) {
               buf[received_bytes] = '\0';
               printf("[Filho %d] Tradução para: %s\n", getpid(), buf);

               char* resultado = translate(buf);
               if (resultado != NULL) {
                  printf("tradução: %s\n", resultado);
                  write(client_socket, resultado, strlen(resultado));
               } else {
                  char* erro = "ERROR:UNKNOWN\n";
                  write(client_socket, erro, strlen(erro));
               }
            }
            close(client_socket);
            exit(EXIT_SUCCESS);
         }
      }
      close(client_socket);
   }

   return 0;
}

char* translate(char* input){
   // Copia a string de entrada
   char buffer[100];
   strncpy(buffer, input, sizeof(buffer));
   buffer[sizeof(buffer)-1] = '\0';

   // Divide em partes: idioma_origem-idioma_destino:palavra
   char* languages = strtok(buffer, ":");
   char* word = strtok(NULL, ":");
   word[strcspn(word, "\n")] = '\0';

   if (!languages || !word) return NULL;

   char* origin = strtok(languages, "-");
   char* goal = strtok(NULL, "-");

   if (!origin || !goal) return NULL;

   if (strcmp(origin, "pt") == 0) {
      for (int i = 0; i < dictionary_size; i++) {
         if (strcmp(words[i].portugues, word) == 0) {
            if(strcmp(goal, "en") == 0){
               return words[i].english;
            }
            else if(strcmp(goal, "de") == 0){
               return words[i].deutsche;
            }
         }
      }
   } else if (strcmp(origin, "en") == 0) {
      for (int i = 0; i < dictionary_size; i++) {
         if (strcmp(words[i].english, word) == 0) {
            if(strcmp(goal, "pt") == 0)
               return words[i].portugues;
            else if(strcmp(goal, "de") == 0)
               return words[i].deutsche;
         }
      }
   } else if (strcmp(origin, "de") == 0) {
      for (int i = 0; i < dictionary_size; i++) {
         if (strcmp(words[i].deutsche, word) == 0) {
            if(strcmp(goal, "en") == 0)
               return words[i].english;
            else if(strcmp(goal, "pt") == 0)
               return words[i].portugues;
         }
      }
   }

   return NULL; // se não encontrou a palavra ou o idioma
   
}