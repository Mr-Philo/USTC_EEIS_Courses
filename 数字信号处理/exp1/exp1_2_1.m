% exp1_2_1.m
% 典型信号序列的产生和特性分析1

n=0:15;                         %定义序列的长度是16 
close all                       %清除已经绘制的 x(n)图形

p=8;q=2;xa=exp(-(n-p).^2/q);    %高斯序列
subplot(2,5,1);stem(xa);title('p=8,q=2高斯序列');
subplot(2,5,6);stem(abs(fft(xa,16)));title('p=8,q=2幅度谱'); 

p=8;q=4;xa=exp(-(n-p).^2/q); 
subplot(2,5,2);stem(xa);title('p=8,q=4高斯序列');
subplot(2,5,7);stem(abs(fft(xa,16)));title('p=8,q=4幅度谱'); 

p=8;q=8;xa=exp(-(n-p).^2/q); 
subplot(2,5,3);stem(xa);title('p=8,q=8高斯序列');
subplot(2,5,8);stem(abs(fft(xa,16)));title('p=8,q=8幅度谱'); 

p=13;q=8;xa=exp(-(n-p).^2/q); 
subplot(2,5,4);stem(xa);title('p=13,q=8高斯序列');
subplot(2,5,9);stem(abs(fft(xa,16)));title('p=13,q=8幅度谱'); 

p=14;q=8;xa=exp(-(n-p).^2/q); 
subplot(2,5,5);stem(xa);title('p=14,q=8高斯序列');
subplot(2,5,10);stem(abs(fft(xa,16)));title('p=14,q=8幅度谱'); 