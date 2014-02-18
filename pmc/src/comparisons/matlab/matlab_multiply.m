size = 2048;

for i=1:5

size=2^(i+7)

printf("%s\n, size");

m1 = rand(size, size);
m2 = rand(size, size);

tic
m3 = m1 * m2;
toc

end