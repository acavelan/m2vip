pkg load image;

clear all;
close all;

N = 14;
Me = 12;
MaxDiff = 12000;
NewFaceFile = "other/lena.jpg";

# 1 - Construction de la matrice des données
#===============================================================================

M = zeros(N, 300*240);

for i=1:N,
    I = imread(strcat("photos/ph", num2str(i), ".jpg"));
    I = rgb2gray(I);
    I = double(I);
    I = reshape(I, 1, 240*300);
    M(i,:) = I;
end;

# Calcule du centre de gravité (moyenne des images)
G = mean(M)';

# On recentre les image par rapport à G
Xtilde = M - ones(N, 1) * G';


# 2 - Recherche des eigenface
#===============================================================================

# Calcule des vecteurs propres de L
L = (Xtilde * Xtilde') / N;

[V D] = eig(full(L));

# Sort eigenvectors in decsending order of eigenvalues
[D order] = sort(diag(D),'descend');
V = V(:,order);

# Calcule des vecteurs propres de M
U = Xtilde' * V;

# U: Vecteurs propres de M en fonction de L.
# U: N eigenface
# Normalisation de U
U = U / norm(U);

figure;
colormap(gray);
for i=1:6
    subplot(2,3,i);
    imagesc(reshape(M(i,:), 300, 240));
end;

figure;
colormap(gray);
subplot(1, 1, 1);
imagesc(reshape(G, 300, 240));
title(strcat('average face'));

figure;
colormap(gray);
for i=1:6
    subplot(2,3,i);
    imagesc(reshape(U(:,i), 300, 240));
    title(strcat('eigenface ', num2str(i)));
end;


# 3 - Recion des visages de la base de données à partir des eigenface
#===============================================================================

C = Xtilde * U;

Rec = zeros(N, 300*240);

for i=1:14
    for j=1:14
	Rec(i,:) += C(i,j) * U(:,j)';
    end;
    Rec(i,:) += G';
end;

ErrorBase = zeros(N, 1);
for j=1:14
    ErrorBase(j) = sum((M(j,:) - Rec(j,:)).^2);
    ErrorBase(j) = sqrt(ErrorBase(j));
end
ErrorBaseMean = mean(ErrorBase);

figure;
colormap(gray);
for i=1:6
    subplot(2,3,i);
    imagesc(reshape(Rec(i,:), 300, 240));
    title(strcat('reconstructed ', num2str(i)));
end;

printf("Confronting myself to the database:\n");
printf("\tMean error in database is %d\n", ErrorBaseMean);
printf("\tError for myself (%d) is %d\n", Me, ErrorBase(Me));

figure;
colormap(gray);
subplot(1,2,1); imagesc(reshape(M(Me,:), 300, 240));
title(strcat('original (', num2str(Me), ')'));
subplot(1,2,2); imagesc(reshape(Rec(Me,:), 300, 240));
title(strcat('reconstructed (', num2str(Me), ')'));


# 4 - Traitement d'un nouveau visage
#===============================================================================

NewFace = imread(NewFaceFile);
NewFace = rgb2gray(NewFace);
NewFace = double(NewFace);
NewFace = reshape(NewFace, 1, 300*240);
NewFaceTilde = NewFace - G';
NewFaceC = NewFaceTilde * U;

NewFaceRec = zeros(1, 300*240);
for j=1:14
    NewFaceRec += NewFaceC(j) * U(:,j)';
end;
NewFaceRec += G';

ErrorNewFace = sum((NewFace - NewFaceRec).^2);
ErrorNewFace = sqrt(ErrorNewFace);

printf("Confronting %s to the database:\n", NewFaceFile);
printf("\tError for the new reconstructed face is %d\n", ErrorNewFace);

if ErrorNewFace > MaxDiff
    printf("\tRESULT: %s is not part of the database\n", NewFaceFile);
else
    printf("\tRESULT: %s is part of the database !\n", NewFaceFile);
end;

figure;
colormap(gray);
subplot(1,2,1); imagesc(reshape(NewFace, 300, 240));
subplot(1,2,2); imagesc(reshape(NewFaceRec, 300, 240));
