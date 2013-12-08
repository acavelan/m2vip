function img = CreateImage(m, imgSize, imgType, ptsSize, useColor)
%  CreateImage(m, [imgSize, [imgType, [ptsSize, [useColor]]]])
%
% Description :
%  Crée l'image 2D des points 3D du monde acquise par une caméra connue.
%
% Paramètres :
%  m : tableau de coordonnées image des points à afficher dans l'image.
%      Chaque colonne correspond au coordonnées (x,y) d'un point.
%  imgSize : Vecteur de taille 2 spécifiant la taille de l'image (defaut [640, 480])
%  imgType : Chaine de caractères spécifiant la classe des données stockées dans 
%            l'image (defaut 'double')
%  ptsSize : Taille des carrés représentant les points sur l'image (defaut 3)
%  useColor : 0 = image noir et blanc
%             1 = (defaut) image couleur
%
% Valeurs retournées :
%  img : image
%

% Vérification des paramètres
if nargin < 5
    useColor = 1;
end
if nargin < 4
    ptsSize = 3;
end
if nargin < 3
    imgType = 'double';
end
if nargin < 2
    imgSize = [640, 480];
end
if nargin < 1
    error('Paramètres incorrect');
end

% initialisation de l'image
img = ones(imgSize(2), imgSize(1), useColor*2+1);
switch imgType
case 'double'
    maxColor = 1;
case 'uint8'
    maxColor = 255;
    img = uint8(maxColor*img);
case 'uint16'
    maxColor = 65535;
    img = uint16(maxColor*img);
otherwise
    error('Type non pris en charge');
end

% création de l'échelle de couleur
nbPoints = size(m, 2);
if ~useColor
    colors = zeros(1, nbPoints);
else
    colors = CreateColors(nbPoints)*maxColor;
end

% dessin des points :
img = imgPoints(img, m, colors, ptsSize);
