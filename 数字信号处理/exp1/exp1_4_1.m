n=1:50;                         %定义序列的长度是 50 
A=1; w0=0.2; T=1;     %设置信号有关的参数和采样率 T
a=0.4;
xa=A*exp(-a*n*T).*sin(w0*n*T);
subplot(2,5,1);stem(xa);title('α=0.4时域序列');axis([0,51,-1,1]);
subplot(2,5,6);stem(abs(fft(xa,50)));title('α=0.4幅度谱'); axis([0,51,0,20]);

a=0.2;
xa=A*exp(-a*n*T).*sin(w0*n*T);
subplot(2,5,2);stem(xa);title('α=0.2时域序列');axis([0,51,-1,1]);
subplot(2,5,7);stem(abs(fft(xa,50)));title('α=0.2幅度谱'); axis([0,51,0,20]);

a=0.1;
xa=A*exp(-a*n*T).*sin(w0*n*T);
subplot(2,5,3);stem(xa);title('α=0.1时域序列');axis([0,50,-1,1]);
subplot(2,5,8);stem(abs(fft(xa,50)));title('α=0.1幅度谱'); axis([0,51,0,20]);

a=0.05;
xa=A*exp(-a*n*T).*sin(w0*n*T);
subplot(2,5,4);stem(xa);title('α=0.05时域序列');axis([0,51,-1,1]);
subplot(2,5,9);stem(abs(fft(xa,50)));title('α=0.05幅度谱'); axis([0,51,0,20]);

a=0.01;
xa=A*exp(-a*n*T).*sin(w0*n*T);
subplot(2,5,5);stem(xa);title('α=0.01时域序列');axis([0,51,-1,1]);
subplot(2,5,10);stem(abs(fft(xa,50)));title('α=0.01幅度谱'); axis([0,51,0,20]);