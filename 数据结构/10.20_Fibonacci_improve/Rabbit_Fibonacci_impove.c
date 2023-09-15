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

int K,P,n;
//成熟立刻生，生完马上死
typedef struct NODE{
	int t;//寿命
	struct NODE *next;
}NODE;

int main() {
    printf("Please input Mature_time:");
    scanf("%d", &K);
    printf("Please input Death_time:");
    scanf("%d", &P);
    printf("Please input Month_Num to compute:");
    scanf("%d", &n);

	NODE *p=(NODE *)malloc(sizeof(NODE)),*q=(NODE *)malloc(sizeof(NODE));
	NODE *pHead=pHead=(NODE *)malloc(sizeof(NODE));
	int i,bN=0,dN=0,N=0;
	pHead->t=0;
	pHead->next=NULL;
	for(i=1;i<=n;i++){
		bN=0,dN=0;
		//printf("Month: %d ",i);
		N=0;
		p=pHead;
		q=p;
		while (p!= NULL) {
			if(p->t<K+P) N++;
			p->t++;
			if((p->t>K)&&(p->t<=K+P)){
				NODE *pnewnode =(NODE *)malloc(sizeof(NODE));
				pnewnode->next=pHead;
				pnewnode->t=1;
				pHead=pnewnode;
				bN++;
				N++;
			}
			
			if(p->t==K+P){
				dN++;
				N--;
			}
			q=p;
			p=p->next;
		}
        printf("The %dth month has %d rabbits\n", i,N);
        printf("The %dth month has %d rabbits born\n", i,bN);
        printf("The %dth month has %d rabbits dead\n", i,dN);
        printf("\n");
	}

        printf("Count done.\n");
}