#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define QUEUE_SIZE 10

typedef struct {
  int id;
  char message[100];
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
  return queue->count == 0;
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

int main() { 
  MessageQueue queue;
  queue_init(&queue);

  Message message;
  message.id = 1;
  strcpy(message.message, "Hello, World!");

  enqueue(&queue, message);

  Message dequeued_message;
  dequeue(&queue, &dequeued_message);

  printf("Message id: %d\n", dequeued_message.id);
  printf("Message: %s\n", dequeued_message.message);

  return 0;
}