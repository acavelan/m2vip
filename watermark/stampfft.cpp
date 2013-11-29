#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdio>
#include <cmath>
#include <string.h>

#include "fft_utils.h"

using namespace sf;
using namespace std;

void apply_tattoo(const Image &input, Image &outFFT, Image &outImg, int alpha)
{		
	int width = input.getSize().x;
	int height = input.getSize().y;
	
	outFFT = input;
	outImg = input;
	
	srand(time(0));
	
	bool *bits = new bool[width * height * 3];
	
	for(int i=0; i<width*height * 3; i++)
		bits[i] = rand() % 2;
	
	// FFT Forward
	float *tmp = new float[width * height * 3];
	float *re = new float[width * height * 3];
	float *im = new float[width * height * 3];
	float *mo = new float[width * height * 3];
	float *pha = new float[width * height * 3];
	
	#define RED 0
	#define GREEN width*height
	#define BLUE width*height*2
	
	double mean_origin = 0.0f;
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			int R = input.getPixel(x, y).r;
			int G = input.getPixel(x, y).g;
			int B = input.getPixel(x, y).b;
			
			tmp[RED + y * width + x] = R;
			tmp[GREEN + y * width + x] = G;
			tmp[BLUE + y * width + x] = B;
			
			mean_origin += R + G + B;
		}
	}
	
	mean_origin /= (width * height);
	
	double var_origin = 0.0f;
	
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			int R = input.getPixel(x, y).r;
			int G = input.getPixel(x, y).g;
			int B = input.getPixel(x, y).b;
			
			double tmp = R + G + B - mean_origin;
			var_origin += square(tmp);
		}
	}
	
	fft(&tmp[RED], &re[RED], &im[RED], width, height);
	fft(&tmp[GREEN], &re[GREEN], &im[GREEN], width, height);
	fft(&tmp[BLUE], &re[BLUE], &im[BLUE], width, height);
	
	// Marking
	int ox = width / 2;
	int oy = height / 2;
	int r = min(width, height) / 4;
	int l = 30;
	
	printf("Circle = x=%d, y=%d, r=%d\n", ox, oy, r);

	int i = 0;
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			float d = distance(x, y, ox, oy);
			if(d < r && r - d < l)
			{
				im[RED + y * width + x] = im[RED + y * width + x] + alpha * (-1 + bits[i++] * 2);
				im[GREEN + y * width + x] = im[GREEN + y * width + x] + alpha * (-1 + bits[i++] * 2);
				im[BLUE + y * width + x] = im[BLUE + y * width + x] + alpha * (-1 + bits[i++] * 2);
			}
		}
	}
	
	fI2MP(&re[RED], &im[RED], &mo[RED], &pha[RED], width, height);
	fI2MP(&re[GREEN], &im[GREEN], &mo[GREEN], &pha[GREEN], width, height);
	fI2MP(&re[BLUE], &im[BLUE], &mo[BLUE], &pha[BLUE], width, height);
	
	// Processing
	float maxr = 0.0f, maxg = 0.0f, maxb = 0.0f;
	for(int y = 0; y<height; y++)
	{
		for(int x = 0; x<width; x++)
		{
			if(maxr < mo[RED + y * width + x]) maxr = mo[RED + y * width + x];
			if(maxg < mo[GREEN + y * width + x + 1]) maxg = mo[GREEN + y * width + x + 1];
			if(maxb < mo[BLUE + y * width + x + 2]) maxb = mo[BLUE + y * width + x + 2];
		}
	}
	
	// Scaling for display
	float fmaxr = 0.0f, fmaxg = 0.0f, fmaxb = 0.0f;
	float ratio = 0.1f * width;
	float facteur = 1.0f;
	
	for(int y = 0; y<height; y++)
	{
		for(int x = 0; x<width; x++)
		{
			if(fmaxr < mo[RED + y * width + x] && (square((x-width/2)) + square((y-height/2)) ) > square(ratio)) fmaxr = mo[RED + y * width + x];
			if(fmaxg < mo[GREEN + y * width + x] && (square((x-width/2)) + square((y-height/2)) ) > square(ratio)) fmaxg = mo[GREEN + y * width + x];
			if(fmaxb < mo[BLUE + y * width + x] && (square((x-width/2)) + square((y-height/2)) ) > square(ratio)) fmaxb = mo[BLUE + y * width + x];
		}
	}
	
	if(fmaxr < 1.0f) fmaxr = 1.0f;
	if(fmaxg < 1.0f) fmaxg = 1.0f;
	if(fmaxb < 1.0f) fmaxb = 1.0f;
	
	printf("max = %f / %f\n", fmaxr, maxr);
	printf("max = %f / %f\n", fmaxg, maxg);
	printf("max = %f / %f\n", fmaxb, maxb);
	
	float scaler = 255 * facteur / fmaxr;
	float scaleg = 255 * facteur / fmaxg;
	float scaleb = 255 * facteur / fmaxb;
	
	for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
			outFFT.setPixel(x, y, Color(clamp(mo[RED + y * width + x] * scaler, 0, 255), clamp(mo[GREEN + y * width + x] * scaleg, 0, 255), clamp(mo[BLUE + y * width + x] * scaleb, 0, 255)));
	
	// FFT Backward
	//fMP2I(&mo[RED], &pha[RED], &re[RED], &im[RED], width, height);
	//fMP2I(&mo[GREEN], &pha[GREEN], &re[GREEN], &im[GREEN], width, height);
	//fMP2I(&mo[BLUE], &pha[BLUE], &re[BLUE], &im[BLUE], width, height);
	
	ifft(&re[RED], &im[RED], &tmp[RED], width, height);
	ifft(&re[GREEN], &im[GREEN], &tmp[GREEN], width, height);
	ifft(&re[BLUE], &im[BLUE], &tmp[BLUE], width, height);
	
	double mean_noise = 0.0f;
	
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			int oR = input.getPixel(x, y).r;
			int oG = input.getPixel(x, y).g;
			int oB = input.getPixel(x, y).b;
			
			int R = clamp(tmp[RED + y * width + x], 0, 255);
			int G = clamp(tmp[GREEN + y * width + x], 0, 255);
			int B = clamp(tmp[BLUE + y * width + x], 0, 255);
			
			double noise = R + G + B - oR - oG - oB;
			
			mean_noise += noise;
			
			outImg.setPixel(x, y, Color(R, G, B));
		}
	}
	
	mean_noise /= (width * height);
	
	double var_noise = 0.0f;
	
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			int oR = input.getPixel(x, y).r;
			int oG = input.getPixel(x, y).g;
			int oB = input.getPixel(x, y).b;
			
			int R = clamp(tmp[RED + y * width + x], 0, 255);
			int G = clamp(tmp[GREEN + y * width + x], 0, 255);
			int B = clamp(tmp[BLUE + y * width + x], 0, 255);
			
			double noise = R + G + B - oR - oG - oB;
			
			double tmp = noise - mean_noise;
			
			var_noise += square(tmp);
		}
	}
	
	double SNR = 10 * log10(var_origin / var_noise);
	
	printf("SNR = %f\n", SNR);
	
	delete[] tmp;
	delete[] re;
	delete[] im;
	delete[] mo;
	delete[] pha;
		
	FILE *mark = fopen("stamp.bin", "wb");
	fwrite(bits, sizeof(bool), width * height * 3, mark);
	fclose(mark);
	
	delete[] bits;
}

int main(int argc, char **argv)
{		
	if(argc < 3)
	{
		printf("usage: %s image alpha\n", argv[0]);
		return 0;
	}
	
	Image image, fft, tattoo;
	if(!image.loadFromFile(argv[1]))
	{
		printf("unable to load %s\n", argv[1]);
		return 0;
	}
	
	apply_tattoo(image, fft, tattoo, atoi(argv[2]));
	
	Texture tImage, tFFT, tTattoo;
	tImage.loadFromImage(image);
	tFFT.loadFromImage(fft);
	tTattoo.loadFromImage(tattoo);
	
	Sprite sImage, sFFT, sTattoo;
	sImage.setTexture(tImage);
	sFFT.setTexture(tFFT);
	sTattoo.setTexture(tTattoo);
	sTattoo.setPosition(512, 0);
	
	int sx = tImage.getSize().x;
	int sy = tImage.getSize().y;
	
	float ratio = (float)512 / sx;
	
	sImage.setScale(ratio, ratio);
	sFFT.setScale(ratio, ratio);
	sTattoo.setScale(ratio, ratio);
	
	RenderWindow window(sf::VideoMode(512 * 2, (sy * ratio)), "Tattoo");
	window.setFramerateLimit(60);
	
    while(window.isOpen())
    {
		window.clear(Color(255, 255, 255));
		
		//window.draw(sImage);
		window.draw(sFFT);
		window.draw(sTattoo);
		
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::KeyPressed)
            {
				if(event.key.code == sf::Keyboard::Escape)
					window.close();
			}
        }
        
        window.display();
    }
	
	tattoo.saveToFile("tattooed.bmp");
	
    return EXIT_SUCCESS;
}
