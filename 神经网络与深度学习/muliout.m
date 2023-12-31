% muliout.m
P=[0.1 0.5;0.3 -0.2];  %输入已知矢量数据
S1=2;S2=3;S3=5;  %各层的节点数
[R,Q]=size(P);  %求出输入矢量的行和列
W1=rands(S1,R);  %给第一隐含层权值赋（-1,1）间的随机值
B1=rands(S1,Q);
W2=rands(S2,S1);  %给第二隐含层权值赋（-1,1）间的随机值
B2=rands(S2,Q);
W3=rands(S3,S2);  %给输出层权值赋（-1,1）间的随机值
B3=rands(S3,Q);

n1=W1*P+B1;  %计算第一层的加权输入和
A1=hardlims(n1)  %计算第一层的输出表达式，函数hardlims为输出为（1，-1）的二值函数

n2=W2*A1+B2;  %计算第二层的加权输入和
A2=logsig(n2)  %计算第二层的输出表达式，函数logsig为对数S型函数

n3=W3*A2+B3;  %计算输出层的加权输入和
A3=purelin(n3)  %计算输出层的输出表达式，函数purelin为线性函数
% 注意各层输入输出矩阵之间的维数关系！