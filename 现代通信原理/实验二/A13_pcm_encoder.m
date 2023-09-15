function [out] = A13_pcm_encoder(x)
% 对输入参数x进行A律13折线pcm编码，方法为逐次比较法
% 其中输入样值x的范围为[-2048, 2048]
    n = length(x);
    for i = 1:n
        % 编写极性码
        if x(i)>0
            out(i,1)=1;
        else
            out(i,1)=0;
        end
        
        % 编写段落码并计算量化间隔和量化起始电平
        if (0<=abs(x(i)) && abs(x(i))<16)
            % 段落码
            out(i,2)=0;out(i,3)=0;out(i,4)=0;
            % 量化间隔
            step = 1; %起始电平
            st = 0;
        elseif (16<=abs(x(i)) && abs(x(i))<32)
            out(i,2)=0; out(i,3)=0; out(i,4)=1; step=1; st=16;
        elseif (32<=abs(x(i)) && abs(x(i))<64)
            out(i,2)=0; out(i,3)=1; out(i,4)=0; step=2; st=32;
        elseif (64<=abs(x(i)) && abs(x(i))<128)
            out(i,2)=0; out(i,3)=1; out(i,4)=1; step=4; st=64;
        elseif (128<=abs(x(i)) && abs(x(i))<256)
            out(i,2)=1; out(i,3)=0; out(i,4)=0; step=8; st=128;
        elseif (256<=abs(x(i)) && abs(x(i))<512)
            out(i,2)=1; out(i,3)=0; out(i,4)=1; step=16; st=256;
        elseif (512<=abs(x(i)) && abs(x(i))<1024)
            out(i,2)=1; out(i,3)=1; out(i,4)=0; step=32; st=512;
        else
            out(i,2)=1; out(i,3)=1; out(i,4)=1; step=64; st=1024; %#ok<*AGROW>
        end
        
        if (abs(x(i))>=2048) % 处理过载现象
            out(i,2:8)=[1 1 1 1 1 1 1];
        else
            % 计算段内码
            tmp = floor((abs(x(i))-st)/step);
            % 十进制转二进制数，
            % 如果不减48，最后4位是ASCII数49 48 48 48 
            t = dec2bin(tmp,4)-48;
            out(i,5:8)=t(1:4);
       end
    end
    
    out = reshape(out',1,8*n);   % 输出：八位二进制码元长序列（一共n个码元，每个码元长8位）
end

