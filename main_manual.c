
#include <stdio.h>
#include <string.h>

#define MAX_QUEUE_SIZE 256              // 2^8
#define MASK_QUEUE (MAX_QUEUE_SIZE - 1) // 255 = 0b11111111

static int queue[MAX_QUEUE_SIZE];
static int queueHead = 0;
static int queueTail = 0;

void init_queue(void) {
  memset(queue, 0, sizeof(queue));
  queueHead = 0;
  queueTail = 0;
}

int push_queue(int v) {

  if ((queueHead - queueTail) >= MAX_QUEUE_SIZE) {
    return 0; // queue overflow
  }

  if (queueHead > MAX_QUEUE_SIZE) {
    queueHead = 0;
  }

  queue[queueHead] = v;

  queueHead++;
  return 1;
}

int pop_queue(int *out) {
  if (queueHead == queueTail) {
    return 0;
  }

  if (queueTail > MAX_QUEUE_SIZE) {
    queueTail = 0;
  }

  *out = queue[queueTail];
  queueTail++;
  return 1;
}

int main(void) {

  init_queue();

  for (int k = 0; k < 257; k++) {
    push_queue(k + 10);
  }

  int queue_item;

  for (int i = 0; i < 257; i++) {
    if (pop_queue(&queue_item)) {
      printf("index %d -> value %d\n", (queueTail - 1) & MASK_QUEUE,
             queue_item);
    } else {
      printf("empty queue index: %d \n", queueTail & MASK_QUEUE);
    }
  }

  printf("manual checks \n");
  return 0;
}
