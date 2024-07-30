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

#define QUEUE_SIZE 10
#define FILENAME "file"

typedef struct {
  int id;
  string msg;
} Message;

typedef struct {
  Message messages[QUEUE_SIZE];
  int head;
  int tail;
  int count;
} MessageQueue;

void queue_init(MessageQueue *queue) {
  queue->head = 0;
  queue->tail = 0;
  queue->count = 0;
}

bool queue_empty(MessageQueue *queue) {
  return queue->head == queue->tail;
}

bool queue_full(MessageQueue *queue) {
  return queue->count == QUEUE_SIZE;
}

bool enqueue(MessageQueue *queue, Message message) {
  if (queue_full(queue)) {
    return false;
  }

  queue->messages[queue->tail] = message;
  queue->tail = (queue->tail + 1) % QUEUE_SIZE;
  queue->count++;
  return true;
}

bool dequeue(MessageQueue *queue, Message *message) {
  if (queue_empty(queue)) {
    return false;
  }

  *message = queue->messages[queue->head];
  queue->head = (queue->head + 1) % QUEUE_SIZE;
  queue->count--;
  return true;
}

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