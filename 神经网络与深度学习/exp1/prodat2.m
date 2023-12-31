function [num,targets] = prodat2
%prodat2 数字识别初始化
%   每个字母可以用7×5的小方格来表示，即用一列15个数字的组合来代表一种字母
num1 = [0 0 1 0 0 ...
        0 0 1 0 0 ...
        0 0 1 0 0 ...
        0 0 1 0 0 ...
        0 0 1 0 0 ...
        0 0 1 0 0 ...
        0 0 1 0 0]';     % 矩阵后的一撇'表示转置
num2 = [0 1 1 1 0 ...
        1 0 0 0 1 ...
        0 0 0 0 1 ...
        0 0 0 1 0 ...
        0 0 1 0 0 ...
        0 1 0 0 0 ...
        1 1 1 1 1]';
num3 = [0 1 1 1 0 ...
        1 0 0 0 1 ...
        0 0 0 0 1 ...
        0 0 0 1 0 ...
        0 0 0 0 1 ...
        1 0 0 0 1 ...
        0 1 1 1 0]';
num4 = [0 0 1 0 0 ...
        0 1 1 0 0 ...
        1 0 1 0 0 ...
        1 1 1 1 1 ...
        0 0 1 0 0 ...
        0 0 1 0 0 ...
        0 0 1 0 0]';
num5 = [1 1 1 1 1 ...
        1 0 0 0 0 ...
        1 0 0 0 0 ...
        1 1 1 1 0 ...
        0 0 0 0 1 ...
        0 0 0 0 1 ...
        1 1 1 1 0]';
num6 = [1 1 1 1 1 ...
        1 0 0 0 0 ...
        1 0 0 0 0 ...
        1 1 1 1 0 ...
        1 0 0 0 1 ...
        1 0 0 0 1 ...
        0 1 1 1 0]';
num7 = [1 1 1 1 1 ...
        0 0 0 0 1 ...
        0 0 0 1 0 ...
        0 0 0 1 0 ...
        0 0 1 0 0 ...
        0 0 1 0 0 ...
        0 0 1 0 0]';
num8 = [0 1 1 1 0 ...
        1 0 0 0 1 ...
        1 0 0 0 1 ...
        0 1 1 1 0 ...
        1 0 0 0 1 ...
        1 0 0 0 1 ...
        0 1 1 1 0]';
num9 = [0 1 1 1 0 ...
        1 0 0 0 1 ...
        1 0 0 0 1 ...
        0 1 1 1 1 ...
        0 0 0 0 1 ...
        0 0 0 0 1 ...
        1 1 1 1 1]';
num0 = [0 1 1 1 0 ...
        1 0 0 0 1 ...
        1 0 0 0 1 ...
        1 0 0 0 1 ...
        1 0 0 0 1 ...
        1 0 0 0 1 ...
        0 1 1 1 0]';

num = [num1,num2,num3,num4,num5,num6,num7,num8,num9,num0];
% num为35×10的矩阵，每一列代表一个数字
targets=eye(10);
end

