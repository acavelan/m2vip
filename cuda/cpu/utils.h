#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <IL/il.h>

void get_histogram(float *hist, unsigned char *pixels, int width, int height);

void get_cumulative_histogram(float *cumhist, float *hist);

void save_image(const char *filename, unsigned char *pixels, int width, int height);

void save_gray_image(const char *filename, unsigned char *pixels, int width, int height);

void save_histogram(const char *filename, float *hist);

void kernelNormalize(float *kernel, int rows, int cols);

void apply_convolution(float *kernel, int krows, int kcols, unsigned char *input, unsigned char *output, int rows, int cols);

float get_time(struct timeval t1, struct timeval t2);

#endif
