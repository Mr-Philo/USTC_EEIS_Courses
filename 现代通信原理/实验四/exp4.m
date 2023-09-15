clear
% 系统仿真参数 
fc = 4;             % 载波频率(Hz) 
snr = 5;            % 高斯白噪声信噪比(dB) 
fs = 16;            % 系统采样率 
ts = 1/fs;          % 系统采样间隔 
N = 1000;           % 码元数 
Ts = 1;             % 码元宽度 
Baud = 1/Ts;        % 波特率 
t = 0:ts:N*Ts-ts; 
t = t.';            % 时域序列，由N个点(码元)构成，每个点(码元)之间间隔ts(即采样间隔) 
Lt = length(t); 
N_sample = Ts*fs ;  % 以码元宽度乘上采样率，表示一个码元内要采多少样值

% 产生二进制信源 
B = randi(2,1,N)-1;         % 生成1×N维(0,1)序列
subplot(241); stairs((1:20), B(1:20));       % 画出调制信号波形，只看前十个周期 
axis([1,20,-1,2]); xlabel('码元周期T'); title('信源序列');
sym = [0;1]; 
A = sym(B+1);               % A为N×1维(0,1)序列（利用sym调制）

% 2ASK调制过程
% rectpulse(t,w)函数：产生幅度为t、宽度w的矩形脉冲信号。
S = rectpulse(A, N_sample); 
subplot(242); pwelch(S,[],[],[],fs,'centered');            % 功率谱
xlabel('Frequency (Hz)'); ylabel('Power (dB)'); title('信源序列功率谱');
X = S.*cos(2*pi*fc*t);      % 生成2ASK时域调制波形X
figure(1)
subplot(243); plot(t(1:20*fs), X(1:20*fs));       % 画出调制信号波形，只看前十个周期 
axis([1,20,-1.5,1.5]); xlabel('码元周期T'); ylabel('时域幅度'); title('2ASK时域调制波形');
% 绘制功率谱
subplot(244); pwelch(X,[],[],[],fs,'centered');            % 功率谱
% [pxx,f] = pwelch(x,window,noverlap,f,fs): returns the power spectral density (PSD) estimate
xlabel('Frequency (Hz)'); ylabel('Power (dB)'); title('2ASK频域功率谱');

% 2ASK解调过程  信道加性高斯白噪声
snr = 10*log10((Baud*2*pi/N_sample)*10^(snr/10));      % 归一化为数字信噪比
X_N = Gnoisegen(X, snr);
subplot(245); plot(t(1:20*fs), X_N(1:20*fs));
axis([1,20,-1.5,1.5]); xlabel('码元周期T'); ylabel('时域幅度'); title('加高斯白噪声后调制波形');

% 同步解调
r = X_N.*cos(2*pi*fc*t);
subplot(246); plot(t(1:20*fs), r(1:20*fs));
axis([1,20,-1.5,1.5]); xlabel('码元周期T'); ylabel('时域幅度'); title('乘相干载波后波形');

% 低通滤波器设计
wp = 2*pi*2*fc*0.5;                         % 通带截止烦率
ws = 2*pi*2*fc*0.9;                         % 阻带截止频率
Rp = 2; As = 10;                            % Rp是通带波纹，As是阻带衰诚
[N_f,wc] = buttord(wp,ws,Rp,As,'s');        % 计算巴特沃斯滤波器阶次和截止频率
[B_f,A_f] = butter(N_f,wc,'s');             % 频率变换法设计巴特沃斯低通滤波器
% 低通滤波
h = tf(B_f,A_f);                % 转换为时域传输函数
Sr = lsim(h,r,t);               % 系统h对由nst和t描述的输入信号的时域响应
subplot(247); plot(t(1:20*fs), Sr(1:20*fs));
axis([1,20,-1.5,1.5]); xlabel('码元周期T'); ylabel('时域幅度'); title('低通滤波后波形');

% 抽样判决
Br = zeros(1,N);
for i = 1:N
    Br(i) = Sr(i*fs) > 0.25;    % 抽样判决：ASK同步解调的判决门限为1/4。Br为解调后的序列，应与B高度相似
end
subplot(248); stairs((1:20), Br(1:20));       % 画出解调信号波形，只看前十个周期 
axis([1,20,-1,2]); xlabel('码元周期T'); title('解码信源序列');
 
error_rate0 = mae(B-Br)        % 误码率
exp_error_rate0 = qfunc(sqrt(10^(snr/10)))

% 探究不同snr对误比特率的影响：
M = 100;
error_rate = zeros(1,M);
exp_error_rate = zeros(1,M);
snr_list = -5:0.1:5-0.1;
for j = 1:M
    snr = snr_list(j);
    % X为ASK调制波形，没有变化
    X_N = Gnoisegen(X, 10*log10((Baud*4*pi/N_sample)*10^(snr/10)));        % 加噪（归一化数字噪声）
    r = X_N.*cos(2*pi*fc*t);        % 乘同步载波
    Sr = lsim(h,r,t);               % 低通滤波
    for i = 1:N
        Br(i) = Sr(i*fs) > 0.25;    % 抽样判决：ASK同步解调的判决门限为1/4
    end
    error_rate(j) = mae(B-Br);        % 误码率
    exp_error_rate(j) = qfunc(sqrt(10^(snr/10)));
end
figure(2)       % 绘制y轴对数型坐标，方便观察
semilogy(snr_list, error_rate,'r-'); hold on
semilogy(snr_list, exp_error_rate,'b-');
xlabel('信噪比/dB'); ylabel('误码率'); title('误码率随信噪比变化规律');
legend('测试信噪比','期望信噪比');