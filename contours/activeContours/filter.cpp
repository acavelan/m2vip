#include "filter.hpp"

#include <cmath>

void apply_sobel(const unsigned char *input, unsigned char *output, int width, int height)
{
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			#define PX(kx) (kx + x)
			#define PY(ky) width * (ky + y)
			
			int gx = 0, gy = 0;
			
			gx += input[PX(-1) + PY(-1)];
			gx += 2 * input[PX(-1) + PY(0)];
			gx += input[PX(-1) + PY(1)];
			gx -= input[PX(1) + PY(-1)];
			gx -= 2 * input[PX(1) + PY(0)];
			gx -= input[PX(1) + PY(1)];
			
			gy += input[PX(-1) + PY(-1)];
			gy += 2 * input[PX(0) + PY(-1)];
			gy += input[PX(1) + PY(-1)];
			gy -= input[PX(-1) + PY(1)];
			gy -= 2 * input[PX(0) + PY(1)];
			gy -= input[PX(1) + PY(1)];
			
			double grad = sqrt(gx * gx + gy * gy);
			output[y * width + x] = grad > 255 ? 255 : grad;
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
