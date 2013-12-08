function [R, t] = PlaceCamera(Pos, WatchedPoint, UpVector)
%  PlaceCamera(Pos, WatchedPoint, UpVector)
%
% Description :
%  Calcule les param�tres extrins�ques (matrice de rotation et vecteur de 
%  translation) d'une cam�ra en fonction de son positionnement dans le monde.
%
% Param�tres :
%  Pos : Vecteur des coordonn�es (X, Y, Z) de la position de la cam�ra dans le
%        monde
%  WatchedPoint : Vecteur de coordonn�es (X, Y, Z) du point vis� par la cam�ra
%  UpVector : Vecteur (X, Y, Z) donnant la direction du haut de la cam�ra. Si ce 
%             vecteur n'est pas dans le plan hortogonal � la direction de vision,
%             la projection hortogonale de ce vecteur sur le plan est utilis�e.
%
% Valeurs retourn�es :
%  [R, t] : R : Matrice de rotation de cette cam�ra � la cam�ra "origine".
%           t : Vecteur de translation de cette cam�ra � l'origine.
%  Pour transformer la cam�ra en l'origine du monde, il faut appliquer la 
%  translation t puis la rotation R � la cam�ra.
%

% mise en forme des donn�es
Pos = reshape(Pos, 3, 1);
WatchedPoint = reshape(WatchedPoint, 3, 1);
UpVector = reshape(UpVector, 3, 1);

% calcul de t :
t = -Pos;

% calcul des axes directeurs de la cam�ra :
% z est la direction de la cam�ra au point regard�
z = WatchedPoint - Pos;
z = z/norm(z, 'fro');

% x est la direction orthogonale � z et � UpVector tel que (x, UpVector, z) forme
% un tri�dre direct.
UpVector = UpVector/norm(UpVector, 'fro');
x = cross(UpVector, z);
x = x/norm(x, 'fro');

% y est tel que (x, y, z) soit un tri�dre orthonormal direct.
y = cross(z, x);

% R est la matrice de changement de base entre la base de la cam�ra et la base 
% canonique.
R = inv([reshape(x, 3, 1), reshape(y, 3, 1), reshape(z, 3, 1)]);