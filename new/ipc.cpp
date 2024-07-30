#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

int consumidor() {
    while (1) {
        
    }
}

int main(int argc, char **argv) {
    int n = atoi(argv[1]);

    // criar fila compartilhada
    pid_t pid = -1;
    for (int i = 0; i < n; i++) {
        // Processo Pai
        if (pid != 0) {
            pid = fork();
            // Processo Filho - Consumidor
            if (pid == 0) {
                consumidor()
            }
        }
    }
}