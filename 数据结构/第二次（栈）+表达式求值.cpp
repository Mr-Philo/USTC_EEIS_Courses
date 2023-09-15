//���ʽ��ֵ�����������Ϊ�������͸�������������ʽ�ڰ������Ǻ�����ָ���� 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define OPSETSIZE 11                                                   //��������� 
#define STACK_INIT_SIZE 100                                            //�洢�ռ��ʼ������
char OPSET[OPSETSIZE]={'+','-','*','/','(',')','#','^','s','c','t'};   //��������� 
unsigned char Prior[OPSETSIZE][OPSETSIZE]={                            //���������ȹ�ϵ
'>','>','<','<','<','>','>','<','<','<','<',
'>','>','<','<','<','>','>','<','<','<','<',
'>','>','>','>','<','>','>','<','<','<','<',
'>','>','>','>','<','>','>','<','<','<','<',
'<','<','<','<','<','=','<','<','<','<','<',
'>','>','>','>',' ','>','>','>',' ',' ',' ',
'<','<','<','<','<',' ','=','<','<','<','<',
'>','>','>','>','<','>','>','>','<','<','<',
'<','<','<','<','<','=','<','<','<','<','<',
'<','<','<','<','<','=','<','<','<','<','<',
'<','<','<','<','<','=','<','<','<','<','<'
}; 
typedef struct{                                                        //�����ջ 
	char *base;
	char *top;
	int stacksize;
}StackChar;
typedef struct{                                                        //������ջ 
	double *base;
	double *top;
	int stacksize;
}StackDouble;
void InitStackChar(StackChar &S){                                      //��ʼ�������ջ 
	S.base=(char*)malloc(STACK_INIT_SIZE * sizeof(char));
	if(!S.base) exit(OVERFLOW);
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE;
}
void InitStackDouble(StackDouble &S){                                  //��ʼ��������ջ
	S.base=(double*)malloc(STACK_INIT_SIZE * sizeof(double));
	if(!S.base) exit(OVERFLOW);
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE;
}
char GetTopChar(StackChar S){                                          //���������ջջ��Ԫ�� 
	return *(S.top-1);
}
double GetTopDouble(StackDouble S){                                    //���ز�����ջջ��Ԫ��
	return *(S.top-1);
}
void PushChar(StackChar &S,char e){                                    //����eΪ�µ������ջ��Ԫ�� 
	*S.top++=e;
}
void PushDouble(StackDouble &S,double e){                              //����eΪ�µĲ�����ջ��Ԫ��
	*S.top++=e;
}
void PopChar(StackChar &S,char &e){                                    //ɾ�������ջջ��Ԫ�أ�����e������ֵ
	e=*--S.top;
} 
void PopDouble(StackDouble &S,double &e){                              //ɾ��������ջջ��Ԫ�أ�����e������ֵ
	e=*--S.top;
}
double Operate(double a,unsigned char op,double b){                    //���ضԲ�����a��b���������op����������ֵ 
	switch(op){
		case '+': return a+b;
		case '-': return a-b;
		case '*': return a*b;
		case '/': return a/b;
		case '^': return pow(a,b);
		default : return 0;
	} 
}
double TridonFunction(unsigned char op,double a){                      //���ضԲ�����a�����Ǻ���op������ֵ 
	switch(op){
		case 's': return sin(a);
		case 'c': return cos(a);
		case 't': return tan(a);
		default : return 0;
	} 
}
int In(char e,char* TestOp){                                           //�ж��ַ�e�Ƿ�Ϊ����� 
	for (int i=0;i<OPSETSIZE;i++) if(e==TestOp[i]) return 1;
	return 0;
}
int ReturnOpOrd(char op,char* TestOp){                                 //���������op������������е�λ�� 
	int i;
	for(i=0;i<OPSETSIZE;i++){
		if(op==TestOp[i]) return i;
	}
	return 0;
}
char precede(char op1, char op2){                                      //�Ƚ��������op1��op2�����ȼ� 
	return Prior[ReturnOpOrd(op1,OPSET)][ReturnOpOrd(op2,OPSET)];
}
double EvaluateExpression(char* Expression){                           //������ʽ��ֵ
	StackChar OPTR; 
	StackDouble OPND;
	double a,b,result,num;
	char *c,x,theta,intermediate[2],data[10]; //intermediate�ǽ�ÿ���ַ�����������ý�飬data�ǽ��ַ�����Ϊ��������ý�� 
	c=Expression;
	InitStackChar(OPTR);
	InitStackDouble(OPND);
	PushChar(OPTR,'#');
	strcpy(data,"\0");
	while (*c!='#'||GetTopChar(OPTR)!='#'){ //ѭ���������������ʽ��ȡ��#�������ջ��Ҳֻ��# 
		if(!In(*c,OPSET)){ //*c�ǲ�����������ʱ�洢��data���棬����������һ�ַ� 
			intermediate[0]=*c;
			intermediate[1]='\0';
			strcat(data,intermediate);
			c++;
			if(In(*c,OPSET)){ //����ȡ�������ʱ��������ֹ����֮ǰ��ʱ�洢data����ַ���ת��Ϊ���������������ջ
				num=(double)atof(data);
				PushDouble(OPND,num);
				strcpy(data,"\0");
			}
		}
		else{ //*c�����������֮���ʱ�����ջջ��Ԫ�رȽ����ȼ� 
			switch(precede(GetTopChar(OPTR),*c)){ 
				case '<': //ջ��Ԫ�����ȼ��ͣ�ֱ�ӽ�*c��ջ 
					if(*c=='s'||*c=='c'||*c=='t'){ //*c�����Ǻ�����ָ���������λ 
						PushChar(OPTR,*c);
						c=c+4;
						break;
					}
					else{ //*c�������������ָ�������һλ 
						PushChar(OPTR,*c); 
						c++;
						break;
					}
				case '=': //���ȼ���ȣ�*c��������
					if(GetTopChar(OPTR)=='s'||GetTopChar(OPTR)=='c'||GetTopChar(OPTR)=='t'){ //��ջ��Ԫ��Ϊ���Ǻ�������ջ������Ͳ����������Ľ���������ջ
						PopChar(OPTR,theta);
						PopDouble(OPND,a);
						result=TridonFunction(theta,a);
						PushDouble(OPND,result);
						c++;
						break;
					}
					else{ //��ջ��Ԫ��Ϊ����������ջ
						PopChar(OPTR,x);
						c++;
						break;
					}
				case '>': //ջ��Ԫ�����ȼ��ߣ���ջ������Ͳ����������Ľ���������ջ
					PopChar(OPTR,theta);
					PopDouble(OPND,b);
					PopDouble(OPND,a);
					result=Operate(a,theta,b);
					PushDouble(OPND,result);
					break;
			}
		}
	}
	return GetTopDouble(OPND); //ѭ�������󷵻ز�����ջջ��Ԫ�� 
}
int main(){                                                            //������
	char s[1000];
	int c=1;
	while(c==1){
		printf("��������ʽ(��#����)��\n");
		scanf("%s",s);
		printf("�������ǣ�%g\n",EvaluateExpression(s));
		printf("�Ƿ������(������1 ������0)\n");
		scanf("%d",&c);
	}
} 
