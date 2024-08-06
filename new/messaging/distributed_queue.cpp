#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

void* receive_messages(void *thread_id);

MessageQueue queue = {{}, 0, 0, 0};

int main(int argc, char **argv) {
    pthread_t threads[1];
    
    // Chamar thread para receber mensagens
    pthread_create(&threads[0], NULL, receive_messages, (void *)0);

    pthread_join(threads[0], NULL);

    return 0;
}

// Função para receber mensagens dos produtores e distribuir para os consumidores
void* receive_messages(void *thread_id){
    int receivingSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in receivingAddr;

    receivingAddr.sin_family = AF_INET;
    receivingAddr.sin_port = htons(8879);
    receivingAddr.sin_addr.s_addr = INADDR_ANY;

    bind(receivingSocket, (struct sockaddr*)&receivingAddr, sizeof(receivingAddr));
    listen(receivingSocket, 5);

    int producerSocket = accept(receivingSocket, nullptr, nullptr);
    
    while(1){
        char buffer[1024] = {0};
        recv(producerSocket, buffer, sizeof(buffer), 0);
        std::cout << buffer << std::endl;
    }
}