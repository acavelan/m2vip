function Show3DScene(M, R, t, newfig)
%
%  Show3DScene(M, R, t)
%
% Description :
%  Affiche la scène en 3D en placant les caméras spécifiées
%
% Paramètres :
%  M : Tableau contenant les coordonnées 3D non-homogènes des points de la scène.
%      Chaque colone du tableau correspond aux coordonnées (X,Y,Z) d'un point
%  R : (facultatif) Matrices de rotations des caméras. R(:,:,i) correspond à la 
%      matrice de rotation de la caméra i.
%  t : (facultatif) Vecteurs de translations des caméras. t(:,i) correspond à la
%      caméra i.
%  newfig : (facultatif) = 1 (défaut) afficher dans une nouvelle figure
%                        = 0 afficher dans la figure courante
%
% Valeurs retournées :
%  Aucune.
%

%vérification des paramètres :

if nargin < 1
    error('Paramètres insuffisants');
end
if nargin < 4
    newfig = 1;
end
if nargin == 1
    N = 0;
elseif nargin == 2
    error('R et t doivent etre spécifiés en meme temps');
elseif nargin >= 3
    if isempty(R) && isempty(t)
        N = 0;
    else
	    N = size(R, 3);
	    if N~=size(t,2)
           error('Paramètres incorrects. R et t doivent correspondrent au meme nombre de caméras');
	    end
    end
end

%Création d'une nouvelle figure :
if newfig
    figure;
else
    hold on;
end

%trace un ligne bleue à travers tous les points 3D
plot3(M(1,:),M(2,:),M(3,:),'b');
hold on;

%Affiche les caméras :
for i=1:N
    DrawCamera(R(:,:,i), t(:,i), 1);
end

%trace les points 3D
N = size(M,2);
colors = CreateColors(N);
for i=1:N
    plot3(M(1,i),M(2,i),M(3,i),'MarkerEdgeColor', colors(:,i), 'Marker', '.', 'MarkerSize', 20);
end

%etablit les paramètre de visualisation
set(gca, 'DataAspectRatio', [1,1,1]);
camPos = get(gca, 'CameraPosition');
camPos(1) = -camPos(1);
camPos(3) = -camPos(3);
set(gca, 'CameraPosition', camPos);
set(gca, 'CameraTarget', [0,0,15]);
set(gca, 'CameraUpVector', [0,-1,0]);
#set(gca, 'Projection', 'perspective');
set(gca, 'XGrid', 'on');
set(gca, 'YGrid', 'on');
set(gca, 'ZGrid', 'on');
