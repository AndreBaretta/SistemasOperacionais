// - Descrição:............ Este programa realiza a soma de dois vetores inteiros utilizando processos filhos 
//                          para dividir e paralelizar o trabalho. Os dados dos vetores são armazenados em memória compartilhada, 
//                          permitindo que todos os processos acessem e modifiquem os mesmos dados. A divisão do trabalho é feita através de pipes, 
//                          pelos quais o processo pai envia a cada filho um intervalo de índices que ele deve processar. 
//                          O número de elementos dos vetores e a quantidade de processos filhos são informados pelo usuário.
// // - Autor:................ Henry Meneguini Farias
// // - Data de Criação:...... 17/05/2025
// - Data de Atualização:.. 18/05/2025

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//Chave para a memória compartilhada
#define KEY 5678
//Struct do tipo intervalo, onde o array começa e termina.
typedef struct Interval {
    int start;
    int end;
} Interval;

//O primeiro e segundo argumento é o número de elementos e o número de processos.
int main(int argc, char **argv) {
    //Caso não tenha 3 argumentos, sai.
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_elements> <num_processes>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Define o número de elementos e de processos.
    int num_elements = atoi(argv[1]);
    int num_process = atoi(argv[2]);

    int size_per_child = num_elements / num_process;

    // Cria memória compartilhada.
    int shm_v1   = shmget(KEY, num_elements * sizeof(int), IPC_CREAT | 0666);
    int shm_v2   = shmget(KEY + 1, num_elements * sizeof(int), IPC_CREAT | 0666);
    int shm_v3   = shmget(KEY + 2, num_elements * sizeof(int), IPC_CREAT | 0666);
    int shm_done = shmget(KEY + 3, num_process   * sizeof(int), IPC_CREAT | 0666);
    
    //Falha ao criar, sai.
    if (shm_v1 < 0 || shm_v2 < 0 || shm_v3 < 0 || shm_done < 0) {
        perror("Shared memory creation failed");
        exit(1);
    }

    //Atribui a memória compartilhada.
    int *v1 = shmat(shm_v1, NULL, 0);
    int *v2 = shmat(shm_v2, NULL, 0);
    int *v3 = shmat(shm_v3, NULL, 0);
    int *done = shmat(shm_done, NULL, 0);

    // Inicializa v1 e v2.
    for (int i = 0; i < num_elements; i++) {
        v1[i] = i;
        v2[i] = i * 2;
    }

    memset(done, 0, num_process * sizeof(int));

    // Cria pipes.
    int pipes[num_process][2];
    for (int i = 0; i < num_process; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Error creating pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Cria processos filhos.
    for (int i = 0; i < num_process; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Failed to create child process");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Filho
            close(pipes[i][1]); // Fecha o "escrever" do pipe
            Interval intervalo;
            read(pipes[i][0], &intervalo, sizeof(Interval));
            close(pipes[i][0]);

            int *v1d = shmat(shm_v1, NULL, 0);
            int *v2d = shmat(shm_v2, NULL, 0);
            int *v3d = shmat(shm_v3, NULL, 0);
            int *doned = shmat(shm_done, NULL, 0);

            for (int j = intervalo.start; j < intervalo.end; j++) {
                v3d[j] = v1d[j] + v2d[j];
            }

            doned[i] = 1;

            //Libera memória
            shmdt(v1d);
            shmdt(v2d);
            shmdt(v3d);
            shmdt(doned);
            exit(0);

        } else { // pai
            close(pipes[i][0]); // Fecha o "ler" do pipe
            Interval intervalo;
            intervalo.start = i * size_per_child;
            intervalo.end = (i == num_process - 1) ? num_elements : intervalo.start + size_per_child;
            write(pipes[i][1], &intervalo, sizeof(Interval));
            close(pipes[i][1]);
        }
    }

    // Espera pelos filhos
    for (int i = 0; i < num_process; i++) {
        wait(NULL);
    }

    // Imprime os resultados
    printf("Resultado da soma:\n");
    for (int i = 0; i < num_elements; i++) {
        printf("V3[%d] = %d\n", i, v3[i]);
    }

    // Libera memória
    shmdt(v1);
    shmdt(v2);
    shmdt(v3);
    shmdt(done);
    shmctl(shm_v1, IPC_RMID, NULL);
    shmctl(shm_v2, IPC_RMID, NULL);
    shmctl(shm_v3, IPC_RMID, NULL);
    shmctl(shm_done, IPC_RMID, NULL);

    return 0;
}

