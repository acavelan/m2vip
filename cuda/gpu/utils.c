#include "utils.h"

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

float get_time(struct timeval t1, struct timeval t2)
{
	return t2.tv_sec - t1.tv_sec + (float)(t2.tv_usec - t1.tv_usec) / 1000000;
}
