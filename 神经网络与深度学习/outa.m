% 例1.1 计算单层S型激活函数输出
% outa.m
Q=4;
R=3;  %Q为列数，R为行数
S=5;  %神经元数
W=ones(S,R);  %将数1赋予S*R维权矩阵W
B=ones(S,Q);  %将数1赋予S*Q维偏差矩阵B
P=ones(R,Q);  %将数1赋予P*Q维输入矩阵P
n=W*P+B;  %计算加权输入和
A=tansig(n)  %计算网络输出，其中激活函数为双曲正切S型
% 每一语句后的分号;表示在运行此命令后不显示其计算结果，若采用逗号或无符号则在运行后给出结果
% 诸如此类的命令使用help命令可以很方便地查询