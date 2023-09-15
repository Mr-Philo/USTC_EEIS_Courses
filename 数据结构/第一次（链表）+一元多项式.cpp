#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define LIST_INIT_SIZE 20      //多项式线性顺序表初始分配量 
#define LISTINCREMENT 5        //多项式线性顺序表储存空间的分配增量
typedef int status;
typedef struct PolynNode{
	double coef;
	int expn;
	struct PolynNode *next;
}PolynNode,*Polynoimal;
typedef struct PolynList{
	 struct PolynNode **pointer;
	 
	 int length;
	 int listsize;
}PolynList;
status InitPolynList(PolynList &L){                                                      //创建空的多项式线性顺序表
	int i;
	L.pointer=(struct PolynNode **)malloc(LIST_INIT_SIZE*sizeof(struct PolynNode*));
	if(!L.pointer) return OVERFLOW;
	for(i=0;i<LIST_INIT_SIZE;i++){
		*(L.pointer+i)=NULL;
	}	
	L.length=0;
	L.listsize=LIST_INIT_SIZE;
	return OK;
}
status AddLength(PolynList &L){                                                          //增加多项式顺序表大小
	L.pointer=(struct PolynNode **)realloc(L.pointer,(L.listsize+LISTINCREMENT)*sizeof(struct PolynNode*));
	if(!L.pointer) return OVERFLOW;
	L.listsize=L.listsize+LISTINCREMENT;
	return OK;
} 
double Readcoef(Polynoimal p){                                                           //读系数
	double a; 
	a=p->coef;
	return a; 
} 
int Readexpn(Polynoimal p){                                                              //读指数
	int a; 
	a=p->expn;
	return a; 
}
Polynoimal MakeNode(){                                                                   //构造节点
	Polynoimal p;
	p=(struct PolynNode*)malloc(sizeof(struct PolynNode));
	p->next=NULL;
	return p; 
}
status SetNode(Polynoimal p,double coef,int expn){                                       //填写节点
	p->coef=coef;p->expn=expn;
	return OK;
}   
Polynoimal Copy(Polynoimal p){                                                           //复制结点
	Polynoimal q;
	q=MakeNode();
	q->coef=Readcoef(p);
	q->expn=Readexpn(p);
	return q;
} 
Polynoimal AdCopy(Polynoimal p){                                                         //复制系数相反结点 
	Polynoimal q;
	q=MakeNode();
	q->coef=-Readcoef(p);
	q->expn=Readexpn(p);
	return q;
} 
status InitPolyn(PolynList &L,int pos){                                                  //初始化空的多项式
	if(pos<0||pos>=L.listsize) return ERROR; 
	Polynoimal p;
	if(*(L.pointer+pos)) return OK; 
	p=MakeNode();
	*(L.pointer+pos)=p;
	L.length++;
	return OK;
}  
status ListExist(PolynList L,int pos){                                                   //判定多项式顺序表是否存在 
	if(pos<0||pos>=L.listsize) return ERROR;
	struct PolynNode *q;
	q=*(L.pointer+pos);
	if(q==NULL) return FALSE;
	return TRUE;
}
status ListEmpty(PolynList L,int pos){                                                   //判定多项式顺序表是否空 
	if(pos<0||pos>=L.listsize) return ERROR;
	struct PolynNode *q;
	Polynoimal p;
	q=*(L.pointer+pos);
	if(q==NULL) return FALSE;
	if(q->next==NULL) return FALSE;
	return TRUE;
}
status OrderInsert(PolynList &L,Polynoimal p,int pos){                                   //有序插入新节点
	if(pos>=L.listsize) return ERROR; 
	int i;
	Polynoimal q=*(L.pointer+pos); 	 
	while(q->next&&q->next->expn<p->expn) q=q->next;
	p->next=q->next;
	q->next=p;
	return OK;		
}  
status PolynDelete(PolynList &L,int pos,double coef,int expn){                           //删除节点
	if(pos<0||pos>=L.listsize) return ERROR;
	Polynoimal q=*(L.pointer+pos),p;
	while(q->next){
		p=q;
		q=q->next;
		if(q->coef==coef&&q->expn==expn){
			p->next=q->next;
			free(q);
			return OK;
		}
	}
	return FALSE;
} 
status ClearZero(PolynList &L,int pos){                                                  //清除多项式中系数为0的项 
	if(pos<0||pos>=L.listsize) return ERROR;
	Polynoimal q=*(L.pointer+pos);
	Polynoimal p;
	while(q&&q->next){
        p=q;
		q=q->next;
		if(q->coef==0){
			p->next=q->next;
			free(q);
			q=p;			
	    }	    
	}
	q=*(L.pointer+pos);
	if(!q->next){
		Polynoimal s;
		s=MakeNode();
        SetNode(s,0,0);
        OrderInsert(L,s,pos);
	}
	return OK; 
}
status CombineSame(PolynList &L,int pos){                                                //合并多项式的同类项
	if(pos<0||pos>=L.listsize) return ERROR;
	Polynoimal q=*(L.pointer+pos),p,r;	
	while(q&&q->next&&q->next->next){
		p=q->next;r=q->next->next;
		if(p->expn==r->expn){
			r->coef=p->coef+r->coef;
			q->next=r;
			free(p);
			continue;
		}
		q=q->next;	
	}
	return OK;
}  
status PolynChange(PolynList &L,int pos,double coef,int expn,double a,int b){            //更改节点的系数和指数
	PolynDelete(L,pos,coef,expn);
	Polynoimal p;
	p=MakeNode();
	SetNode(p,a,b);
	OrderInsert(L,p,pos);
	CombineSame(L,pos);
	ClearZero(L,pos);
	return OK;
}         
Polynoimal Multiply(Polynoimal p,Polynoimal q){                                          //元素相乘
	Polynoimal r=MakeNode();
	r->coef=p->coef*q->coef;
	r->expn=p->expn+q->expn;
	return r;
} 
Polynoimal Differential(Polynoimal p,int n){                                             //元素微分 
	Polynoimal q=MakeNode();
	q->coef=Readcoef(p);
	q->expn=Readexpn(p);
	int i;
	for(i=0;i<n;i++){
		q->coef=q->coef*q->expn;
		q->expn=q->expn-1;
	}
	return q;
}
status Reverse(PolynList &L,int pos){                                                    //逆置多项式 
	if(pos<0||pos>=L.listsize) return ERROR;
	Polynoimal q,p,r;
	p=q=*(L.pointer+pos);
	if(!q) return ERROR; 
	q=q->next;
	p->next=NULL;
	while(q){
		r=q;
		q=q->next;		
		r->next=p->next;
		p->next=r;		
	}
	return OK;
}
status PrintPolyn(PolynList L,int pos){                                                  //打印多项式 
	if(pos<0||pos>=L.listsize) return ERROR;
	printf("多项式%d : ",pos);
	if(!ListEmpty(L,pos)) {
		printf("空\n");
		return OK;
	}
    Polynoimal q;
	q=*(L.pointer+pos);
	q=q->next;
	if(q->expn==0) printf("f(x)=%g",q->coef);
	else printf("f(x)=%g*x^%d",q->coef,q->expn);
	q=q->next;
	while(q){
		if(q->expn==0) printf("%+g",q->coef);
		else printf("%+g*x^%d",q->coef,q->expn);
		q=q->next;
	}
	printf("\n"); 
	return OK;
}
status CreatePolyn(PolynList &L,int pos){                                                //创建多项式 
	if(pos<0||pos>=L.listsize) return ERROR;
	double a;
	int b; 
	printf("输入元素的系数和指数(以逗号隔开,指数为999时停止录入)\n");
	while(1){
		scanf("%lf,%d",&a,&b);
		if(b==999) break;
		Polynoimal p;
		p=MakeNode();
		SetNode(p,a,b);
		InitPolyn(L,pos);
		OrderInsert(L,p,pos);
	}
	CombineSame(L,pos);
	ClearZero(L,pos);
	return OK;	
}
status DestroyPolyn(PolynList &L,int pos){                                               //销毁多项式
	if(pos<0||pos>=L.listsize) return ERROR;
    Polynoimal q=*(L.pointer+pos),p;
    while(q){
	    p=q;	    
	    q=q->next;
	    free(p);
	}
	*(L.pointer+pos)=NULL;
	return OK;
}  
status ClearPolyn(PolynList &L,int pos){                                                 //清空多项式
	if(pos<0||pos>=L.listsize) return ERROR;
	DestroyPolyn(L,pos);
	Polynoimal p;
	p=MakeNode();
	SetNode(p,0,0);
	InitPolyn(L,pos);
	OrderInsert(L,p,pos);
	return OK;
}  
status CopyPolyn(PolynList &L,int pos1,int pos2){                                        //复制多项式（在同一个多项式链表） 
	if(pos1<0||pos2<0||pos1>=L.listsize||pos2>=L.listsize) return ERROR;
	if(!ListExist(L,pos2)) return ERROR; 
	if(!ListExist(L,pos1)) InitPolyn(L,pos1);
	if(pos1==pos2) return OK;
	Polynoimal la=*(L.pointer+pos1);
	Polynoimal lb=*(L.pointer+pos2);
	while(lb->next){
		lb=lb->next;
		double coef;int expn;
		Polynoimal p;
		p=MakeNode();
	    coef=Readcoef(lb);
		expn=Readexpn(lb);
	    SetNode(p,coef,expn);
	    OrderInsert(L,p,pos1);
	}
	return OK;	
} 
status CopyYi(PolynList &L,PolynList &Q,int pos1,int pos2){                              //复制多项式（不在同一个多项式链表） 
	if(!ListExist(L,pos2)) return ERROR; 
	if(!ListExist(Q,pos1)) InitPolyn(Q,pos1);
	Polynoimal la=*(Q.pointer+pos1);
	Polynoimal lb=*(L.pointer+pos2);
	while(lb->next){
		lb=lb->next;
		double coef;int expn;
		Polynoimal p;
		p=MakeNode();
	    coef=Readcoef(lb);
		expn=Readexpn(lb);
	    SetNode(p,coef,expn);
	    OrderInsert(Q,p,pos1);
	}
	return OK;	
}
int LengthPolyn(PolynList &L,int pos){                                                   //多项式长度
	if(pos<0||pos>=L.listsize) return ERROR;
	int a=0;
	PolynNode **q;
	q=L.pointer+pos;
	if(!q) return -1;
	Polynoimal p;
	p=*q;
	while(p->next){
		a++;
		p=p->next;
	}
	return a;
} 
double PolynValue(PolynList &L,int pos,double x){                                        //多项式求值
	if(pos<0||pos>=L.listsize) return ERROR;
	double a=0;
	if(!ListEmpty(L,pos)){
		printf("空多项式\n");
		return OK;
	}
	if(!ListExist(L,pos)){
		printf("无多项式\n");
		return OK;
	}
	Polynoimal q=*(L.pointer+pos);	
	while(q->next){
		q=q->next;
		a+=Readcoef(q)*pow(x,Readexpn(q));
	}
	return a;
}
status AddPolyn(PolynList &L,int pos1,int pos2,int pos3){                                //多项式相加 
	if(pos3<0||pos2<0||pos1<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListExist(L,pos2))){
		printf("有多项式不存在\n");
		return OK;
	}
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("内存不够！！！\n");
		return 0;
	}
	CopyYi(L,Q,1,pos1);
	CopyYi(L,Q,2,pos2);
	Polynoimal la=*(Q.pointer+1);
	Polynoimal lb=*(Q.pointer+2);
	Polynoimal p;
	if(!ListExist(Q,3)) InitPolyn(Q,3);
	while(la->next){
		la=la->next;
		p=Copy(la);
		OrderInsert(Q,p,3);
    }
    while(lb->next){
    	lb=lb->next;
    	p=Copy(lb);
		OrderInsert(Q,p,3);
	}
	CombineSame(Q,3);
	ClearZero(Q,3);
	DestroyPolyn(L,pos3);
	CopyYi(Q,L,pos3,3);
	DestroyPolyn(Q,1);
	DestroyPolyn(Q,2);
	DestroyPolyn(Q,3);
	free(Q.pointer);
	return OK;
}
status SubPolyn(PolynList &L,int pos1,int pos2,int pos3){                                //多项式相减 
	if(pos3<0||pos2<0||pos1<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListExist(L,pos2))){
		printf("有多项式不存在\n");
		return OK;
	}
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("内存不够！！！\n");
		return 0;
	}
	CopyYi(L,Q,1,pos1);
	CopyYi(L,Q,2,pos2);
	Polynoimal la=*(Q.pointer+1);
	Polynoimal lb=*(Q.pointer+2);
	Polynoimal p;
	if(!ListExist(Q,3)) InitPolyn(Q,3);
	while(la->next){
		la=la->next;
		p=Copy(la);
		OrderInsert(Q,p,3);
    }
    while(lb->next){
    	lb=lb->next;
    	p=AdCopy(lb);
		OrderInsert(Q,p,3);
	}
	CombineSame(Q,3);
	ClearZero(Q,3);	
	DestroyPolyn(L,pos3);
	CopyYi(Q,L,pos3,3);
	DestroyPolyn(Q,1);
	DestroyPolyn(Q,2);
	DestroyPolyn(Q,3);
	free(Q.pointer);
}
status MultiplyPolyn(PolynList &L,int pos1,int pos2,int pos3){                           //多项式相乘 
	if(pos3<0||pos2<0||pos1<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListExist(L,pos2))){
		printf("有多项式不存在\n");
		return OK;
	}
	double coef;int expn;
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("内存不够！！！\n");
		return 0;
	}
	CopyYi(L,Q,1,pos1);
	CopyYi(L,Q,2,pos2);
	Polynoimal la=*(Q.pointer+1);
	Polynoimal lb=*(Q.pointer+2);
	Polynoimal p;
	if(!ListExist(Q,3)) InitPolyn(Q,3);
	while(la->next){
		la=la->next;
		while(lb->next){
			lb=lb->next;
    	    p=Multiply(lb,la);
    	    OrderInsert(Q,p,3);
	    }
	    lb=*(Q.pointer+2);
	}
	CombineSame(Q,3);
	ClearZero(Q,3);
	DestroyPolyn(L,pos3);
	CopyYi(Q,L,pos3,3);
	DestroyPolyn(Q,1);
	DestroyPolyn(Q,2);
	DestroyPolyn(Q,3);
	free(Q.pointer);
	return OK;
}
status DivPolyn(PolynList &L,int pos1,int pos2,int pos3,int pos4){                       //多项式相除
	if(pos1<0||pos2<0||pos3<0||pos4<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize||pos4>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListEmpty(L,pos2))){
		printf("有多项式不符合条件\n");
	}
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("内存不够！！！\n");
		return 0;
	}
	CopyYi(L,Q,1,pos1);
	CopyYi(L,Q,2,pos2);
	Polynoimal tb=*(Q.pointer+2);
	Polynoimal ta=*(Q.pointer+1);
	Reverse(Q,1);
	Reverse(Q,2);
	Polynoimal lb=*(Q.pointer+2);
	Polynoimal la=*(Q.pointer+1);
	lb=lb->next;
	Polynoimal p,q;
	if(!ListExist(Q,3)) InitPolyn(Q,3);
	if(!ListExist(Q,4)) InitPolyn(Q,4);	
	while(la->next){
		la=la->next;
		if(la->expn>=lb->expn&&la->coef!=0){
		    if(!ListExist(Q,5)) InitPolyn(Q,5);
			p=MakeNode();
			p->expn=la->expn-lb->expn;
			p->coef=la->coef/lb->coef;
			q=Copy(p);
			OrderInsert(Q,p,3);
			OrderInsert(Q,q,5);;
			MultiplyPolyn(Q,2,5,4);       
			SubPolyn(Q,1,4,1); 
			Reverse(Q,1);
			la=*(Q.pointer+1);
			DestroyPolyn(Q,5);
		}
		else break;		
	}
	ClearZero(Q,3);
	DestroyPolyn(L,pos4);
	DestroyPolyn(L,pos3);
	CopyYi(Q,L,pos3,3);
	CopyYi(Q,L,pos4,1);
	DestroyPolyn(Q,1);
	DestroyPolyn(Q,2);
	DestroyPolyn(Q,3);
	DestroyPolyn(Q,4);
	DestroyPolyn(Q,5);
	free(Q.pointer);
	return OK;
} 
status Multiplyfang(PolynList &L,int pos1,int pos2,int n){                               //多项式乘方 
    int i;
    if(n<=0) return ERROR;
    else if(n==0){
    	Polynoimal p;
    	p=MakeNode();
		SetNode(p,1,0);
		InitPolyn(L,pos2);
		OrderInsert(L,p,pos2);
	} 
	else if(n==1){
		CopyPolyn(L,pos2,pos1);
	} 
	else{
		MultiplyPolyn(L,pos1,pos1,pos2);
    	for(i=0;i<n-2;i++){
    		MultiplyPolyn(L,pos1,pos2,pos2);
		}
	}
	return OK; 
}
status IndefiniteIntegral(PolynList &L,int pos1,int pos2){                               //多项式不定积分
	if(pos1<0||pos2<0||pos1>=L.listsize||pos2>=L.listsize) return ERROR;
	if(!ListExist(L,pos2)) return ERROR; 
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("内存不够！！！\n");
		return 0;
	}
	InitPolyn(Q,1);
	InitPolyn(Q,2);
	CopyYi(L,Q,1,pos2);
	Polynoimal lb=*(Q.pointer+1);
	Polynoimal p;
	while(lb->next){
		p=MakeNode();
		lb=lb->next;
		p->expn=lb->expn+1;
		p->coef=lb->coef/p->expn;
		OrderInsert(Q,p,2);
	}
	DestroyPolyn(L,pos1);
	InitPolyn(L,pos1);
	CopyYi(Q,L,pos1,2);
	DestroyPolyn(Q,1);
	DestroyPolyn(Q,2);
	free(Q.pointer);
	return OK;
}
double DefiniteIntegral(PolynList &L,int pos,double c,double d){                         //多项式定积分
	if(pos<0||pos>=L.listsize) return ERROR;
	PolynList Q;
	double C,D;
	if(InitPolynList(Q)==-2){
		printf("内存不够！！！\n");
		return 0;
	}
	double a,b; 
	InitPolyn(Q,1);
	InitPolyn(Q,2);
	CopyYi(L,Q,1,pos);
	Polynoimal lb=*(Q.pointer+1);
	Polynoimal p;
	while(lb->next){
		p=MakeNode();
		lb=lb->next;
		p->expn=lb->expn+1;
		p->coef=lb->coef/p->expn;
		OrderInsert(Q,p,2);
	}
	C=PolynValue(Q,2,c);
	D=PolynValue(Q,2,d);
	DestroyPolyn(Q,1);
	DestroyPolyn(Q,2);
	free(Q.pointer);
	return D-C;
}
status Differentialfen(PolynList &L,int pos1,int pos2,int n){                            //多项式微分
	if(pos1<0||pos2<0||pos1>=L.listsize||pos2>=L.listsize) return ERROR;
	if(!ListExist(L,pos2)) return ERROR; 
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("内存不够！！！\n");
		return 0;
	}
	InitPolyn(Q,1);
	InitPolyn(Q,2);
	CopyYi(L,Q,1,pos2);
	Polynoimal lb=*(Q.pointer+1);
	Polynoimal p;
	while(lb->next){
		lb=lb->next;
		p=Differential(lb,n);
		OrderInsert(Q,p,2);
	}
	DestroyPolyn(L,pos1);
	InitPolyn(L,pos1);
	CopyYi(Q,L,pos1,2);
	DestroyPolyn(Q,1);
	DestroyPolyn(Q,2);
	free(Q.pointer);
	return OK;
}
status GCD(PolynList &L,int pos1,int pos2,int pos3){                                     //多项式最大公约式
	if(pos1<0||pos2<0||pos3<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListExist(L,pos2))){
		printf("有多项式不存在\n");
	}
	PolynList Q;
	InitPolynList(Q);
	CopyYi(L,Q,1,pos1);
	CopyYi(L,Q,2,pos2);	
	Polynoimal p,la,lb,q;
	int flag=0;
	while(1){
		Reverse(Q,1);
	    Reverse(Q,2);
		la=*(Q.pointer+1);
	    lb=*(Q.pointer+2);	    
	    if(la->next->expn>=lb->next->expn){
		    Reverse(Q,1);
	        Reverse(Q,2);
		    DivPolyn(Q,1,2,3,1);
		    Reverse(Q,1);
		    p=*(Q.pointer+1);
		    if(p->next->expn==0){
		    	flag=1;
		    	break;
			}
			Reverse(Q,1);
	    }
	    else{
		    Reverse(Q,1);
	        Reverse(Q,2);
		    DivPolyn(Q,2,1,3,2);
		    Reverse(Q,2);
		    p=*(Q.pointer+2);
		    if(p->next->expn==0){		    	
		    	flag=2;
		    	break;
			}
			Reverse(Q,2);
	    }
    }    
    if(flag=1){
		p=*(Q.pointer+1);
		if(p->next->coef==0){
			Reverse(Q,2);
			la=*(Q.pointer+2);
			q=MakeNode();
			SetNode(q,Readcoef(la->next),0);
			Reverse(Q,2);
			InitPolyn(Q,4);
			OrderInsert(Q,q,4);
			DivPolyn(Q,2,4,2,4);
			DestroyPolyn(L,pos3);
			CopyYi(Q,L,pos3,2);
			return OK;
		}
	}
	if(flag=2){
		p=*(Q.pointer+2);
		if(p->next->coef==0){
			Reverse(Q,1);
			la=*(Q.pointer+1);
			q=MakeNode();
			SetNode(q,Readcoef(la->next),0);
			Reverse(Q,1);
			InitPolyn(Q,4);
			OrderInsert(Q,q,4);
			DivPolyn(Q,1,4,1,4);
			DestroyPolyn(L,pos3);
			CopyYi(Q,L,pos3,1);
			return OK;
		}
    }
    p=MakeNode();
    SetNode(p,1,0);
    DestroyPolyn(L,pos3);
    InitPolyn(L,pos3);
	OrderInsert(L,p,pos3);
	DestroyPolyn(Q,1);
	DestroyPolyn(Q,2);
	DestroyPolyn(Q,3);
	DestroyPolyn(Q,4);
	free(Q.pointer);
    return OK;
}
status LCM(PolynList &L,int pos1,int pos2,int pos3){                                     //多项式最小公倍式
	if(pos1<0||pos2<0||pos3<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListExist(L,pos2))){
		printf("有多项式不存在\n");
	}
	PolynList Q;
	Polynoimal p,q; 
	InitPolynList(Q);
	CopyYi(L,Q,1,pos1);
	CopyYi(L,Q,2,pos2);
	MultiplyPolyn(Q,1,2,3);
	GCD(Q,1,2,4);
	DivPolyn(Q,3,4,5,6);
	Reverse(Q,5);
	p=*(Q.pointer+5);
	q=MakeNode();
	SetNode(q,Readcoef(p->next),0);
	Reverse(Q,5);
	InitPolyn(Q,6);
	OrderInsert(Q,q,6);
	DivPolyn(Q,5,6,5,6);
	DestroyPolyn(L,pos3);
	CopyYi(Q,L,pos3,5);
	DestroyPolyn(Q,1);
	DestroyPolyn(Q,2);
	DestroyPolyn(Q,3);
	DestroyPolyn(Q,4);
	DestroyPolyn(Q,5);
	DestroyPolyn(Q,6);
	free(Q.pointer);
	return OK;
}
void makemenu(PolynList &L){                                                             //创建菜单
	int menu,pos; 
	printf("                     **************  创建 菜单  **************\n");
	printf("                                   1 -创建多项式\n");
	printf("                     2 -增加多项式储存上限(此时为%d，默认加%d)\n",L.listsize,LISTINCREMENT); 
	printf("                     *****************************************\n");
	printf("请选择(数字)： ");
	scanf("%d",&menu);
	switch(menu){
		case 1:
			printf("输入你要插入的位置(0~%d)\n",L.listsize-1);
			scanf("%d",&pos);
			DestroyPolyn(L,pos);
			if(CreatePolyn(L,pos)==0){
		        printf("出错了，请检查是否有地方不符合要求\n");		            
	        }
			break;
		case 2:
			if(AddLength(L)==-2){
				printf("内存不够！！！\n");
			}
			break;
		default :
			break;
	}
} 
void ptmenu(PolynList &L){                                                               //显示菜单
	int menu,pos; 
	printf("                      ******************  显示 菜单  ******************\n");
	printf("                       1 -显示某一多项式(升序)  2 -显示某一多项式(降序)       \n");
	printf("                       3 -显示全体多项式(升序)  4 -显示全体多项式(降序) \n");
	printf("                      ************************************************\n");
	printf("请选择(数字)： ");
	scanf("%d",&menu);
	switch(menu){
		case 1:
			printf("输入你要显示的多项式(0~%d)\n",L.listsize-1);
			scanf("%d",&pos);
			if(PrintPolyn(L,pos)==0){
		        printf("出错了，请检查是否有地方不符合要求\n");		            
	        }
			break;
		case 2:
			printf("输入你要显示的多项式(0~%d)\n",L.listsize-1);
			scanf("%d",&pos);
			if(Reverse(L,pos)==0){
		        printf("出错了，请检查是否有地方不符合要求\n");		            
	        }
	        PrintPolyn(L,pos);
	        Reverse(L,pos);
			break;
		case 3:
			int i;
			for(i=0;i<L.listsize;i++){	
				PrintPolyn(L,i);
			}
			break;
		case 4:
			int j;
			for(j=0;j<L.listsize;j++){
				Reverse(L,j);
				PrintPolyn(L,j);
				Reverse(L,j);
			}
			break;
		default :
			break;
	}
}
void coefumenu(PolynList &L){                                                            //修改菜单
	int menu,pos; 
	double coef,a;int expn,b; 
	printf("                      ******************  修改 菜单  ******************\n");
	printf("                          1 -添加                     2 -删除      \n");
	printf("                                    3 -修改系数或指数\n");             
	printf("                      ************************************************\n");
	printf("请选择(数字)： ");
	scanf("%d",&menu);
	switch(menu){
		case 1:
		    printf("输入你要插入的位置(0~%d)\n",L.listsize-1);
			scanf("%d",&pos);
			if(CreatePolyn(L,pos)==0){
		        printf("出错了，请检查是否有地方不符合要求\n");		            
	        }	        
			break;
		case 2:
			printf("输入你要删除的位置(0~%d)\n",L.listsize-1);
			printf("输入你要插入的系数和指数(以逗号隔开)\n");
			scanf("%d,%lf,%d",&pos,&coef,&expn);
			if(PolynDelete(L,pos,coef,expn)==0){
		        printf("出错了，请检查是否有地方不符合要求\n");		            
	        }	        
			break;
		case 3:
			printf("输入你要更改的位置(0~%d)\n",L.listsize-1);
			printf("输入原来的系数和指数，新的系数和指数(以逗号隔开)\n");
			scanf("%d,%lf,%d,%lf,%d",&pos,&coef,&expn,&a,&b);
			if(PolynChange(L,pos,coef,expn,a,b)==0){
		        printf("出错了，请检查是否有地方不符合要求\n");		            
	        }	        
			break;
        default :
			break;
    }
}  
void opmenu(PolynList &L){                                                               //基本操作菜单
	int menu,pos1,pos2,pos3;
	double x; 
	printf("                      ******************  操作 菜单  ******************\n");
	printf("                          1 -复制多项式             2 -销毁多项式\n");
	printf("                          3 -修改多项式             4 -清空多项式\n");
	printf("                          5 -求 和                  6 -求 值\n");             
	printf("                          7 -求 差\n");       
	printf("                      ************************************************\n");
	printf("请选择(数字)： ");
	scanf("%d",&menu);
	switch(menu){
		case 1:
			printf("输入你要复制的多项式编号和新位置(0~%d,以逗号隔开)\n",L.listsize-1);
			scanf("%d,%d",&pos1,&pos2);
			if(CopyPolyn(L,pos2,pos1)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;
		case 2:
			printf("输入你要销毁的多项式位置(0~%d)\n",L.listsize-1);
			scanf("%d",&pos1); 
			if(DestroyPolyn(L,pos1)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;
		case 3:
			coefumenu(L);
			break;
		case 4:
			printf("输入你要清空的多项式位置(0~%d)\n",L.listsize-1);
			scanf("%d",&pos1); 
			if(ClearPolyn(L,pos1)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;
		case 5:
			printf("输入你要求和的多项式位置和储存位置(0~%d,以逗号隔开)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&pos3);
			if(AddPolyn(L,pos1,pos2,pos3)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;
		case 6:
		    printf("输入你要求值的多项式位置(0~%d)和变量的值\n",L.listsize-1);
			scanf("%d,%lf",&pos1,&x);
			printf("多项式值为%lf\n",PolynValue(L,pos1,x));
			break;
		case 7:
			printf("输入你要求差的多项式位置和储存位置(0~%d,以逗号隔开)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&pos3);
			if(SubPolyn(L,pos1,pos2,pos3)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;
		default :
			break;
	}
}
void opmenu2(PolynList &L){                                                              //进阶操作菜单
	int menu,pos1,pos2,pos3,pos4,n; 
	double a,b;
	printf("                      ******************  操作 菜单  ******************\n");
	printf("                        1 -微分                       2 -不定积分\n");
	printf("                        3 -定积分                     4 -乘法\n");
	printf("                        5 -除法                       6 -乘方\n");
	printf("                        7 -最大公约式                 8 -最小公倍式  \n"); 
	printf("                                   (7,8默认最高项系数为1) \n");  
	printf("                      ************************************************\n");
	printf("请选择(数字)： ");
	scanf("%d",&menu);
	switch(menu){
		case 1:
			printf("输入你要微分的多项式编号,新位置和微分次数(0~%d,以逗号隔开)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&n);
			if(n<0||Differentialfen(L,pos2,pos1,n)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;
		case 2:
			printf("输入你要不定积分的多项式位置和储存位置(0~%d,以逗号隔开)\n",L.listsize-1);
			scanf("%d,%d",&pos1,&pos2); 
			if(IndefiniteIntegral(L,pos2,pos1)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
		    }
		    break;
		case 3:
			printf("输入你要定积分的多项式位置(0~%d,以逗号隔开)，积分上下限\n",L.listsize-1);
			scanf("%d,%lf,%lf",&pos1,&a,&b); 
			if(pos1<0||pos1>=L.listsize){
				printf("出错了，请检查是否有地方不符合要求\n");
		    }
		    printf("积分值为：%lf\n",DefiniteIntegral(L,pos1,a,b));
		    break;
		case 4:
			printf("输入你要相乘的多项式位置和储存位置(0~%d,以逗号隔开)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&pos3);
			if(MultiplyPolyn(L,pos1,pos2,pos3)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;
		case 5:
			printf("输入你要相除的多项式位置，商和余数储存位置(0~%d,位置逗号隔开,类型以空格)\n",L.listsize-1);
			scanf("%d,%d,%d,%d",&pos1,&pos2,&pos3,&pos4);
			if(DivPolyn(L,pos1,pos2,pos3,pos4)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;
		case 6:
			printf("输入你要乘方的多项式位置和储存位置(0~%d,以逗号隔开)，以及乘方次数\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&n);
			if(Multiplyfang(L,pos1,pos2,n)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;
		case 7:
			printf("输入你要求的多项式位置和储存位置(0~%d,以逗号隔开)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&pos3);
			if(GCD(L,pos1,pos2,pos3)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;
		case 8:
			printf("输入你要求的多项式位置和储存位置(0~%d,以逗号隔开)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&pos3);
			if(LCM(L,pos1,pos2,pos3)==0){
				printf("出错了，请检查是否有地方不符合要求\n");
			}
			break;	
		default :
			break;
    }
} 
main(){                                                                                  //主菜单 
	int menu;
	PolynList L;
	if(InitPolynList(L)==-2){
		printf("内存不够！！！\n");
		return 0;
	}
	for(;;){
		printf("                     **************  多项式运算器  **************\n");
		printf("                      0 -退出                  1 -创建\n");
		printf("                      2 -显示                  3 -基本操作\n");
		printf("                      4 -进阶操作              5 -清屏\n");
		printf("                                此时储存数目为%d\n",L.length);
		printf("                    ************************************************\n");
		printf("请选择(数字)： ");
		scanf("%d",&menu);
		switch(menu){
			case 0:
			    return 0;
			case 1:
				makemenu(L);
				break;
			case 2:
			    ptmenu(L);
				break; 
			case 3:
			    opmenu(L);
			    break;
			case 4:
				opmenu2(L);
				break;
			case 5:
				system("cls");
				break;
			default :
				break;
		}
		system("pause");
	}
}
