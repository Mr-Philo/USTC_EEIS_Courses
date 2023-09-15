//表达式求值（允许操作数为长整数和浮点数，允许表达式内包含三角函数和指数） 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define OPSETSIZE 11                                                   //运算符个数 
#define STACK_INIT_SIZE 100                                            //存储空间初始分配量
char OPSET[OPSETSIZE]={'+','-','*','/','(',')','#','^','s','c','t'};   //运算符集合 
unsigned char Prior[OPSETSIZE][OPSETSIZE]={                            //算符间的优先关系
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
typedef struct{                                                        //运算符栈 
	char *base;
	char *top;
	int stacksize;
}StackChar;
typedef struct{                                                        //操作数栈 
	double *base;
	double *top;
	int stacksize;
}StackDouble;
void InitStackChar(StackChar &S){                                      //初始化运算符栈 
	S.base=(char*)malloc(STACK_INIT_SIZE * sizeof(char));
	if(!S.base) exit(OVERFLOW);
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE;
}
void InitStackDouble(StackDouble &S){                                  //初始化操作数栈
	S.base=(double*)malloc(STACK_INIT_SIZE * sizeof(double));
	if(!S.base) exit(OVERFLOW);
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE;
}
char GetTopChar(StackChar S){                                          //返回运算符栈栈顶元素 
	return *(S.top-1);
}
double GetTopDouble(StackDouble S){                                    //返回操作数栈栈顶元素
	return *(S.top-1);
}
void PushChar(StackChar &S,char e){                                    //插入e为新的运算符栈顶元素 
	*S.top++=e;
}
void PushDouble(StackDouble &S,double e){                              //插入e为新的操作数栈顶元素
	*S.top++=e;
}
void PopChar(StackChar &S,char &e){                                    //删除运算符栈栈顶元素，并用e返回其值
	e=*--S.top;
} 
void PopDouble(StackDouble &S,double &e){                              //删除操作数栈栈顶元素，并用e返回其值
	e=*--S.top;
}
double Operate(double a,unsigned char op,double b){                    //返回对操作数a，b进行运算符op操作的运算值 
	switch(op){
		case '+': return a+b;
		case '-': return a-b;
		case '*': return a*b;
		case '/': return a/b;
		case '^': return pow(a,b);
		default : return 0;
	} 
}
double TridonFunction(unsigned char op,double a){                      //返回对操作数a求三角函数op的运算值 
	switch(op){
		case 's': return sin(a);
		case 'c': return cos(a);
		case 't': return tan(a);
		default : return 0;
	} 
}
int In(char e,char* TestOp){                                           //判断字符e是否为运算符 
	for (int i=0;i<OPSETSIZE;i++) if(e==TestOp[i]) return 1;
	return 0;
}
int ReturnOpOrd(char op,char* TestOp){                                 //返回运算符op在运算符数组中的位置 
	int i;
	for(i=0;i<OPSETSIZE;i++){
		if(op==TestOp[i]) return i;
	}
	return 0;
}
char precede(char op1, char op2){                                      //比较两个算符op1和op2的优先级 
	return Prior[ReturnOpOrd(op1,OPSET)][ReturnOpOrd(op2,OPSET)];
}
double EvaluateExpression(char* Expression){                           //计算表达式的值
	StackChar OPTR; 
	StackDouble OPND;
	double a,b,result,num;
	char *c,x,theta,intermediate[2],data[10]; //intermediate是将每个字符连接起来的媒介，data是将字符串变为浮点数的媒介 
	c=Expression;
	InitStackChar(OPTR);
	InitStackDouble(OPND);
	PushChar(OPTR,'#');
	strcpy(data,"\0");
	while (*c!='#'||GetTopChar(OPTR)!='#'){ //循环结束条件：表达式读取到#且运算符栈内也只有# 
		if(!In(*c,OPSET)){ //*c是操作数，先临时存储到data里面，继续接收下一字符 
			intermediate[0]=*c;
			intermediate[1]='\0';
			strcat(data,intermediate);
			c++;
			if(In(*c,OPSET)){ //当读取到运算符时，接收中止，将之前临时存储data里的字符串转换为浮点数并入操作数栈
				num=(double)atof(data);
				PushDouble(OPND,num);
				strcpy(data,"\0");
			}
		}
		else{ //*c是运算符，将之与此时运算符栈栈顶元素比较优先级 
			switch(precede(GetTopChar(OPTR),*c)){ 
				case '<': //栈顶元素优先级低，直接将*c入栈 
					if(*c=='s'||*c=='c'||*c=='t'){ //*c是三角函数，指针向后跳四位 
						PushChar(OPTR,*c);
						c=c+4;
						break;
					}
					else{ //*c是其他运算符，指针向后跳一位 
						PushChar(OPTR,*c); 
						c++;
						break;
					}
				case '=': //优先级相等，*c是右括号
					if(GetTopChar(OPTR)=='s'||GetTopChar(OPTR)=='c'||GetTopChar(OPTR)=='t'){ //若栈顶元素为三角函数则退栈并将其和操作数运算后的结果入操作数栈
						PopChar(OPTR,theta);
						PopDouble(OPND,a);
						result=TridonFunction(theta,a);
						PushDouble(OPND,result);
						c++;
						break;
					}
					else{ //若栈顶元素为左括号则退栈
						PopChar(OPTR,x);
						c++;
						break;
					}
				case '>': //栈顶元素优先级高，退栈并将其和操作数运算后的结果入操作数栈
					PopChar(OPTR,theta);
					PopDouble(OPND,b);
					PopDouble(OPND,a);
					result=Operate(a,theta,b);
					PushDouble(OPND,result);
					break;
			}
		}
	}
	return GetTopDouble(OPND); //循环结束后返回操作数栈栈顶元素 
}
int main(){                                                            //主函数
	char s[1000];
	int c=1;
	while(c==1){
		printf("请输入表达式(以#结束)：\n");
		scanf("%s",s);
		printf("运算结果是：%g\n",EvaluateExpression(s));
		printf("是否继续？(继续：1 结束：0)\n");
		scanf("%d",&c);
	}
} 
