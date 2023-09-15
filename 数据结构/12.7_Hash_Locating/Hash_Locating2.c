/*                      ////第五次上机实验——哈希查找////
1.基本要求，文件读取40--80位同学信息，英文名(课程群内提供)或者中文的拼音(自己创建),设计一个散
列函数，将同学们的信息记录分布到散列表中，设计冲突解决方案，计算散列表的平均查找次数，并实
现对学生信息的查找；
2.扩展，可将同学们信息进行补充，增加为1000个同学的信息(课程群内提供)，添加除姓名外的其他信
息如：年龄，邮箱，电话，性别(课程群内提供)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NUM 1000          //字符串最多个数
#define MAX_HASH_NUM 1200     //哈希表最大容量为1.2倍的原字符串个数
#define MAX_STR_LEN 20      //字符串最长长度

typedef struct HNode{
    int data;
    struct HNode *next;
} HNode;        //开散列方法构造哈希表——表中各节点采用链表形式

char Name[MAX_NUM][MAX_STR_LEN];        //存储Name_List的二维数组
HNode *HashTable = NULL;        //声明一哈希表

void InitList();        //初始化姓名表函数
unsigned int ELFHash(char *str);        //面向字符串的Hash处理函数——ELF算法
void Create_Hash_Table();       //哈希表构造函数，采用开散列方法来解决冲突
void GetASL();     //求解哈希表的平均查找次数

int main()
{
    InitList();
    Create_Hash_Table();
    GetASL();
    /****调试代码****
    for (int i = 0; i < MAX_NUM;i++)
    {
        printf("%u--test\n", ELFHash(Name[i]));
    }*/
}

void InitList()      //初始化姓名表函数
{
    FILE *fp;
    if((fp=fopen("name.txt","r"))==NULL){
        printf("\ncannot open this file!\n");
        exit(1);
    }
    int i = 0;
    while(!feof(fp)){
        fscanf(fp,"%s",Name[i++]);      //按字符串格式从文本文件中读取各Name存入姓名序列数组中
    }
    /****调试代码****
    for (i = 0; i < MAX_NUM;i++)
    {
        printf("%s--test\n", Name[i]);
    }*/
}

unsigned int ELFHash(char *str)     //面向字符串的Hash处理函数——ELF算法
{
    unsigned int hash = 0;
    unsigned int x = 0;

    while (*str)
    {
        hash = (hash << 4) + (*str++);//hash左移4位，把当前字符ASCII存入hash低四位。
        if ((x = hash & 0xF0000000L) != 0)
        {
            //如果最高的四位不为0，则说明字符多余7个，现在正在存第7个字符，如果不处理，再加下一个字符时，第一个字符会被移出，因此要有如下处理。
            //该处理，如果最高位为0，就会仅仅影响5-8位，否则会影响5-31位，因为C语言使用的算数移位
            //因为1-4位刚刚存储了新加入到字符，所以不能>>28
            hash ^= (x >> 24);
            //上面这行代码并不会对X有影响，本身X和hash的高4位相同，下面这行代码&~即对28-31(高4位)位清零。
            hash &= ~x;
        }
    }
    //返回一个符号位为0的数，即丢弃最高位，以免函数外产生影响。(我们可以考虑，如果只有字符，符号位不可能为负)
    //return (hash & 0x7FFFFFFF);
    return ((hash & 0x7FFFFFFF) % MAX_HASH_NUM);       //额外增加取余操作
}

void Create_Hash_Table()       //哈希表构造函数，采用开散列方法来解决冲突
{
    HashTable = (HNode *)malloc(sizeof(HNode) * MAX_HASH_NUM);      //申请空间
    if(HashTable==NULL)
        printf("ERROR");

    int i;   //循环变量
    for (i = 0; i < MAX_HASH_NUM; i++)
    {
        HashTable[i].data = -1;     //因为数据值只可能是0—MAX_NUM-1的Name数组下标值，故赋值-1表示这里没有数据
        HashTable[i].next = NULL;       //元素初始化
        //printf("%d\n", HashTable[i].data);
    }
    //printf("%d",HashTable[7].data);

    unsigned int hash_num;       //临时存储计算出的hash关键码数值
    HNode *p = NULL;    //临时指针，指针类型为哈希表结点类型
    int count = 0;
    for (i = 0; i < MAX_NUM; i++)
    {
        hash_num = ELFHash(Name[i]);
        //p = &HashTable[hash_num];       //令p指向对应关键码的哈希表存储位置
        count = 0;
        //printf("%d  ",HashTable[hash_num].data);
        //if((p->data) == -1){   //如果p的data域为-1，则说明此关键字位置未被占用，则将对应字符串存入哈希表结点即可
        if((HashTable[hash_num].data) == -1){
            HashTable[hash_num].data = i;
        }
        else{       //如果p的data域不为MAX_NUM，则说明此关键字位置已被占用，需额外挂一个结点
            count++;
            p = HashTable[hash_num].next;
            /*while((p->next)!=NULL){
                p = p->next;
                count++;
            }*/
            /*do{
                count++;
            }while (p = p->next);*/   //注意终止条件，如何判断移到链表末尾了
            //do-while循环的目的是，可能之前已经有结点外挂在这里了，需要在最后面外挂
            HNode *temp = (HNode *)malloc(sizeof(HNode)); //申请分配一个新节点
            temp->data=i;
            temp->next = NULL; //新节点初始化，str处赋上字符串

            if(p==NULL)
                HashTable[hash_num].next = temp;
            else{
                for (; p->next != NULL;p=p->next)
                    count++;
                p->next = temp;    //新节点外挂
            }
            //p = temp;    //新节点外挂,注意这个时候p已经在链表末尾而且是其next域！！！
        }   //end else
        //printf("第%d个数据串入哈希表成功，哈希关键码值为%u,位于外挂位置为%d\n", i+1, hash_num, count);
    }   //end for

    printf("Create_Done.\n");
}

void GetASL()     //求解哈希表的平均查找次数
{
    unsigned int hash_num;       //临时存储计算出的hash关键码数值
    HNode *p = NULL;    //临时指针，指针类型为哈希表结点类型
    int total_count = 0;    //总查找次数
    int single_count = 0;   //每个元素查找次数

    printf("NUM       String   Search_Num  Total_Search_Num\n");
    for (int i = 0; i < MAX_NUM; i++)       //从第一个name开始顺次查找，以计算总查找次数
    {
        hash_num = ELFHash(Name[i]);
        p = &HashTable[hash_num];       //令p指向对应关键码的哈希表存储位置
        single_count = 0;

        if((p->data)==i){   //如果直接找到
            single_count++;
            total_count++;
        }
        else{       //如果没有直接找到
            do{
                p = p->next;
                if(p==NULL){
                    printf("///FAIL TO SEARCH!!!!///");       //要是到了链表末尾还没找到就出大问题
                    exit(0);
                }
                single_count++;
                total_count++;
            }while (p->data!=i);       //直到查到为止,注意while里面的写法！！
        }
        //printf("第%d个数据串%s查找成功，查找了%d次，目前一共查找了%d次\n", i, Name[i], single_count, total_count);
        printf("%2d %12s %8d %10d\n", i+1, Name[i], single_count, total_count);
    }   //end for

    printf("Search_Done.\n");
    float ASL = (float)total_count / (float)MAX_NUM;
    printf("The Average Search Length(ASL) is %.4f\n", ASL);
}