function [A, err] = SelfCalibrate(F, Aest)
%
%  SelfCalibrate(F, Aest)
%
% Description :
%  Calcule les paramètres intrinsèques d'une caméra à partir de deux vues non
%  calibrées.
% 
% Paramètres :
%  F : Matrice fondamentale régissant la géométrie épipolaire entre les deux vues
%  Aest : Première estimation de la matrice de calibrage :
%    Aest = [r*f0    0   cx]
%           [   0   f0   cy]
%           [   0    0    1]
%    avec (cx, cy) : coordonnées du point principal dans les deux images
%         f0 : estimation de la distance focale exprimée en pixels. Si f0 est mis 
%              égal à -1, la fonction calcule f0 = 2*sqrt(cx^2+cy^2) qui 
%              correspond à une focale normale pour l'objectif (si (cx, cy) est le
%              centre de l'image).
%         r : rapport largeur/hauteur des pixels
%
% Valeurs Retournées :
%  A : Matrice de calibration définie de la meme facon que Aest.
%

%vérification des paramètres :
if nargin < 2
    error('Paramètre manquant');
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

%calcul de la matrice fondamentale semi-calibrée
G = [r, 0, 0; 0, 1, 0; cx, cy, 1]*F*[r, 0, cx; 0, 1, cy; 0, 0, 1];
% mise à l'échelle de cette matrice pour stabilité numérique :
G = diag([f0, f0, 1])*G*diag([f0, f0, 1]);
G = G/norm(G, 'fro');

%Calcul de la SVD de G et récupération des données utiles
[U S V] = svd(G);
u13 = U(3,1);
u23 = U(3,2);
v13 = V(3,1);
v23 = V(3,2);
a = S(1,1);
b = S(2,2);

%résolution des équations de kruppa :
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
