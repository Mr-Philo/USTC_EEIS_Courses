function [y,noise] = Gnoisegen(x,snr)
% Gnoisegen函数是叠加高斯白噪声到信号x中
% [y,noise] = Gnoisegen(x,snr)
% x是原信号，snr是设置的信噪比，单位为dB
% y是叠加高斯白噪声后的带噪语音，noise是被叠加的噪声
    noise = randn(size(x));                             % 用randn函数产生高斯白噪声
    Nx = length(x);                                     % 求出信号x长
    signal_power = 1/Nx*sum(x.*x);                      % 求出信号的平均能量
    noise_power = 1/Nx*sum(noise.*noise);               % 求出噪声的能量
    noise_variance = signal_power / ( 10^(snr/10) );    % 计算出噪声设定的方差值
    noise = sqrt(noise_variance/noise_power)*noise;     % 按噪声的平均能量构成相应的白噪声
    y = x+noise;                                        % 构成带噪语音
end



