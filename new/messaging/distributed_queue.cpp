#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

void* receive_messages(void *thread_id);
void consumirMensagem(MessageQueue *queue, int consumerIndex);
MessageQueue *queue;

int main(int argc, char **argv) {
    int n = atoi(argv[1]);

    // Criar Fila
    queue = (MessageQueue *) mmap(NULL, sizeof(MessageQueue), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (queue == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    queue_init(queue);
    
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        // Processo Filho - Consumidor
        if (pid == 0) {
            while(1) {
                consumirMensagem(queue, i);
            }
        }
    }

    int receivingSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (receivingSocket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    sockaddr_in receivingAddr;

    receivingAddr.sin_family = AF_INET;
    receivingAddr.sin_port = htons(8889);
    receivingAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(receivingSocket, (struct sockaddr*)&receivingAddr, sizeof(receivingAddr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(receivingSocket, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Ouvindo porta 8879...\n";
    
    int producerSocket = accept(receivingSocket, nullptr, nullptr);

    if (producerSocket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    std::cout << "Conectou. \n";
    
    while(1){
        char buffer[1024] = {0};
        int bytesReceived = recv(producerSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            Message msgStruct;
            msgStruct.id = buffer[0] - '0';
            msgStruct.msg = std::string(buffer + 1, bytesReceived - 1);
            if (enqueue(queue, msgStruct)) {
                std::cout << "Colocou mensagem na fila para: " << msgStruct.id << ".\n";
            }
        }
    }

    return 0;
}

void consumirMensagem(MessageQueue *queue, int consumerIndex) {
    std::stringstream filename;
    filename  << "msgs";
    std::ofstream outfile(filename.str(), std::ios::app);

    Message *message;
    if (!queue_empty(queue) && queue->messages[queue->head].id == consumerIndex) {
        std::cout << "Tem mensagem para" << consumerIndex << ".\n";
        if(dequeue(queue, message)){
            std::cout << "Consumiu a mensagem certa " << message->msg << ".\n";
            outfile << "Consumidor " << consumerIndex << " recebeu a mensagem:\n";
            outfile << (*message).msg << "\n";
            outfile.flush();
        }
        
    }
}