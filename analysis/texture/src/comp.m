clear all;
close all;

f1 = '../im9.tif';
f2 = '../im44.tif';

# calcule de l'histogramme

pkg load image;

A = imread(f1);
B = imread(f2);

DA = im2double(A);
DB = im2double(B);

# 1
HA = imhist(DA);
HB = imhist(DB);

figure;
subplot(2,2,1);imshow(A); title("OP");
subplot(2,2,2);imshow(B); title("CT");
subplot(2,2,3);plot(HA);
subplot(2,2,4);plot(HB);


analyse("== Original", A, B);
QA4 = uint8(A / 64) * 64;
QB4 = uint8(B / 64) * 64;

QA8 = uint8(A / 32) * 32;
QB8 = uint8(B / 32) * 32;

QA16 = uint8(A / 16) * 16;
QB16 = uint8(B / 16) * 16;

HQA4 = imhist(QA4);
HQB4 = imhist(QB4);

HQA8 = imhist(QA8);
HQB8 = imhist(QB8);

HQA16 = imhist(QA16);
HQB16 = imhist(QB16);

analyse("== 4 levels", QA4, QB4);
analyse("== 8 levels", QA8, QB8);
analyse("== 16 levels", QA16, QB16);

figure;
subplot(3,2,1);imshow(QA4, []); title("OP");
subplot(3,2,2);imshow(QB4, []); title("CT");
subplot(3,2,3);imshow(QA8, []); title("OP");
subplot(3,2,4);imshow(QB8, []); title("CT");
subplot(3,2,5);imshow(QA16, []); title("OP");
subplot(3,2,6);imshow(QB16, []); title("CT");

figure;
subplot(3,2,1);plot(HQA4); title("OP");
subplot(3,2,2);plot(HQB4); title("CT");
subplot(3,2,3);plot(HQA8); title("OP");
subplot(3,2,4);plot(HQB8); title("CT");
subplot(3,2,5);plot(HQA16); title("OP");
subplot(3,2,6);plot(HQB16); title("CT");

hf = [-1, -1, -1; -1, 8, -1; -1, -1, -1];

FA = imfilter(A, hf);
FB = imfilter(B, hf);

QA4 = uint8(FA / 64) * 64;
QB4 = uint8(FB / 64) * 64;

QA8 = uint8(FA / 32) * 32;
QB8 = uint8(FB / 32) * 32;

QA16 = uint8(FA / 16) * 16;
QB16 = uint8(FB / 16) * 16;

HQA4 = imhist(QA4);
HQB4 = imhist(QB4);

HQA8 = imhist(QA8);
HQB8 = imhist(QB8);

HQA16 = imhist(QA16);
HQB16 = imhist(QB16);

analyse("== High-Pass 4 levels", QA4, QB4);
analyse("== High-Pass 8 levels", QA8, QB8);
analyse("== High-Pass 16 levels", QA16, QB16);

figure;
subplot(1,2,1);imshow(FA); title("OP");
subplot(1,2,2);imshow(FB); title("CT");

figure;
subplot(3,2,1);imshow(QA4, []); title("OP");
subplot(3,2,2);imshow(QB4, []); title("CT");
subplot(3,2,3);imshow(QA8, []); title("OP");
subplot(3,2,4);imshow(QB8, []); title("CT");
subplot(3,2,5);imshow(QA16, []); title("OP");
subplot(3,2,6);imshow(QB16, []); title("CT");
#
figure;
subplot(3,2,1);plot(HQA4); title("OP");
subplot(3,2,2);plot(HQB4); title("CT");
subplot(3,2,3);plot(HQA8); title("OP");
subplot(3,2,4);plot(HQB8); title("CT");
subplot(3,2,5);plot(HQA16); title("OP");
subplot(3,2,6);plot(HQB16); title("CT");

SP=[-1 -1; -1 0; -1 1; 0 -1; 0 1; 1 -1; 1 0; 1 1];

LBPA = lbp(A, SP, 0, 'i');
LBPB = lbp(B, SP, 0, 'i');

analyse("== LBP", LBPA, LBPB);

figure;
subplot(1,2,1);imshow(LBPA); title("OP");
subplot(1,2,2);imshow(LBPB); title("CT");


