A = zeros(50);
for i = 1:50
    for j = 1:50
        A(i,j) = i+j-1;
    end
end
B = fliplr(A);  % flip left right 左右/水平翻转
C = flipud(A);  % flip upside down 上下/垂直翻转
D = A;
D(1:10,:) = 0;
D(:,1:10) = 0;
% disp(A);