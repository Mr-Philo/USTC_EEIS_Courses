/*          ///第三次上机实验：文本相似度检测///
一、字符串相似度比较
    从文件读取两个字符串，自定义字符串的相似度（参考建议：最长公共子串长度/总长度，所有非 重叠
公共子串的和/总长度）；
    对非重叠公共子串做下解释：如abcdef和abcde的最大公共子串是abcde，计算所有非 重叠公共 子串的
和 只需要计算这一个子串就行了，不需要再加入abc,bcd等比这个小的子串;
二、段落相似度比较
    从文件读取两个段落，自定义段落的相似度；
三、文章相似度比较
    从文件读取两篇文章，自定义文章的相似度，并计算两篇文章的相似度
四、支持求不连续的字符串的公共子串
    说明：如 I love apple 和I love the apple 求他们的公共子串是 I love apple ，中间隔了多少
个字符还算是同一个子串，你可以自己规定；
*/

#include<stdio.h>
#include<stdlib.h>
#define MAX_STRING_NUM 100

int udf_strlen(char S[]);
void Similarity_Detection(char A[], char B[]);
//void InitMatrix(char A[], char B[], char M[][], int len1, int len2);
//void GetDia(char M[][], int len1, int len2, int *max_substr_len, int *eff_substr_num);

int main()
{
    char A[MAX_STRING_NUM];
    char B[MAX_STRING_NUM];
    FILE *fp1;
    FILE *fp2;
    if((fp1=fopen("mytest.txt","r"))==NULL){
        printf("\ncannot open this file!\n");
        exit(1);
    }
    fgets(A, MAX_STRING_NUM, fp1);
    fgets(B, MAX_STRING_NUM, fp1);
    puts(A);
    puts(B);
    //printf("%d-%d", udf_strlen(A),udf_strlen(B));
    Similarity_Detection(A, B);
}

int udf_strlen(char S[])
{
    int i=0;
    char *p = S;
    while(*p){
        i++;
        p++;
    }
    return i;
}

void Similarity_Detection(char A[], char B[])
{
    int i = 0, j = 0, max_substr_len = 0, eff_substr_num = 0, eff_substr_len = 0;
    int len1 = udf_strlen(A), len2 = udf_strlen(B);
    int M[len1][len2];
    //InitMatrix(A, B, M, len1, len2);      //初始化矩阵
    for (i = 0; i < len1; i++){
        for (j = 0; j < len2; j++){
            if(A[i]==B[j])
                M[i][j] = 1;
            else
                M[i][j] = 0;
        }
    }
    //printf("Init done\n");
    //GetDia(M, len1, len2, &max_substr_len, &eff_substr_num);      //搜寻矩阵的元素1对角线
    int temp_len = 0;
    int ii=0, jj=0;     //临时循环变量
    for (i = 0; i < len1; i++){
        for (j = 0; j < len2; j++){
            if(M[i][j]==1){
                ii = i;
                jj = j;
                do{
                    M[ii][jj] = 0;       //表示这个格子已经被找过
                    temp_len++;
                    ii++;
                    jj++;
                    //printf("4");
                } while (M[ii][jj] == 1 && ii < len1 && jj < len2);
                if(temp_len>max_substr_len)
                    max_substr_len = temp_len;  //找到最长子串长度
                if(temp_len>2){     //子串长度大于3时定义为有效子串
                    eff_substr_num++;       //个数+1
                    eff_substr_len += temp_len;     //总长度累计
                }
                temp_len = 0;       //临时存储长度变量清零
                //printf("123");
            }
        }
    }   //对M矩阵的遍历找对角线
    printf("The Max SubString len is %d\n", max_substr_len);
    printf("The similarity of max_substring is %.2f\n", (float)2 * max_substr_len / (len1 + len2));
    printf("The Total eff_SubString num and len are %d and %d\n", eff_substr_num, eff_substr_len);
    printf("The similarity of eff_substring is %.2f\n", (float)2 * eff_substr_len / (len1 + len2));
}

/*void InitMatrix(char A[], char B[], char M[][], int len1, int len2)
{
    int i, j;
    for (i = 0; i < len1; i++){
        for (j = 0; j < len2; j++){
            if(A[i]==B[j])
                *M[i][j] = 1;
            else
                *M[i][j] = 0;
        }
    }
}*/

//void GetDia(char M[][], int len1, int len2, int *max_substr_len, int *eff_substr_num)
