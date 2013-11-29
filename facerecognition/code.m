pkg load image;

clear all;
close all;

I = imread("photos/ph1.jpg");
I = rgb2gray(I);
I = double(I);
I = reshape(I, 1, 240*300);

X = I;

for i=2:14,
	I = imread(strcat("photos/ph", num2str(i), ".jpg"));
	I = double(I);
	I = rgb2gray(I);
	I = reshape(I, 1, 240*300);
	X = cat(1, X, I);
end;

G = mean(X)';

#Gr = reshape(G, 300, 240);

Xtilde = X - ones(14, 1) .* G';
size(Xtilde)

L = (Xtilde * Xtilde') / 14;
size(L)

[V,lambda] = sort(eig(L), 'descend');


