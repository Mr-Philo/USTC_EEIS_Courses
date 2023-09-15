% exp1_bp.m
% 采用BP网络

% 初始化
[alphabet,targets]=prodat;
P = alphabet;
T = targets;
[R,Q] = size(P);
S1=10;
[S2,Q] = size(T);
net = newff(minmax(P),[S1,S2],{'logsig','logsig'},'traingdx');     % 注意，这里采用的是感知器网络
net.LW{2,1}=net.LW{2,1}*0.01;
net.b{2}=net.b{2}*0.01;
net.performFcn='sse';               %执行函数为误差平方和函数
net.trainParam.epochs = 800;        %最大训练步长
net.trainParam.lr = 0.01;           %学习率
net.trainParam.lr_inc = 1.05;       %增长的学习率
net.trainParam.lr_dec = 0.7;
net.trainParam.goal = 0.1;          %执行函数目标值
net.trainParam.mc = 0.9;            %附加动量因子
net.trainParam.min_grad = 1e-10;    %最小执行梯度
net.trainParam.show = 50;
[net,tr] = train(net,P,T);       %训练网络
W = net.iw{1,1};     %训练后的网络权值
B = net.b{1};        %训练后的网络偏差

% 具有噪声的输入矢量识别网络——训练一个网络，其具有两组理想输入矢量alphabet和两组带有噪声的输入矢量
% 目标矢量为4组期望矢量targets，噪声矢量为均值0.1-0.2的随机噪声
netn = net;
netn.trainParam.goal = 0.1;         %目标误差
netn.trainParam.epochs = 300;       %最大训练步长
for pass=1:10
    T=[targets targets targets targets];
    fprintf('Pass=%.0f\n',pass);
    P=[alphabet,alphabet,(alphabet+randn(R,Q)*0.1),(alphabet+randn(R,Q)*0.2)];
    [netn,tr]=train(netn,P,T);
end
% 有必要再用理想无噪声输入矢量对网络再训练一次，以保证网络可以准确识别理想字母
P = alphabet;
T = targets;
[net,tr] = train(net,P,T);       %训练网络

% 系统性能测试——向神经网络输入任意字母，加入均值0.1-0.2的噪声，由此随机产生100个输入矢量，通过网络识别后输出
% 为了增强BP网络的抗干扰能力，网络在使用时，其后应再加上一个竞争网络，以使网络对每一个字母的输出只有一个位置为1，其他元素均为0
noise_range=0:0.02:0.2;
max_test=100;
network1=[];
network2=[];
T=targets;
for noiselevel=noise_range
    fprintf('Testing networks with noise level of %.2f.\n',noiselevel);
    errors1=0;
    errors2=0;
    for i=1:max_test
        P=alphabet+randn(R,Q)*noiselevel;
        A=sim(net,P);       %测试用理想字母训练的网络
        AA=compet(A);
        errors1=errors1+sum(sum(abs(AA-T)))/2;
        An=sim(netn,P);     %测试噪声字母训练的网络
        AAn=compet(An);
        errors2=errors2+sum(sum(abs(AAn-T)))/2;
    end

network1=[network1 errors1/26/100];
network2=[network2 errors2/26/100];
end
figure(1);      % 显示上述两种网络辨识结果的出错率
plot(noise_range,network1*100,'--',noise_range,network2*100);
xlabel('Noise Level');
ylabel('Percentage of Recognition');