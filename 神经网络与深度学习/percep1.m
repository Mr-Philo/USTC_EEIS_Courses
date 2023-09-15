% percep1.m
% 例2.1 设计一个感知器，将二维的四组输入矢量分成两类
P=[-0.5 -0.5 0.3 0; -0.5 0.5 -0.5 1];  %输入矢量初始化
T=[1 1 0 0];  %目标矢量初始化
[R,Q]=size(P);    [S,Q]=size(T);
W=rands(S,R);
B=rands(S,Q);
max_epoch=20;  %最大训练次数
%表达式
A=hardlim(W*P+B);
for epoch=1:max_epoch
%学习
e=T-A;
dW=learnp([],P,[],[],[],[],e,[],[],[],[],[]);  %求权值的修正值
db=learnp(B,ones(1,S),[],[],[],[],e,[],[],[],[],[]);  %求偏差的修正值
W=W+dW;  %修正后的权值
B=B+db;  %修正后的偏差
A=hardlim(W*P+B)  %计算权值修正后的网络输出
end