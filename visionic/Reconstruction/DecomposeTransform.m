%
%  DecomposeTransform(E)
%
% Description :
%  Décompose la matrice essentielle E en un produit d'une matrice de rotation 
%  et d'une matrice de translation.
%  Il y a plusieurs solutions possibles.
%
% Paramètres :
%  E : Matrice essentielle à décomposer
%
% Valeurs retournées :
%  [T1, T2, R1, R2] = DecomposeTransform(E) : T1 et T2 sont deux matrices de 
%    translations et R1 et R2 sont deux matrices de rotation. On a E=k*Ri*Tj pour 
%    i=j et i~=j avec k réel non nul
function [T1, T2, R1, R2] = DecomposeTransform(E)

[U S V] = svd(E);

R1 = U*[0,1,0; -1,0,0; 0,0,1]*V';
R2 = U*[0,-1,0; 1,0,0; 0,0,1]*V';
T = V*[0,-1,0; 1,0,0; 0,0,0]*V';

%vérifie le déterminant des matrices de rotations
if det(R1)<0
    R1 = -R1;
end
if det(R2)<0
    R2 = -R2;
end

%lambda : norme de T
% E2 = R2*T;
% lambda = [];
% for i=1:9
%     if abs(E2(i)*E(i)) > 1e-4
%         lambda = vertcat(lambda, E(i)/E2(i));
%     end
% end
% lambda = mean(lambda);
T1 = T;
T2 = -T;
