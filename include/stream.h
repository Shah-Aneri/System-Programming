typedef struct stream {
  sid32 spaces;
  sid32 items;
  sid32 mutex;
  int head;
  int tail;
  struct data_element *queue;
}stream;

typedef struct data_element {
  int time;
  int value;
} de;

void stream_consumer(int id, struct stream *str);