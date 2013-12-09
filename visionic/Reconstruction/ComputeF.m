function [F, inliers] = ComputeF(m1, m2, method, normalize)
%  ComputeF(m1, m2, method, normalize)
%
% Description :
%  Calcule la matrice fondamentale à partir des correspondances données par
%  les matrices m1 et m2. La méthode de calcul doit etre spécifiée.
%
% Paramètres :
%  m1 : points sur l'image 1. Matrice de taille 2*N. La première ligne
%       correspond aux absisses, la seconde aux ordonnées. 
%  m2 : points sur l'image 2. Chaque colonne doit correspondre à la meme
%       colonne en m1.
%  method : entier spécifiant la méthode pour calculer F. Utiliser la 
%           fonction GetMethodName pour connaitre le nom de la méthode
%           associée a un entier.
%  normalize : = 1 : Normalise les coordonnées avant le calcul de F.
%              = 0 : Ne normalise pas les coordonnées.
%
% Valeur retournée :
%  F : matrice fondamentale
%  inliers : index des points correct de m1 et m2.
%

% vérification des paramètres
if nargin ~= 4
    error('La fonction nécessite 4 paramètres');
end
if size(m1) ~= size(m2)
    error('Les paramètres m1 et m2 doivent etre de meme taille');
end
if size(m1,1) ~= 2 | size(m2,1) ~= 2
    error('m1 et m2 doivent avoir exactement 2 lignes');
end
if normalize ~= 0 && normalize ~= 1
    error('Le paramètre normalize doit etre égal à 0 ou 1');
end

if normalize == 1
	%Normalisation des coordonnées :
	%Calcul des matrices T1 et T2 de transformation des coordonnées
	% T est de la forme :
	%   [    su,     0, su*tu]
	%   [     0,    sv, sv*tv]
	%   [     0,     0,     1]
	% avec su et sv les facteur de normalisation, et (tu, tv) 
	% le vecteur de translation pour ramener le centroide des 
	% points à l'origine
	% on utilise le mode de normalisation isotropique => su = sv = s
	% s est tel que la distance moyenne des points à l'origine soit égal à sqrt(2)
	
	% calcul de tu et tv :
	tu1 = -mean(m1(1,:));
	tv1 = -mean(m1(2,:));
	tu2 = -mean(m2(1,:));
	tv2 = -mean(m2(2,:));
	% calcul de s :
	s1 = 1.414214/mean(sqrt((m1(1,:)+tu1).^2+(m1(2,:)+tv1).^2));
	s2 = 1.414214/mean(sqrt((m2(1,:)+tu2).^2+(m2(2,:)+tv2).^2));
	
	T1 = [s1, 0, s1*tu1; 0, s1, s1*tv1; 0, 0, 1];
	T2 = [s2, 0, s2*tu2; 0, s2, s2*tv2; 0, 0, 1];
	
	%Normalisation :
	m1 = T1*[m1; ones(1, size(m1,2))];
    m1 = m1(1:2, :);
	m2 = T2*[m2; ones(1, size(m2,2))];
    m2 = m2(1:2, :);
end

%prépare la valeur de sortie
inliers = 1:size(m1,2);

% Calcul des paramètres d'optimisation des algorithmes
switch method
    case {1, 8}
        %calcul du nombre d'itération
        % pour une estimation de 50% d'outliers présents dans les
        % données, il suffit de 386 itérations pour avoir 95% de chance
        % de sélectionner 1 ensemble de 7 points sans outliers
        optim_parameter(1) = 400;
        %calcul du seuil de tolérance pour les outliers.
        % Estimons que le bruit sur les donées et de sigma = 1 pixel.
        % Une fois normalisé, ce bruit est diminué de s1 et s2 sur les
        % images. On fixe le bruit normalisé commun aux deux images 
        % comme le bruit le plus grand des bruits normalisés.
        if normalize == 1
            sigma = max([s1, s2]);
            U = 2*sqrt(2);
        else
            sigma = 1;
            U = max([max(max(m1)), max(max(m2))]);
        end
        % le seuil T est :
        optim_parameter(2) = log(U^2/(2*pi*sigma))-4.4;
    case {9, 10}
        %calcul du nombre d'itération
        % pour une estimation de 50% d'outliers présents dans les
        % données, il suffit de 766 itérations pour avoir 95% de chance
        % de sélectionner 1 ensemble de 8 points sans outliers
        optim_parameter(1) = 800;
        %calcul du seuil de tolérance pour les outliers.
        % Estimons que le bruit sur les donées et de sigma = 1 pixel.
        % Une fois normalisé, ce bruit est diminué de s1 et s2 sur les
        % images. On fixe le bruit normalisé commun aux deux images 
        % comme le bruit le plus grand des bruits normalisés.
        if normalize == 1
            sigma = max([s1, s2]);
            U = 2*sqrt(2);
        else
            sigma = 1;
            U = max([max(max(m1)), max(max(m2))]);
        end
        % le seuil T est :
        optim_parameter(2) = log(U^2/(2*pi*sigma));%-4.4;
    otherwise
        optim_parameter = [];
end

% Calcul de la matrice fondamentale
switch method
    %case {1, 2, 3, 4, 5, 6}
    case 1
        % pour une estimation de 50% d'outliers présents dans les
        % données, il suffit de 386 itérations pour avoir 95% de chance
        % de sélectionner 1 ensemble de 7 points sans outliers
        [f,f_sq_errors, n_inliers,inlier_index] = MapsacF(m1(1,:)',m1(2,:)',m2(1,:)',m2(2,:)', numel(m1(1,:)), 1, 400);
        inliers = inlier_index;
        F = reshape(f, 3, 3);
        F = F';
        %Assurance du rang 2
        if rank(F)>2
            [U S V] = svd(F);
            S(3,3)=0;
            F = U*S*V';
        end
    case {2, 3, 4, 5, 6}
        [f, e1, n_inliers, inlier_index, F] = torr_estimateF([m1(1,:)',m1(2,:)',m2(1,:)',m2(2,:)'], 1, [], method, 1);
    case 7 %8-point
        F = F_EightPoints(m1, m2);
    case 8 %MAPSAC 7 points + 8-point
        % pour une estimation de 50% d'outliers présents dans les
        % données, il suffit de 386 itérations pour avoir 95% de chance
        % de sélectionner 1 ensemble de 7 points sans outliers
        [f,f_sq_errors, n_inliers,inlier_index] = MapsacF(m1(1,:)',m1(2,:)',m2(1,:)',m2(2,:)', numel(m1(1,:)), 1, 400);
        inliers = inlier_index;
        m1 = m1(:,inlier_index);
        m2 = m2(:,inlier_index);
        F = F_EightPoints(m1, m2);
    case 9 %MAPSAC modifié sur 8 points
        % pour une estimation de 50% d'outliers présents dans les
        % données, il suffit de 766 itérations pour avoir 95% de chance
        % de sélectionner 1 ensemble de 8 points sans outliers
        [f,f_sq_errors, n_inliers,inlier_index] = MapsacF8(m1(1,:)',m1(2,:)',m2(1,:)',m2(2,:)', length(m1(1,:)'), 1, 800);
        inliers = inlier_index;
        F = reshape(f, 3, 3);
        F = F';
        %Assurance du rang 2
        if rank(F)>2
            [U S V] = svd(F);
            S(3,3)=0;
            F = U*S*V';
        end
    case 10 %MAPSAC 8 points + 8-points
        % pour une estimation de 50% d'outliers présents dans les
        % données, il suffit de 766 itérations pour avoir 95% de chance
        % de sélectionner 1 ensemble de 8 points sans outliers
        [f,f_sq_errors, n_inliers,inlier_index] = MapsacF8(m1(1,:)',m1(2,:)',m2(1,:)',m2(2,:)', length(m1(1,:)'), 1, 800);
        inliers = inlier_index;
        m1 = m1(:,inlier_index);
        m2 = m2(:,inlier_index);
        F = F_EightPoints(m1, m2);
        %Assurance du rang 2
        if rank(F)>2
            [U S V] = svd(F);
            S(3,3)=0;
            F = U*S*V';
        end
end


if normalize == 1
    % Dénormalisation des coordonnées
    F = T2'*F*T1;
end