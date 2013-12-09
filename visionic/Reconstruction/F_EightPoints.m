%
%  F_EightPoints(m1, m2)
%
% Description :
%  Calcule la matrice fondamentale � partir des correspondances m1,m2 en appliquant
%  l'algorithme 8-points.
%
% Param�tres :
%  m1 : Coordonn�es des points dans l'image 1. Cette matrice doit comporter 2 lignes. la 
%  premi�re ligne corresponds aux absisses des points, la seconde aux ordonn�es.
%  m2 : Coordonn�es des points dans l'image 2. Doit etre de meme taille que m1.
%
% Valeurs retourn�es :
%  F = F_EightPoints(m1, m2) : F est la matrice fondamentale
%
function F = F_EightPoints(m1, m2)

if(nargin < 2)
    error('Vous devez sp�cifier les coordonn�es des points');
end
if size(m1)~=size(m2)
    error('les matrices m1 et m2 doivent etre de meme taille');
end

m1 = vertcat(m1, ones(1,size(m1,2)));
m2 = vertcat(m2, ones(1,size(m1,2)));

%placement des coordonn�es des points dans la matrice A
%en vue de la r�solution du syst�me Af=0

A = zeros(size(m1, 2), 9);

for i=1:size(m1, 2)
    temp = m2(:,i)*transpose(m1(:,i));
    temp = reshape(temp, 1, 9);
    A(i,:) = temp;
end

[U,S,V] = svd(transpose(A)*A);

F = V(:,9);
F = reshape(F, 3, 3);

[U,S,V] = svd(F);
S(3,3) = 0;
F = U*S*V';