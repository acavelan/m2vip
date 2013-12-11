pkg load image;
close all;
clear all;

addpath('Synthese');
load('Pts3D.mat');

# TD1
# ==============================================================================

# Matrice de rotation R et le vecteur de translation t des caméras
R1 = eye(3);
t1 = [0; 0; 0];

[R2, t2] = PlaceCamera([20, 0, 4], [0, 0, 16], [0, 1, 0]);

R = cat(3, R1, R2);
t = cat(2, t1, t2);

DrawCamera(R1, t1, 5.0);
DrawCamera(R2, t2, 5.0);
Show3DScene(Pts3D, R, t, 0);

# Matrices de changement de repère
T1 = [R1, R1*t1; [0, 0, 0, 1]];
T2 = [R2, R2*t2; [0, 0, 0, 1]];

# CCD 640 * 480 px, capteur 4 * 3 mm
ku = 640 / 4;
kv = 480 / 3;

u0 = 640 / 2;
v0 = 480 / 2;

f = 3.75;

# Matrice de calibrage A
A = [f * ku, 0, u0; 0, f * kv, v0; 0, 0, 1];

# Matrices de projections des caméras
P1 = [A [0; 0; 0]] * T1;
P2 = [A [0; 0; 0]] * T2;

# Projection des points 3D
m1 = P1 * Pts3D;
m2 = P2 * Pts3D;

m1 = m1 ./ [m1(3,:); m1(3,:); m1(3,:)];
m2 = m2 ./ [m2(3,:); m2(3,:); m2(3,:)];

IM1 = CreateImage(m1);
IM2 = CreateImage(m2);

figure;
image(IM1);

figure;
image(IM2);

# Calcule de la matrice fondamentale
# (-20) x1      (0    -x3   x2)
# ( 0)	x2	->  (x3    0   -x1)
# (-4)  x3      (-x2   x1    0)

t2 = [0,4,0;-4,0,20;0,-20,0];

E = R2 * t2;
F = inv(A') * E * inv(A);

# TD2
# ==============================================================================

addpath('Reconstruction');

m1 = m1(1:2,:);
m2 = m2(1:2,:);

# Estimation de la matrice fondamentale à l'aide de la méthode des 8 points
Fest = ComputeF(m1, m2, 7, 1);

# Facteur d'échelle: 3.19048
scalef = F ./ Fest;

# Estimation de la matrice de calibrage
Aest = [-1, 0, 640/2; 0, -1, 480/2; 0, 0, 1];

# A = Aest !
Aest = SelfCalibrate(Fest, Aest);

# Estimation des paramètres extrinsèques
Est = Aest' * Fest * Aest;

[T21, T22, R21, R22] = DecomposeTransform(Est);

# Estimation de la matrice de projection P1
R1est = eye(3);
t1est = [0; 0; 0];

T1est = [R1est, R1est*t1est; [0, 0, 0, 1]];
P1est = [Aest [0; 0; 0]] * T1est;

# Estimation des matrices de projection de P2
t21 = [T21(2,3); -T21(1,3); T21(1,2)];
t22 = [T22(2,3); -T22(1,3); T22(1,2)];

T11 = [R21, R21*t21; [0, 0, 0, 1]];
T12 = [R21, R21*t22; [0, 0, 0, 1]];
T21 = [R22, R22*t21; [0, 0, 0, 1]];
T22 = [R22, R22*t22; [0, 0, 0, 1]];

P11 = [Aest [0; 0; 0]] * T11;
P12 = [Aest [0; 0; 0]] * T12;
P21 = [Aest [0; 0; 0]] * T21;
P22 = [Aest [0; 0; 0]] * T22;

# Reconstruction par triangulation
M11 = Triangulation(m1, m2, P1est, P11);
M12 = Triangulation(m1, m2, P1est, P12);
M21 = Triangulation(m1, m2, P1est, P21);
M22 = Triangulation(m1, m2, P1est, P22);

# Res 1
R = cat(3, R1est, R21);
t = cat(2, t1est, t21);

Show3DScene(M11, R, t, 1);
title("P11");

# Res 2
R = cat(3, R1est, R21);
t = cat(2, t1est, t22);

Show3DScene(M12, R, t, 1);
title("P12");

# Res 3
R = cat(3, R1est, R22);
t = cat(2, t1est, t21);

Show3DScene(M21, R, t, 1);
title("P21");

# Res 4
R = cat(3, R1est, R22);
t = cat(2, t1est, t22);

Show3DScene(M22, R, t, 1);
title("P22");

#VerifierReconstruction([M11;M12;M21;M22], [640,480])
