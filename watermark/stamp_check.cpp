#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdio>
#include <cmath>
#include <string.h>

using namespace sf;
using namespace std;

inline int clamp(int value, int min, int max)
{
	if(value < min)
		return 0;
	else if(value > max)
		return max;
	else
		return value;
}

void check_tattoo(const Image &image, const bool *bits)
{	
	int width = image.getSize().x;
	int height = image.getSize().y;
	
	int mI = 0, mIw = 0;
	/*
	 * E(I.m) = E(I)E(m) = 0
	 * 
	 * E(Iw.m) = E(I.m) + alpha * E(m^2)
	 * = 
	 * 
	 * Seuil = alpha/2
	 * 
	 * si C < alpha / 2 non marqué
	 */
	 
	int i = 0;
	int count = (width * height * 3);
	
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			Color ci = image.getPixel(x, y);
			
			int R = ci.r;
			int G = ci.g;
			int B = ci.b;
			
			int r = -1 + bits[i++] * 2;
			int g = -1 + bits[i++] * 2;
			int b = -1 + bits[i++] * 2;
			
			mIw += R * r;
			mIw += G * g;
			mIw += B * b;
			
			mI += (R - r) * r;
			mI += (G - g) * g;
			mI += (B - b) * b;
		}
	}
	
	printf("%d, %d\n", mIw, count);
	
	float fmI = (float)(mI) / count;
	float fmIw = (float)(mIw) / count;
	
	int alpha = floorf(fmIw + 0.5);
	
	printf("E(I) = %f,  E(Iw) = %f, a = %d\n", fmI, fmIw, alpha);
	
	if(alpha > 0)
		printf("Marked\n");
	else
		printf("Unmarked\n");
}

int main(int argc, char **argv)
{		
	if(argc < 3)
	{
		printf("usage: %s stamp mage\n", argv[0]);
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
