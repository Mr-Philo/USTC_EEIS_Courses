#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define LIST_INIT_SIZE 20      //����ʽ����˳����ʼ������ 
#define LISTINCREMENT 5        //����ʽ����˳�����ռ�ķ�������
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
status InitPolynList(PolynList &L){                                                      //�����յĶ���ʽ����˳���
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
status AddLength(PolynList &L){                                                          //���Ӷ���ʽ˳����С
	L.pointer=(struct PolynNode **)realloc(L.pointer,(L.listsize+LISTINCREMENT)*sizeof(struct PolynNode*));
	if(!L.pointer) return OVERFLOW;
	L.listsize=L.listsize+LISTINCREMENT;
	return OK;
} 
double Readcoef(Polynoimal p){                                                           //��ϵ��
	double a; 
	a=p->coef;
	return a; 
} 
int Readexpn(Polynoimal p){                                                              //��ָ��
	int a; 
	a=p->expn;
	return a; 
}
Polynoimal MakeNode(){                                                                   //����ڵ�
	Polynoimal p;
	p=(struct PolynNode*)malloc(sizeof(struct PolynNode));
	p->next=NULL;
	return p; 
}
status SetNode(Polynoimal p,double coef,int expn){                                       //��д�ڵ�
	p->coef=coef;p->expn=expn;
	return OK;
}   
Polynoimal Copy(Polynoimal p){                                                           //���ƽ��
	Polynoimal q;
	q=MakeNode();
	q->coef=Readcoef(p);
	q->expn=Readexpn(p);
	return q;
} 
Polynoimal AdCopy(Polynoimal p){                                                         //����ϵ���෴��� 
	Polynoimal q;
	q=MakeNode();
	q->coef=-Readcoef(p);
	q->expn=Readexpn(p);
	return q;
} 
status InitPolyn(PolynList &L,int pos){                                                  //��ʼ���յĶ���ʽ
	if(pos<0||pos>=L.listsize) return ERROR; 
	Polynoimal p;
	if(*(L.pointer+pos)) return OK; 
	p=MakeNode();
	*(L.pointer+pos)=p;
	L.length++;
	return OK;
}  
status ListExist(PolynList L,int pos){                                                   //�ж�����ʽ˳����Ƿ���� 
	if(pos<0||pos>=L.listsize) return ERROR;
	struct PolynNode *q;
	q=*(L.pointer+pos);
	if(q==NULL) return FALSE;
	return TRUE;
}
status ListEmpty(PolynList L,int pos){                                                   //�ж�����ʽ˳����Ƿ�� 
	if(pos<0||pos>=L.listsize) return ERROR;
	struct PolynNode *q;
	Polynoimal p;
	q=*(L.pointer+pos);
	if(q==NULL) return FALSE;
	if(q->next==NULL) return FALSE;
	return TRUE;
}
status OrderInsert(PolynList &L,Polynoimal p,int pos){                                   //��������½ڵ�
	if(pos>=L.listsize) return ERROR; 
	int i;
	Polynoimal q=*(L.pointer+pos); 	 
	while(q->next&&q->next->expn<p->expn) q=q->next;
	p->next=q->next;
	q->next=p;
	return OK;		
}  
status PolynDelete(PolynList &L,int pos,double coef,int expn){                           //ɾ���ڵ�
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
status ClearZero(PolynList &L,int pos){                                                  //�������ʽ��ϵ��Ϊ0���� 
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
status CombineSame(PolynList &L,int pos){                                                //�ϲ�����ʽ��ͬ����
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
status PolynChange(PolynList &L,int pos,double coef,int expn,double a,int b){            //���Ľڵ��ϵ����ָ��
	PolynDelete(L,pos,coef,expn);
	Polynoimal p;
	p=MakeNode();
	SetNode(p,a,b);
	OrderInsert(L,p,pos);
	CombineSame(L,pos);
	ClearZero(L,pos);
	return OK;
}         
Polynoimal Multiply(Polynoimal p,Polynoimal q){                                          //Ԫ�����
	Polynoimal r=MakeNode();
	r->coef=p->coef*q->coef;
	r->expn=p->expn+q->expn;
	return r;
} 
Polynoimal Differential(Polynoimal p,int n){                                             //Ԫ��΢�� 
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
status Reverse(PolynList &L,int pos){                                                    //���ö���ʽ 
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
status PrintPolyn(PolynList L,int pos){                                                  //��ӡ����ʽ 
	if(pos<0||pos>=L.listsize) return ERROR;
	printf("����ʽ%d : ",pos);
	if(!ListEmpty(L,pos)) {
		printf("��\n");
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
status CreatePolyn(PolynList &L,int pos){                                                //��������ʽ 
	if(pos<0||pos>=L.listsize) return ERROR;
	double a;
	int b; 
	printf("����Ԫ�ص�ϵ����ָ��(�Զ��Ÿ���,ָ��Ϊ999ʱֹͣ¼��)\n");
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
status DestroyPolyn(PolynList &L,int pos){                                               //���ٶ���ʽ
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
status ClearPolyn(PolynList &L,int pos){                                                 //��ն���ʽ
	if(pos<0||pos>=L.listsize) return ERROR;
	DestroyPolyn(L,pos);
	Polynoimal p;
	p=MakeNode();
	SetNode(p,0,0);
	InitPolyn(L,pos);
	OrderInsert(L,p,pos);
	return OK;
}  
status CopyPolyn(PolynList &L,int pos1,int pos2){                                        //���ƶ���ʽ����ͬһ������ʽ���� 
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
status CopyYi(PolynList &L,PolynList &Q,int pos1,int pos2){                              //���ƶ���ʽ������ͬһ������ʽ���� 
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
int LengthPolyn(PolynList &L,int pos){                                                   //����ʽ����
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
double PolynValue(PolynList &L,int pos,double x){                                        //����ʽ��ֵ
	if(pos<0||pos>=L.listsize) return ERROR;
	double a=0;
	if(!ListEmpty(L,pos)){
		printf("�ն���ʽ\n");
		return OK;
	}
	if(!ListExist(L,pos)){
		printf("�޶���ʽ\n");
		return OK;
	}
	Polynoimal q=*(L.pointer+pos);	
	while(q->next){
		q=q->next;
		a+=Readcoef(q)*pow(x,Readexpn(q));
	}
	return a;
}
status AddPolyn(PolynList &L,int pos1,int pos2,int pos3){                                //����ʽ��� 
	if(pos3<0||pos2<0||pos1<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListExist(L,pos2))){
		printf("�ж���ʽ������\n");
		return OK;
	}
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("�ڴ治��������\n");
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
status SubPolyn(PolynList &L,int pos1,int pos2,int pos3){                                //����ʽ��� 
	if(pos3<0||pos2<0||pos1<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListExist(L,pos2))){
		printf("�ж���ʽ������\n");
		return OK;
	}
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("�ڴ治��������\n");
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
status MultiplyPolyn(PolynList &L,int pos1,int pos2,int pos3){                           //����ʽ��� 
	if(pos3<0||pos2<0||pos1<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListExist(L,pos2))){
		printf("�ж���ʽ������\n");
		return OK;
	}
	double coef;int expn;
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("�ڴ治��������\n");
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
status DivPolyn(PolynList &L,int pos1,int pos2,int pos3,int pos4){                       //����ʽ���
	if(pos1<0||pos2<0||pos3<0||pos4<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize||pos4>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListEmpty(L,pos2))){
		printf("�ж���ʽ����������\n");
	}
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("�ڴ治��������\n");
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
status Multiplyfang(PolynList &L,int pos1,int pos2,int n){                               //����ʽ�˷� 
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
status IndefiniteIntegral(PolynList &L,int pos1,int pos2){                               //����ʽ��������
	if(pos1<0||pos2<0||pos1>=L.listsize||pos2>=L.listsize) return ERROR;
	if(!ListExist(L,pos2)) return ERROR; 
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("�ڴ治��������\n");
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
double DefiniteIntegral(PolynList &L,int pos,double c,double d){                         //����ʽ������
	if(pos<0||pos>=L.listsize) return ERROR;
	PolynList Q;
	double C,D;
	if(InitPolynList(Q)==-2){
		printf("�ڴ治��������\n");
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
status Differentialfen(PolynList &L,int pos1,int pos2,int n){                            //����ʽ΢��
	if(pos1<0||pos2<0||pos1>=L.listsize||pos2>=L.listsize) return ERROR;
	if(!ListExist(L,pos2)) return ERROR; 
	PolynList Q;
	if(InitPolynList(Q)==-2){
		printf("�ڴ治��������\n");
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
status GCD(PolynList &L,int pos1,int pos2,int pos3){                                     //����ʽ���Լʽ
	if(pos1<0||pos2<0||pos3<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListExist(L,pos2))){
		printf("�ж���ʽ������\n");
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
status LCM(PolynList &L,int pos1,int pos2,int pos3){                                     //����ʽ��С����ʽ
	if(pos1<0||pos2<0||pos3<0||pos1>=L.listsize||pos2>=L.listsize||pos3>=L.listsize) return ERROR;
	if((!ListExist(L,pos1))||(!ListExist(L,pos2))){
		printf("�ж���ʽ������\n");
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
void makemenu(PolynList &L){                                                             //�����˵�
	int menu,pos; 
	printf("                     **************  ���� �˵�  **************\n");
	printf("                                   1 -��������ʽ\n");
	printf("                     2 -���Ӷ���ʽ��������(��ʱΪ%d��Ĭ�ϼ�%d)\n",L.listsize,LISTINCREMENT); 
	printf("                     *****************************************\n");
	printf("��ѡ��(����)�� ");
	scanf("%d",&menu);
	switch(menu){
		case 1:
			printf("������Ҫ�����λ��(0~%d)\n",L.listsize-1);
			scanf("%d",&pos);
			DestroyPolyn(L,pos);
			if(CreatePolyn(L,pos)==0){
		        printf("�����ˣ������Ƿ��еط�������Ҫ��\n");		            
	        }
			break;
		case 2:
			if(AddLength(L)==-2){
				printf("�ڴ治��������\n");
			}
			break;
		default :
			break;
	}
} 
void ptmenu(PolynList &L){                                                               //��ʾ�˵�
	int menu,pos; 
	printf("                      ******************  ��ʾ �˵�  ******************\n");
	printf("                       1 -��ʾĳһ����ʽ(����)  2 -��ʾĳһ����ʽ(����)       \n");
	printf("                       3 -��ʾȫ�����ʽ(����)  4 -��ʾȫ�����ʽ(����) \n");
	printf("                      ************************************************\n");
	printf("��ѡ��(����)�� ");
	scanf("%d",&menu);
	switch(menu){
		case 1:
			printf("������Ҫ��ʾ�Ķ���ʽ(0~%d)\n",L.listsize-1);
			scanf("%d",&pos);
			if(PrintPolyn(L,pos)==0){
		        printf("�����ˣ������Ƿ��еط�������Ҫ��\n");		            
	        }
			break;
		case 2:
			printf("������Ҫ��ʾ�Ķ���ʽ(0~%d)\n",L.listsize-1);
			scanf("%d",&pos);
			if(Reverse(L,pos)==0){
		        printf("�����ˣ������Ƿ��еط�������Ҫ��\n");		            
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
void coefumenu(PolynList &L){                                                            //�޸Ĳ˵�
	int menu,pos; 
	double coef,a;int expn,b; 
	printf("                      ******************  �޸� �˵�  ******************\n");
	printf("                          1 -���                     2 -ɾ��      \n");
	printf("                                    3 -�޸�ϵ����ָ��\n");             
	printf("                      ************************************************\n");
	printf("��ѡ��(����)�� ");
	scanf("%d",&menu);
	switch(menu){
		case 1:
		    printf("������Ҫ�����λ��(0~%d)\n",L.listsize-1);
			scanf("%d",&pos);
			if(CreatePolyn(L,pos)==0){
		        printf("�����ˣ������Ƿ��еط�������Ҫ��\n");		            
	        }	        
			break;
		case 2:
			printf("������Ҫɾ����λ��(0~%d)\n",L.listsize-1);
			printf("������Ҫ�����ϵ����ָ��(�Զ��Ÿ���)\n");
			scanf("%d,%lf,%d",&pos,&coef,&expn);
			if(PolynDelete(L,pos,coef,expn)==0){
		        printf("�����ˣ������Ƿ��еط�������Ҫ��\n");		            
	        }	        
			break;
		case 3:
			printf("������Ҫ���ĵ�λ��(0~%d)\n",L.listsize-1);
			printf("����ԭ����ϵ����ָ�����µ�ϵ����ָ��(�Զ��Ÿ���)\n");
			scanf("%d,%lf,%d,%lf,%d",&pos,&coef,&expn,&a,&b);
			if(PolynChange(L,pos,coef,expn,a,b)==0){
		        printf("�����ˣ������Ƿ��еط�������Ҫ��\n");		            
	        }	        
			break;
        default :
			break;
    }
}  
void opmenu(PolynList &L){                                                               //���������˵�
	int menu,pos1,pos2,pos3;
	double x; 
	printf("                      ******************  ���� �˵�  ******************\n");
	printf("                          1 -���ƶ���ʽ             2 -���ٶ���ʽ\n");
	printf("                          3 -�޸Ķ���ʽ             4 -��ն���ʽ\n");
	printf("                          5 -�� ��                  6 -�� ֵ\n");             
	printf("                          7 -�� ��\n");       
	printf("                      ************************************************\n");
	printf("��ѡ��(����)�� ");
	scanf("%d",&menu);
	switch(menu){
		case 1:
			printf("������Ҫ���ƵĶ���ʽ��ź���λ��(0~%d,�Զ��Ÿ���)\n",L.listsize-1);
			scanf("%d,%d",&pos1,&pos2);
			if(CopyPolyn(L,pos2,pos1)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;
		case 2:
			printf("������Ҫ���ٵĶ���ʽλ��(0~%d)\n",L.listsize-1);
			scanf("%d",&pos1); 
			if(DestroyPolyn(L,pos1)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;
		case 3:
			coefumenu(L);
			break;
		case 4:
			printf("������Ҫ��յĶ���ʽλ��(0~%d)\n",L.listsize-1);
			scanf("%d",&pos1); 
			if(ClearPolyn(L,pos1)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;
		case 5:
			printf("������Ҫ��͵Ķ���ʽλ�úʹ���λ��(0~%d,�Զ��Ÿ���)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&pos3);
			if(AddPolyn(L,pos1,pos2,pos3)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;
		case 6:
		    printf("������Ҫ��ֵ�Ķ���ʽλ��(0~%d)�ͱ�����ֵ\n",L.listsize-1);
			scanf("%d,%lf",&pos1,&x);
			printf("����ʽֵΪ%lf\n",PolynValue(L,pos1,x));
			break;
		case 7:
			printf("������Ҫ���Ķ���ʽλ�úʹ���λ��(0~%d,�Զ��Ÿ���)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&pos3);
			if(SubPolyn(L,pos1,pos2,pos3)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;
		default :
			break;
	}
}
void opmenu2(PolynList &L){                                                              //���ײ����˵�
	int menu,pos1,pos2,pos3,pos4,n; 
	double a,b;
	printf("                      ******************  ���� �˵�  ******************\n");
	printf("                        1 -΢��                       2 -��������\n");
	printf("                        3 -������                     4 -�˷�\n");
	printf("                        5 -����                       6 -�˷�\n");
	printf("                        7 -���Լʽ                 8 -��С����ʽ  \n"); 
	printf("                                   (7,8Ĭ�������ϵ��Ϊ1) \n");  
	printf("                      ************************************************\n");
	printf("��ѡ��(����)�� ");
	scanf("%d",&menu);
	switch(menu){
		case 1:
			printf("������Ҫ΢�ֵĶ���ʽ���,��λ�ú�΢�ִ���(0~%d,�Զ��Ÿ���)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&n);
			if(n<0||Differentialfen(L,pos2,pos1,n)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;
		case 2:
			printf("������Ҫ�������ֵĶ���ʽλ�úʹ���λ��(0~%d,�Զ��Ÿ���)\n",L.listsize-1);
			scanf("%d,%d",&pos1,&pos2); 
			if(IndefiniteIntegral(L,pos2,pos1)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
		    }
		    break;
		case 3:
			printf("������Ҫ�����ֵĶ���ʽλ��(0~%d,�Զ��Ÿ���)������������\n",L.listsize-1);
			scanf("%d,%lf,%lf",&pos1,&a,&b); 
			if(pos1<0||pos1>=L.listsize){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
		    }
		    printf("����ֵΪ��%lf\n",DefiniteIntegral(L,pos1,a,b));
		    break;
		case 4:
			printf("������Ҫ��˵Ķ���ʽλ�úʹ���λ��(0~%d,�Զ��Ÿ���)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&pos3);
			if(MultiplyPolyn(L,pos1,pos2,pos3)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;
		case 5:
			printf("������Ҫ����Ķ���ʽλ�ã��̺���������λ��(0~%d,λ�ö��Ÿ���,�����Կո�)\n",L.listsize-1);
			scanf("%d,%d,%d,%d",&pos1,&pos2,&pos3,&pos4);
			if(DivPolyn(L,pos1,pos2,pos3,pos4)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;
		case 6:
			printf("������Ҫ�˷��Ķ���ʽλ�úʹ���λ��(0~%d,�Զ��Ÿ���)���Լ��˷�����\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&n);
			if(Multiplyfang(L,pos1,pos2,n)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;
		case 7:
			printf("������Ҫ��Ķ���ʽλ�úʹ���λ��(0~%d,�Զ��Ÿ���)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&pos3);
			if(GCD(L,pos1,pos2,pos3)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;
		case 8:
			printf("������Ҫ��Ķ���ʽλ�úʹ���λ��(0~%d,�Զ��Ÿ���)\n",L.listsize-1);
			scanf("%d,%d,%d",&pos1,&pos2,&pos3);
			if(LCM(L,pos1,pos2,pos3)==0){
				printf("�����ˣ������Ƿ��еط�������Ҫ��\n");
			}
			break;	
		default :
			break;
    }
} 
main(){                                                                                  //���˵� 
	int menu;
	PolynList L;
	if(InitPolynList(L)==-2){
		printf("�ڴ治��������\n");
		return 0;
	}
	for(;;){
		printf("                     **************  ����ʽ������  **************\n");
		printf("                      0 -�˳�                  1 -����\n");
		printf("                      2 -��ʾ                  3 -��������\n");
		printf("                      4 -���ײ���              5 -����\n");
		printf("                                ��ʱ������ĿΪ%d\n",L.length);
		printf("                    ************************************************\n");
		printf("��ѡ��(����)�� ");
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
