#include<stdio.h>
#include<stdlib.h>
#define MAX_CODE_LEN 15

typedef struct{
    char *elem;
    int stacksize;
    int top;
}SqStack;       //SqStack是一个字符类型的顺序栈，用于存储哈夫曼编码

void InitStack(SqStack *S);         //初始化栈
void Push(SqStack *S, char ch);     //入栈
void Pop(SqStack *S);               //出栈

void InitStack(SqStack *S)
{
    S->elem = (char *)malloc(sizeof(char) * MAX_CODE_LEN);
    S->stacksize = MAX_CODE_LEN;
    S->top = -1;        //初始化时为空栈
}

void Push(SqStack *S,char ch)
{
    if(S->top==S->stacksize-1){
        printf("///FULL STACK!!!///");
        return;
    }
    S->elem[++S->top] = ch;
}

void Pop(SqStack *S)
{
    if(S->top==-1){
        printf("///EMPTY STACK!!!///");
        return;
    }
    S->elem[S->top--] = '\0';
}