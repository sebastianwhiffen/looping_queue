#include "tracy/public/tracy/TracyC.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

  TracyCZone(ctx, 1);
  TracyCZoneName(ctx, "push name", 0);
  // Add custom text to the zone measurement.
  TracyCZoneText(ctx, "push text", 0);

  if ((queueHead - queueTail) >= MAX_QUEUE_SIZE) {
    return 0;
  }

  queue[queueHead & MASK_QUEUE] = v;
  queueHead++;

  TracyCZoneEnd(ctx);
  return 1;
}

int pop_queue(int *out) {

  TracyCZone(ctx, 1);

  TracyCZoneName(ctx, "pop name", 0);
  // Add custom text to the zone measurement.
  TracyCZoneText(ctx, "pop text", 0);
  // Add custom value to the zone measurement.
  // TracyCZoneValue(ctx, 0);

  if (queueHead == queueTail) {
    return 0;
  }

  *out = queue[queueTail & MASK_QUEUE];

  queueTail++;

  TracyCZoneEnd(ctx);
  return 1;
}

int main(void) {
  TracyCSetThreadName("Main");

  init_queue();

  int p = 1;
  while (1) {
    if (p) {
      for (int k = 0; k < 257; k++) {
        push_queue(k + 10);
      }

    } else {
      int queue_item;

      for (int i = 0; i < 257; i++) {
        if (pop_queue(&queue_item)) {
          // printf("index %d -> value %d\n", (queueTail - 1) & MASK_QUEUE,
          //        queue_item);
        } else {
          // printf("empty queue index: %d \n", queueTail & MASK_QUEUE);
        }
      }
    }
    p = !p;

    usleep(16000);
    TracyCFrameMark;
  }
  printf("bit shifting \n");
  return 0;
}
