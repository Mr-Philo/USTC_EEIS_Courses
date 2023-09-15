clear;
n=0:999;
a=0.005;f=0.002;xa=exp(-a*n).*sin(2*pi*f*n);     % 衰减正弦序列
p=500;q=5000;xb=exp(-(n-p).^2/q);                    % 高斯序列
N=100;xc=sign(sign(N-n)+1);                     % 矩形窗序列

subplot(3,3,1);stem(xa);title('衰减正弦序列');
subplot(3,3,2);stem(xb);title('高斯序列');
subplot(3,3,3);stem(xc);title('矩形窗序列');
fa=fft(xa,1024);subplot(3,3,4);stem((1:1024),abs(fa),'b');title('系统FFT幅度谱');
fb=fft(xb,1024);subplot(3,3,5);stem((1:1024),abs(fb),'b');title('系统FFT幅度谱');
fc=fft(xc,1024);subplot(3,3,6);stem((1:1024),abs(fc),'b');title('系统FFT幅度谱');
mfa=myfft(xa);subplot(3,3,7);stem((1:1024),abs(mfa),'r');title('自编写FFT幅度谱');
mfb=myfft(xb);subplot(3,3,8);stem((1:1024),abs(mfb),'r');title('自编写FFT幅度谱');
mfc=myfft(xc);subplot(3,3,9);stem((1:1024),abs(mfc),'r');title('自编写FFT幅度谱');

sum1=0;sum2=0;sum3=0;
for i=1:1000
	sum1=sum1+abs(abs(mfa(i))-abs(fa(i)));
    sum2=sum2+abs(abs(mfb(i))-abs(fb(i)));
    sum3=sum3+abs(abs(mfc(i))-abs(fc(i)));
end
disp(sum1);disp(sum2);disp(sum3);