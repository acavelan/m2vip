function y = Quant_Im(x,N)
%% Quant_Im.m
% quantize an image and return resultant image 
% N : bits per pixel
% x : is a matrix
% copyright : 2010 by houlotfi@yahoo.fr
%%
if isvector(x)
    error('Warning: x must be a matrix')
end 
x = double(x); 
a = min(min(x));
b = max(max(x));
%reducing the image to the [0, 1] range
Rd = (x-a)/(b-a);
%scaling the result up to the [0, 2^N – 1] range
Sc = (2^N)-1;
%backing up to the [0 255] range
%C = ((b-a)/B) + a;
% y = uint16(Rd*Sc);
y = floor(Rd*Sc);
%y = im2uint8(y);
end
