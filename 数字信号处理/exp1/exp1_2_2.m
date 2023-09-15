% exp1_2_2.m
% 典型信号序列的产生和特性分析2

n=0:15;                         %定义序列的长度是16 
a=0.1;                            %衰减因子
f=0.0625;                            
%①f=0.0625，检查谱峰出现的位置是否正确(此时周期恰好为16），注意频谱的形状，绘制幅频特性曲线。
xb=exp(-a*n).*sin(2*pi*f*n);           %衰减正弦序列

close all                       %清除已经绘制的 x(n)图形
subplot(2,3,1);stem(xb);title('衰减正弦序列，f=0.0625');
subplot(2,3,4);stem(abs(fft(xb,16)));title('对应衰减正弦序列幅度谱'); 

%②改变 f=0.4375，再变化 f=0.5625，观察这两种情况下，频谱的形状和谱峰出现的位置，有无混淆和泄漏现象发生？
a=0.1; f=0.4375; xb=exp(-a*n).*sin(2*pi*f*n);
subplot(2,3,2);stem(xb);title('衰减正弦序列，f=0.4375');
subplot(2,3,5);stem(abs(fft(xb,16)));title('对应衰减正弦序列幅度谱'); 
a=0.1; f=0.5625; xb=exp(-a*n).*sin(2*pi*f*n);
subplot(2,3,3);stem(xb);title('衰减正弦序列，f=0.5625');
subplot(2,3,6);stem(abs(fft(xb,16)));title('对应衰减正弦序列幅度谱'); 

%选做内容
n=0:63; N=64;
a=0.1; f=0.0625; xb=exp(-a*n).*sin(2*pi*f*n);
w=0.1*randn(1,N);       % 噪声信号，在前面乘系数可控制其噪声幅度
close all; subplot(2,1,1); stem(xb+w); title('加入随机噪声的衰减正弦序列');
subplot(2,1,2); stem(abs(fft(xb+w,64)));title('加入随机噪声的衰减正弦序列幅度谱');