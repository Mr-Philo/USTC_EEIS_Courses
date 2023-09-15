clear;
N=12;     % ѭ������
A=444.128;a=50*sqrt(2.0)*pi;T=0.001;w0=50*sqrt(2.0)*pi;     % �����źŵĳ�������
T1=zeros(1,N);T2=zeros(1,N);T3=zeros(1,N);
for i=1:N    % ѭ��N��
    n=1:(2^i);     % ÿ�����г�Ϊ2^i
    x=A*exp(-a*n*T).*sin(w0*n*T); %�����ź�

    loop=50; %ѭ��50����ƽ������ʱ��
    t1=zeros(1,loop);
    t2=zeros(1,loop);
    t3=zeros(1,loop);

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
    
%     for j=1:loop
%         tic;
%         x_dft=mydft(x); %�Լ���д��dft�㷨
%         t3(j)=toc;
%     end
    
    Et1=mean(t1);T1(i)=Et1;
    Et2=mean(t2);T2(i)=Et2;
%     Et3=mean(t3);T3(i)=Et3;
end
figure;
hold on
title('�㷨���ܶԱ�'); ylabel('��ʱ'); xlabel('���е�����2^x');
plot(T1,'r-o');
plot(T2,'b-o');
% plot(T3,'g-o');
legend('matlab�Դ�FFT','�Լ���д��FFT','�Լ���д��DFT','Location','Best'); 