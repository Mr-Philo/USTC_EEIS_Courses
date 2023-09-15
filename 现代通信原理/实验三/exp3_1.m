clear
Fs = 64;        % 采样频率 
Fd = 1;         % 符号率1Hz
N = 200;        % 码元序列长度
% 模拟生成N=200长的BPSK调制后的序列（即-1，1序列）
BPSK0 = 2*randi([0,1], N, 1) - 1;       % 生成N×1维度的随机1,-1序列（用随机0/2序列减1构造）
BPSKN0 = Gnoisegen(BPSK0,20);             % 加20dB高斯白噪声的模拟BPSK序列
BPSK = upsample(BPSK0, Fs/Fd);          % y = upsample(x,n) 通过在样本之间插入 n - 1个零来增加 x 的采样率.如果 x 是矩阵，则该函数将每一列视为单独的序列。
BPSKN = upsample(BPSKN0, Fs/Fd); 

% 模拟生成N=200长的4ASK调制后的序列（即-1，1序列）
ASK0 = randi([1,4], N, 1)-1;           % 生成N×1维度的随机0,1,2,3序列
ASKN0 = Gnoisegen(ASK0,20);
ASK = upsample(ASK0, Fs/Fd);
ASKN = upsample(ASKN0, Fs/Fd);

H1 = rcosine(Fd, Fs, 'fir', 0.5);
H2 = rcosine(Fd, Fs, 'fir', 1);

ch = [0.9806; -0.1961];
ch = ch/norm(ch);       % 模拟引入码间串扰信道

% BPSK: 无噪声，滚降系数0.5，无码间串扰
X1 = filter(H1, 1, BPSK);       % filter函数的用法：Y = FILTER(B,A,X) ，输入X为滤波前序列，Y为滤波结果序列，B/A 提供滤波器系数，B为分子， A为分母
% plot (X1(1:1280));           % A(i:j): 取出矩阵A的第i~j个值操作（按列优先）
Y1 = X1(Fs*4+1:end-Fs*4);
eyediagram (Y1, Fs*2);       % 创建y的眼图，每个轨迹包括2倍Fs的采样点
title('BPSK:no noise, a=0.5, no interference');

% BPSK: 20dB加性白噪声，滚降系数0.5，无码间串扰
X2 = filter(H1, 1, BPSKN);
% plot (X2(1:1280));
Y2 = X2(Fs*4+1:end-Fs*4);
eyediagram (Y2, Fs*2);
title('BPSK:20dB noise, a=0.5, no interference');

% BPSK: 无噪声，滚降系数1，无码间串扰
X3 = filter(H2, 1, BPSK);
% plot (X3(1:1280));
Y3 = X3(Fs*4+1:end-Fs*4);
eyediagram (Y3, Fs*2);
title('BPSK:no noise, a=1, no interference');

% BPSK: 无噪声，滚降系数1，有码间串扰
% X4 = filter(H1, 1, BPSK);
X4 = filter(ch, 1, X1);     % 加入码间串扰后的序列
% plot (X4(1:1280));
Y4 = X4(Fs*4+1:end-Fs*4);
eyediagram (Y4, Fs*2);
title('BPSK:no noise, a=0.5, interference');

% 4ASK: 无噪声，滚降系数0.5，无码间串扰
X5 = filter(H1, 1, ASK);
% plot (X5(1:1280));
Y5 = X5(Fs*4+1:end-Fs*4);
eyediagram (Y5, Fs*2);
title('4ASK:no noise, a=0.5, no interference');

% ASK: 20dB加性白噪声，滚降系数0.5，无码间串扰
X6 = filter(H1, 1, ASKN);
% plot (X6(1:1280));
Y6 = X6(Fs*4+1:end-Fs*4);
eyediagram (Y6, Fs*2);
title('4ASK:20dB noise, a=0.5, no interference');

% ASK: 无噪声，滚降系数1，无码间串扰
X7 = filter(H2, 1, ASK);
% plot (X7(1:1280));
Y7 = X7(Fs*4+1:end-Fs*4);
eyediagram (Y7, Fs*2);
title('4ASK:no noise, a=1, no interference');

% ASK: 无噪声，滚降系数0.5，有码间串扰
X8 = filter(ch, 1, X5);
% plot (X8(1:1280));
Y8 = X8(Fs*4+1:end-Fs*4);
eyediagram (Y8, Fs*2);
title('4ASK:no noise, a=0.5, interference');
