N = 12;
n = 0:(2*N-1);
M = 4; x1 = sin(2*pi*M*n/N);
M = 5; x2 = sin(2*pi*M*n/N);
M = 7; x3 = sin(2*pi*M*n/N);
M = 10; x4 = sin(2*pi*M*n/N);

figure(1);
subplot(2,4,1); 
plot(n,x1,'r'); title('"plot":M=4'); xlabel('n'); ylabel('x');
subplot(2,4,2); 
plot(n,x2,'g'); title('"plot":M=5'); xlabel('n'); ylabel('x');
subplot(2,4,3); 
plot(n,x3,'b'); title('"plot":M=7'); xlabel('n'); ylabel('x');
subplot(2,4,4); 
plot(n,x4,'m'); title('"plot":M=10'); xlabel('n'); ylabel('x');

subplot(2,4,5); 
stem(n,x1,'r'); title('"stem":M=4'); xlabel('n'); ylabel('x');
subplot(2,4,6); 
stem(n,x2,'g'); title('"stem":M=5'); xlabel('n'); ylabel('x');
subplot(2,4,7); 
stem(n,x3,'b'); title('"stem":M=7'); xlabel('n'); ylabel('x');
subplot(2,4,8); 
stem(n,x4,'m'); title('"stem":M=10'); xlabel('n'); ylabel('x');