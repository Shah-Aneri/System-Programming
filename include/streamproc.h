#include<future.h>


typedef struct future {
  //sid32 spaces;
  //sid32 items;
 // sid32 mutex;
  int head;
  int tail;
  struct data_elements *queue;
   future_mode_t mode;
    int count;
    int nelems;
    qid16 set_queue;
    qid16 get_queue;
}future;

typedef struct data_elements {
  int time;
  int value;
} des;

void stream_consumer_future(int id, future *f);