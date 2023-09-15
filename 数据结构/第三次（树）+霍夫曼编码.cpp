#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char sentence[100];
int times[100]={0};
char HuffmanCode[100][10];
typedef struct{
    int weight;
    int parent;
    int lchild;
    int rchild;
    char value;
}HTNode,*HuffmanTree;
void Count(char *text){
	int i,j,k;
	char c;
	for(i=0;text[i]!='\0';i++){
		k=0;
		for(j=0;times[j]!=0;j++){
			if(sentence[j]==text[i]){
			times[j]++;
			k=1;
			}
		}
		if(k==0){
			sentence[j]=text[i];
			times[j]++;
		}
	}
}
void HuffmanCoding(HuffmanTree &HT){
	int n=0,i,j,k,m1,m2,x1,x2,start;
	char *cd;
	while(times[n]!=0) n++;
	HT=(HuffmanTree)malloc((2*n)*sizeof(HTNode));
	for(i=0;i<2*n-1;i++){
		HT[i].lchild=-1;
		HT[i].rchild=-1;
		HT[i].parent=-1;
		HT[i].value=sentence[i];
		HT[i].weight=times[i];
	}
	for(i=0;i<n-1;i++){
        m1=m2=100;
        x1=x2=0;
        for(j=0;j<n+i;j++){
            if(HT[j].weight<m1&&HT[j].parent==-1){
                m2=m1; 
                x2=x1; 
                m1=HT[j].weight;
                x1=j;
			}
            else if(HT[j].weight<m2&&HT[j].parent==-1){
                m2=HT[j].weight;
                x2=j;
            }
        }
        HT[x1].parent=n+i;
    	HT[x2].parent=n+i;
        HT[n+i].weight=HT[x1].weight+HT[x2].weight;
        HT[n+i].lchild=x1;
        HT[n+i].rchild=x2;
    }
    cd=(char *)malloc(n*sizeof(char));
    cd[n-1]='\0';
    for(i=0;i<n;i++){
    	start=n-1;
    	for(j=i,k=HT[j].parent;k!=-1;j=k,k=HT[k].parent){
    		if(HT[k].lchild==j) cd[--start]='0';
    		else cd[--start]='1';
    	}
		strcpy(HuffmanCode[i],&cd[start]);
	}
    free(cd);
}
void EnCode(char *text){
	char code[1000];
	int i,j;
	for(i=0;text[i]!='\0';i++)
		for(j=0;times[j]!=0;j++)
			if(sentence[j]==text[i]) strcat(code,HuffmanCode[j]);
	printf("二进制编码为：%s\n",code);
}
void DeCode(char *code,HuffmanTree HT){
	int n=0,i=0,j=0,k=0;
	char text[1000];
	while(times[n]!=0) n++;
	while(code[i]!='\0'){
		j=2*n-2;
		while(HT[j].lchild!=-1){
			if(code[i++]=='0') j=HT[j].lchild;
			else j=HT[j].rchild;
		}
	text[k++]=HT[j].value;
	}
	printf("解码文本为：%s\n",text);
}
main(){
	int i,menu;
	HuffmanTree HT;
	char text[100],t[100],c[100];
	printf("输入原始文本：\n");
	scanf("%s",text);
	Count(text);
	HuffmanCoding(HT);
	printf("字符 出现次数 二进制编码\n");
	for(i=0;times[i]!=0;i++) printf(" %c      %d        %s\n",sentence[i],times[i],HuffmanCode[i]);
	printf("退出：0 编码：1 解码：2\n") ;
	scanf("%d",&menu);
	while(menu!=0){
		switch(menu){
			case 1:	
				printf("输入要编码的文本：\n");
				scanf("%s",t);
				EnCode(t);
				break;
			case 2:	
				printf("输入要解码的二进制串：\n");
				scanf("%s",c);
				DeCode(c,HT);
				break;
			default: break;
		}
	}
}
