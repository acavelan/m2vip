function [A, err] = SelfCalibrate(F, Aest)
%
%  SelfCalibrate(F, Aest)
%
% Description :
%  Calcule les param�tres intrins�ques d'une cam�ra � partir de deux vues non
%  calibr�es.
% 
% Param�tres :
%  F : Matrice fondamentale r�gissant la g�om�trie �pipolaire entre les deux vues
%  Aest : Premi�re estimation de la matrice de calibrage :
%    Aest = [r*f0    0   cx]
%           [   0   f0   cy]
%           [   0    0    1]
%    avec (cx, cy) : coordonn�es du point principal dans les deux images
%         f0 : estimation de la distance focale exprim�e en pixels. Si f0 est mis 
%              �gal � -1, la fonction calcule f0 = 2*sqrt(cx^2+cy^2) qui 
%              correspond � une focale normale pour l'objectif (si (cx, cy) est le
%              centre de l'image).
%         r : rapport largeur/hauteur des pixels
%
% Valeurs Retourn�es :
%  A : Matrice de calibration d�finie de la meme facon que Aest.
%

%v�rification des param�tres :
if nargin < 2
    error('Param�tre manquant');
end

Aest = Aest / Aest(3,3);
r = Aest(1,1)/Aest(2,2);
cx = Aest(1,3);
cy = Aest(2,3);
if Aest(2,2) == -1
    f0 = 2*sqrt(cx^2+cy^2);
else
    f0 = Aest(2,2);
end

err = 0;

%calcul de la matrice fondamentale semi-calibr�e
G = [r, 0, 0; 0, 1, 0; cx, cy, 1]*F*[r, 0, cx; 0, 1, cy; 0, 0, 1];
% mise � l'�chelle de cette matrice pour stabilit� num�rique :
G = diag([f0, f0, 1])*G*diag([f0, f0, 1]);
G = G/norm(G, 'fro');

%Calcul de la SVD de G et r�cup�ration des donn�es utiles
[U S V] = svd(G);
u13 = U(3,1);
u23 = U(3,2);
v13 = V(3,1);
v23 = V(3,2);
a = S(1,1);
b = S(2,2);

%r�solution des �quations de kruppa :
a2 = a^2*(1-u13^2)*(1-v13^2)-b^2*(1-u23^2)*(1-v23^2);
a1 = a^2*(u13^2+v13^2-2*u13^2*v13^2) - b^2*(u23^2+v23^2-2*u23^2*v23^2);
a0 = a^2*u13^2*v13^2 - b^2*u23^2*v23^2;

delta = a1^2-4*a2*a0;
if delta < 0
    if nargout < 2
        warning('Pas de solution');
    else
        err = 1;
    end
    f = f0;
elseif delta == 0
    f = -a1/(2*a2);
    if f<0
        if nargout < 2
            warning('Pas de solution');
        else
            err = 1;
        end
        f = f0;
    else
        f = sqrt(f)*f0;
    end
else
    f1 = (-a1+sqrt(delta))/(2*a2);
    f2 = (-a1-sqrt(delta))/(2*a2);
    if f1<0 & f2>=0
        f = sqrt(f2)*f0;
    elseif f2<0 & f1>=0
        f = sqrt(f1)*f0;
    elseif f1>=0 & f2>=0
        f = sqrt([f1 f2])*f0;
    else
        if nargout < 2
            warning('Pas de solution');
        else
            err = 1;
        end
        f = f0;
    end
end

if size(f, 2) == 2
    if abs(f(1)-f0) < abs(f(2)-f0)
        f = f(1);
    else
        f = f(2);
    end
end

A = [r*f, 0, cx; 0, f, cy; 0, 0, 1];
