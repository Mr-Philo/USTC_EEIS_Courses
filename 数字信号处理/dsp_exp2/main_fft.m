clear;
n=0:999;
A=444.128;
a=50*sqrt(2.0)*pi;
T=0.001;
w0=50*sqrt(2.0)*pi;
x=A*exp(-a*n*T).*sin(w0*n*T); %测试信号

loop=200; %循环200次求平均运行时间
t1=zeros(1,loop);
t2=zeros(1,loop);
t3=zeros(1,loop);

for j=1:loop
    tic;
    x_fft=fft(x,1024); %matlab自带的fft函数
    t1(j)=toc;
end

for j=1:loop
    tic;
    x_myfft=myfft(x); %自己编写的fft算法
    t2(j)=toc;
end

for j=1:loop
    tic;
    x_mydft=mydft(x); %dft算法
    t3(j)=toc;
end

figure(1) %对比三种方法算出的结果是否一致
stem((1:length(x_myfft))-1,abs(x_myfft),'b');hold on;grid on;
stem((1:length(x_fft))-1,abs(x_fft),'r');
stem((1:length(x_mydft))-1,abs(x_mydft),'g');hold off;

Et1=mean(t1); %对比三种方法运行花费的时间
Et2=mean(t2);
Et3=mean(t3);
disp('自带fft：'),disp(Et1);
disp('我的fft：'),disp(Et2);
disp('我的dft：'),disp(Et3);