function X=mydft(x)
M=nextpow2(length(x));
N=2^M;
if length(x)<=N
    x=[x,zeros(1,N-length(x))];
end%补全至2的幂次方

n=0:N-1;
k=0:N-1;
X=x*(exp(-2i*pi/N)).^(n'*k);