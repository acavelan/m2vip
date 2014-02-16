clear all;

file = './sample.bmp';

pkg load image;

img = imread(file);
#imgg = rgb2gray(img);

imgd = im2double(img);

# filter operators
canny = edge(imgd, "canny");
kirsch = edge(imgd, "kirsch", 1.5);
sobel = edge(imgd, "sobel", 0.45);
prewitt = edge(imgd, "prewitt", 0.3);
roberts = edge(imgd, "roberts", 0.15);

# Filtre median
imgmed = medfilt2(imgd);
median = abs(imgd - imgmed) * 5;

imwrite(canny, "results/origin/canny.jpg");
imwrite(kirsch, "results/origin/kirsch.jpg");
imwrite(sobel, "results/origin/sobel.jpg");
imwrite(prewitt, "results/origin/prewitt.jpg");
imwrite(roberts, "results/origin/roberts.jpg");

# Salt and Pepper noise from 0.01 to 0.1
for i=0.01:0.01:0.05,
	imgb = imnoise(img, 'salt & pepper', i);
	imgd = im2double(imgb);

	imwrite(imgd, strcat("results/saltAndPepper/noise_", num2str(i), ".jpg"));
	
	# filter operators
	canny = edge(imgd, "canny");
	kirsch = edge(imgd, "kirsch", 1.5);
	sobel = edge(imgd, "sobel", 0.45);
	prewitt = edge(imgd, "prewitt", 0.3);
	roberts = edge(imgd, "roberts", 0.15);

	imwrite(canny, strcat("results/saltAndPepper/canny", num2str(i), ".jpg"));
	imwrite(kirsch, strcat("results/saltAndPepper/kirsch", num2str(i), ".jpg"));
	imwrite(sobel, strcat("results/saltAndPepper/sobel", num2str(i), ".jpg"));
	imwrite(prewitt, strcat("results/saltAndPepper/prewitt", num2str(i), ".jpg"));
	imwrite(roberts, strcat("results/saltAndPepper/roberts", num2str(i), ".jpg"));
end

# Gaussian noise from 0.01 to 0.1
for i=0.002:0.002:0.01,
	imgb = imnoise(img, 'gaussian', 0, i);
	imgd = im2double(imgb);

	imwrite(imgd, strcat("results/gaussian/noise_", num2str(i), ".jpg"));
	
	# filter operators
	canny = edge(imgd, "canny");
	kirsch = edge(imgd, "kirsch", 1.5);
	sobel = edge(imgd, "sobel", 0.45);
	prewitt = edge(imgd, "prewitt", 0.3);
	roberts = edge(imgd, "roberts", 0.15);

	imwrite(canny, strcat("results/gaussian/canny", num2str(i), ".jpg"));
	imwrite(kirsch, strcat("results/gaussian/kirsch", num2str(i), ".jpg"));
	imwrite(sobel, strcat("results/gaussian/sobel", num2str(i), ".jpg"));
	imwrite(prewitt, strcat("results/gaussian/prewitt", num2str(i), ".jpg"));
	imwrite(roberts, strcat("results/gaussian/roberts", num2str(i), ".jpg"));
end 
