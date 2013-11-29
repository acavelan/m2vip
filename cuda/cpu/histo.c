#include <sys/time.h>

#include "utils.h"

inline unsigned char clamp(double value, int min, int max)
{
	if(value < min)
		return 0;
	else if(value > max)
		return max;
	else
		return value;
}

int main(int argc, char **argv)
{
	struct timeval start, last, now, computation;
	
	gettimeofday(&start, 0);
	
	if(argc < 2)
	{
		printf("usage: %s image\n", argv[0]);
		return 0;
	}
	
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilEnable(IL_FILE_OVERWRITE);
	
	ILboolean result = ilLoadImage(argv[1]);

	if(!result)
	{
		ILenum err = ilGetError() ;
		printf("Failed to load %s\n", argv[1]);
		printf("Error: %s\n", ilGetString(err));
	}

	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
		
	ILuint width = ilGetInteger(IL_IMAGE_WIDTH);
	ILuint height = ilGetInteger(IL_IMAGE_HEIGHT);
	
	unsigned int pixelCount = width * height;
	
	ILubyte * pixels = ilGetData();
	
	gettimeofday(&now, 0);
	computation = last = now;
	
	printf("Image (%d * %d) loaded in %f\n", width, height, get_time(start, now));
	
	// RGB IMAGE EQUALIZATION
	const float KB = 0.0593;
	const float KR = 0.2627;
	const float KG = 1 - KB - KR;
	
	//RGB to YCBCR
	float *ycbcrimg = (float*)malloc(pixelCount * 3 * sizeof(float));
	unsigned char *yimg = (unsigned char*)malloc(pixelCount * sizeof(unsigned char));
	
	for (unsigned int y = 0; y < height; y++)
	{
	   for (unsigned int x = 0; x < width; x++)
	   {
		  unsigned char R = pixels[(y * width + x)*3 + 0];
		  unsigned char G = pixels[(y * width + x)*3 + 1];
		  unsigned char B = pixels[(y * width + x)*3 + 2]; 
		  
		  float Y = KR * R + (1 - KR - KB) * G + KB * B;
		  float CB = (B - Y) / (1 + KR + KG - KB);
		  float CR = (R - Y) / (1 - KR + KG + KB);
		  
		  yimg[y * width + x] = clamp(Y, 0, 255);

		  ycbcrimg[(y * width + x)*3 + 0] = Y;
		  ycbcrimg[(y * width + x)*3 + 1] = CB;
		  ycbcrimg[(y * width + x)*3 + 2] = CR;
	   }
	}
	
	// Histogram
	float yhist[256], ycumhist[256];
	get_histogram(yhist, yimg, width, height);
	get_cumulative_histogram(ycumhist, yhist);
	
	// Equalization
	double ylut[256], ysum = 0;
	for(unsigned int i = 0; i < 256; ++i)
	{
		ysum += yhist[i];
		ylut[i] = (ysum * 255);
	}
	
	// YCBCR to RGB
	unsigned char *eqrgbimg = (unsigned char*)malloc(pixelCount * 3 * sizeof(unsigned char));
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			float Y = ylut[yimg[y * width + x]];
			float CB = ycbcrimg[(y * width + x) * 3 + 1];
			float CR = ycbcrimg[(y * width + x) * 3 + 2];
			
			unsigned char R = clamp((Y + CR * (1 - KR + KG + KB)), 0, 255);
			unsigned char B = clamp((Y + CB * (1 + KR + KG - KB)), 0, 255);
			unsigned char G = clamp((Y - KR * R - KB * B) / (1 - KB - KR), 0, 255);
			
			yimg[y * width + x] = Y;
			eqrgbimg[(y * width + x) * 3 + 0] = R;
			eqrgbimg[(y * width + x) * 3 + 1] = G;
			eqrgbimg[(y * width + x) * 3 + 2] = B;
		}
	}
	
	gettimeofday(&now, 0);
	printf("Equalization completed in %f\n", get_time(computation, now));
	last = now;
	
	float eqyhist[256], eqycumhist[256];
	get_histogram(eqyhist, yimg, width, height);
	get_cumulative_histogram(eqycumhist, eqyhist);
	
	free(yimg);
	free(ycbcrimg);
	
	// Save images
	save_image("rgbequalized.jpg", eqrgbimg, width, height);
	free(eqrgbimg);
	
	gettimeofday(&now, 0);
	printf("Result saved in %f\n", get_time(last, now));
	printf("Total time %f\n", get_time(start, now));
	
	// Save histograms
	save_histogram("rgb_histogram.bin", yhist);
	save_histogram("rgb_cumulative_histogram.bin", ycumhist);
	
	save_histogram("equalized_rgb_histogram.bin", eqyhist);
	save_histogram("equalized_rgb_cumulative_histogram.bin", eqycumhist);

	return 0;
}
