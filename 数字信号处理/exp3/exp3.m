% 第四部分：滤波器的实际应用

% 用Matlab产生信号x(t)=0.5+1.2sin(2πf1 t)+0.5cos(2πf2 t)+ω(t)
% 其中f1=50Hz，f2=200Hz，ω(t)为高斯白噪声，采样频率fs=1000Hz。观察其频谱特性。
f1=50;f2=200;fs=1000;
n=1:1024;   % 取1024点
x=0.5+1.2*sin(2*pi*f1*n/fs)+0.5*cos(2*pi*f2*n/fs);
x=x+awgn(x,10,'measured');      %在信号中加入高斯白噪声;参数二为信噪比SNR=10，单位dB;参数'measured'表示在添加噪声之前测量x的能量

figure('name','β=4');
freqz(Num1_1+Num1_2);
figure('name','β=6');
freqz(Num2_1+Num2_2);
figure('name','β=8');
freqz(Num3_1+Num3_2);