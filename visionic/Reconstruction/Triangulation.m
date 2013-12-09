%
%  Triangulation(m1, m2, P1, P2)
%
% Description :
%  Reconstruit en 3D les points 2D m1 et m2 par triangulation en déprojetant par 
%  les matrices P1 et P2.
%
% Paramètres :
%  m1, m2 : Points 2D des images 1 et 2
%  P1, P2 : Matrices de projections des caméras 1 et 2
%
% Valeurs retournées :
%  M = Triangulation(m1, m2, P1, P2) : M est la représentation 3D en coordonnées 
%  homogènes (4 lignes) des couples de points 2D (m1,m2)
%
function M = Triangulation(m1, m2, P1, P2)

if size(m1) ~= size(m2), error('les matrices m1 et m2 doivent etre de meme taille'); end
if size(m1,2) == 2, m1 = m1'; end
if size(m2,2) == 2, m2 = m2'; end

m1 = vertcat(m1, ones(1, size(m1,2)));
m2 = vertcat(m2, ones(1, size(m2,2)));

M=ones(4, size(m1,2));

%systeme à résoudre pour chaque point :
for i=1:size(m1,2)
    S1 = m1(1,i)*P1(3,:)-P1(1,:);
    S2 = m1(2,i)*P1(3,:)-P1(2,:);
    S3 = m2(1,i)*P2(3,:)-P2(1,:);
    S4 = m2(2,i)*P2(3,:)-P2(2,:);
    S = vertcat(S1, S2, S3, S4);
    A = S(:,1:3);
    B = -S(:,4);
    %le point M(x,y,z) est tel que A*M = B
    M(1:3, i) = A\B;
end