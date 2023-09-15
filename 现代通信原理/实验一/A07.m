T = 1;
N_sample = 128;     % 为了画波形，设置每个周期的采样点数
dt = 1/N_sample;    % 时间分辨率
t= 0:dt:T-dt;
st = [ones(1, N_sample/2), zeros(1, N_sample/2)];   % 依据T将信号离散化

subplot(411);
% 画出st的原始时域信号
plot(t,st);axis([0,1,-2,2]);xlabel('t');ylabel('原信号s(t)');
hold on;

subplot(412);
% 画出sf的幅度谱，不含相位
[f,sf] = T2F(t,st);
plot(f,abs(sf)); axis([-10,10,0,1]); xlabel('f'); ylabel('FFT法计算|S(f)|');
hold on;

subplot(413); 
% 依据傅里叶变换求信号频谱
sff = (exp(-1i*pi*f*T)-1)./(-1i*2*pi*f);    % 傅里叶变换公式
sff(N_sample/2+1) = 0.5;     % 信号频谱在f=0时取0.5，单独列出
plot(f,abs(sff)); axis([-10,10,0,1]); xlabel('f'); ylabel('公式法计算|S(f)|');
hold on;

subplot(414);
% 进行离散傅立叶反变换，求原始信号
[t,st]= F2T (f,sf);         
plot(t,st); axis([0,1,-2,2]); xlabel('t');ylabel('恢复的 s(t)');
hold off;