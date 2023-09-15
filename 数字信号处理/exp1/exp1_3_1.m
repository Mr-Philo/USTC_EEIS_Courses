% exp1_3_1.m
% 离散信号、系统和系统响应的分析
% 观察信号xb (n)和系统hb (n)的时域和幅频特性；利用线性卷积求信号通过系统以后的响应。
% 比较系统响应和系统hb (n)的时域及幅频特性。注意它们之间有无差异，绘出图形。

% xb(n):单位脉冲序列
n=1:50;                         %定义序列的长度是 50
xb=zeros(1,50);                 %注意：MATLAB 中数组下标从 1 开始
xb(1)=1;
subplot(2,3,1);stem(xb);title('单位冲击信号序列xb(n)');
subplot(2,3,4);stem(abs(fft(xb,50)));title('单位冲击信号序列幅度谱'); 
% 特定冲激串序列  hb(n) = δ (n) + 2.5δ (n −1) + 2.5δ (n − 2) + δ (n − 3)
n=1:50;
hb=zeros(1,50);
hb(1)=1; hb(2)=2.5; hb(3)=2.5; hb(4)=1;
subplot(2,3,2);stem(hb);title('特定冲击串序列hb(n)');
subplot(2,3,5);stem(abs(fft(hb,50)));title('单位冲击信号序列幅度谱');

% 利用线性卷积求信号通过系统以后的响应
y = conv(xb,hb);
subplot(2,3,3);stem(y);title('系统响应y(n)时域特性');
subplot(2,3,6);stem(abs(fft(y,50)));title('系统响应频域特性');