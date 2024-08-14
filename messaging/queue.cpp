#include "queue.h"
#include <iostream>

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
