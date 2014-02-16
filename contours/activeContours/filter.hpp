#ifndef FILTER_H
#define FILTER_H

void apply_sobel(const unsigned char *input, unsigned char *output, int width, int height);

void apply_grayscale(const unsigned char *input, unsigned char *output, int width, int height);

#endif
