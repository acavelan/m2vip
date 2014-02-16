clear all;

f1 = './rice.jpg';
f2 = './chromosomes.jpg';

# calcule de l'histogramme

pkg load image;

A = imread(f1);
B = imread(f2);

A = im2double(A);
B = im2double(B);

A = 255 * (A - min(min(A)));
A = A ./ (max(max(A)) - min(min(A)));

B = 255 * (B - min(min(B)));
B = B ./ (max(max(B)) - min(min(B)));

HA = imhist(A);
HB = imhist(B);

SA = 0.5; #graythresh(A);
SB = 0.5; #graythresh(B);

BA = im2bw(A, SA);
BB = im2bw(B, SB);

#BA = ~BA;
#BB = ~BB;

close all;
figure;
subplot(2,2,1);imshow(A);
subplot(2,2,2);imshow(B);
subplot(2,2,3);imshow(BA);
subplot(2,2,4);imshow(BB);
