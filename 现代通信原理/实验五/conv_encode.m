function [Output] = conv_encode(Input, G_x)
% Conv_encode 卷积码编码器函数
%   输入Input：待编码序列  G_x：生成矩阵  输出Output：编码后序列
    len = length(Input);
    k = 1;                              % 表示每次对k个码元进行编码
    [n,N] = size(G_x);                  % G_x为矩阵，行数n表示一个输入码元拥有几个输出，列数N表示每次监督的输入码元数
    Output = zeros(1, n*(len+N-1));     % 输出序列长度为（输入序列长度+约束长度-1）*（一个输入码元对应输出长度）
    Input_add0 = [zeros(1,N-1), Input, zeros(1,N+1)];     % 在输入序列头尾补0，方便卷积输出和寄存器清洗

    % 循环每一位输入符号，获得输出矩阵
    register = fliplr(Input_add0(1,1:N));    % 水平翻转，倒序输入
    for i = 1:len+N-1
        % 生成每一位输入符号的n位输出
        Output(n*i-(n-1):n*i) = mod(register*G_x.',2);    % 模2加

        % 更新寄存器序列+待输出符号（共N个符号）
        register = [Input_add0(i+N), register];    % 添加新符号
        register(end) = [];                 % 挤掉旧符号
    end
end

