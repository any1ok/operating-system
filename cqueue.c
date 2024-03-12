#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"



struct cqueue {
    int procidx[QSIZE];
    int head;
    int tail;
};

struct cqueue mlfq[3];

// mlfq의 idx번째 큐가 비어있는지 확인하는 함수
int isEmpty( int lev) {
    return mlfq[lev].head == mlfq[lev].tail;
}

// mlfq의 idx번째 큐가 가득 찼는지 확인하는 함수
int isFull( int lev) {
    return (mlfq[lev].tail + 1) % QSIZE == mlfq[lev].head;
}

// mlfq의 idx번째 큐에 value를 삽입하는 함수
int enqueue( int lev, int idx) {
    if (isFull(lev) || idx == -1) { // 큐가 가득 찬 경우
        return -1;
    }
    if(!isEmpty(lev)){
        for(int i=0;i<QSIZE;i++){
            if(mlfq[lev].procidx[i] == idx){
                return -1;
            }
        }
    }

    if (lev == 2) {
    int priority = getPriority(idx);
    int i;
    for (i = mlfq[lev].head; i != mlfq[lev].tail; i = (i + 1) % QSIZE) {
        if (getPriority(mlfq[lev].procidx[i]) > priority) {
            break;
        }
    }
    int insert_idx = i;//(i - 1 + QSIZE) % QSIZE;
    mlfq[lev].tail = (mlfq[lev].tail + 1) % QSIZE;
    for (i = mlfq[lev].tail; i != insert_idx; i = (i - 1 + QSIZE) % QSIZE) {
        mlfq[lev].procidx[i] = mlfq[lev].procidx[(i - 1 + QSIZE) % QSIZE];
    }
    mlfq[lev].procidx[insert_idx] = idx;
    }

    mlfq[lev].procidx[mlfq[lev].tail] = idx;
    mlfq[lev].tail = (mlfq[lev].tail + 1) % QSIZE;
    return idx;
}

// int enqueue( int lev, int idx) {
//     if (isFull(0) ) { // 큐가 가득 찬 경우
//         return -1;
//     }
//     if(!isEmpty(0)){
//         for(int i=0;i<QSIZE;i++){
//             if(mlfq[lev].procidx[i] == idx){
//                 return -1;
//             }
//         }
//     }

//     mlfq[lev].procidx[mlfq[lev].tail] = idx;
//     mlfq[lev].tail = (mlfq[lev].tail + 1) % QSIZE;
//     return idx;
// }

// mlfq의 idx번째 큐에서 값을 제거하고 반환하는 함수
int dequeue( int lev) {
    if (isEmpty(lev)) { // 큐가 비어있는 경우
        return -1;
    }

    int idx = mlfq[lev].procidx[mlfq[lev].head];
    mlfq[lev].procidx[mlfq[lev].head] = -1;
    mlfq[lev].head = (mlfq[lev].head + 1) % QSIZE;
    return idx;
}


int
getIdx()
{
  
  if (!isEmpty(0)){
    return dequeue(0);
  }
  else if(!isEmpty(1)){
    return dequeue(1);
  }
  else if(!isEmpty(2)){
    return dequeue(2);
  }
  else{
    return -1;
  }
}
