clear;
n=0:999;
A=444.128;
a=50*sqrt(2.0)*pi;
T=0.001;
w0=50*sqrt(2.0)*pi;
x=A*exp(-a*n*T).*sin(w0*n*T); %�����ź�

loop=200; %ѭ��200����ƽ������ʱ��
t1=zeros(1,loop);
t2=zeros(1,loop);
t3=zeros(1,loop);

for j=1:loop
    tic;
    x_fft=fft(x,1024); %matlab�Դ���fft����
    t1(j)=toc;
end

for j=1:loop
    tic;
    x_myfft=myfft(x); %�Լ���д��fft�㷨
    t2(j)=toc;
end

for j=1:loop
    tic;
    x_mydft=mydft(x); %dft�㷨
    t3(j)=toc;
end

figure(1) %�Ա����ַ�������Ľ���Ƿ�һ��
stem((1:length(x_myfft))-1,abs(x_myfft),'b');hold on;grid on;
stem((1:length(x_fft))-1,abs(x_fft),'r');
stem((1:length(x_mydft))-1,abs(x_mydft),'g');hold off;

Et1=mean(t1); %�Ա����ַ������л��ѵ�ʱ��
Et2=mean(t2);
Et3=mean(t3);
disp('�Դ�fft��'),disp(Et1);
disp('�ҵ�fft��'),disp(Et2);
disp('�ҵ�dft��'),disp(Et3);