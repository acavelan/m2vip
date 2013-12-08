function img = CreateImage(m, imgSize, imgType, ptsSize, useColor)
%  CreateImage(m, [imgSize, [imgType, [ptsSize, [useColor]]]])
%
% Description :
%  Cr�e l'image 2D des points 3D du monde acquise par une cam�ra connue.
%
% Param�tres :
%  m : tableau de coordonn�es image des points � afficher dans l'image.
%      Chaque colonne correspond au coordonn�es (x,y) d'un point.
%  imgSize : Vecteur de taille 2 sp�cifiant la taille de l'image (defaut [640, 480])
%  imgType : Chaine de caract�res sp�cifiant la classe des donn�es stock�es dans 
%            l'image (defaut 'double')
%  ptsSize : Taille des carr�s repr�sentant les points sur l'image (defaut 3)
%  useColor : 0 = image noir et blanc
%             1 = (defaut) image couleur
%
% Valeurs retourn�es :
%  img : image
%

% V�rification des param�tres
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
    error('Param�tres incorrect');
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

% cr�ation de l'�chelle de couleur
nbPoints = size(m, 2);
if ~useColor
    colors = zeros(1, nbPoints);
else
    colors = CreateColors(nbPoints)*maxColor;
end

% dessin des points :
img = imgPoints(img, m, colors, ptsSize);
