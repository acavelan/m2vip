#include "utils.h"

int main(int argc, char **argv)
{
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
	
	//printf("Image (%d * %d) loaded in %f\n", width, height, timer.tick());
	
	// Gray scale
	unsigned char *grayimg = (unsigned char*)malloc(pixelCount * sizeof(unsigned char));
	for (unsigned int y = 0; y < height; y++)
	{
	   for (unsigned int x = 0; x < width; x++)
	   {
		  const unsigned char R = pixels[(y * width + x)*3 + 0];
		  const unsigned char G = pixels[(y * width + x)*3 + 1];
		  const unsigned char B = pixels[(y * width + x)*3 + 2]; 
		  
		  unsigned char gray = (307 * R + 604 * G + 113 * B) >> 10;
		  
		  grayimg[y * width + x] = gray;
	   }
	}
	
	// Histogram
	float hist[256];
	get_histogram(hist, grayimg, width, height);
	
	// Histogram Equalization		
	double lut[256], sum = 0;
	for (unsigned int i = 0; i < 256; ++i)
	{
		sum += hist[i];
		lut[i] = (sum * 255);
	}

	// Transform image using sum histogram as a LUT
	unsigned char *eqimg = (unsigned char*)malloc(pixelCount * sizeof(unsigned char));
	for (unsigned int i=0; i<pixelCount; ++i)
	{
		eqimg[i] = lut[grayimg[i]];
	}
	
	//printf("Computation done in %f\n", timer.tick());
	
	float cumhist[256], eqhist[256], eqcumhist[256];
	get_histogram(eqhist, eqimg, width, height);
	get_cumulative_histogram(cumhist, hist);
	get_cumulative_histogram(eqcumhist, eqhist);
	
	// Save result
	save_gray_image("gray.jpg", grayimg, width, height);
	free(grayimg);

	save_gray_image("equalized.jpg", eqimg, width, height);
	free(eqimg);
	
	// Save histograms
	save_histogram("histogram.bin", hist);
	save_histogram("equalized_histogram.bin", eqhist);
	
	save_histogram("cumulative_histogram.bin", cumhist);
	save_histogram("cumulative_equalized_histogram.bin", eqcumhist);
	
	//printf("Result saved in %f\n", timer.tick());

	return 0;
}
