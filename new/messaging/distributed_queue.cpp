#include <queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

MessageQueue queue = {{}, 0, 0, 0};

int main(int argc, char **argv) {
    pthread_t threads[2];
    
    // Chamar thread para receber mensagens
    pthread_create(&threads[0], NULL, receive_messages, (void *)0);

    // Chamar thread para distribuir mensagens
    pthread_create(&threads[1], NULL, receive_messages, (void *)1);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}

// Função para receber mensagens dos produtores
void* receive_messages(void *thread_id){
    while(1){

    }
}

// Função para distribuir mensagens para os consumidores
void* distribute_messages(void *thread_id){
    while(1){

    }
}