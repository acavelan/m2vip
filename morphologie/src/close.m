clear all;

f1 = './rice.jpg';
f2 = './chromosomes.jpg';

# calcule de l'histogramme

pkg load image;

A = imread(f1);
B = imread(f2);

A = im2double(A);
B = im2double(B);

EA = A;
EB = B;

EA = 255 * (EA - min(min(EA)));
EA = EA ./ (max(max(EA)) - min(min(EA)));

EB = 255 * (EB - min(min(EB)));
EB = EB ./ (max(max(EB)) - min(min(EB)));

SA = 0.45; #graythresh(EA);
SB = 0.5; #graythresh(EB);

BA = im2bw(A, SA);
BB = im2bw(B, SB);

# logicalisation
ES = [1 1 1; 1 0 1; 1 1 1];
ES = logical(ES);

BA = imerode(BA, ES);
BA = imerode(BA, ES);
BA = imdilate(BA, ES);
BA = imdilate(BA, ES);

BB = imerode(BB, ES);
BB = imerode(BB, ES);
BB = imdilate(BB, ES);
BB = imdilate(BB, ES);

BB = imdilate(BB, ES);
BB = imerode(BB, ES);

#BA = ~BA;
#BB = ~BB;

close all;
figure;
subplot(2,2,1);imshow(A);
subplot(2,2,2);imshow(B);
subplot(2,2,3);imshow(BA);
subplot(2,2,4);imshow(BB);
