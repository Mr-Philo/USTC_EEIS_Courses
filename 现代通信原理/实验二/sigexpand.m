function [out] = sigexpand(d,M)
% 将输入d进行扩张，方法是在d的每个数据中插入M-1个零值。
%   如输入d=[1,2,3,4]; M=3;
%   输出out=[1,0,0,2,0,0,3,0,0,4,0,0]
    N = length(d);              % 基带信号码元长度
    out = zeros(M,N);           % 矩阵M为采样点  N为基带信号码元数量
    out(1,:) = d;               % 将零矩阵第一列换成基带信号中的N个码元
    out = reshape(out,1,M*N);   % 重整为1行 m*n 列
end

