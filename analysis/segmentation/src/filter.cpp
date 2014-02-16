#include "filter.hpp"
#include <cstdio>
#include <cmath>

using namespace sf;

inline unsigned char getVal(const unsigned char *image, int width, int height, int x, int y)
{
	if(y < 0 || y >= height || x < 0 || x >= width)
		return 0;
	else
		return image[y * width + x];
}

inline unsigned char clamp(float value, int min, int max)
{
	if(value < min)
		return 0;
	else if(value > max)
		return max;
	else
		return value;
}

void apply_convolution(const char *kernel, int kwidth, int kheight, const unsigned char *input, unsigned char *output, int height, int width)
{	
	for(int y=1; y<height-1; y++)
	{
		for(int x=1; x<width-1; x++)
		{
			float color = 0;
			for(int ky=0; ky<kheight; ky++)
			{
				for(int kx=0; kx<kwidth; kx++)
				{
					unsigned char p = getVal(input, width, height, -1 + x + kx, -1 + y + ky);
					color += kernel[ky * 3 + kx] * p;
				}
			}
			output[y * width + x] = clamp(color, 0, 255);
		}
	}
}

void apply_grayscale(const unsigned char *input, unsigned char *output, int width, int height)
{	
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			unsigned char R = input[y * width * 3 + x + 0];
			unsigned char G = input[y * width * 3 + x + 1];
			unsigned char B = input[y * width * 3 + x + 2];
			
			unsigned char gray = (307 * R + 604 * G + 113 * B) >> 10;
			
			output[y*width+x] = gray;
		}
	}
}

void apply_sobel(unsigned char *input, unsigned char *output, int width, int height)
{
	const char Gx[3 * 3] = 
	{
		1, 0, -1,
		2, 0, -2,
		1, 0, -1
	};
	
	const char Gy[3 * 3] = 
	{
		1, 2, 1,
		0, 0, 0,
		-1, -2, -1
	};
	
	unsigned char *sobelx = new unsigned char[width * height];
	unsigned char *sobely = new unsigned char[width * height];
	
	apply_convolution(Gx, 3, 3, input, sobelx, width, height);
	apply_convolution(Gy, 3, 3, input, sobely, width, height);
	
	for(int i=0; i<width*height; i++)
	{
		float grad = sqrt(sobelx[i] * sobelx[i] + sobely[i] * sobely[i]);
		grad = grad > 255 ? 255: grad;
		output[i] = grad;
	}
	
	delete[] sobelx;
	delete[] sobely;
}
