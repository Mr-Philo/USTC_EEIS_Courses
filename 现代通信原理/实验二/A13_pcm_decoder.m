function [out] = A13_pcm_decoder(in,v)
% 对输入进行A律13折线pcm解码
% 输入in为8位二进制码，(-v,+v)为量化区间
    n = length(in);
    in = reshape(in', 8, n/8)';     % 重整原输入码序列——按每行8个二进制数字重整，一共n/8个码字，每行即对应一个8位长码字
    slot = [0, 16, 32, 64, 128, 256, 512, 1024];		% 段落码解码手动打表
    step = [1, 1, 2, 4, 8, 16, 32, 64];					% 段内码解码手动打表
    % 解码
    for i = 1:n/8
        ss = 2*in(i,1)-1;   % 解极性码
        tmp = in(i,2)*4+in(i,3)*2+in(i,4)+1;    % 解段落码
        st = slot(tmp);
        dt = (in(i,5)*8+in(i,6)*4+in(i,7)*2+in(i,8))*step(tmp)+0.5*step(tmp);   % 解段内码
        out(i) = ss*(st+dt)/2048*v;     % 重整为最终解码十进制数
    end
end

