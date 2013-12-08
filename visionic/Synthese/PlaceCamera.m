function [R, t] = PlaceCamera(Pos, WatchedPoint, UpVector)
%  PlaceCamera(Pos, WatchedPoint, UpVector)
%
% Description :
%  Calcule les paramètres extrinsèques (matrice de rotation et vecteur de 
%  translation) d'une caméra en fonction de son positionnement dans le monde.
%
% Paramètres :
%  Pos : Vecteur des coordonnées (X, Y, Z) de la position de la caméra dans le
%        monde
%  WatchedPoint : Vecteur de coordonnées (X, Y, Z) du point visé par la caméra
%  UpVector : Vecteur (X, Y, Z) donnant la direction du haut de la caméra. Si ce 
%             vecteur n'est pas dans le plan hortogonal à la direction de vision,
%             la projection hortogonale de ce vecteur sur le plan est utilisée.
%
% Valeurs retournées :
%  [R, t] : R : Matrice de rotation de cette caméra à la caméra "origine".
%           t : Vecteur de translation de cette caméra à l'origine.
%  Pour transformer la caméra en l'origine du monde, il faut appliquer la 
%  translation t puis la rotation R à la caméra.
%

% mise en forme des données
Pos = reshape(Pos, 3, 1);
WatchedPoint = reshape(WatchedPoint, 3, 1);
UpVector = reshape(UpVector, 3, 1);

% calcul de t :
t = -Pos;

% calcul des axes directeurs de la caméra :
% z est la direction de la caméra au point regardé
z = WatchedPoint - Pos;
z = z/norm(z, 'fro');

% x est la direction orthogonale à z et à UpVector tel que (x, UpVector, z) forme
% un trièdre direct.
UpVector = UpVector/norm(UpVector, 'fro');
x = cross(UpVector, z);
x = x/norm(x, 'fro');

% y est tel que (x, y, z) soit un trièdre orthonormal direct.
y = cross(z, x);

% R est la matrice de changement de base entre la base de la caméra et la base 
% canonique.
R = inv([reshape(x, 3, 1), reshape(y, 3, 1), reshape(z, 3, 1)]);