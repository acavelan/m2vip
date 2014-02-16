# Compacité = (4 x pi x aire) / périmètre

clear all;

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

EA = A;
EB = B;

EA = 255 * (EA - min(min(EA)));
EA = EA ./ (max(max(EA)) - min(min(EA)));

EB = 255 * (EB - min(min(EB)));
EB = EB ./ (max(max(EB)) - min(min(EB)));

SA = 0.45;#graythresh(EA);
SB = 0.5;#graythresh(EB);

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

[labelA, c1] = bwlabel(BA, 4);
[labelB, c2] = bwlabel(BB, 8);

areaA = regionprops(labelA, 'area');
areaB = regionprops(labelB, 'area');

#areaA.Area
#areaB.Area

perimA = regionprops(labelA, 'perimeter');
perimB = regionprops(labelB, 'perimeter');

#preimA.Perimeter
#perimB.Perimeter

for i=1:10
	a = areaA(1, i).Area;
	p = perimA(1, i).Perimeter;
	comp = (4 * pi * a) / p;
	printf("Grain %d, aire = %d, périmètre = %d, compacité = %d\n", i, a, p, comp);
end;

for i=1:10
	a = areaB(1, i).Area;
	p = perimB(1, i).Perimeter;
	comp = (4 * pi * a) / p;
	printf("Chromosome %d, aire = %d, périmètre = %d, compacité = %d\n", i, a, p, comp);
end;

skelA = bwmorph(BA, 'skel', Inf);
skelB = bwmorph(BB, 'skel', Inf);

shrinkA = bwmorph(BA, 'shrink', Inf);
shrinkB = bwmorph(BB, 'shrink', Inf);

thinA = bwmorph(BA, 'thin', Inf);
thinB = bwmorph(BB, 'thin', Inf);

close all;
#figure;
#subplot(2,2,1);imshow(labelA);
#subplot(2,2,2);imshow(labelB);

figure;
subplot(1,2,1);imshow(skelA);
subplot(1,2,2);imshow(skelB);
        
figure;
subplot(1,2,1);imshow(shrinkA);
subplot(1,2,2);imshow(shrinkB);

figure;
subplot(1,2,1);imshow(thinA);
subplot(1,2,2);imshow(thinB);

printf("Rice = %d\n", c1);
printf("Chromosomes = %d\n", c2);

printf("Rice = %d\n", c21);
printf("Chromosomes = %d\n", c22);
