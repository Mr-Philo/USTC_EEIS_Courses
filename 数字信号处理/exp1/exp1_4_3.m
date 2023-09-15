% xc(n):十点矩形序列 1<=N<=10
% 欲求其奇偶序列，要将其定义域扩展至n=负数的情况
n=-10:10;     % 算上原点处xc(0)=1，共21个点
N=10;
xc=sign(sign(n)+sign(N-n));
xcc=sign(sign(-n)+sign(N+n));   %xc做时域反转
xe=0.5*(xc+xcc);    % 偶分量
xo=0.5*(xc-xcc);    % 奇分量

figure(1);
subplot(2,3,1);stem(n,xc);title('矩形窗序列xc(n)');axis([-10,10,-1,1]);
subplot(2,3,4);stem(abs(fft(xc)));title('矩形窗序列幅度谱');
subplot(2,3,2);stem(n,xe);title('偶分量xe(n)');axis([-10,10,-1,1]);
subplot(2,3,5);stem(abs(fft(xe)));title('偶分量幅度谱');
subplot(2,3,3);stem(n,xo);title('奇分量xo(n)');axis([-10,10,-1,1]);
subplot(2,3,6);stem(abs(fft(xo)));title('奇分量幅度谱');

figure(2);
subplot(2,2,3);stem(abs(fft(xc)));title('矩形窗序列幅度谱');
subplot(2,2,2);stem(abs(fft(xe)));title('偶分量幅度谱');
subplot(2,2,1);stem(abs(fft(xo)));title('奇分量幅度谱');
subplot(2,2,4);stem(abs(fft(xe))+abs(fft(xo)));title('奇偶分量幅度谱之和');