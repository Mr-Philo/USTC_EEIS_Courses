clear;
T = [1 -1;-1 1];                                        %初始化目标平衡点
net = newhop(T);                                       	%建立霍普菲尔德网络
W = net.lw{1,1};
b = net.b{1};                                           %W,b分别为网络的权值和误差
N = 50;                                                 %最大迭代次数

% 随机生成输入矢量P进行实验
% P = unifrnd (-1, 1, 2, 6);
% P = [0.6294 -0.7460 0.2647 -0.4430 0.9150 -0.5000;
%      0.8116 0.8268 -0.8049 0.0938 0.9298 0.5000];       %调整最后一组在目标平衡点连线的垂直平分线上
P = [-0.6294 0.2316 0.1460 0.9150 -0.8268 0.5000;
     -0.8116 0.8480 -0.8268 0.9298 0.2460 -0.5000];       %调整最后一组在目标平衡点连线的垂直平分线上
[Y,Pf,Af] = sim(net,{6,N},[],P);                       %计算其网络循环结束后的输出
% sim函数中{x,y}参数的意义：x为输入矢量列数，y为迭代次数； A25 = Y{25}代表循环25次的结果
A1=Y{1},A2=Y{2},A3=Y{3}                                   %显示3次循环的每一次的输出结果

% 绘图展示       P(1,:)表示提取矩阵P的第一行，P(:,1)表示提取矩阵P的第一列
figure;
hold on
title('Hopfield Network Training State'); ylabel('Y'); xlabel('X');
Sum1=[];Sum2=[];Sum3=[];Sum4=[];Sum5=[];Sum6=[];
for i=0:N
    if i == 0
        A = P;          % 初始输入矢量也画出
    else
        A = Y{i};       % 计算第i次迭代输出
    end
    Sum1 = cat(2,Sum1,A(:,1));      % 把每次迭代输出的第一列都按列合并入Sum1中，使Sum1能够完整地表示第一组矢量的变化情况，下同
    Sum2 = cat(2,Sum2,A(:,2));
    Sum3 = cat(2,Sum3,A(:,3));
    Sum4 = cat(2,Sum4,A(:,4));
    Sum5 = cat(2,Sum5,A(:,5));
    Sum6 = cat(2,Sum6,A(:,6));
end
plot(Sum1(1,:),Sum1(2,:),'r-o','LineWidth',1,'MarkerEdgeColor','k','MarkerSize',6,'MarkerFaceColor','red');            %第一组矢量用红色
plot(Sum2(1,:),Sum2(2,:),'y-o','LineWidth',1,'MarkerEdgeColor','k','MarkerSize',6,'MarkerFaceColor','yellow');         %第二组矢量用黄色
plot(Sum3(1,:),Sum3(2,:),'b-o','LineWidth',1,'MarkerEdgeColor','k','MarkerSize',6,'MarkerFaceColor','blue');           %第三组矢量用蓝色
plot(Sum4(1,:),Sum4(2,:),'g-o','LineWidth',1,'MarkerEdgeColor','k','MarkerSize',6,'MarkerFaceColor','green');          %第四组矢量用绿色
plot(Sum5(1,:),Sum5(2,:),'m-o','LineWidth',1,'MarkerEdgeColor','k','MarkerSize',6,'MarkerFaceColor','magenta');        %第五组矢量用品红色
plot(Sum6(1,:),Sum6(2,:),'c-o','LineWidth',1,'MarkerEdgeColor','k','MarkerSize',6,'MarkerFaceColor','cyan');           %第六组矢量用青蓝色
legend('P1','P2','P3','P4','P5','P6','Location','Best'); 