% exp1_1.m
% 理想采样信号序列的特性分析:对单边衰减正弦信号理想采样，分析频谱特性

n=0:49;                         %定义序列的长度是 50 
A=444.128;                      %设置信号有关的参数
a=50*sqrt(2.0)*pi; 
w0=50*sqrt(2.0)*pi;             %ω符号在 MatLab 中不能输入，用 w 代替

T=1/1000;                        %采样率
% T=1/300;                        %改变参数设置1
% T=1/200;                        %改变参数设置2
x=A*exp(-a*n*T).*sin(w0*n*T);   %pi 是 MATLAB 定义的π，信号乘可采用“.*”，单一*符号表示矩阵乘
close all                       %清除已经绘制的 x(n)图形
subplot(2,3,1);                 %函数subplot用于在一个视窗中同时绘出多个曲线
stem(x);                        %绘制 x(n)的图形（针状图）
title('理想采样信号序列:T=1/1000');      %设置结果图形的标题

% 绘制信号x(n)的幅度谱和相位谱
k=0:49;
W=exp(-1i*pi/25); X=x*W.^(n'*k);
% li：复数单位  W:旋转因子
magX=abs(X);        %绘制 x(n)的幅度谱 
subplot(2,3,4);stem(magX);title('理想采样信号序列的幅度谱:T=1/1000'); 
% angX=angle(X);      %绘制 x(n)的相位谱
% subplot(3,1,3);stem(angX);title('理想采样信号序列的相位谱');

T=1/300;x=A*exp(-a*n*T).*sin(w0*n*T);
subplot(2,3,2);stem(x);title('理想采样信号序列:T=1/300');
k=0:49;W=exp(-1i*pi/25); X=x*W.^(n'*k);
subplot(2,3,5);stem(abs(X));title('理想采样信号序列的幅度谱:T=1/300'); 

T=1/200;x=A*exp(-a*n*T).*sin(w0*n*T);
subplot(2,3,3);stem(x);title('理想采样信号序列:T=1/200');
k=0:49;W=exp(-1i*pi/25); X=x*W.^(n'*k);
subplot(2,3,6);stem(abs(X));title('理想采样信号序列的幅度谱:T=1/200'); 