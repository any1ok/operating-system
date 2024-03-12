struct cqueue {
  int procidx[QSIZE];
  int head;
  int tail;
};


// check state of the queue
int isEmpty(int);
int isFull(int);
// return the input value on success or -1 on failure
int enqueue(int, int);
// return the head of the queue on success or -1 on failure
int dequeue(int);
int getIdx();
