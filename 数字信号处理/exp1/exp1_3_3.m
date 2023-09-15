% exp1_3_3.m
% 离散信号、系统和系统响应的分析
% 将实验步骤 2-（2）中的信号换为xa (n)，其中A =1,a = 0.4,Ω0= 2.0734,T =1。重复实验 2-（2）各步骤
% 改变xa (n)的参数a = 0.1再重复实验 2-（2）各步骤
% 改变参数Ω0=1.2516，重复实验 2-（2）各步骤
% 在实验中观察改变a和Ω0对信号及系统响应的时域和幅频特性的影响，绘制相应的图形。

% xa(n):理想采样信号序列
n=1:50;                         %定义序列的长度是 50 
A=1; a=0.4; w0=2.0734; T=1;     %设置信号有关的参数和采样率 T
xa=A*exp(-a*n*T).*sin(w0*n*T);  %pi 是 MATLAB 定义的π，信号乘可采用“.*”，单一*符号表示矩阵乘
subplot(2,3,1);stem(xa);title('理想采样信号序列xa(n)');
subplot(2,3,4);stem(abs(fft(xa,50)));title('理想采样信号序列幅度谱'); 
% 特定冲激串序列  ha(n) = R10 (n)
n=1:50;
N=10;
ha=sign(sign(N-n)+1);
subplot(2,3,2);stem(ha);title('特定冲击串序列ha(n)');
subplot(2,3,5);stem(abs(fft(ha,50)));title('特定冲击串序列幅度谱');

% 利用线性卷积求信号通过系统以后的响应
y = conv(xa,ha);
subplot(2,3,3);stem(y);title('系统响应y(n)时域特性');
subplot(2,3,6);stem(abs(fft(y,100)));title('系统响应频域特性');

close all
a=0.1;xa=A*exp(-a*n*T).*sin(w0*n*T);
subplot(2,4,1);stem(xa);title('α=0.1原序列x(n)时域图');
subplot(2,4,5);stem(abs(fft(xa,50)));title('α=0.1原序列x(n)幅度谱'); 
y = conv(xa,ha);
subplot(2,4,2);stem(y);title('α=0.1系统响应y(n)时域特性');
subplot(2,4,6);stem(abs(fft(y,100)));title('α=0.1系统响应y(n)频域特性');

a=0.4;w0=1.2516;xa=A*exp(-a*n*T).*sin(w0*n*T);
subplot(2,4,3);stem(xa);title('w0=1.2516原序列x(n)时域图');
subplot(2,4,7);stem(abs(fft(xa,50)));title('w0=1.2516原序列x(n)幅度谱'); 
y = conv(xa,ha);
subplot(2,4,4);stem(y);title('w0=1.2516系统响应y(n)时域特性');
subplot(2,4,8);stem(abs(fft(y,100)));title('w0=1.2516系统响应y(n)频域特性');

close all
a=0.1;w0=0.3142;xa=A*exp(-a*n*T).*sin(w0*n*T);
subplot(2,2,1);stem(xa);title('α=0.1,w0=0.3142原序列x(n)时域图');
subplot(2,2,3);stem(abs(fft(xa,50)));title('α=0.1,w0=0.3142原序列x(n)幅度谱'); 
y = conv(xa,ha);
subplot(2,2,2);stem(y);title('α=0.1,w0=0.3142系统响应y(n)时域特性');
subplot(2,2,4);stem(abs(fft(y,100)));title('α=0.1,w0=0.3142系统响应y(n)频域特性');