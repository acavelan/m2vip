#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <IL/il.h>

void save_image(const char *filename, unsigned char *pixels, int width, int height);

void save_gray_image(const char *filename, unsigned char *pixels, int width, int height);

float get_time(struct timeval t1, struct timeval t2);

#endif
