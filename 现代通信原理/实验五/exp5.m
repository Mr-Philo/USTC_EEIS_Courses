% 测试主函数
clear
n = 2; k = 1; N = 3;            % (2,1,3)卷积码
G_x = [1, 1, 1; 1, 0, 1];       % 生成矩阵

Num = 5000;                                     % 信源长度
B = randi(2,1,Num)-1;                           % 生成1×N维(0,1)序列
Encode = conv_encode(B, G_x);                   % 卷积编码

snrlist = 0:0.5:10 ;
error_rate0 = zeros(1, length(snrlist));
error_rate1 = zeros(1, length(snrlist));
for i = 1:length(snrlist)
    snr = snrlist(i);
    % 不经过信道编码，直接加噪并简单门限判决
    B_d = Gnoisegen(B, snr);
    B_d(B_d > 0.5) = 1;
    B_d(B_d < 0.5) = 0;
    error_rate0(i) = mae(B-B_d);
    % 经过信道编解码
    Encode_Noise = Gnoisegen(Encode, snr);          % 信道加噪
    Encode_Noise(Encode_Noise > 0.5) = 1;
    Encode_Noise(Encode_Noise < 0.5) = 0;
    Decode = conv_decode(G_x, k, Encode_Noise);     % 卷积解码
    error_rate1(i) = mae(B-Decode);                    % 计算误码率
end
figure(1)
semilogy(snrlist, error_rate0, 'b', 'linewidth',2); hold on
semilogy(snrlist, error_rate1, 'r', 'linewidth',2); 
grid on;
xlabel('信噪比 SNR / dB');ylabel('误码率 BER');
title('采用卷积码的信道编解码方案与不采用信道编码性能对比');
legend('无信道编码', '(2,1,3)卷积码');
    