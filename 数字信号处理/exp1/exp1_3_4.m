% exp1_3_4.m
% 卷积定律的验证
% 利用式（1-14）将xa (n)和系统ha (n)的傅氏变换相乘，直接求得Y(e^jwk)
% 将得到的幅频特性曲线和实验1-3-3中得到的曲线进行比较，观察二者有无差异。验证卷积定律。

% xa(n):理想采样信号序列
n=1:50;                         %定义序列的长度是 50 
A=1; a=0.4; w0=2.0734; T=1;     %设置信号有关的参数和采样率 T
xa=A*exp(-a*n*T).*sin(w0*n*T);  %pi 是 MATLAB 定义的π，信号乘可采用“.*”，单一*符号表示矩阵乘
% subplot(2,3,1);stem(xa);title('理想采样信号序列xa(n)');
% subplot(2,3,4);stem(abs(fft(xa,50)));title('理想采样信号序列幅度谱'); 
% 特定冲激串序列  ha(n) = R10 (n)
n=1:50;
N=10;
ha=sign(sign(N-n)+1);
% subplot(2,3,2);stem(ha);title('特定冲击串序列ha(n)');
% subplot(2,3,5);stem(abs(fft(ha,50)));title('特定冲击串序列幅度谱');

% 利用线性卷积求信号通过系统以后的响应
y1 = conv(xa,ha);
Y1 = fft(y1,50);
% subplot(2,3,3);stem(y1);title('系统响应y(n)时域特性');
% subplot(2,3,6);stem(abs(fft(y1,100)));title('系统响应频域特性');

% 直接将xa (n)和系统ha (n)的傅氏变换相乘，求得Y(e^jwk)
Xa = fft(xa,50);
Ha = fft(ha,50);
Y2 = Xa.*Ha;
subplot(2,1,1);stem(abs(Y2));title('xa(n)的幅度谱与 ha(n)幅度谱相乘'); 
subplot(2,1,2);stem(abs(Y1));title('y(n)的幅度谱');