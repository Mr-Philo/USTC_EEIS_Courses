% exp2_4.m
% 目标：设计实现模糊控制规则为T=int((e+ec)/2)的模糊神经网络控制器

% 第四阶段：采取各种改进方法。
% 相比于前面的实验，只需在新建网络函数处改变参数

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

for i=1:5
    switch i
        case 1
            net = newff(minmax(P),[S1,S2],{'logsig','purelin'},'traingd');      % 标准梯度下降法
        case 2
            net = newff(minmax(P),[S1,S2],{'logsig','purelin'},'traingda');     % 自适应学习速率
        case 3
            net = newff(minmax(P),[S1,S2],{'logsig','purelin'},'traingdm');     % 附加动量法
        case 4
            net = newff(minmax(P),[S1,S2],{'logsig','purelin'},'trainbfg');     % BFGS拟牛顿法
        case 5
            net = newff(minmax(P),[S1,S2],{'logsig','purelin'},'trainlm');      % Levenberg-Marquardt法
    end

net.IW{1,1}= W1; %IW 是输入层到第一层的权值矩阵  LW 是中间层和输出层的权值矩阵
net.LW{2,1}= W2;
net.b{1}= B1 ; %b{1} 表示隐含层的阀值矩阵
net.b{2}= B2;
net.performFcn='sse';               %执行函数为误差平方和函数
net.trainParam.epochs = 10000;        %最大训练步长
net.trainParam.goal = 0.001;          %执行函数目标值
net.trainParam.lr = 0.03;       %初始学习速率
[net,tr] = train(net,P,T);       %训练网络
% W = net.iw{1,1};     %训练后的网络权值
% B = net.b{1};        %训练后的网络偏差

Y = sim(net,P);
t(i)=tr.time(end);
n(i)=tr.num_epochs();
SSE(i) = perform(net,T,Y);
end

xl=['1','2','22','33','12'];
subplot(1,3,1);bar(t);title('训练时间');
subplot(1,3,2);bar(n);title('训练次数');
subplot(1,3,3);bar(SSE);title('均方误差');
% 记录10.24
