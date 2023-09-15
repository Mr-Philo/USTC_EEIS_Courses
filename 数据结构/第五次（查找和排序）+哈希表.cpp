#include<stdio.h>
#include<stdlib.h>
#define MAXSIZE 100                                          //哈希表中关键字数的上限 
#define MAXLENGTH 10                                         //拉链法每条拉链的长度上限 
typedef struct{                                              //哈希表1 
	int table[MAXSIZE];
	int element[MAXSIZE];
	int n;
	int p;
}HashTable1;
typedef struct{                                              //哈希表2 
	int table[MAXSIZE][10];
	int element[MAXSIZE];
	int n;
	int p;
}HashTable2;
void CreatHashTable1(HashTable1 &HT1,int n,int x[]){         //用线性探测法解决冲突构建哈希表1 
	int i,m;
	HT1.n=n;
	printf("输入哈希表的表长：");
	scanf("%d",&HT1.p);
	for(i=0;i<HT1.p;i++) HT1.table[i]=-1;
	for(i=0;i<HT1.n;i++) HT1.element[i]=x[i];
	for(i=0;i<HT1.n;i++){
		m=x[i]%HT1.p;
		while(HT1.table[m]!=-1) m=(m+1)%HT1.p;
		HT1.table[m]=x[i];
	}
	printf("构造成功\n");
}
void CreatHashTable2(HashTable2 &HT2,int n,int x[]){         //用拉链法解决冲突构建哈希表2 
	int i,j,m,k;;
	HT2.n=n;
	printf("输入哈希表的表长：");
	scanf("%d",&HT2.p);
	for(i=0;i<HT2.p;i++)
		for(j=0;j<MAXLENGTH;j++)
			HT2.table[i][j]=-1;
	for(i=0;i<HT2.n;i++) HT2.element[i]=x[i];
	for(i=0;i<HT2.n;i++){
		k=0;
		m=x[i]%HT2.p;
		while(HT2.table[m][k]!=-1) k++;
		HT2.table[m][k]=x[i];
	}
	printf("构造成功\n");
}
void PrintHashTable1(HashTable1 HT1){                        //显示哈希表1 
	int i;
	printf("哈希表1如下：\n");
	for(i=0;i<HT1.p;i++) printf("%3d",i);
	printf("\n");
	for(i=0;i<HT1.p;i++){
		if(HT1.table[i]!=-1) printf("%3d",HT1.table[i]);
		else printf(" --");
	}
	printf("\n");
}
void PrintHashTable2(HashTable2 HT2){                        //显示哈希表2 
	int i,j;
	printf("哈希表2如下：\n");
	for(i=0;i<HT2.p;i++){
		printf("%3d",i);
		if(HT2.table[i][0]==-1) printf(" --");
		else for(j=0;HT2.table[i][j]!=-1;j++)
			printf("%3d",HT2.table[i][j]);
		printf("\n");
	}
}
double HashTable1_ASL1(HashTable1 HT1){                      //求哈希表1查找成功的平均查找长度
	int i,m,count=0;
	for(i=0;i<HT1.n;i++){
		count++;
		m=HT1.element[i]%HT1.p;
		while(HT1.table[m]!=HT1.element[i]){
			m=(m+1)%HT1.p;
			count++;
		}
	}
	return double(count)/double(HT1.n);
}
double HashTable1_ASL2(HashTable1 HT1){                      //求哈希表1查找失败的平均查找长度
	int i,m,count=0;
	for(i=0;i<HT1.p;i++){
		count++;
		m=i;
		while(HT1.table[m]!=-1){
			m=(m+1)%HT1.p;
			count++;
		}
	}
	return double(count)/double(HT1.p);
}
double HashTable2_ASL1(HashTable2 HT2){                      //求哈希表2查找成功的平均查找长度
	int i,m,k,count=0;
	for(i=0;i<HT2.n;i++){
		count++;
		k=0;
		m=HT2.element[i]%HT2.p;
		while(HT2.table[m][k]!=HT2.element[i]){
			k++;
			count++;
		}
	}
	return double(count)/double(HT2.n);
}
double HashTable2_ASL2(HashTable2 HT2){                      //求哈希表2查找失败的平均查找长度
	int i,m,k,count=0;
	for(i=0;i<HT2.p;i++){
		count++;
		k=0;
		m=i;
		while(HT2.table[m][k]!=-1){
			k++;
			count++;
		}
	}
	return double(count)/double(HT2.p);
}
int CompareTimes1(HashTable1 HT1,int x){                     //求在哈希表1中查找关键字x所需的比较次数 
	int m,count=0;
	m=x%HT1.p;
	count++;
	while(HT1.table[m]!=x&&HT1.table[m]!=-1){
		m=(m+1)%HT1.p;
		count++;
	}
	return count;
}
int CompareTimes2(HashTable2 HT2,int x){                     //求在哈希表2中查找关键字x所需的比较次数
	int m,k=0,count=0;
	m=x%HT2.p;
	count++;
	while(HT2.table[m][k]!=x&&HT2.table[m][k]!=-1){
		k++;
		count++;
	}
	return count;
}
int DelectElement1(HashTable1 &HT1,int x){                   //删除哈希表1中的关键字x 
	int i,j,k,m;
	for(i=0;i<HT1.n;i++){
		if(HT1.element[i]==x){
			HT1.n--;
			for(j=i;j<HT1.n;j++) HT1.element[j]=HT1.element[j+1];
			HT1.element[j]=-1;
			for(k=0;k<HT1.p;k++) HT1.table[k]=-1;
			for(k=0;k<HT1.n;k++){
				m=HT1.element[k]%HT1.p;
				while(HT1.table[m]!=-1) m=(m+1)%HT1.p;
				HT1.table[m]=HT1.element[k];
			}
			printf("删除成功\n");
			return NULL;
		}
	}
	printf("找不到关键字%d\n",x);
}
int InsertElement1(HashTable1 &HT1,int x){                   //将关键字x插入哈希表1中 
	int i,m;
	for(i=0;i<HT1.n;i++) if(HT1.element[i]==x){
		printf("哈希表1已经含有同名关键字了\n");
		return NULL;
	}
	if(HT1.n>=HT1.p){
		printf("哈希表1已经满了\n");
		return NULL;
	}
	else{
		HT1.element[HT1.n++]=x;
		m=x%HT1.p;
		while(HT1.table[m]!=-1) m=(m+1)%HT1.p;
		HT1.table[m]=x;
		printf("插入成功\n");
	}
}
int main(){                                                  //菜单 
	int i,m,n,menu;
	int x[MAXSIZE];
	HashTable1 HT1;
	HashTable2 HT2;
	while(1){
		printf("***********************************************************\n");
		printf("         0：退出                1：输入关键字序列\n");
		printf("         2：构建哈希表          3：显示哈希表\n");
		printf("         4：求平均查找长度      5：求比较次数\n");
		printf("         6：插入关键字          7：删除关键字\n");
		printf("           (6和7是附加题，只对哈希表1操作)\n"); 
		printf("***********************************************************\n");
		scanf("%d",&menu);
		switch(menu){
			case 0:
				return NULL;
			case 1:
				printf("输入要输入的关键字的个数：");
				scanf("%d",&n);
				printf("输入关键字：\n");
				for(i=0;i<n;i++) scanf("%d",&x[i]);
				break;
			case 2:
				int menu2;
				printf("***********************************************************\n");
				printf("         1：以线性探测法解决冲突，构造哈希表1\n");
				printf("         2：以拉链法解决冲突，构造哈希表2\n");
				printf("***********************************************************\n");
				scanf("%d",&menu2);
				switch(menu2){
					case 1:
						CreatHashTable1(HT1,n,x);
						break;
					case 2:
						CreatHashTable2(HT2,n,x);
						break;
					default:
						break;
				}
				break;
			case 3:
				int menu3;
				printf("***********************************************************\n");
				printf("         1:显示哈希表1          2：显示哈希表2\n");
				printf("***********************************************************\n");
				scanf("%d",&menu3);
				switch(menu3){
					case 1:
						PrintHashTable1(HT1);
						break;
					case 2:
						PrintHashTable2(HT2);
						break;
					default:
						break;
				}
				break;
			case 4:
				int menu4;
				printf("***********************************************************\n");
				printf("         1:求哈希表1查找成功ASL 2：求哈希表1查找失败ASL\n");
				printf("         3:求哈希表2查找成功ASL 4：求哈希表2查找失败ASL\n");
				printf("***********************************************************\n");
				scanf("%d",&menu4);
				switch(menu4){
					case 1:
						printf("哈希表1查找成功的平均查找长度为%g\n",HashTable1_ASL1(HT1));
						break;
					case 2:
						printf("哈希表1查找失败的平均查找长度为%g\n",HashTable1_ASL2(HT1));
						break;
					case 3:
						printf("哈希表2查找成功的平均查找长度为%g\n",HashTable2_ASL1(HT2));
						break;
					case 4:
						printf("哈希表2查找失败的平均查找长度为%g\n",HashTable2_ASL2(HT2));
						break;
					default:
						break;
				}
				break;
			case 5:
				int menu5;
				printf("输入要查找的关键字：");
				scanf("%d",&m);
				printf("***********************************************************\n");
				printf("         1:求哈希表1的比较次数  2：求哈希表2的比较次数\n");
				printf("***********************************************************\n");
				scanf("%d",&menu5);
				switch(menu5){
					case 1:
						printf("在哈希表1中查找关键字%d需要比较%d次\n",m,CompareTimes1(HT1,m));
						break;
					case 2:
						printf("在哈希表2中查找关键字%d需要比较%d次\n",m,CompareTimes2(HT2,m));
						break;
					default:
						break;
				}
				break;
			case 6:
				printf("输入要插入的关键字：");
				scanf("%d",&m);
				InsertElement1(HT1,m);
				break;
			case 7:
				printf("输入要删除的关键字：");
				scanf("%d",&m);
				DelectElement1(HT1,m);
				break;
			default: break;
		}
	}	
}
