#include<stdio.h>
#define MAX_VertexNum 100                                              //ͼ�е���󶥵��� 
int visited[MAX_VertexNum]={0};                                        //���ʱ�־���� 
typedef struct{                                                        //����ṹ�嶨�壨Ŀǰֻ�����֣����Զ�����ӣ� 
	char* name[10];
}node;
typedef struct{                                                        //ͼ�ṹ�嶨�� �����ڽӾ���洢�� 
	node vertexs[MAX_VertexNum];
	int edges[MAX_VertexNum][MAX_VertexNum];
	int v,e;
}MGraph;
void CreatMGraph(MGraph *G){                                           //����ͼG 
	int i,j,a,b;
	char c;
	printf("�����Ķ������ͱ���\n");
	scanf("%d,%d",&G->v,&G->e);
	getchar();
	printf("����ÿ���������Ϣ(����)\n");
	for(i=1;i<=G->v;i++){
		printf("���Ϊ%d�ĵ㣺",i);
		scanf("%s",G->vertexs[i].name);
	}
	for(i=1;i<=G->v;i++)
		for(j=1;j<=G->v;j++)
			G->edges[i][j]=0;
	printf("����ÿ�������˵Ķ�����\n");
	for(i=1;i<=G->e;i++){
		scanf("%d,%d",&a,&b);
		G->edges[a][b]=1;
		G->edges[b][a]=1;
	}
}
void PrintMGraph(MGraph G){                                            //��ʾͼG 
	int i,j;
	printf("ͼG����%d�������%d����\n",G.v,G.e);
	printf("ͼG���ڽӾ���Ϊ��\n");
	for(i=1;i<=G.v;i++) printf("  %s",G.vertexs[i].name);
	printf("\n");
	for(i=1;i<=G.v;i++){
		printf("%s",G.vertexs[i].name);
		for(j=1;j<=G.v;j++){
			printf(" %d ",G.edges[i][j]);
		}
		printf("\n");
	}
}
void DFS(MGraph *G,int i){                                             //��ͼG�еĽڵ�Vi��ʼ��ȱ��� 
	int j;
	printf("%s ",G->vertexs[i].name);
	visited[i]=1;
	for(j=1;j<=G->v;j++)
		if(G->edges[i][j]==1&&visited[j]==0)
			DFS(G,j);
}
void DFSMGraph(MGraph *G){                                             //������ȱ���ͼG 
	int i;
	for(i=1;i<=G->v;i++) visited[i]=0;
	for(i=1;i<=G->v;i++) if(visited[i]==0) DFS(G,i);
}
void BFS(MGraph *G,int k){                                             //��ͼG�еĽڵ�Vk��ʼ��ȱ���
	int i,j,f=0,r=0;
	int cq[MAX_VertexNum];
	for(i=1;i<=G->v;i++) cq[i]=-1;
	printf("%s ",G->vertexs[k].name);
	visited[k]=1;
	cq[r]=k;
	while(cq[f]!=-1){
		i=cq[f++];
		for(j=1;j<=G->v;j++)
			if(G->edges[i][j]==1&&visited[j]==0){
				printf("%s ",G->vertexs[j].name);
				visited[j]=1;
				cq[++r]=j;
			}
	}
}
void BFSMGraph(MGraph *G){                                             //������ȱ���ͼG 
	int k; 
	for(k=1;k<=G->v;k++) visited[k]=0;
	for(k=1;k<=G->v;k++) if(visited[k]==0) BFS(G,k);
}
void InsertVertex(MGraph *G){                                          //��ͼG�в����µĶ��� 
	int i;
	printf("�������ĵ�����ƣ�\n");
	scanf("%s",G->vertexs[++G->v].name);
	for(i=1;i<=G->v;i++) G->edges[i][G->v]=0;
	for(i=1;i<=G->v;i++) G->edges[G->v][i]=0;
	printf("�������\n"); 
}
void InsertEdge(MGraph *G){                                            //��ͼG�в����µı� 
	int a,b;
	printf("�������ı����˵Ķ����ţ�\n");
	scanf("%d,%d",&a,&b);
	G->edges[a][b]=1;
	G->edges[b][a]=1;
	G->e++;
	printf("�������\n"); 
}
int main(){                                                            //���˵� 
	MGraph G;
	int menu=1;
	while(menu!=0){
		printf("**************************************************\n");
		printf("     1������ͼ            2����ʾͼ����Ϣ\n"); 
		printf("     3��������ȱ���ͼ    4��������ȱ���ͼ\n");
		printf("     5�������µĶ���      6�������µı�\n");
		printf("     0���˳�\n");
		printf("**************************************************\n");
		printf("ѡ�����\n");
		scanf("%d",&menu);
		switch(menu){
			case 0:
				return 0;
			case 1:
				CreatMGraph(&G);
				break;
			case 2:
				PrintMGraph(G);
				break;
			case 3:
				DFSMGraph(&G);
				printf("\n");
				break;
			case 4:
				BFSMGraph(&G);
				printf("\n");
				break;
			case 5:
				InsertVertex(&G);
				break;
			case 6:
				InsertEdge(&G);
				break;
			default:
				break;
		}
	}
}
