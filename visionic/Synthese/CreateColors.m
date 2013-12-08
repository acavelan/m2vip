function colors = CreateColors(N)
%
%  CreateColors(N)
%
% Description :
%  Crée une palette de N couleurs de type double.
%
% Paramètres :
%  N : Nombre de couleurs
%
% Valeurs Retournées :
%  colors : Tableaux de taille 3xN. Chaque colonne correspond à une couleur de 
%           type RGB. Chaque composante est comprise entre 0 (pas de couleur) et 
%           1 (100%).
%
if N == 1
    colors = [1, 0, 0]';
else
    for i=1:N
        if i < (N+3)/4
            r = 1;
            g = 4/(1-N)*(1-i);
            b = 0;
        elseif i < (N+1)/2
            r = (4*i/(1-N)+2*(N+1)/(N-1));
            g = 1;
            b = 0;
        elseif i < (3*N+1)/4
            r = 0;
            g = 1;
            b = (4*i-2*(N+1))/(N-1);
        else
            r = 0;
            g = (4*(i-N)/(1-N));
            b = 1;
        end
        colors(:,i) = [r,g,b]';
    end
end
