clear;
N=12;     % 循环次数
A=444.128;a=50*sqrt(2.0)*pi;T=0.001;w0=50*sqrt(2.0)*pi;     % 测试信号的常数参数
T1=zeros(1,N);T2=zeros(1,N);T3=zeros(1,N);
for i=1:N    % 循环N次
    n=1:(2^i);     % 每次序列长为2^i
    x=A*exp(-a*n*T).*sin(w0*n*T); %测试信号

    loop=50; %循环50次求平均运行时间
    t1=zeros(1,loop);
    t2=zeros(1,loop);
    t3=zeros(1,loop);

    for j=1:loop
        tic;
        x_fft=fft(x); %matlab自带的fft函数
        t1(j)=toc;
    end

    for j=1:loop
        tic;
        x_myfft=myfft(x); %自己编写的fft算法
        t2(j)=toc;
    end
    
%     for j=1:loop
%         tic;
%         x_dft=mydft(x); %自己编写的dft算法
%         t3(j)=toc;
%     end
    
    Et1=mean(t1);T1(i)=Et1;
    Et2=mean(t2);T2(i)=Et2;
%     Et3=mean(t3);T3(i)=Et3;
end
figure;
hold on
title('算法性能对比'); ylabel('耗时'); xlabel('序列点数：2^x');
plot(T1,'r-o');
plot(T2,'b-o');
% plot(T3,'g-o');
legend('matlab自带FFT','自己编写的FFT','自己编写的DFT','Location','Best'); 