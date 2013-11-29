#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdio>
#include <cmath>
#include <string.h>

using namespace sf;
using namespace std;

void apply_tattoo(const Image &image)
{	
	int width = input.getSize().x;
	int height = input.getSize().y;
		
	bool *bits = new bool[size * 8 + 24];
	
	for(int i=0; i<24; i++)
	{
		bits[i] = size % 2;
		size /= 2;
	}
	
	for(int i=24, ci=0; ci<size; ci++)
	{
		char c = str[ci];
		
		for(int j=0; j<8; j++)
		{
			bits[i] = c % 2;
			c /= 2;
		}
	}
	
	int i = 0;
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			if(i+3 < size * 8 + 24)
			{
				Color ci = input.getPixel(x, y);
				
				int r = (ci.r & 0xFE) + bits[i++];
				int g = (ci.g & 0xFE) + bits[i++];
				int b = (ci.b & 0xFE) + bits[i++];
				
				Color co(r, g, b);
				output.setPixel(x, y, co);
			}
		}
	}
}

int main(int argc, char **argv)
{		
	if(argc < 2)
		printf("usage: %s image\n", argv[0]);
	
	Image image;
	if(!image.loadFromFile(argv[1]))
	{
		printf("unable to load %s\n", argv[1]);
		return 0;
	}
	 
	read_tattoo(image, "coucou");

    return EXIT_SUCCESS;
}
