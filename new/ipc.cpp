#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


#define FILENAME "file"

int consumidor(MessageQueue *queue, int consumerIndex) {
    stringstream filename;
    filename  << FILENAME;
    ofstream outfile(filename.str(), ios::app);
    Message *message;
    if (!queue_empty(queue) && queue->messages[queue->head].id == consumerIndex) {
        dequeue(queue, message);
        outfile << "Consumidor " << consumerIndex << " recebeu a mensagem:\n";
        outfile << (*message).msg << "\n";
        outfile.flush();
    }
}

int main(int argc, char **argv) {
    int n = atoi(argv[1]);

    // Criar Fila
    MessageQueue *queue = (MessageQueue *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (queue == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    queue_init(queue);

    // criar fila compartilhada
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        // Processo Filho - Consumidor
        if (pid == 0) {
            while (1) {
                consumidor(queue, i);
            }
        }
    }

    // produtor
    while (1) {
        int id;
        cout << "id: ";
        cin >> id;
        cout << "mensagem: ";
        string msg;
        cin >> msg;
        Message msgStruct;
        msgStruct.id = id;
        msgStruct.msg = msg;
        enqueue(queue, msgStruct);
    }
}