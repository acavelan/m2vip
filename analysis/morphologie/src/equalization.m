f1 = './rice.jpg';
f2 = './chromosomes.jpg';

# calcule de l'histogramme

pkg load image;

A = imread(f1);
B = imread(f2);

A = im2double(A);
B = im2double(B);

#A = histeq(A, 256);
#B = histeq(B, 256);

A = 255 * (A - min(min(A)));
A = A ./ (max(max(A)) - min(min(A)));

B = 255 * (B - min(min(B)));
B = B ./ (max(max(B)) - min(min(B)));


HA = imhist(A);
HB = imhist(B);

close all;
figure;
subplot(2,2,1);imshow(A);
subplot(2,2,2);imshow(B);
subplot(2,2,3);plot(HA);
subplot(2,2,4);plot(HB);
