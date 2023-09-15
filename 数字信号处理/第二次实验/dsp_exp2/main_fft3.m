clear;
N=5;
A=444.128;a=50*sqrt(2.0)*pi;T=0.001;w0=50*sqrt(2.0)*pi;     % �����źŵĳ�������
t1=0;t2=0;
T1=zeros(1,N);T2=zeros(1,N);
for i=1:N    % ��1������һֱ��1000������
    n=0:(10*i);     % ÿ�����г���ѭ�������ϳ�10
    x=A*exp(-a*n*T).*sin(w0*n*T); %�����ź�

    loop=20; %ѭ��20����ƽ������ʱ��
    t1=zeros(1,loop);
    t2=zeros(1,loop);

    for j=1:loop
        tic;
        x_fft=fft(x); %matlab�Դ���fft����
        t1(j)=toc;
    end

    for j=1:loop
        tic;
        x_myfft=myfft(x); %�Լ���д��fft�㷨
        t2(j)=toc;
    end
    
    for j=1:loop
        tic;
        x_dft=mydft(x); %�Լ���д��fft�㷨
        t2(j)=toc;
    end
    
    Et1=mean(t1);T1(i)=Et1;
    Et2=mean(t2);T2(i)=Et2;
    Et1=mean(t1);T1(i)=Et1;
    Et2=mean(t2);T2(i)=Et2;
end
plot(T1, 'b-o');hold on
plot(T2, 'r-o');