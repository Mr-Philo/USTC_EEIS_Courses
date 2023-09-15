% 低通抽样定律
clear;
close all;
dt = 0.01;              % 时域步进
t = 0:dt:10;            % 时域范围：[0:10]，共取1001个点
xt = 0.1*cos(0.15*pi*t) + 0.5*cos(4*pi*t);
[f,xf] = T2F(t,xt);     % 求其频谱

% 抽样信号，抽样频率为4Hz
fs = 4;
sdt = 1/fs;
t1 = 0:sdt:10;
st = 0.1*cos(0.15*pi*t1) + 0.5*cos(4*pi*t1);
[f1,sf] = T2F(t1,st);

% 恢复原始信号
t2 = -50:dt:50;
gt = sinc(fs*t2);           % 抽样函数sinc，为频域门信号的傅里叶反变换
stt = sigexpand(st,sdt/dt);
xt_t = conv(stt,gt);        % 时域卷积恢复

figure(1);
subplot(311); plot(t,xt,'linewidth',2); title('原始信号'); axis([0 10 -1 1]);
subplot(312); stem(t1,st,'linewidth',2); title('抽样信号'); axis([0 10 -1 1]);
subplot(313);
t3= -50:dt:60+sdt-dt;
plot(t3,xt_t,'linewidth',2); title('抽样信号恢复'); axis([0 10 -1 1]);

% 计算均方误差
xt_t_c = xt_t(1,5001:6001);     % 截取长输出数组xt_t的0<t<10部分，以便和原信号xt对比
mse = mse(xt_t_c - xt)
