#ifndef FILTER_H
#define FILTER_H

#include <SFML/Graphics.hpp>

// Generic
void apply_grayscale(const unsigned char *input, unsigned char *output, int width, int height);

void apply_convolution(const char *kernel, int kwidth, int kheight, const unsigned char *input, unsigned char *output, int width, int height);


// Operators
void apply_sobel(unsigned char *input, unsigned char *output, int width, int height);

#endif
