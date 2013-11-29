#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdio>
#include <cmath>
#include <string.h>

#include "filter.hpp"

using namespace sf;
using namespace std;

void tosfImage(unsigned char *input, Image &output, int width, int height)
{
	output.create(width, height);
	
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			unsigned char c = input[y * width + x];
			output.setPixel(x, y, Color(c, c, c));
		}
	}
}

void filter_gray(const Image &origin, unsigned char *gray)
{	
	int width = origin.getSize().x;
	int height = origin.getSize().y;
	
	unsigned char *input = new unsigned char[width * height * 3];
	
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			input[y * width * 3 + x + 0] = origin.getPixel(x, y).r;
			input[y * width * 3 + x + 1] = origin.getPixel(x, y).g;
			input[y * width * 3 + x + 2] = origin.getPixel(x, y).b;
		}
	}
	
	apply_grayscale(input, gray, width, height);
	
	delete[] input;
}

int main(int argc, char **argv)
{
	if(argc < 2)
		printf("usage: %s image\n", argv[0]);
	
	Image sfImage, sfGray, sfSobel;
	if(!sfImage.loadFromFile(argv[1]))
	{
		printf("unable to load %s\n", argv[1]);
		return 0;
	}
	
	int width = sfImage.getSize().x;
	int height = sfImage.getSize().y;
	
	// Processing
	unsigned char *gray = new unsigned char[width * height];
	unsigned char *sobel = new unsigned char[width * height];
	
	filter_gray(sfImage, gray);
	
	apply_sobel(gray, sobel, width, height);
	
	tosfImage(gray, sfGray, width, height);
	tosfImage(sobel, sfSobel, width, height);
	
	delete[] gray;
	delete[] sobel;
	
	Texture tgray, tsobel;
	tgray.loadFromImage(sfGray);
	tsobel.loadFromImage(sfSobel);
	
	Sprite sgray, ssobel;
	sgray.setTexture(tgray);
	ssobel.setTexture(tsobel);
	ssobel.setPosition(Vector2f(sfGray.getSize().x, 0));

	RenderWindow window(sf::VideoMode(sfGray.getSize().x * 2, sfGray.getSize().y), "Segmentation");
	window.setFramerateLimit(60);
	
    while(window.isOpen())
    {
		window.clear();
		
		window.draw(sgray);
		window.draw(ssobel);
		
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::KeyPressed)
            {
				if(event.key.code == sf::Keyboard::Escape)
					window.close();
			}
        }
        
        window.display();
    }

    return EXIT_SUCCESS;
}
