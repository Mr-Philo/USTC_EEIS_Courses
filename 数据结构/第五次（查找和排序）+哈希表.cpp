#include<stdio.h>
#include<stdlib.h>
#define MAXSIZE 100                                          //��ϣ���йؼ����������� 
#define MAXLENGTH 10                                         //������ÿ�������ĳ������� 
typedef struct{                                              //��ϣ��1 
	int table[MAXSIZE];
	int element[MAXSIZE];
	int n;
	int p;
}HashTable1;
typedef struct{                                              //��ϣ��2 
	int table[MAXSIZE][10];
	int element[MAXSIZE];
	int n;
	int p;
}HashTable2;
void CreatHashTable1(HashTable1 &HT1,int n,int x[]){         //������̽�ⷨ�����ͻ������ϣ��1 
	int i,m;
	HT1.n=n;
	printf("�����ϣ��ı���");
	scanf("%d",&HT1.p);
	for(i=0;i<HT1.p;i++) HT1.table[i]=-1;
	for(i=0;i<HT1.n;i++) HT1.element[i]=x[i];
	for(i=0;i<HT1.n;i++){
		m=x[i]%HT1.p;
		while(HT1.table[m]!=-1) m=(m+1)%HT1.p;
		HT1.table[m]=x[i];
	}
	printf("����ɹ�\n");
}
void CreatHashTable2(HashTable2 &HT2,int n,int x[]){         //�������������ͻ������ϣ��2 
	int i,j,m,k;;
	HT2.n=n;
	printf("�����ϣ��ı���");
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
	printf("����ɹ�\n");
}
void PrintHashTable1(HashTable1 HT1){                        //��ʾ��ϣ��1 
	int i;
	printf("��ϣ��1���£�\n");
	for(i=0;i<HT1.p;i++) printf("%3d",i);
	printf("\n");
	for(i=0;i<HT1.p;i++){
		if(HT1.table[i]!=-1) printf("%3d",HT1.table[i]);
		else printf(" --");
	}
	printf("\n");
}
void PrintHashTable2(HashTable2 HT2){                        //��ʾ��ϣ��2 
	int i,j;
	printf("��ϣ��2���£�\n");
	for(i=0;i<HT2.p;i++){
		printf("%3d",i);
		if(HT2.table[i][0]==-1) printf(" --");
		else for(j=0;HT2.table[i][j]!=-1;j++)
			printf("%3d",HT2.table[i][j]);
		printf("\n");
	}
}
double HashTable1_ASL1(HashTable1 HT1){                      //���ϣ��1���ҳɹ���ƽ�����ҳ���
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
double HashTable1_ASL2(HashTable1 HT1){                      //���ϣ��1����ʧ�ܵ�ƽ�����ҳ���
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
double HashTable2_ASL1(HashTable2 HT2){                      //���ϣ��2���ҳɹ���ƽ�����ҳ���
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
double HashTable2_ASL2(HashTable2 HT2){                      //���ϣ��2����ʧ�ܵ�ƽ�����ҳ���
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
int CompareTimes1(HashTable1 HT1,int x){                     //���ڹ�ϣ��1�в��ҹؼ���x����ıȽϴ��� 
	int m,count=0;
	m=x%HT1.p;
	count++;
	while(HT1.table[m]!=x&&HT1.table[m]!=-1){
		m=(m+1)%HT1.p;
		count++;
	}
	return count;
}
int CompareTimes2(HashTable2 HT2,int x){                     //���ڹ�ϣ��2�в��ҹؼ���x����ıȽϴ���
	int m,k=0,count=0;
	m=x%HT2.p;
	count++;
	while(HT2.table[m][k]!=x&&HT2.table[m][k]!=-1){
		k++;
		count++;
	}
	return count;
}
int DelectElement1(HashTable1 &HT1,int x){                   //ɾ����ϣ��1�еĹؼ���x 
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
			printf("ɾ���ɹ�\n");
			return NULL;
		}
	}
	printf("�Ҳ����ؼ���%d\n",x);
}
int InsertElement1(HashTable1 &HT1,int x){                   //���ؼ���x�����ϣ��1�� 
	int i,m;
	for(i=0;i<HT1.n;i++) if(HT1.element[i]==x){
		printf("��ϣ��1�Ѿ�����ͬ���ؼ�����\n");
		return NULL;
	}
	if(HT1.n>=HT1.p){
		printf("��ϣ��1�Ѿ�����\n");
		return NULL;
	}
	else{
		HT1.element[HT1.n++]=x;
		m=x%HT1.p;
		while(HT1.table[m]!=-1) m=(m+1)%HT1.p;
		HT1.table[m]=x;
		printf("����ɹ�\n");
	}
}
int main(){                                                  //�˵� 
	int i,m,n,menu;
	int x[MAXSIZE];
	HashTable1 HT1;
	HashTable2 HT2;
	while(1){
		printf("***********************************************************\n");
		printf("         0���˳�                1������ؼ�������\n");
		printf("         2��������ϣ��          3����ʾ��ϣ��\n");
		printf("         4����ƽ�����ҳ���      5����Ƚϴ���\n");
		printf("         6������ؼ���          7��ɾ���ؼ���\n");
		printf("           (6��7�Ǹ����⣬ֻ�Թ�ϣ��1����)\n"); 
		printf("***********************************************************\n");
		scanf("%d",&menu);
		switch(menu){
			case 0:
				return NULL;
			case 1:
				printf("����Ҫ����Ĺؼ��ֵĸ�����");
				scanf("%d",&n);
				printf("����ؼ��֣�\n");
				for(i=0;i<n;i++) scanf("%d",&x[i]);
				break;
			case 2:
				int menu2;
				printf("***********************************************************\n");
				printf("         1��������̽�ⷨ�����ͻ�������ϣ��1\n");
				printf("         2���������������ͻ�������ϣ��2\n");
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
				printf("         1:��ʾ��ϣ��1          2����ʾ��ϣ��2\n");
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
				printf("         1:���ϣ��1���ҳɹ�ASL 2�����ϣ��1����ʧ��ASL\n");
				printf("         3:���ϣ��2���ҳɹ�ASL 4�����ϣ��2����ʧ��ASL\n");
				printf("***********************************************************\n");
				scanf("%d",&menu4);
				switch(menu4){
					case 1:
						printf("��ϣ��1���ҳɹ���ƽ�����ҳ���Ϊ%g\n",HashTable1_ASL1(HT1));
						break;
					case 2:
						printf("��ϣ��1����ʧ�ܵ�ƽ�����ҳ���Ϊ%g\n",HashTable1_ASL2(HT1));
						break;
					case 3:
						printf("��ϣ��2���ҳɹ���ƽ�����ҳ���Ϊ%g\n",HashTable2_ASL1(HT2));
						break;
					case 4:
						printf("��ϣ��2����ʧ�ܵ�ƽ�����ҳ���Ϊ%g\n",HashTable2_ASL2(HT2));
						break;
					default:
						break;
				}
				break;
			case 5:
				int menu5;
				printf("����Ҫ���ҵĹؼ��֣�");
				scanf("%d",&m);
				printf("***********************************************************\n");
				printf("         1:���ϣ��1�ıȽϴ���  2�����ϣ��2�ıȽϴ���\n");
				printf("***********************************************************\n");
				scanf("%d",&menu5);
				switch(menu5){
					case 1:
						printf("�ڹ�ϣ��1�в��ҹؼ���%d��Ҫ�Ƚ�%d��\n",m,CompareTimes1(HT1,m));
						break;
					case 2:
						printf("�ڹ�ϣ��2�в��ҹؼ���%d��Ҫ�Ƚ�%d��\n",m,CompareTimes2(HT2,m));
						break;
					default:
						break;
				}
				break;
			case 6:
				printf("����Ҫ����Ĺؼ��֣�");
				scanf("%d",&m);
				InsertElement1(HT1,m);
				break;
			case 7:
				printf("����Ҫɾ���Ĺؼ��֣�");
				scanf("%d",&m);
				DelectElement1(HT1,m);
				break;
			default: break;
		}
	}	
}
