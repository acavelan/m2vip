function DrawCamera(R, t, length)
%  DrawCamera(R, t, length)
%
% Description :
%  Représente une caméra par un repère orthonormal l'axe des x est
%  représenté par un trait rouge, y par un trait vert et z par un trait
%  bleu.
%
% Paramètres :
%  R : matrice de rotation associée à la caméra.
%  t : Vecteur de translation associé à la caméra.
%  length : longueur des traits représentant les axes.
%
% Valeurs retournées :
%  aucune.
%

if nargin ~= 3
    error('Nombre de paramètres incorrect');
end

xyz = eye(3)*length;

% Rotation des axes
xyz = inv(R) * xyz;

% Translation des axes
t = reshape(t,3,1);
xyz = xyz - [t,t,t];

% définition des traits pour les axes
x = [-t, xyz(:,1)];
y = [-t, xyz(:,2)];
z = [-t, xyz(:,3)];

% Affichage de la caméra
modehold = ishold;
hold on;
% Affichage du centre optique
plot3(-t(1), -t(2), -t(3), 'LineStyle', 'none', 'Marker', '.', 'MarkerEdgeColor', [0,0,0]);
%affichage de l'axe x
plot3(x(1,:), x(2,:), x(3,:), 'LineStyle', '-', 'Marker', 'none', 'Color', [1,0,0]);
%affichage de l'axe y
plot3(y(1,:), y(2,:), y(3,:), 'LineStyle', '-', 'Marker', 'none', 'Color', [0,1,0]);
%affichage de l'axe z
plot3(z(1,:), z(2,:), z(3,:), 'LineStyle', '-', 'Marker', 'none', 'Color', [0,0,1]);

%affiche les noms des axes
text(x(1,2),x(2,2),x(3,2),'x');text(y(1,2),y(2,2),y(3,2),'y');text(z(1,2),z(2,2),z(3,2),'z');

if ~modehold
    hold off;
end
