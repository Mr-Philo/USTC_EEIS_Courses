 /*第⼀次上机实验 Fibonacci sequence
1. 编程实现兔⼦问题，历史上有⼀个有名的关于兔⼦的问题：假设有⼀对兔⼦，⻓两个月它们就算
⻓⼤成年了。然后以后每个月都会⽣出1对兔⼦，⽣下来的兔⼦也都是⻓两个月就算成年，然后每
个月也都会⽣出1对兔⼦了。这⾥假设兔⼦不会死，每次都是只⽣1对兔⼦，实现计算第20个（第
15）月的兔⼦对数
2. 加入条件，每对兔⼦⽣完2次（N次）兔⼦后会死亡，实现计算第20个（第15）月的兔⼦对数
3. 隔⼏个月可以繁殖设置为参数K, 繁殖⼏代后死亡设置成参数P,计算第N个月的兔⼦对数
*/

#include <stdio.h>
#include <stdlib.h>

//直接进行实验3的编写，对于1设参数K=2，P=0，N=20（15）即可；2设K=2，P=0，N=20（15）即可
//注释10.13：长大那个月立刻生，生完第P代立刻死，且不计入那个月的综述

typedef struct Rabbit{
    int *state;     //指示兔子当前状态（幼年/成年/死亡）
    struct Rabbit *next;    //链表中指向下一节点的指针
} Ra;

void countRabbit(int k, int p, int n);

void main()
{
    int k, p, n;
    printf("Please input Mature_time:");
    scanf("%d", &k);
    printf("Please input Death_time:");
    scanf("%d", &p);
    printf("Please input Month_Num to compute:");
    scanf("%d", &n);
    countRabbit(k, p, n);
    printf("Count done.\n");
}

void countRabbit(int k, int p, int n)
{
    int i,j;  //循环变量
    int sum, count_birth, count_death;
    int S[100];     
    //State数组从State[0]开始到State[k]赋值0，表示从第0个月到第k个月兔子状态为幼年
    //从State[k+1]开始到State[k+p]赋值1，表示从第k个月到第k+p个月兔子状态为成年
    //再将State[k+p+1]赋值为-1，表示兔子死亡
    if(p>100){      //即几乎不考虑兔子死亡的情况
        for (i = 0; i <= k; i++)
            S[i] = 0;
        for (i = k+1; i < 100;i++)
            S[i] = 1;
    }
    else {
        for (i = 0; i <= k; i++)
            S[i] = 0;
        for (i = k+1; i <= k + p;i++)
            S[i] = 1;
        S[k + p + 1] = -1;
    }

    Ra *pHead = NULL;   //定义头指针
    Ra *pl,*q,*temp,*pll = NULL;   //定义临时指针
    //初始化第一对兔子
    pl = (Ra*)malloc(sizeof(Ra));
    pl->state = S;  //初始兔子指针指向S数组最开始
    pl->next = NULL;    //为防止指针指向不明，新建立的结点next指针均指向NULL，很重要！！！
    pHead = pl;
    pll = pHead;

    if(n>100){
        printf("//Too Much Number to count//");
        return;
    }
    for (i = 1,sum=1; i <= n;i++)   //i循环为月份循环
    {
        pl = pHead;  //从链表头开始遍历
        q = pl->next;
        for (j = 1,count_birth=0,count_death=0; j <= sum; j++)   //j循环为兔子链表的循环，先检查兔子生育死亡的情况
        {
            pl->state++;
            if(*(pl->state)==1){
                temp=(Ra*)malloc(sizeof(Ra));
                temp->state = &S[1];    //如果为1，表示该兔子可以生小兔子，建立新结点
                //temp->next = NULL;
                temp->next = q;     //若此时后继没有元素，则q值应为NULL且赋入
                pl->next = temp;    //小兔子结点插入到大兔子结点后
                count_birth++;  //出生数+1
                //生完后还要判断会不会立刻死
                q = pl->next;
                pl->state++;
                if(*(pl->state++)==(-1)){   //如果下一元素为-1，则该兔子死亡（生完立刻死，不计入当月数量），释放结点空间
                    if(pl==pHead){
                        pHead = q;
                        free(pl);
                        pl = q;
                        q = pl->next;   //如果是头指针的删除方法
                    }
                    //如果是中间的指针
                    else{
                        //while(pll->next!=pl) pll++;     //找到p前面的指针所在的位置
                        pll->next = q;
                        free(pl);
                        pl = pll;
                    }
                    
                    count_death++;    //死亡数+1
                }
                else pl->state--; //复位
                pl = temp;
                q = pl->next;   //在小兔子的位置准备往下一个结点遍历，否则小兔子要被多算一次
            }
            /*if(pl->next!=NULL&&*(pl->next->state)==(-1)){   //如果下一元素为-1，则该兔子死亡（生完立刻死，不计入当月数量），释放结点空间
                if(pl==pHead){
                    pHead = q;
                    free(pl);
                    pl = q;
                    q = pl->next;   //如果是头指针的删除方法
                }
                //如果是中间的指针
                else{
                    //while(pll->next!=pl) pll++;     //找到p前面的指针所在的位置
                    pll->next = q;
                    free(pl);
                    pl = pll;
                }
                count_death++;  //死亡数+1
            }
            */
            //对下一节点的移动统一放在这里进行
            if(pl->next!=NULL){
                pll = pl;   //pll规定为pl的前置指针，方便结点删除来用
                pl = pl->next;  //此处若链表中只有一个结点pl，则pl不动，否则会报错
                if(pl->next!=NULL) 
                    q = pl->next;    //q继续置在后继元素位，注意此时pl->next不能为空，否则q会指向不明
                else
                    q = NULL;
            }
        }
        sum = sum + count_birth - count_death;
        printf("The %dth month has %d rabbits\n", i,sum);
        printf("The %dth month has %d rabbits born\n", i,count_birth);
        printf("The %dth month has %d rabbits dead\n", i,count_death);
        printf("\n");
    }
}