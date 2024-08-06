#include <string>

#define QUEUE_SIZE 10


typedef struct {
  int id;
  std::string msg;
} Message;

typedef struct {
  Message messages[QUEUE_SIZE];
  int head;
  int tail;
  int count;
} MessageQueue;

void queue_init(MessageQueue *queue);
bool queue_empty(MessageQueue *queue);
bool queue_full(MessageQueue *queue);
bool enqueue(MessageQueue *queue, Message message);
bool dequeue(MessageQueue *queue, Message *message);

