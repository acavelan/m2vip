#include <math.h>

#include "fft_utils.h"

void fft(const float* in, float* reOut, float* imOut, unsigned int width, unsigned int height)
{
	fftw_complex* spatial_repr;
	fftw_complex* frequency_repr;
	unsigned int i;
	unsigned int j;
	fftw_plan plan;
	int x,y;

	spatial_repr= (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*width*height);
	frequency_repr= (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*width*height);

	for(i=0;i<width*height;i++)
	{
		spatial_repr[i][0] = in[i];
		spatial_repr[i][1] =  0.0f;
	}

	/*on calcule le plan d'exécution*/
	plan=fftw_plan_dft_2d(height, width, spatial_repr, frequency_repr, FFTW_FORWARD, FFTW_ESTIMATE);

	/*on calcule la transformée*/
	fftw_execute(plan);

	for(j=0;j<height;j++)
	for(i=0;i<width;i++)
	{
		/*on recentre l'image*/
		x=i;
		y=j;
		if (i<width/2 && j<height/2){ x=i+width/2; y=j+height/2; }
		if (i>=width/2 && j<height/2){ x=i-width/2; y=j+height/2; }
		if (i<width/2 && j>=height/2){ x=i+width/2; y=j-height/2; }
		if (i>=width/2 && j>=height/2){ x=i-width/2; y=j-height/2; }
		reOut[y*width+x]=frequency_repr[j*width+i][0];
		imOut[y*width+x]=frequency_repr[j*width+i][1];
	}

	fftw_destroy_plan(plan);
	fftw_free(spatial_repr);
	fftw_free(frequency_repr);
}

void ifft(const float* reIn, const float* imIn, float* out, unsigned int width, unsigned int height)
{
	fftw_complex* spatial_repr;
	fftw_complex* frequency_repr;
	unsigned int i;
	unsigned int j;
	int x,y;
	fftw_plan plan;

	spatial_repr= (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*width*height);
	frequency_repr= (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*width*height);

	for(j=0;j<height;j++)
	{
		for(i=0;i<width;i++)
		{
			/*on décentre*/
			x=i;
			y=j;
			if (i<width/2 && j<height/2){ x=i+width/2; y=j+height/2; }
			if (i>=width/2 && j<height/2){ x=i-width/2; y=j+height/2; }
			if (i<width/2 && j>=height/2){ x=i+width/2; y=j-height/2; }
			if (i>=width/2 && j>=height/2){ x=i-width/2; y=j-height/2; }
			frequency_repr[j*width+i][0]=reIn[y*width+x];
			frequency_repr[j*width+i][1]=imIn[y*width+x];
		}
	}

	plan=fftw_plan_dft_2d(height, width, frequency_repr, spatial_repr, FFTW_BACKWARD, FFTW_ESTIMATE);

	fftw_execute(plan);

	/*on retranscrit l'image complexe en image réelle, sans oublier de diviser par width*height*/
	for(i=0;i<width*height;i++)
	{
		out[i]=spatial_repr[i][0]/(width*height);
	}

	fftw_destroy_plan(plan);
	fftw_free(spatial_repr);
	fftw_free(frequency_repr);
}

void fI2MP(const float* reIn, const float* imIn, float* moduleOut, float* phaseOut, unsigned int width, unsigned int height)
{
	for(unsigned int i = 0 ; i < height * width;  i++)
	{
		moduleOut[i] = sqrt(reIn[i]*reIn[i] + imIn[i]*imIn[i]); /*on applique la formule de base*/
		if (reIn[i]!= 0)
			phaseOut[i] = atan2(imIn[i],reIn[i]);
	}
}

void fMP2I(const float* moduleIn, const float* phaseIn, float* reOut, float* imOut, unsigned int width, unsigned int height)
{
	for(unsigned int i = 0; i < width * height; i++)
	{
		reOut[i]=moduleIn[i]*cos(phaseIn[i]);
		imOut[i]=moduleIn[i]*sin(phaseIn[i]);
	}
}

float distance(float x1, float y1, float x2, float y2)
{
    return sqrt( square(x1 - x2) + square(y1 - y2) );
}
