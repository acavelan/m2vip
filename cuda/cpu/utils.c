#include "utils.h"

void get_histogram(float *hist, unsigned char *pixels, int width, int height)
{
	unsigned int histcount[256];
	unsigned int pixelCount = width * height;
	
	memset(histcount, 0, 256 * sizeof(unsigned int));
	
	for(int i = 0; i < width * height; i++)
	{
		histcount[pixels[i]]++;
	}
	
	for(int i = 0; i < 256; i++)
	{
		hist[i] = (float)histcount[i] / pixelCount;
	}
}

void get_cumulative_histogram(float *cumhist, float *hist)
{
	for(int i = 0; i < 256; ++i)
	{
		cumhist[i] = 0;
		for(int j = 0; j < i; ++j)
		{
			cumhist[i] += hist[j];
		}
	}
}

void save_image(const char *filename, unsigned char *pixels, int width, int height)
{
	ILuint imageID = ilGenImage();
	ilBindImage(imageID);
	
	ilTexImage(width, height,
		0,  // 2D texture
		3,  // RGB
		IL_RGB,
		IL_UNSIGNED_BYTE,
		pixels
	);

	ilSave(IL_JPG, filename);
}

void save_gray_image(const char *filename, unsigned char *pixels, int width, int height)
{
	ILuint imageID = ilGenImage();
	ilBindImage(imageID);
	
	ilTexImage(width, height,
		0,  // 2D texture
		1,  // GRAY
		IL_LUMINANCE,
		IL_UNSIGNED_BYTE,
		pixels
	);

	ilSave(IL_JPG, filename);
}

void save_histogram(const char *filename, float *hist)
{
	FILE *fhisto = fopen(filename, "wb");
	fwrite(hist, sizeof(float), 256, fhisto);
	fclose(fhisto);
}

float get_time(struct timeval t1, struct timeval t2)
{
	return t2.tv_sec - t1.tv_sec + (float)(t2.tv_usec - t1.tv_usec) / 1000000;
}
