% exp1-alter

% 初始化
% 修改：利用prodat2数据初始化，即改为数字识别
[num,targets]=prodat2;
P = num;
T = targets;
[R,Q] = size(P);
[S,Q] = size(T);
net = newp(minmax(P),S);     % 注意，这里采用的是感知器网络
net.trainParam.goal = 0.0001;     %定义目标误差
net.trainParam.epochs = 40;     %定义最大循环次数
[net,tr] = train(net,P,T);       %训练网络
W = net.iw{1,1};     %训练后的网络权值
B = net.b{1};        %训练后的网络偏差

% 具有噪声的输入矢量识别网络——训练一个网络，其具有两组理想输入矢量num和两组带有噪声的输入矢量
% 目标矢量为4组期望矢量targets，噪声矢量为均值0.1-0.2的随机噪声
% 修改：训练噪声矢量范围扩大至0.2-0.5
netn = net;
netn.trainParam.goal = 0.0001;     %定义目标误差
netn.trainParam.epochs = 300;     %定义最大循环次数
for pass=1:10
    T=[targets targets targets targets];
    fprintf('Pass=%.0f\n',pass);
    P=[num,num,(num+randn(R,Q)*0.2),(num+randn(R,Q)*0.5)];
    [netn,tr]=train(netn,P,T);
end
% 有必要再用理想无噪声输入矢量对网络再训练一次，以保证网络可以准确识别理想字母
P = num;
T = targets;
[net,tr] = train(net,P,T);       %训练网络

%系统性能测试——向神经网络输入任意字母，加入均值0.2-0.5的噪声，由此随机产生100个输入矢量，通过网络识别后输出
noise_range=0:0.05:0.5;
max_test=100;
network1=[];
network2=[];
T=targets;
for noiselevel=noise_range
    fprintf('Testing networks with noise level of %.2f.\n',noiselevel);
    errors1=0;
    errors2=0;
    for i=1:max_test
        P=num+randn(R,Q)*noiselevel;
        A=sim(net,P);       %测试用理想字母训练的网络
        errors1=errors1+sum(sum(abs(A-T)))/2;
        An=sim(netn,P);     %测试噪声字母训练的网络
        errors2=errors2+sum(sum(abs(An-T)))/2;
    end

network1=[network1 errors1/10/100];
network2=[network2 errors2/10/100];
end
figure(1);      % 显示上述两种网络辨识结果的出错率
plot(noise_range,network1*100,'--',noise_range,network2*100);
xlabel('Noise Level');
ylabel('Percentage of Recognition');