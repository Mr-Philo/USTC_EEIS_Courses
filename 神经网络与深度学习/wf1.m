% wf1.m
% 例2.4 设计自适应线性网络实现从输入矢量到输出矢量的变换关系
% 其中输入矢量为P=[1.0 -1.2]  输出矢量为T=[0.5 1.0]
P=[1 -1.2];
T=[0.5 1];
[R,Q]=size(P);
[S,Q]=size(T);
lr = 0.4*maxlinlr(P);                   %最佳学习速率
net=newlin(minmax(P),S,[0],lr);         %创建线性网络
net.inputWeights{1,1}.initFcn='rands';  %初始化权值
net.biases{1}.initFcn='rands';          %初始化偏差
net=init(net);                          %把初始化的权值和偏差函数赋给网络
W0=net.iw{1,1}                          %显示初始化权值和偏差
B0=net.b{1}
net.trainParam.epochs=20;               %最大循环次数
net.trainParam.goal=0.001;              %期望误差
[net,tr]=train(net,P,T);                %进行线性网络权值训练
W=net.iw{1,1}                           %显示训练后最终的权值和偏差
B=net.b{1}