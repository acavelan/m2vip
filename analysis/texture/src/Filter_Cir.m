function [retour] = Filter_Cir(img,r)
%% Filter_Cir.m
% Filter_Cir is a function that allows you to make a filtering in frequency
% domain this type of filtering is called: circular filtering and often 
% called flattening of a number of well-defined lines from 2D images FFT.
% img : imput image
% retour : output image after filtering
% Example
% I = imread('imgname');
% r = 25;
% J = Filter_Cir(I,25)
% copyright by HOUAM Lotfi 2010, houlotfi@yahoo.fr
%%
if isvector(img)
    error('Warning: img must be a matrix')
end 
[Xn Yn] = size(img);
if Xn ~= Yn
    error('Warning: img must be a squared matrix')
end
%% Circular filtering
img = im2double(img);  % for 8-bits grey level images
% img = double(img)/65535; % only for 16-bits grey level images

   %calculation of the 2D FFT 
Img_fr = fft2(img);
                 %im_r=real(Img_fr);
                 %im_i=imag(Img_fr);
% Affichage de la partie réelle 
           %figure(1),imshow(real(Img_fr));title('Partie Réelle');
% Affichage de la partie imaginaire
           %figure(2),imshow(imag(Img_fr));title('Partie Imaginaire');
[N M] = size(Img_fr); %#ok<NASGU>

% Circular filtering of the real part of the image
% tmp1=im_r(1,1);
tmp1 = Img_fr(1,1);      % preserving of the DC component
%r=26;                    % r, the radius of screening
warning off     %#ok<WNOFF>
for i=1:r
    for j=1:floor(sqrt(r^2-(i*i)))
        Img_fr(i,j)=0; 
    for j=N-floor(sqrt(r^2-(i*i))):N
        Img_fr(i,j)=0;
    end
    end
end

for i=r-N:N
    im = i-N;
    for j=1:floor(sqrt(r^2-(im*im)))
        Img_fr(i,j)=0;
    for j=N-floor(mod((sqrt(r^2-im^2)),N)):N
        Img_fr(i,j)=0;
    end
    end
end
                im_r = real(Img_fr);
                im_i = imag(Img_fr);
% restitution of the DC component          
Img_fr(1,1) = tmp1;  
% Affichage des parties selectionnées
%        figure(3),imshow(im_r);title('Partie réelle Select');
%        figure(4),imshow(im_i);title('Partie imagin Select');
%imwrite(im_r, 'R_K_Filt_01.tif');
%imwrite(im_i, 'I_K_Filt_01.tif');
%calculation of the 2D FFT inverse
 retour = ifft2(Img_fr);
 retour = im2uint8(retour);
% retour = uint16(round(abs(retour)*65535));
% retour = im2uint16(retour);
 % Affichage de l'image filtrée
 %figure,imshow(retour);title('Image filtrée');
 %figure(5),imshow(real(retour));title('Image filtrée');
 %imwrite(retour, 'K_Filt_01.tif');
 %retour
end