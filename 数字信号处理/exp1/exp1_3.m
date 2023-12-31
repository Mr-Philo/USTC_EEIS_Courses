% exp1_3.m
% 离散信号、系统和系统响应的分析

% 信号序列的产生：①理想采样信号序列
n=1:50;                         %定义序列的长度是 50 
A=1; a=0.4; w0=2.0734; T=1;     %设置信号有关的参数和采样率 T
xa=A*exp(-a*n*T).*sin(w0*n*T);  %pi 是 MATLAB 定义的π，信号乘可采用“.*”，单一*符号表示矩阵乘
close all
subplot(4,1,1);stem(xa);title('理想采样信号序列');      %设置结果图形的标题
% ②单位脉冲序列
n=1:50;                         %定义序列的长度是 50
xb=zeros(1,50);                 %注意：MATLAB 中数组下标从 1 开始
xb(1)=1;
subplot(4,1,2);stem(xb);title('单位冲击信号序列');
% ③矩形序列 1<=N<=10
n=1:50;
xc=sign(sign(10-n)+1);
subplot(4,1,3); stem(xc);title('矩形窗序列');
% ④特定冲激串序列  x(n) = δ (n) + 2.5δ (n −1) + 2.5δ (n − 2) + δ (n − 3)
n=1:50;
hb=zeros(1,50);
hb(1)=1; hb(2)=2.5; hb(3)=2.5; hb(4)=1;
subplot(4,1,4);stem(hb);title('特定冲击串序列');