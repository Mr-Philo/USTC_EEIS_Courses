% exp1_3_2.m
% 离散信号、系统和系统响应的分析
% 观察信号xc (n)和系统ha (n)的时域和幅频特性；利用线性卷积求信号通过系统以后的响应。
% 判断响应序列图形及序列非零值长度是否与理论结果一致，说出一种定性判断响应序列图形正确与否的方法.提示：xc (n) = ha (n) = R10 (n)
% 利用序列的傅立叶变换数值计算子程序求出Y(e^jwk)，观察响应序列的幅频特性。定性判断结果正确与否
% 改变信号xc (n)的矩形宽度，使 N=5，重复以上动作，观察变化，记录改变参数前后的差异。

% xc(n):矩形序列 1<=N<=10
n=1:50;
N=5;
xc=sign(sign(N-n)+1);
subplot(2,3,1);stem(xc);title('矩形窗序列xc(n)');
subplot(2,3,4);stem(abs(fft(xc,50)));title('矩形窗序列幅度谱'); 
% 特定冲激串序列  ha(n) = R10 (n)
n=1:50;
N=10;
ha=sign(sign(N-n)+1);
subplot(2,3,2);stem(ha);title('特定冲击串序列ha(n)');
subplot(2,3,5);stem(abs(fft(ha,50)));title('特定冲击串序列幅度谱');

% 利用线性卷积求信号通过系统以后的响应
y = conv(xc,ha);
subplot(2,3,3);stem(y);title('系统响应y(n)时域特性');
subplot(2,3,6);stem(abs(fft(y,100)));title('系统响应频域特性');