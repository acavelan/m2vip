f1 = './rice.jpg';
f2 = './chromosomes.jpg';

# calcule de l'histogramme

pkg load image;

A = imread(f1);
B = imread(f2);

A = im2double(A);
B = im2double(B);

HA = imhist(A);
HB = imhist(B);

figure;
subplot(2,2,1);imshow(A);
subplot(2,2,2);imshow(B);
subplot(2,2,3);plot(HA);
subplot(2,2,4);plot(HB);


