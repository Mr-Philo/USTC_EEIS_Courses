/*利用循环链表解决约瑟夫环问题，参数n为总人数，m为报数到第m个即出列，k为起始报数处*/

#include<stdio.h>
#include<stdlib.h>
void Joesphus(int n,int m,int k);

int main()
{
    int n, m, k;
    printf("请输入总人数、报数间隔及起始报数处：");
    scanf("%d%d%d", &n, &m, &k);
    Joesphus(n, m, k);
    return (0);
}

void Joesphus(int n,int m,int k)
{
    //函数功能：接收传来的参数，按次序打印出报数后出列的人的编号，并提供出最后m-1个未能出列的人

    typedef struct JNODE{
        int num;
        struct JNODE *next;
    } Jnode;        //结构体定义

    int i;  //循环变量
    Jnode *pHead = NULL;    //定义头指针
    Jnode *p,*q = NULL;        //定义临时指针
    for (i = 1; i <= n; i++)        //链表初始化，其中每一个结点存储从1到n的编号
    {
        p = (Jnode *)malloc(sizeof(Jnode));
        p->num = i;
        if(pHead==NULL)
            pHead = p;      //pHead置于1所在的结点处
        else
            q->next = p;
        q = p;      //尾插法
    }
    q->next = pHead;    //尾部结点n的next指针指向头结点1，形成循环链表

    //p = LocateItem_L(pHead, k);     //链表中查找k的元素，并令p指针指向它

    p=pHead;
    while(p->num!=k){
        q = p;
        p=p->next;      //找到k所在的位置，使p指向它，而q指向它的前驱结点
    }

    int count=0,sum = 0;  //count为计数器，sum为总出列人数
    while(sum<n-m+1){
        count++;    // 先从此结点开始报数
        if(count==m){
            count = 0;
            sum++;
            printf("第%d个出列的序号：%d\n", sum, p->num);
           
            //ListDelete_L(pHead, p);     //删除p结点
            q->next=p->next;
            free(p);
            p=q->next;
        }
        else{
            q = p;
            p = p->next;
        }
    }
    printf("\n剩余未能出列的序号是：");
    for(i = 1;i <= m - 1;i++){
        printf("%4d,", p->num);
        p = p->next;
    }       //遍历打出剩下未能出列的序号
}