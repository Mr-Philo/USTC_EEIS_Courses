% exp1_2_3.m
% 典型信号序列的产生和特性分析3

n=0:15;                         %定义序列的长度是16
for i=1:4
    xc(i)=(i-1)+1;xcc(i)=xc(i);             %注意：matlab中数组下标以1开始
    xd(i)=4-(i-1);xdd(i)=xd(i);
end
for i=5:8
    xc(i)=8-(i-1);xcc(i)=xc(i);
    xd(i)=(i-1)-3;xdd(i)=xd(i);
end
for i=9:16
    xcc(i)=0;xdd(i)=0;
end
% xc为8点有限长三角波序列，xcc为16点的xc末尾补0序列；
% xd为8点有限长三角波序列，xdd为16点的xd末尾补0序列；

close all                       %清除已经绘制的 x(n)图形
subplot(3,2,1);stem(xc);title('三角波序列');
subplot(3,2,2);stem(xd);title('反三角波序列');

% k=0:15;   % W=exp(-1i*2*pi/16); X=xbb*W.^(n'*k);  % li：复数单位  W:旋转因子
% 改用直接调用fft函数
subplot(3,2,3);stem(abs(fft(xc,8)));title('三角波序列8点FFT的幅度谱'); 
subplot(3,2,4);stem(abs(fft(xd,8)));title('反三角波序列8点FFT的幅度谱'); 
subplot(3,2,5);stem(abs(fft(xc,16)));title('三角波末尾补0序列16点FFT的幅度谱'); 
subplot(3,2,6);stem(abs(fft(xd,16)));title('反三角波末尾补0序列16点FFT的幅度谱');