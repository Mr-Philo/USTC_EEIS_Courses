% xc(n):矩形序列 1<=N<=10
n=1:50;

N=10;
xc=sign(sign(N-n)+1);
subplot(2,4,1);stem(xc);title('矩形窗序列xc(n)');
subplot(2,4,5);stem(abs(fft(xc,50)));title('矩形窗序列幅度谱');

h=0.9.^n;
subplot(2,4,2);stem(h);title('系统单位冲激响应h(n)');
subplot(2,4,6);stem(abs(fft(h,50)));title('h(n)幅度谱');

y = conv(xc,h);
subplot(2,4,3);stem(y);title('系统响应y(n)时域特性');
subplot(2,4,7);stem(abs(fft(y,100)));title('系统响应频域特性');

y = conv(xc,xc);
subplot(2,4,4);stem(y);title('h=xc时，系统响应y(n)时域特性');
subplot(2,4,8);stem(abs(fft(y,100)));title('h=xc时，系统响应频域特性');