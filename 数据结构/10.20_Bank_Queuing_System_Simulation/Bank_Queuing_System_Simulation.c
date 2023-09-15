/*                  \\\\\\第⼆次上机实验——银⾏排队系统模拟\\\\\

·算法输入
    服务窗⼝数量：Service_Num,客户数量：Clients_Num,每个客户有2个主要描述参数，服务时间5~10秒之
间的随机数：Remain_Serve_Time和到达时间：Arrive_Time，其他参数比如每个客户的ID，服务窗⼝的ID，等
待时间等根据⾃⼰的需要添加

·算法输出
    客户平均服务等待时间：Average_Wait_Time

·基本要求
    1. 每⼀秒到达⼀个客户，产⽣随机服务时间和到达时间(当前系统时间也可以⾃⼰定义时间)，然后将
其插入到较短的服务窗⼝中
    2. 每⼀秒对Service_Num个服务队列进⾏处理，每个队头客户已服务时间加1，若已达到其所需的服
务时间，则从队列中删去，同时计算其等待时间Wait_Time
    3. 最后汇总所有的Wait_Time，并计算出客户平均等待时间

·扩展要求
    由于此过程是⼀个累计随机事件发⽣次数的最基本的独立增量过程，也就是⼀个泊松过程，因此此过程
客户的到达或者不到达我们可以⽤泊松过程来模拟。每秒到达客户概率按照泊松分布来计算，实现⽣成
客户到达的数量，然后产⽣随机服务时间和到达时间(当前系统时间也可以⾃⼰定义时间)，并将其插入
到较短服务窗⼝中，其余操作和基本要求中的1和2相同，⽣成服从泊松分布的数据
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<queue.h>
#define MAX_QUEUE_SIZE 100

int Service_Num, Clients_Num;      //全局变量——服务窗口数量、客户数量

//子函数声明
Person PersonInit(Win_Queue W[], int cur_time); 
//初始化顾客函数，参数为当前服务窗口状态，返回值为Person类型变量
int RandomTime();      
//产生一个随机时间
void Serving(Win_Queue W[], Person P[], int cur_time);  
//主体服务函数，参数为窗口集合、顾客集合和当前时间，返回值为空
int Check_Service_Done(Win_Queue W[]);  
//检查是否服务完成函数，参数为当前服务窗口状态，返回值为真假型变量(1/0)
float GetAverage(Person P[]);   
//计算平均时间函数
void ShowResult(Person P[]);
//打印输出各位顾客的结果

void main()
{
    float Average_Wait_Time;       //最终输出的平均等待时间
    int i=0, j=0;       //循环变量
    printf("Please input the Service_Num and the Clients_Num: ");
    scanf("%d%d", &Service_Num, &Clients_Num);

    srand(time(NULL));
    
    Win_Queue W[Service_Num];       //定义结构体数组，表示各个服务窗口的集合
    Person P[Clients_Num];      //定义结构体数组，表示各个客户的集合
    for (i = 0; i < Service_Num; i++)
        InitQueue(&W[i]);   //初始化各个服务窗口

    //S1:若人还没来齐，则产生一名顾客，按照时间T从0开始，每隔一秒来一个
    //并插入最短的服务窗口队列中

    //S2:时间经过一秒，遍历各服务窗口，所有顾客服务时间-1
    //同时检查，若某顾客服务时间到0，则退出队列，同时计算出等待时间

    //S3:注意顾客全部到来之后，服务窗口依然没有服务完
    //需要一个功能函数来检测是否全部服务完。若是，则汇总Wait_time算出平均，输出即可
    //若否，则返回S1

    int cur_time = 0;   //当前时间
    //下面程序中用j表示已经到来的顾客数量
    do{     //大循环，函数Check_Service_Done读取当前服务窗口状态。因为一开始窗口也是没有人的，所以要先do一遍，再while判据
        if(j < Clients_Num){
            P[j]=PersonInit(W,cur_time);   //如果人还没来满，则根据当前窗口形式(参数W)，初始化Person
            EnQueue(&W[P[j].Win_Num-1], &P[j]);
            j++;
        }
        cur_time++;
        Serving(W, P, cur_time); //执行各服务窗口的服务
    } while (Check_Service_Done(W));
    ShowResult(P);
    Average_Wait_Time = GetAverage(P);
    printf("\nThe Average_Wait_Time is %.2f s\n", Average_Wait_Time);
}

Person PersonInit(Win_Queue W[], int cur_time) //初始化顾客函数，参数为当前服务窗口状态，返回值为Person类型变量
{
    Person temp;
    temp.Arrive_Time = cur_time;
    //temp.Service_Time = RandomTime();  //产生一个随机的服务时间
    temp.Service_Time = 5 + rand() % 6;  //产生一个随机的服务时间
    temp.Remain_Service_Time = temp.Service_Time;
    temp.Wait_Time = 0;     //留待后续计算    
    //查找当前最短的窗口。如果有两个窗口一样短，默认选择小号的
    int i=0,j=i;
    int t = QueueLength(W[0]);
    for (i = 1; i < Service_Num;i++){
        if(QueueLength(W[i])<t){
            j = i;
            t = QueueLength(W[i]);      //找到所有窗口中最短的那个窗口，返回窗口序号值j
        }
    }
    temp.Win_Num = j + 1;   //+1是为了对应正常的阅读习惯
    return temp;
}

int RandomTime()      //产生一个随机时间
{
    int t;
    srand(time(NULL));
    t = 5 + rand() % 6;
    return t;
}

void Serving(Win_Queue W[], Person P[], int cur_time)  //主体服务函数，参数为窗口集合、顾客集合和当前时间，返回值为空
{
    int i;   //循环变量
    int t;
    for (i = 0; i < Service_Num; i++){      //对所有服务窗口遍历
        if(QueueLength(W[i])>0){     //空队列不进行遍历
            //printf("123");
            (W+i)->p[W[i].front].Remain_Service_Time = (W+i)->p[W[i].front].Remain_Service_Time - 1;
            //这个语句的意思是，当前检索的是W[i]窗口，该窗口的第一个顾客(即下标为W[i]里的front的指针p所指的顾客)的服务时间(用到结构体成员运算符.)减一
            if((W+i)->p[W[i].front].Remain_Service_Time==0){     
                //如果服务时间结束
                //printf("456");
                t = cur_time - (W+i)->p[W[i].front].Arrive_Time-(W+i)->p[W[i].front].Service_Time;
                P[(W + i)->p[W[i].front].Arrive_Time].Wait_Time = t;
                //先利用当前时间减去到达时间计算出等待时间
                DeQueue(&W[i]);         //出队列
            }
        }
    }
}

int Check_Service_Done(Win_Queue W[])   //检查是否服务完成函数，参数为当前服务窗口状态，返回值为真假型变量(1/0)
{
    int i=0, flag=0;
    for (i = 0; i < Service_Num; i++){
        flag += QueueLength(W[i]);      //只有各服务窗口长度均为0时，返回值才为0
    }
    return flag;
}

float GetAverage(Person P[])    //计算平均时间函数
{
    int i,sum=0;
    float average;
    for (i = 0; i < Clients_Num; i++)
        sum += P[i].Wait_Time;
    average = (float) sum / Clients_Num;
    return average;
}

void ShowResult(Person P[])     //打印输出各位顾客的结果
{
    printf("\nClient_Num  Arrive_Time  Service_Time  Win_Num  Wait_Time\n");
    int i;
    for (i = 0; i < Clients_Num; i++){
        printf("%8d%8d%16d%8d%8d\n", i + 1, P[i].Arrive_Time, P[i].Service_Time, P[i].Win_Num, P[i].Wait_Time);
    }
}