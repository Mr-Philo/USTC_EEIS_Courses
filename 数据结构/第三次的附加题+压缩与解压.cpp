#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
typedef struct{
	char item;
	int weight;
	int parent;
	int lchild;
	int rchild;
}HTNode;
typedef struct{
	HTNode *Htree;
	int root;
	int length;
}Tree;
typedef struct BitNode{
	char bite;
	BitNode *next;
}BitNode;
typedef struct{
	BitNode *st;
	BitNode *rear;
	int length;
}Bit;
char key[257][257]={'\0'};  //用来存放每个字节的密码的数组
HTNode an[513];             //用来存放每个字节的密码的数组 
void Creat(Tree &Huf){
	int i;
	for(i=1;i<=513;i++){
		an[i].item='\0';
		an[i].lchild=0;
		an[i].parent=0;
		an[i].rchild=0;
		an[i].weight=0;
	}
	Huf.Htree=an;
	Huf.root=0;
	Huf.length=0;
}//Creat                              创建空的哈夫曼树 
void read(FILE *fp,Tree &Huf){
	char a;
	int i,j=1;	
	while(1){
		fread(&a,1,1,fp);
		if(feof(fp)) break;
		if(Huf.length==0){
			Huf.Htree[1].item=a;
			Huf.Htree[1].weight++;
			Huf.length++;
			continue;
		}
		for(i=1;i<=Huf.length;i++){
			if(a==Huf.Htree[i].item){
				Huf.Htree[i].weight++;
				j=2;
				break;
			}			
		}
		if(j==2){
			j=1;
			continue;
		}
		Huf.Htree[i].item=a;
		Huf.Htree[i].weight++;
		Huf.length++;
	}
	rewind(fp);	
}                               //读取文件填写哈夫曼树
void abc(FILE *fp,Tree &Huf){
	char a;
	int i,j=1;	
    fread(&a,1,1,fp);
	printf("%d",a);
}
int SearchMin(Tree Huf){
	int m=-1,n=-1,i;
	for(i=1;i<=Huf.length;i++){
		if(m<0&&Huf.Htree[i].parent==0){
			m=Huf.Htree[i].weight;
			n=i;
			continue;
		}
		if(Huf.Htree[i].parent==0&&Huf.Htree[i].weight<m){
			n=i;
			m=Huf.Htree[i].weight;
		}
	}
    return n;
} //SearchMin                         寻找哈夫曼树中权重最小的数组
void CreatTree(Tree &Huf){
	int i,s1,s2;
	int max=Huf.length*2-1;
	for(i=Huf.length+1;i<=max;i++){
		s1=SearchMin(Huf);
		Huf.Htree[s1].parent=i;
		s2=SearchMin(Huf);
		Huf.Htree[s2].parent=i;
		Huf.Htree[i].weight=Huf.Htree[s1].weight+Huf.Htree[s2].weight;
		Huf.Htree[i].lchild=s1;
		Huf.Htree[i].rchild=s2;
		Huf.length++;
	}
	Huf.root=max;
}//CreatTree                         创建哈夫曼树 
int Search(Tree &Huf,char a){
	int i;
	for(i=1;i<=Huf.length;i++){
		if(Huf.Htree[i].item==a) return i;
	}
}// Search                               在树中寻找指定叶子
int Height(Tree Huf,int root){
	if(root==0) return 0; 
	int h1,h2;
	h1=Height(Huf,Huf.Htree[root].lchild);	
	h2=Height(Huf,Huf.Htree[root].rchild);
	return 1+(h1>h2?h1:h2);
}//Height                                求树高 
void Key(Tree &Huf){
	int i,start,c,f;
	int h=Height(Huf,Huf.root),n=(Huf.root+1)/2;
	char *b=(char *)malloc(sizeof(char)*h);
	for(i=0;i<h;i++) b[i]='\0';
	for(i=1;i<=n;i++){
		start=h-1;
		for(c=i,f=Huf.Htree[c].parent;f!=0;c=f,f=Huf.Htree[f].parent){
			if(Huf.Htree[f].lchild==c) b[--start]='0';
			else b[--start]='1';
		}
		strcpy(key[i],&b[start]);		
	}
	free(b);		
}//Key                                  给key数组填数
void CreatBit(Bit &Mi){
	BitNode *p=(BitNode *)malloc(sizeof(BitNode));
	p->next=NULL;
	Mi.st=p;
	Mi.rear=p;
	Mi.length=0;
}//CreatMi                            初始化生成比特流 
void Bite(FILE *fp,Tree Huf,Bit &Mi){
	char a;
	int i;
	int pos;
	BitNode *p;
	while(1){
		fread(&a,1,1,fp);
		if(feof(fp)) break;
		pos=Search(Huf,a);
		for(i=0;key[pos][i]!='\0';i++){
			p=(BitNode *)malloc(sizeof(BitNode));
			p->bite=key[pos][i];
			p->next=NULL;
			Mi.rear->next=p;
			Mi.rear=p;
			Mi.length++;
		}
	}
	rewind(fp);
}//Bite                                   填写比特流
void WriteBite(FILE *&fp,Bit &Mi){
	int eight=0;
	char a=0;
	BitNode *p=Mi.st->next;
	while(p){
		if(p->bite=='0') a=a<<1;
		else{
			a=a<<1;
			a=a|1;
		}		
		eight++;
		if(eight==8){
			fwrite(&a,1,1,fp);
			eight=0;
			a=0;
		}
		p=p->next;
	}
	if(eight!=0){
		a=a<<(8-eight);
		fwrite(&a,1,1,fp);
	}
}//WriteBite                            将比特流写入传入的是fp2 
void WriteKey(FILE *&fp,Tree Huf){
	int lenght=0;
	int n=(Huf.root+1)/2;
	int i,j;
	char a=0;
	int eight=0;
	for(i=1;i<=n;i++){
		lenght=0;
		fwrite(&Huf.Htree[i].item,1,1,fp);
		for(j=0;key[i][j]!='\0';j++) lenght++;
		fwrite(&lenght,sizeof(int),1,fp);
		for(j=0;j<lenght;j++){
			if(key[i][j]=='0') a=a<<1;
		    else{
			    a=a<<1;
		    	a=a|1;
		    }		
		    eight++;
		    if(eight==8){
	 		    fwrite(&a,1,1,fp);
			    eight=0;
			    a=0;
		    }
		}
	    if(eight!=0){
		    a=a<<(8-eight);
		    fwrite(&a,1,1,fp);
			a=0;
			eight=0;
	    }
	}
} //WriteKey                          将哈夫曼密码写入   传入的是fp2 
char* Trans(char t,int n){
	char a;
	int i;
	char *p=(char *)malloc(9);
	for(i=0;i<9;i++) p[i]='\0';
	for(i=0;i<n;i++){
		a=t;
		a=a<<i;
		a=a>>7;
		a=a&1;
		switch(a){
			case 0:
				p[i]='0';
				break;
			case 1:
				p[i]='1';
				break;
		}
	}
	return p;
} //trans   *                          将单个字节化为数组 
void Adcreat(FILE *&fp,Tree &Huf,int num){
	int i,j;
	int n,x,y;
	char a;
	char *p=(char *)malloc(257);
	for(i=1;i<=num;i++){
		fread(&a,1,1,fp);
		Huf.Htree[i].item=a;
		Huf.length++;
		fread(&n,4,1,fp);
		x=n/8;
		y=n%8;
		for(j=0;j<x;j++){
			fread(&a,1,1,fp);
			p=Trans(a,8);
			strcat(key[i],p);
		}
		if(y!=0){
			fread(&a,1,1,fp);
			p=Trans(a,y);
			strcat(key[i],p);                      //溢出了 
		}	
	}
}//Adcreat                           反身填霍夫曼表
int SearchEmpty(Tree &Huf,int num){
	int i;
	for(i=num+2;i<2*num;i++){
		if(Huf.Htree[i].parent==0) return i;
	}
}
void AdTree(Tree &Huf,int num){
	int i,j,x,y,z;
	char a;
	Huf.root=num+1;
	Huf.length++;
	for(i=1;i<=num;i++){
		x=num+1;
		for(j=0;key[i][j]!='\0'&&key[i][j+1]!='\0';j++){
			a=key[i][j];
			if(a=='0'){
				if(Huf.Htree[x].lchild==0){
					z=SearchEmpty(Huf,num);
					Huf.Htree[x].lchild=z;
					Huf.Htree[z].parent=x;
					Huf.length++;
				}
				x=Huf.Htree[x].lchild;
			}
			else{
			    if(Huf.Htree[x].rchild==0){
					z=SearchEmpty(Huf,num);
					Huf.Htree[x].rchild=z;
					Huf.Htree[z].parent=x;
					Huf.length++;
				}
				x=Huf.Htree[x].rchild;	
			}		    
		}
		a=key[i][j];
		if(a=='0'){
			Huf.Htree[x].lchild=i;
			Huf.Htree[i].parent=x;
		}
		else{
			Huf.Htree[x].rchild=i;
			Huf.Htree[i].parent=x;
		}		    
    }
} //AdTree                         反身建树 
void Trans2(char t,int num,Bit &Mi){
	int i,j;
	char a;
	BitNode *p;
	for(i=0;i<num;i++){
		a=t;
		a=a<<i;
		a=a>>7;
		a=a&1;
		switch(a){
			case 0:
				p=(BitNode *)malloc(sizeof(BitNode));
				p->bite='0';
				p->next=NULL;
				Mi.rear->next=p;
				Mi.rear=p;
				break;
			case 1:
			    p=(BitNode *)malloc(sizeof(BitNode));
				p->bite='1';
				p->next=NULL;
				Mi.rear->next=p;
				Mi.rear=p;
				break;	
	        }
	}
}//Trans2                                
void Bites(FILE *fp,Bit &Mi){
	int n=Mi.length;
	int i,j,x,y;
	char a;
	x=n/8;
	y=n%8;
	for(i=0;i<x;i++){
		fread(&a,1,1,fp);
		Trans2(a,8,Mi);
	}
	if(y!=0){
		fread(&a,1,1,fp);
		Trans2(a,y,Mi);
	}
}//Bites                          把文件内容编程bite流 
void NewWrite(Tree Huf,Bit &Mi,FILE *&fp){
	BitNode *p=Mi.st->next;
	int n=Huf.root;
	while(p){
		if(p->bite=='0'){
			n=Huf.Htree[n].lchild;
		}
		else n=Huf.Htree[n].rchild;
		if(Huf.Htree[n].lchild==0){
			fwrite(&Huf.Htree[n].item,1,1,fp);
			n=Huf.root;
		}
		p=p->next;
	}
}//NewWrite                        还原文件 
main(){
	char a;
	int pos;
	int i,j;
	FILE *fp,*fp2;
	Tree Huf;
	Bit Mi;
	BitNode *p;
	char name1[100]={'\0'},name2[100]={'\0'};
	int length,num;
	while(1){
		printf("1.压缩  2.解压   3.退出\n");
		scanf("%d",&pos);
		for(i=0;i<257;i++){
			for(j=0;j<257;j++)
		 	   key[i][j]='\0';
		}
		switch(pos){
			case 1:	
		 	   getchar();	    
				printf("输出需要压缩的文件名(含拓展名)\n");
				gets(name1);
				fp=fopen(name1,"rb");
				if(!fp){
					printf("不存在\n");
					break;
				} 
				printf("输出需要输出的文件名(含拓展名)\n");
				gets(name2); 			
				fp2=fopen(name2,"wb+");
		    	Creat(Huf);
				read(fp,Huf);			
				CreatTree(Huf);
				Key(Huf);
				CreatBit(Mi);
				Bite(fp,Huf,Mi);
				p=Mi.st->next;
				fwrite(&Mi.length,sizeof(int),1,fp2);
				int n;
				n=(Huf.root+1)/2;
				fwrite(&n,sizeof(int),1,fp2);
				WriteKey(fp2,Huf);
				WriteBite(fp2,Mi);
				fclose(fp);
				fclose(fp2);
				printf("Success\n");
				break;			
			case 2:
				getchar();
				printf("输出需要解压的文件名(含拓展名)\n");	
				gets(name1);
				fp=fopen(name1,"rb");
				if(!fp){
					printf("不存在\n");
					break;
				} 
				printf("输出需要输出的文件名(含拓展名)\n");
				gets(name2); 			
				fp2=fopen(name2,"wb+");
				fread(&length,sizeof(int),1,fp);
				CreatBit(Mi);
				Mi.length=length;
				fread(&num,sizeof(int),1,fp);
				Creat(Huf);           
				Adcreat(fp,Huf,num);
				AdTree(Huf,num);
				Bites(fp,Mi);
				p=Mi.st->next;
				NewWrite(Huf,Mi,fp2);
				fclose(fp);
				fclose(fp2);
				printf("Success\n");
				break;
			case 3: 
				return 0;
		}
	}
}
