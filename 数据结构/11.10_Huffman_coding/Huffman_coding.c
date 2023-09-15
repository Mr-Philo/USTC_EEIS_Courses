/*          ///第四次上机实验：哈夫曼(Huffman)树及哈夫曼编码的应用///

    输入一篇英文短文，输出对应的Huffman树和Huffman编码方式(即输出每个字符的编码)，并计算压缩比

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stack.c>
#define MAX_LEN 9999     //定义报文最长长度
#define MAX_CHA_NUM 120  //定义最多字符的个数
#define MAX_CODE_LEN 15  //定义一个字符的HF编码最长长度

typedef struct{
    int weight;     //权值
    int parent;
    int lchild;
    int rchild;
    char value;
}HTNode ;       //存放哈夫曼树的静态三叉链表类型 同时加入value值以便解码

typedef struct{
    int s1;
    int s2;
} BiGroup;  //  定义二元组

//静态数组定义
char Char[2*MAX_CHA_NUM]={'\0'};   //存放所有字符的数组
int Weight[2*MAX_CHA_NUM]={0};   //存放各字符权值的数组
char HuffmanCode[MAX_CHA_NUM][MAX_CODE_LEN];  //存放各字符哈夫曼编码的数组
//**第一个空间统一不用**

//**子函数声明**
int Weight_Count(char *S);  
//统计文章(存储在S中)出现不同字符的个数n以及每种字符出现的频率Wi
HTNode *HuffmanTree(HTNode *HT, int n);    
//哈夫曼树构造
//void Select(HTNode *HT, int n, int i, int *s1, int *s2);    //旧代码替换
BiGroup *Select(HTNode *HT, int n, int i);    
//上一个函数里面用到的小函数
//作用是在HT[1……i-1]中选择parent为0且weight最小的两个结点，返回值为下标s1,s2
void HuffmanCoding(HTNode *HT, int n);
//从哈夫曼树HT上求得n个叶子节点的哈夫曼编码并存入数组HC中
void Coding(HTNode *HT, int root, SqStack *S);   
//HuffmanCoding中的递归算法
void EnCode(int n, FILE * fp1, FILE * fp2, HTNode *HT, char *S_Output);      
//输出编码文件
void Decode(int n, FILE * fp3, HTNode *HT, char *S);                
//将01序列解码

int main()
{
    //读取文件并存入S中
    char S[MAX_LEN];
    int i = 1;
    FILE *fp1=NULL;
    if((fp1=fopen("exp4_input_file.py","r"))==NULL){
        printf("\ncannot open this file!\n");
        exit(1);
    }
    while(!feof(fp1)){
        S[i++] = fgetc(fp1);
    }       //将文件读取到字符数组S中
    puts(S);
    
    //统计文章中出现不同字符的个数n以及每种字符出现的频率Wi
    int n = Weight_Count(S);
    //**调试区**
    printf("字符共有%d个\n", n);

    //用n个字符的Wi作为权，构造哈夫曼树
    HTNode *HT=NULL;
    HT=HuffmanTree(HT, n);      //  ！！！！！！！一定要把建好的HT传回来！！！不然后面的函数收不到HT信息！！

    //从哈夫曼树上获得每个字符的哈夫曼编码
    HuffmanCoding(HT, n);

    //输出各字符的哈夫曼编码
    for (i = 1; i <= n;i++){
        printf("%c--->%s\n", Char[i], HuffmanCode[i]);
    }
    printf("\nCoding Done.\n");

    //计算压缩率
    int Original_Len=0, New_Len=0;
    float rate;
    for (i = 1; i <= n;i++){
        Original_Len += 8 * Weight[i];
        New_Len += strlen(HuffmanCode[i]) * Weight[i];
    }
    rate = (float)New_Len / (float)Original_Len;
    printf("The compression ratio is %f\n", rate);

    //输出编码后的文件
    FILE *fp2 = NULL;
    fp1 = fopen("Test_Paragraph.txt", "r");      //fp1要回来
    if((fp2=fopen("Test_Output_Paragraph.txt","w"))==NULL){
        printf("\ncannot open this file!\n");
        exit(1);
    }
    char S_Output[MAX_LEN];     //存储输出的01序列
    EnCode(n, fp1, fp2, HT, S_Output);      //顺次读取fp1中字符并将其解码输入进文件和字符数组S_Output中
    //puts(S_Output);
    printf("Code Output Done.\n");

    //将01序列文件解码
    FILE *fp3 = NULL;
    if((fp3=fopen("Test_DeCode_Paragraph.txt","w"))==NULL){
        printf("\ncannot open this file!\n");
        exit(1);
    }
    Decode(n, fp3, HT, S_Output);
    printf("Decode Done.\n");
}

int Weight_Count(char *S)      //统计文章中出现不同字符的个数n以及每种字符出现的频率Wi
{
	int i,j,flag;
	for(i=1;S[i]!='\0';i++){
		flag=0;
		for(j=1;Weight[j]!=0;j++){
			if(Char[j]==S[i]){
			Weight[j]++;
			flag=1;
			}
		}
		if(flag==0){        //扫描到新字符进Char数组中
			Char[j]=S[i];
			Weight[j]++;
		}
	}
    i = 1;
    while(Char[i]!='\0') i++;
    return i-1;       //计算出现的总字符数并返回之
}

HTNode *HuffmanTree(HTNode *HT, int n)     //哈夫曼树构造
{
    int i;      //循环变量
    int m = 2 * n - 1;      //哈夫曼树结点总数
    HT = (HTNode *)malloc(sizeof(HTNode) * (m + 1));    //分配HT存储空间，0号单元闲置不用
    if(HT==NULL){
        printf("\nERROR!\n");
        exit(0);
    }
    for (i = 1; i <= m;i++){        //HT数组初始化
        HT[i].weight = i <= n ? Weight[i] : 0;
        HT[i].lchild = HT[i].rchild = HT[i].parent = 0;
        HT[i].value = i <= n ? Char[i] : '\0';
        //**调试区**
        //printf("第%d个结点创建完毕，lchlid为%d,rchild为%d,parent值为%d,权值为%d,对应字符为%c\n", i, HT[i].lchild, HT[i].rchild,HT[i].parent,HT[i].weight,HT[i].value);
    }
    BiGroup *B=(BiGroup*)malloc(sizeof(BiGroup));
    //int s1=0, s2=0;
    for (i = n + 1; i <= m;i++){        //主循环，完成n-1次合并
        //Select(HT, n, i, &s1, &s2);
        //**老代码替换**
        B = Select(HT, n, i);
        //**调试区**
        //if(i==87)
        //    printf("测试///%d %d\n", B->s1, B->s2);
        
        //下面是哈夫曼树构造的具体过程
        HT[i].lchild = B->s1;      //注：左子树挂更小的哪一个
        HT[i].rchild = B->s2;
        HT[i].weight = HT[B->s1].weight + HT[B->s2].weight;
        HT[i].parent = 0;
        HT[B->s1].parent = HT[B->s2].parent = i;      //parent不为0表示该结点已经参与构建，则select函数中不再挑选
        //**调试区**
        //printf("第%d个结点创建完毕，lchlid为%d,rchild为%d,parent值为%d,权值为%d\n", i, HT[i].lchild, HT[i].rchild,HT[i].parent,HT[i].weight);
    }

    return HT;
}

//void Select(HTNode *HT, int n, int i, int *s1, int *s2)
BiGroup *Select(HTNode *HT, int n, int i)
{   
    //在HT[1……i-1]中选择parent为0且weight最小的两个结点，返回值为下标s1,s2
    //**老代码替换**改成返回二元组

    int j;       //循环变量
    //*s1 = *s2 = 0;      //初始化
    BiGroup *B=(BiGroup*)malloc(sizeof(BiGroup));
    B->s1 = 0;
    B->s2 = 0;      //二元组声明及初始化

    int temp1 = 99999;     
    int temp2 = 99999;   //定义临时量并赋初值便于后面比对

    for (j = 1; j < i ;j++){        //注意检索范围
        if(HT[j].weight<temp1&&HT[j].parent==0){
            temp2 = temp1;
            //*s2 = *s1;    //如果找到了最小的，那么第二小的值就赋为上一个最小的值
            B->s2 = B->s1;
            temp1 = HT[j].weight;
            //*s1 = j;     //再把最小的值赋给temp1和s1
            B->s1 = j;
        }
        else if(HT[j].weight<temp2&&HT[j].parent==0){
            temp2 = HT[j].weight;
            //*s2 = j;     //和上面类似，但表示只找到了第二小的值
            B->s2 = j;
        }
        //printf("调试000\n");
    }//end for

    return B;
}

void HuffmanCoding(HTNode *HT, int n)   //从哈夫曼树HT上求得n个叶子节点的哈夫曼编码并存入数组HC中
{
    SqStack S;
    InitStack(&S);       //初始化栈S
    //printf("调试1");
    int m = 2 * n - 1;
    Coding(HT, m, &S);      //HT为哈树，HuffmanCode为全局数组，2n-1为哈树根节点下标
}

void Coding(HTNode *HT, int root, SqStack *S)   //HuffmanCoding中的递归算法
{
    //先序遍历哈树HT，求得每个叶子的编码字符串，存入数组HC
    //S是一个顺序栈，用来记录遍历路径
    //root是哈树数组HT中根节点的下标
    if(root!=0){                        //当二叉树非空
        //printf("调试3\n");
        //printf("%d\n", HT[root].lchild);
        if(HT[root].lchild==0){         //root为树叶
            Push(S, '\0');              //字符串结束标志'\0'进栈
            strcpy(HuffmanCode[root], S->elem);   //复制叶子的编码
            Pop(S);                     //字符串结束标志'\0'出栈
        }
        //printf("调试2");
        Push(S, '0');      //向左转，'0'进栈
        Coding(HT, HT[root].lchild, S);       //遍历左子树
        Pop(S);
        Push(S, '1');       //向右转，'1'进栈
        Coding(HT, HT[root].rchild, S);        //遍历右子树
        Pop(S);
    }
}

void EnCode(int n, FILE * fp1, FILE * fp2, HTNode *HT, char *S_Output)      //输出编码文件
{
    char ch;    //临时字符变量
    int i;      //循环变量
    while (fscanf(fp1, "%c", &ch) == 1)      //先从原文件中读取一个字符
    {
        for (i = 0; i < n; i++)     //遍历，找到对应字符的HT树存储位置下标
        {
            if (HT[i].value == ch)
            {
                fprintf(fp2, "%s", HuffmanCode[i]);
                strcat(S_Output, HuffmanCode[i]);       //分别将对应的哈夫曼编码写入文件和数组
                break;
            }
        }
    }  //end while
}

void Decode(int n,FILE * fp3, HTNode *HT, char *S)      //将01序列解码,S存储待解码的01序列
{
    int root, p;     //和HT哈夫曼树有关的序号变量，根节点序列root为2n-1,p为临时遍历变量
    int i = 0;       //循环变量
    p = root = 2 * n - 1;       //根节点位置，以及临时变量p的初始化
    for (i = 0; i < strlen(S); i++)     //从01序列始端开始，依次读取
    {
        if (S[i] == '0')    //读到0向左走
        {
            p = HT[p].lchild;
        }
        else if (S[i] == '1')   //读到1向右走
        {
            p = HT[p].rchild;
        }
        if (HT[p].lchild == 0 && HT[p].rchild == 0)     //如果读取到叶子结点了
        {

            fprintf(fp3, "%c", HT[p].value);        //输出该叶子结点对应的字符进入fp3文件中
            p = root;       //回到根节点重新开始读取
        }
    } //end for
}

//Last Corrected in 2020.11.23
//淦 终于他喵的写完了