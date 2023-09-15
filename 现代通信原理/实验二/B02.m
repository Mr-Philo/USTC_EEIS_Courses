% A律13折线PCM编码测试
clear;
x1 = -2*pi:0.4:2*pi;    x1_s = -2*pi:0.04:2*pi;   % 设置x1_s单纯为了画图好看
y1 = 2048*sin(x1);  y1_s = 2048*sin(x1_s);
w1 = A13_pcm_encoder(y1);
table1 = reshape(w1', 8, length(w1)/8)';    % 更方便地观看输出码字列表
y1_de = A13_pcm_decoder(w1,2048);        % 解码输出
subplot(121); plot(x1_s,y1_s,'b'); hold on
stairs(x1,y1_de,'r','linewidth',2);     % 绘制阶梯图，更方便展示离散编码输出特性
title('正弦信号PCM编解码'); xlabel('x'); ylabel('y'); legend('原始信号','PCM编解码信号','location','NorthEast');

x2 = -2.7:0.2:2.7;  x2_s = -2.7:0.02:2.7;
y2 = 2048*tanh(x2);  y2_s = 2048*tanh(x2_s);
w2 = A13_pcm_encoder(y2);
table2 = reshape(w2', 8, length(w2)/8)';    % 更方便地观看输出码字列表
y2_de = A13_pcm_decoder(w2,2048);        % 解码输出
subplot(122); plot(x2_s,y2_s,'b'); hold on
stairs(x2,y2_de,'r','linewidth',2);
title('双曲正切信号PCM编解码'); xlabel('x'); ylabel('y'); legend('原始信号','PCM编解码信号','location','SouthEast');