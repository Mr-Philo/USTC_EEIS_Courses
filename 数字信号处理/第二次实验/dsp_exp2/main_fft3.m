clear;
N=5;
A=444.128;a=50*sqrt(2.0)*pi;T=0.001;w0=50*sqrt(2.0)*pi;     % 测试信号的常数参数
t1=0;t2=0;
T1=zeros(1,N);T2=zeros(1,N);
for i=1:N    % 从1点序列一直到1000点序列
    n=0:(10*i);     % 每次序列长在循环次数上乘10
    x=A*exp(-a*n*T).*sin(w0*n*T); %测试信号

    loop=20; %循环20次求平均运行时间
    t1=zeros(1,loop);
    t2=zeros(1,loop);

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
    
    for j=1:loop
        tic;
        x_dft=mydft(x); %自己编写的fft算法
        t2(j)=toc;
    end
    
    Et1=mean(t1);T1(i)=Et1;
    Et2=mean(t2);T2(i)=Et2;
    Et1=mean(t1);T1(i)=Et1;
    Et2=mean(t2);T2(i)=Et2;
end
plot(T1, 'b-o');hold on
plot(T2, 'r-o');