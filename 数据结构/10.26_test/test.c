/*借助作业里的一题，测试函数调用的功能*/

#include <stdio.h>
#include <stdlib.h>
#include <queue.h>
#define ArrayNum 10
#define MAX_QUEUE_SIZE 100

//函数功能：实现将数组A分成两部分，左侧均比A[0]小，右侧均比A[0]大

/*typedef struct {
    int *elem;          //int类型指针
    int queuesize;      //分配的初始数组长度
    int front;
    int rear;
} Queue;
*/

void function(int A[], int B[], int n);     //函数声明

void main()
{
    int A[ArrayNum] = {0};
    int B[ArrayNum] = {0};
    int i;
    printf("input A element: ");
    for (i = 0; i < ArrayNum; i++)
        scanf("%d", &A[i]);
    function(A, B, ArrayNum);
    printf("\ntest done.\n");
}

void function(int A[],int B[],int n)
{
    Queue Q1, Q2, Q3;
    InitQueue(&Q1);
    InitQueue(&Q2);
    InitQueue(&Q3);
    int i;      //循环变量
    for (i = 0; i < n;i++){
        if(A[i]<A[0])
            EnQueue(&Q1, A[i]);
        else if(A[i]==A[0])
            EnQueue(&Q3, A[i]);
        else if(A[i]>A[0])
            EnQueue(&Q2, A[i]);
    }
    int L1, L2, L3;
    L1 = QueueLength(Q1);
    L2 = QueueLength(Q2);
    L3 = QueueLength(Q3);
    for (i = 0; i < L1;i++)
        DeQueue(&Q1, &B[i]);
    for (i = L1; i < L1 + L3;i++)
        DeQueue(&Q3, &B[i]);
    for (i = L1 + L3; i < L1 + L3 + L2;i++)
        DeQueue(&Q2, &B[i]);
    for (i = 0; i < n;i++)
        printf("%4d", B[i]);
}