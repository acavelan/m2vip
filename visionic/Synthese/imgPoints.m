function imgOut = imgPoints(img, pts, colors, ptsSize)
%
%  imgPoints(img, pts, colors, ptsSize)
%
% Description :
%  Affiche des points sur une image
%
% Param�tres :
%  img : image sur laquelle afficher les points
%  pts : coordonn�es des n points � afficher. pts est une matrice de taille 2*n o� 
%        la premi�re ligne correspond aux abscisses x des points et la seconde 
%        lignes aux ordonn�es y des points. L'origine de l'image est dans le coin
%        sup�rieur gauche, l'axe x croit de gauche � droite et l'axe y de haut en 
%        bas.
%  colors : couleurs � appliquer aux points. Tableau de 1*n si l'image est en 
%           niveaux de gris, ou de taille 3*n si l'image est en couleur. Le type 
%           de donn�es doit correspondre au type de donn�es de l'image. Le tableau
%           peut ne contenir qu'une seule colone si tous les points doivent avoir 
%           la meme couleur.
%  ptsSize : Taille du carr� repr�sentant les points
%
% Valeurs retourn�es :
%  imgOut : image modifi�e par l'affichage des points.
%

n = size(pts,2);
xmin = 1;
ymin = 1;
xmax = size(img, 2);
ymax = size(img, 1);

if size(colors, 2) == 1
    colors = repmat(colors, 1, n);
end

imgOut = img;

for i=1:n
    for j=ceil(-ptsSize/2):floor(ptsSize/2)
        for k=ceil(-ptsSize/2):floor(ptsSize/2)
            newx = round(pts(1,i)+j);
            newy = round(pts(2,i)+k);
            if newx>=xmin && newx<=xmax && newy>=ymin && newy<=ymax
                imgOut(newy, newx, :) = colors(:,i);
            end
        end
    end
end
