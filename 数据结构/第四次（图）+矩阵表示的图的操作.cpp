#include<stdio.h>
#define MAX_VertexNum 100                                              //图中的最大顶点数 
int visited[MAX_VertexNum]={0};                                        //访问标志数组 
typedef struct{                                                        //顶点结构体定义（目前只有名字，可以额外添加） 
	char* name[10];
}node;
typedef struct{                                                        //图结构体定义 （以邻接矩阵存储） 
	node vertexs[MAX_VertexNum];
	int edges[MAX_VertexNum][MAX_VertexNum];
	int v,e;
}MGraph;
void CreatMGraph(MGraph *G){                                           //创建图G 
	int i,j,a,b;
	char c;
	printf("输入表的顶点数和边数\n");
	scanf("%d,%d",&G->v,&G->e);
	getchar();
	printf("输入每个顶点的信息(名称)\n");
	for(i=1;i<=G->v;i++){
		printf("编号为%d的点：",i);
		scanf("%s",G->vertexs[i].name);
	}
	for(i=1;i<=G->v;i++)
		for(j=1;j<=G->v;j++)
			G->edges[i][j]=0;
	printf("输入每条边两端的顶点编号\n");
	for(i=1;i<=G->e;i++){
		scanf("%d,%d",&a,&b);
		G->edges[a][b]=1;
		G->edges[b][a]=1;
	}
}
void PrintMGraph(MGraph G){                                            //显示图G 
	int i,j;
	printf("图G包含%d个顶点和%d条边\n",G.v,G.e);
	printf("图G的邻接矩阵为：\n");
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
void DFS(MGraph *G,int i){                                             //从图G中的节点Vi开始深度遍历 
	int j;
	printf("%s ",G->vertexs[i].name);
	visited[i]=1;
	for(j=1;j<=G->v;j++)
		if(G->edges[i][j]==1&&visited[j]==0)
			DFS(G,j);
}
void DFSMGraph(MGraph *G){                                             //深度优先遍历图G 
	int i;
	for(i=1;i<=G->v;i++) visited[i]=0;
	for(i=1;i<=G->v;i++) if(visited[i]==0) DFS(G,i);
}
void BFS(MGraph *G,int k){                                             //从图G中的节点Vk开始广度遍历
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
void BFSMGraph(MGraph *G){                                             //广度优先遍历图G 
	int k; 
	for(k=1;k<=G->v;k++) visited[k]=0;
	for(k=1;k<=G->v;k++) if(visited[k]==0) BFS(G,k);
}
void InsertVertex(MGraph *G){                                          //在图G中插入新的顶点 
	int i;
	printf("输入插入的点的名称：\n");
	scanf("%s",G->vertexs[++G->v].name);
	for(i=1;i<=G->v;i++) G->edges[i][G->v]=0;
	for(i=1;i<=G->v;i++) G->edges[G->v][i]=0;
	printf("插入完成\n"); 
}
void InsertEdge(MGraph *G){                                            //在图G中插入新的边 
	int a,b;
	printf("输入插入的边两端的顶点编号：\n");
	scanf("%d,%d",&a,&b);
	G->edges[a][b]=1;
	G->edges[b][a]=1;
	G->e++;
	printf("插入完成\n"); 
}
int main(){                                                            //主菜单 
	MGraph G;
	int menu=1;
	while(menu!=0){
		printf("**************************************************\n");
		printf("     1：创建图            2：显示图的信息\n"); 
		printf("     3：深度优先遍历图    4：广度优先遍历图\n");
		printf("     5：插入新的顶点      6：插入新的边\n");
		printf("     0：退出\n");
		printf("**************************************************\n");
		printf("选择操作\n");
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
