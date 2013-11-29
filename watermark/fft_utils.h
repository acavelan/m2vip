#ifndef FFT_UTILS_H
#define FFT_UTILS_H

#include <fftw3.h>

void fft(const float* in, float* reOut, float* imOut, unsigned int width, unsigned int height);

void ifft(const float* reIn, const float* imIn, float* out, unsigned int width, unsigned int height);

void fI2MP(const float* reIn, const float* imIn, float* moduleOut, float* phaseOut, unsigned int width, unsigned int height);
			
void fMP2I(const float* moduleIn, const float* phaseIn, float* reOut, float* imOut, unsigned int width, unsigned int height);

float distance(float x1, float y1, float x2, float y2);

inline double square(double a)
{
	return (a*a);
}

inline float clamp(float value, float min, float max)
{
	if(value < min)
		return 0;
	else if(value > max)
		return max;
	else
		return value;
}
	
#endif
