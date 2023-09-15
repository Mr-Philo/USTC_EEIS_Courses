function X=myfft(x)
% Myfft fuction by 王瑞哲 PB19071509
%   基本思路：基2-FFT算法，采用matlab矩阵并行运算以减少for循环所带来的运算量
M=nextpow2(length(x));      % 找比length(x)长的最小的2的幂，M即为这个幂的指数，也即为要做的2点FFT次数
N=2^M;
if length(x)<=N
    x=[x,zeros(1,N-length(x))];
end     %补全至2的幂次方

% 查阅资料得知，matlab中对循环运算耗费的时间很多，因此需要尽量避免循环运算
% 采用输入输出均为自然时序的基2-FFT算法
for i=1:M    % 循环做2点FFT，一共做M=log2(N)次
    % 求n=当前列数
    if(i==1)
        n=N/2;
    else
        n=n/2;
    end
    % 利用matlab语法x(m:n,j:k)将x的第m到n行，j到k列挑出来
    x1=x(:,1:n);                % 前一半序列
    x2=x(:,n+1:n*2);            % 后一半序列
    W=exp(-1i*2*pi/(2^i));      % 计算当前旋转因子——对应点数为2^k
    col=(0:2^(i-1)-1)';         % 当前行数×1的一个纵向递增序列，作为W的指数
    W=W.^col;
    for j=1:n
        x2(:,j)=x2(:,j).*W;     % 后一半序列每列按从0到n的指数乘旋转因子
    end
    x=[x1+x2;x1-x2];     % 将x的前一半和后一半纵向叠合并行计算
    % 循环结束后，原x列数减半M次最终至1，行数加倍M次最终至N=2^M
end
X=x';     % 按照上面一层一层并行的算法，结果应为N×1矩阵，转置即得最终的X(k)序列
end

