pkg load image;
close all;
clear all;

load('Pts3D.mat');

R1 = eye(3);
t1 = [0; 0; 0];

[R2, t2] = PlaceCamera([20, 0, 4], [0, 0, 16], [0, 1, 0]);

R = cat(3, R1, R2);
t = cat(2, t1, t2);

#DrawCamera(R1, t1, 5.0);
#DrawCamera(R2, t2, 5.0);
Show3DScene(Pts3D, R, t, 0);

T1 = [R1, R1*t1; [0, 0, 0, 1]];
T2 = [R2, R2*t2; [0, 0, 0, 1]];

ku = 640 / 4;
kv = 480 / 3;

u0 = 640 / 2;
v0 = 480 / 2;

f = 3.75;

A = [f * ku, 0, u0; 0, f * kv, v0; 0, 0, 1];

P1 = [A [0; 0; 0]] * T1;
P2 = [A [0; 0; 0]] * T2;

Pts2D1 = P1 * Pts3D;
Pts2D2 = P2 * Pts3D;

Pts2D1 = Pts2D1(1:2,:) ./ Pts2D1(3,:);
Pts2D2 = Pts2D2(1:2,:) ./ Pts2D2(3,:);

IM1 = CreateImage(Pts2D1);
IM2 = CreateImage(Pts2D2);

figure;
image(IM1);

figure;
image(IM2);

t1 = [0,0,0; 0,0,0; 0,0,0];
t2 = [0,4,0;-4,0,20;0,-20,0];

E1 = R1 * t1;
F1 = inv(A') * E1 * inv(A);

E2 = R2 * t2;
F2 = inv(A') * E2 * inv(A);
