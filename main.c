
#include <stdio.h>
#include <string.h>

// max size must be a power of 2
#define MAX_QUEUE_SIZE 256              // 2^8
#define MASK_QUEUE (MAX_QUEUE_SIZE - 1) // 255 = 0b11111111

static int queue[MAX_QUEUE_SIZE];
static int queueHead = 0;
static int queueTail = 0;

// documenting this trick for whenever I need to:
// create an in memory queue and have its push functionality wrap to the
// beginning of its memory when maxxed out seems like a pretty common thing for
// a queue to do. and a fantastic way of managing the memory of that queue.

void init_queue(void) {
  memset(queue, 0, sizeof(queue));
  queueHead = 0;
  queueTail = 0;
}

// pushing moves the head forward
int push_queue(int v) {
  // difference between queueHead and queueTail must not be greater than the
  // possible queue size the actual value of queueHead or tail could be any
  // number greater than 256. keeps increasing each read. possible head and tail
  // tracking ints overflow at 2 billion queue items :)

  if ((queueHead - queueTail) >= MAX_QUEUE_SIZE) {
    return 0; // queue overflow
  }


  // 	bitwise AND speed/safety trick -----------
  queue[queueHead & MASK_QUEUE] = v;
  //
  //	bitwise AND is used here to index the next element in the queue
  //	regardles of the value of queue head.
  //	it will index to the beginning of the queue neatly around on each overflow.
  //
  //    ----------------------------------------
  //	(257 is one more than the size of the queue and would usually cause an
  //	overflow)
  //
  //	*257*
  //	queueHead  = 0000 0001 1111 1111
  //
  //	*255*
  //    MASK_QUEUE = 0000 0000 1111 1111
  //
  //    *257 & 255*
  //    i          = 0000 0000 0000 0000	*resulting index is 0, the start of the array :) *
  //
  //    ----------------------------------------
  //	----------------------------------------
  //
  //	*5*
  //	queueHead  = 0000 0000 0000 0101
  //
  //	*255*
  //    MASK_QUEUE = 0000 0000 1111 1111
  //
  //    *5 & 255*
  //    i          = 0000 0000 0000 0101	*unchanged*
  //
  //    ----------------------------------------
  //	----------------------------------------
  //
  //	*1234*
  //	queueHead  = 0000 0100 1101 0010
  //
  //	*255*
  //    MASK_QUEUE = 0000 0000 1111 1111
  //
  //    *1234 & 255*
  //    i          = 0000 0000 1101 0010	*resulting index is 210*
  //
  //    ----------------------------------------
  //
  //--------------------------------------------

  queueHead++;
  return 1;
}

int pop_queue(int *out) {
  if (queueHead == queueTail) {return 0;}

  //same index trick
  int i = queueTail & MASK_QUEUE;
  *out = queue[i];
  queueTail++;
  return 1;
}

int main(void) {
  init_queue();
   
  for (int k = 0; k < 257; k++) {
    push_queue(k + 10);
  }

  int queue_item;

  //this should print all 256 queue items and then cut back to the first element in the queue 
  //(which should be empty, because on the first pass: pop_queue removed the item)
  //ready for re-population.
  for (int i = 0; i < 257; i++) {
    if (pop_queue(&queue_item)) {
      printf("index %d -> value %d\n",
      //remember to -1 to index. popping moves tail index to next element.
      (queueTail - 1) & MASK_QUEUE, queue_item); 
    } else { printf("empty queue index: %d \n", queueTail & MASK_QUEUE); }
  }

  return 0;
}
