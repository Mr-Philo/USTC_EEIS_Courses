#include<stdio.h>
#include<stdlib.h>
#include<queue.h>

void InitQueue(Queue *Q)    //初始化循环队列
{
    Q->elem = (int *)malloc(MAX_QUEUE_SIZE * sizeof(int));
    Q->queuesize = MAX_QUEUE_SIZE;
    Q->front = Q->rear = 0;     //front和rear均为0时，队列为空
}

void EnQueue(Queue *Q, int e)    //将e插入队列成为新的队尾元素
{
    if((Q->rear+1)%Q->queuesize==Q->front){       //如果队列已满
        printf("//FULL QUEUE!//");
        exit(-1);
    }
    Q->elem[Q->rear] = e;       //循环队列队尾赋值为e，即入队列
    Q->rear = (Q->rear + 1) % Q->queuesize;     //队列尾指针加一，注意循环问题
}

void DeQueue(Queue *Q, int *e)     //若队列非空，则删除Q的队首元素并赋给e
{
    if(Q->rear==Q->front){      //如果队列为空
        printf("//EMPTY QUEUE!//");
        exit(-1);
    }
    *e = Q->elem[Q->front];      //队首元素出队列并赋给e
    Q->front = (Q->front + 1) % Q->queuesize;      //队列头指针减一，注意循环问题
}

int QueueLength(Queue Q)       //返回队列长度
{
    return ((Q.rear + Q.queuesize - Q.front) % Q.queuesize);
}