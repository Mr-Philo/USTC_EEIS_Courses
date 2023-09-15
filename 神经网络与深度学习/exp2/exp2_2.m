% exp2_2.m
% 目标：设计实现模糊控制规则为T=int((e+ec)/2)的模糊神经网络控制器

% 第二阶段：标准梯度下降法，在S1=12固定的情况下，确定lr值实验。

% 输入矢量P（维数R×Q）依题意知为2行，即R=2，分别为e矢量和ec矢量；至于Q值，题目并未明确叙述，不妨取Q=10
R = 2; Q = 10;
% e = round(rand(1,Q)*4-2);       % 生成[-2,2]间的Q个随机整数
% ec = round(rand(1,Q)*4-2);
e = [1,2,-1,2,1,-2,-1,0,2,2];
ec = [-1,2,2,0,1,-1,0,2,1,2];   % 此时为了控制变量，固定输入矢量以比对性能
P = [e;ec];                     % 合成输入矢量P
S2 = 1;
T = fix((e+ec)/2);              % 计算输出矢量T（维数为S2×Q，这里S2=1），并依据题目算法利用fix函数对该矩阵去小数取整

% 网络结构：考虑单层隐含层神经网络，即2-S1-1型
% 隐含层采用S型正切激活函数，输出层采用线性激活函数
% 隐含层节点数S1为12
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

Y = sim(net,P);         %查看训练时间
t=tr.time(end)          %计算均方差
SSE = perform(net,T,Y)

% 记录10.24
