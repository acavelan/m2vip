#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdio>
#include <cmath>
#include <string.h>

#include "fft_utils.h"

using namespace sf;
using namespace std;

void check_tattoo(const Image &image, const bool *bits)
{	
	int width = image.getSize().x;
	int height = image.getSize().y;
	
	// FFT Forward
	float *tmp = new float[width * height * 3];
	float *re = new float[width * height * 3];
	float *im = new float[width * height * 3];
	
	#define RED 0
	#define GREEN width*height
	#define BLUE width*height*2
	
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			tmp[RED + y * width + x] = image.getPixel(x, y).r;
			tmp[GREEN + y * width + x] = image.getPixel(x, y).g;
			tmp[BLUE + y * width + x] = image.getPixel(x, y).b;
		}
	}
	
	fft(&tmp[RED], &re[RED], &im[RED], width, height);
	fft(&tmp[GREEN], &re[GREEN], &im[GREEN], width, height);
	fft(&tmp[BLUE], &re[BLUE], &im[BLUE], width, height);
	
	//fI2MP(&re[RED], &im[RED], &mo[RED], &pha[RED], width, height);
	//fI2MP(&re[GREEN], &im[GREEN], &mo[GREEN], &pha[GREEN], width, height);
	//fI2MP(&re[BLUE], &im[BLUE], &mo[BLUE], &pha[BLUE], width, height);
	
	// Processing
	double mIw = 0.0f;
	int count = 0;
	
	int ox = width / 2;
	int oy = height / 2;
	int r = min(width, height) / 4;
	int l = 30;
	
	int i = 0;
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{				
			float d = distance(x, y, ox, oy);
			if(d < r && r - d < l)
			{				
				float R = im[RED + y * width + x];
				float G = im[GREEN + y * width + x];
				float B = im[BLUE + y * width + x];
			
				int r = -1 + bits[i++] * 2;
				int g = -1 + bits[i++] * 2;
				int b = -1 + bits[i++] * 2;
						
				mIw += R * r;
				mIw += G * g;
				mIw += B * b;
				
				count += 3;
			}
		}
	}
	
	printf("%f, %d\n", mIw, count);
	
	double fmIw = (float)mIw / count;
	
	int alpha = fmIw * 2;
	
	printf("a = %d\n", alpha);
	
	if(abs(alpha) > 500)
		printf("Marked\n");
	else
		printf("Unmarked\n");
	
	delete[] tmp;
	delete[] re;
	delete[] im;
}

int main(int argc, char **argv)
{		
	if(argc < 3)
	{
		printf("usage: %s stamp image\n", argv[0]);
		return 0;
	}
	
	Image image;
	if(!image.loadFromFile(argv[2]))
	{
		printf("unable to load %s\n", argv[2]);
		return 0;
	}
	
	bool *bits = 0;
	int width = image.getSize().x;
	int height = image.getSize().y;
	
	FILE *stamp = fopen(argv[1], "rb");
	if(stamp == 0)
	{
		printf("Unable to load %s\n", argv[1]);
		return 0;
	}
	else
	{
		bits = new bool[width * height * 3];
		fread((void*)bits, sizeof(bool), width * height * 3, stamp);
		fclose(stamp);
	}
	
	check_tattoo(image, bits);
   
	delete[] bits;
	
    return 0;
}
