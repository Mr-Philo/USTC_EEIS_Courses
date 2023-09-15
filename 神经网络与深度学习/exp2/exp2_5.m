% exp2_5.m
% 目标：设计实现模糊控制规则为T=int((e+ec)/2)的模糊神经网络控制器

% 第五阶段：验证——采用插值法选取多于训练时的输入，对所设计的网络进行验证，给出验证的A与T值

% 输入矢量P（维数R×Q）依题意知为2行，即R=2，分别为e矢量和ec矢量；至于Q值，题目并未明确叙述，不妨取Q=10
R = 2; Q = 10;
e = [1,2,-1,2,1,-2,-1,0,2,2];
ec = [-1,2,2,0,1,-1,0,2,1,2];   
P = [e;ec];                     
S2 = 1;
T = fix((e+ec)/2);              
S1 = 12;

W1 = rands(S1,R);
B1 = rands(S1,1);
W2 = rands(S2,S1);
B2 = rands(S2,1);
% 初始化随机权值，限制在（-1，1）范围内，防止因初始权值过大而发散
net = newff(minmax(P),[S1,S2],{'logsig','purelin'},'traingd');     
% 隐含层logsig，输出层线性，标准梯度下降
net.IW{1,1}= W1; %IW 是输入层到第一层的权值矩阵  LW 是中间层和输出层的权值矩阵
net.LW{2,1}= W2;
net.b{1}= B1 ; %b{1} 表示隐含层的阀值矩阵
net.b{2}= B2;
net.performFcn='sse';               %执行函数为误差平方和函数
net.trainParam.epochs = 10000;        %最大训练步长
net.trainParam.goal = 0.001;          %执行函数目标值
net.trainParam.lr = 0.03;       %调整学习速率
[net,tr] = train(net,P,T);       %训练网络
W = net.iw{1,1};     %训练后的网络权值
B = net.b{1};        %训练后的网络偏差

Y = sim(net,P);         
t=tr.time(end)          %查看训练时间
SSE = perform(net,T,Y)  %计算均方差

%样本插值进行测试
e1 = [1,1.5,2,0.5,-1,0.5,2,1.5,1,-0.5,-2,-1.5,-1,-0.5,0,1,2,1,2];
ec1 = [-1,0.5,2,2,2,1,0,0.5,1,0,-1,-0.5,0,1,2,1.5,1,1.5,2];   
P1 = [e1;ec1];                     
T1 = fix((e1+ec1)/2); 

A = sim(net,P1);
SSE1 = perform(net,T1,A)  %计算均方差

% 记录10.24
