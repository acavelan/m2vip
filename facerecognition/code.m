pkg load image;

clear all;
close all;

N = 14;


# 1 - Construction de la matrice des données
#===============================================================================

M = zeros(N, 300*240);

for i=1:N,
	I = imread(strcat("photos/ph", num2str(i), ".jpg"));
	I = double(I);
	I = rgb2gray(I);
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

#figure;
#colormap(gray);
#for i=1:14
#	subplot(4,4,i);
#	I = reshape(M(i,:), 300, 240);
#	imagesc(I);
#end;

#figure;
#colormap(gray);
#subplot(1, 1, 1);
#imagesc(reshape(G, 300, 240));
#title(strcat('average face'));

#figure;
#colormap(gray);
#for i=1:14
#	subplot(4,4,i);
#	imagesc(reshape(U(:,i), 300, 240));
#	title(strcat('eigenface ', num2str(i)));
#end;


# 3 - Reconstructedion des visages de la base de données à partir des eigenface
#===============================================================================

C = Xtilde * U;

Reconstructed = zeros(N, 300*240);

for i=1:14
	for j=1:14
		Reconstructed(i,:) += C(i,j) * U(:,j)';
	end;
	Reconstructed(i,:) += G';
end;

ErrorBase = zeros(N, 1);
for j=1:14
    ErrorBase(j) = sum((M(j,:) - Reconstructed(j,:)).^2);
    ErrorBase(j) = sqrt(ErrorBase(j));
    printf("Error for reconstructed face %d is %d\n", j, ErrorBase(j));
end
ErrorBaseMean = mean(ErrorBase);
printf("Mean error is %d\n", ErrorBaseMean);

#figure;
#colormap(gray);
#for i=1:14
#	subplot(4,4,i);
#	I = reshape(Reconstructed(i,:), 300, 240);
#	imagesc(I);
#end;

