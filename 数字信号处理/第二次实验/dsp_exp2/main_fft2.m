clear;
n=0:999;
a=0.005;f=0.002;xa=exp(-a*n).*sin(2*pi*f*n);     % ˥����������
p=500;q=5000;xb=exp(-(n-p).^2/q);                    % ��˹����
N=100;xc=sign(sign(N-n)+1);                     % ���δ�����

subplot(3,3,1);stem(xa);title('˥����������');
subplot(3,3,2);stem(xb);title('��˹����');
subplot(3,3,3);stem(xc);title('���δ�����');
fa=fft(xa,1024);subplot(3,3,4);stem((1:1024),abs(fa),'b');title('ϵͳFFT������');
fb=fft(xb,1024);subplot(3,3,5);stem((1:1024),abs(fb),'b');title('ϵͳFFT������');
fc=fft(xc,1024);subplot(3,3,6);stem((1:1024),abs(fc),'b');title('ϵͳFFT������');
mfa=myfft(xa);subplot(3,3,7);stem((1:1024),abs(mfa),'r');title('�Ա�дFFT������');
mfb=myfft(xb);subplot(3,3,8);stem((1:1024),abs(mfb),'r');title('�Ա�дFFT������');
mfc=myfft(xc);subplot(3,3,9);stem((1:1024),abs(mfc),'r');title('�Ա�дFFT������');

sum1=0;sum2=0;sum3=0;
for i=1:1000
	sum1=sum1+abs(abs(mfa(i))-abs(fa(i)));
    sum2=sum2+abs(abs(mfb(i))-abs(fb(i)));
    sum3=sum3+abs(abs(mfc(i))-abs(fc(i)));
end
disp(sum1);disp(sum2);disp(sum3);