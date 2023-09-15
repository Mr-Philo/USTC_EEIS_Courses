function [f,sf] = T2F(t,st)
% 利用fft, fftshift定义函数T2F计算信号的傅立叶变换
%   该子函数需要两个参数t和st: t—离散时间；st—离散信号
    dt = t(2)-t(1) ;        % 时间分辨率
    T = t(end) ;
    df = 1/T ;              % 频率分辨率
    N = length(st) ;        % 离散傅立叶变换长度
    f = -N/2*df :df :N/2*df-df ;        % 设定频谱区间，注意要关于原点对称，共有N个点，包括0点，故要减去一个df
    sf = fft(st);
    sf = T/N*fftshift(sf);  % 信号的频谱与离散傅立叶变换之间的关系，fftshift(x)是将信号%的频谱 x 进行移位，与原点对称。
end

