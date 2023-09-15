#include<stdio.h>
#include<stdlib.h>
#define MAX_QUEUE_SIZE 100

typedef struct Person{
    int Remain_Service_Time;   //每位顾客剩余的服务时间
    int Service_Time;       //用于输出用
    int Arrive_Time;    //每位顾客的到达时间
    int Win_Num;       //该顾客的窗口编号
    int Wait_Time;      //每位顾客的等待时间
} Person;

typedef struct Window{
    Person *p;          //Person类型指针
    int queuesize;      //分配的初始数组长度
    int front;
    int rear;
} Win_Queue;

void InitQueue(Win_Queue *Q);   //初始化循环队列
void EnQueue(Win_Queue *Q, Person *e);   //将e插入队列成为新的队尾元素
void DeQueue(Win_Queue *Q);     //若队列非空，则删除Q的队首元素，而且这个程序中不用赋给e了
int QueueLength(Win_Queue Q);       //返回队列长度

void InitQueue(Win_Queue *Q)    //初始化循环队列
{
    Q->p = (Person *)malloc(MAX_QUEUE_SIZE * sizeof(Person));
    Q->queuesize = MAX_QUEUE_SIZE;
    Q->front = Q->rear = 0;     //front和rear均为0时，队列为空
}

void EnQueue(Win_Queue *Q, Person *e)    //将e插入队列成为新的队尾元素
{
    if((Q->rear+1)%Q->queuesize==Q->front){       //如果队列已满
        printf("//FULL QUEUE!//");
        exit(-1);
    }
    Q->p[Q->rear] = *e;       //循环队列队尾赋值为e，即入队列
    Q->rear = (Q->rear + 1) % Q->queuesize;     //队列尾指针加一，注意循环问题
}

void DeQueue(Win_Queue *Q)     //若队列非空，则删除Q的队首元素
{
    if(Q->rear==Q->front){      //如果队列为空
        printf("//EMPTY QUEUE!//");
        exit(-1);
    }
    //*e = Q->elem[Q->front];      //队首元素出队列并赋给e
    Q->front = (Q->front + 1) % Q->queuesize;      //队列头指针减一，注意循环问题
}

int QueueLength(Win_Queue Q)       //返回队列长度
{
    return ((Q.rear + Q.queuesize - Q.front) % Q.queuesize);
}