#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdio>
#include <cmath>
#include <string.h>

using namespace sf;
using namespace std;

inline int clamp(int value, int min, int max)
{
	if(value < min)
		return 0;
	else if(value > max)
		return max;
	else
		return value;
}

void apply_tattoo(const Image &input, Image &output, int alpha)
{	
	int width = input.getSize().x;
	int height = input.getSize().y;
	
	output = input;
		
	bool *bits = new bool[width * height * 3];
	
	srand(time(0));
	
	for(int i=0; i<width*height*3; i++)
		bits[i] = rand() % 2;

	int i = 0;
	int db = 0;
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			Color ci = input.getPixel(x, y);
			
			int r = clamp(ci.r + alpha * (-1 + bits[i++] * 2), 0, 255);
			int g = clamp(ci.g + alpha * (-1 + bits[i++] * 2), 0, 255);
			int b = clamp(ci.b + alpha * (-1 + bits[i++] * 2), 0, 255);

			db += ci.r * ci.r;
			db += ci.g * ci.g;
			db += ci.b * ci.b;
			
			output.setPixel(x, y, Color(r, g, b));
		}
	}
	
	float count = (width * height * 3);
	
	double fdb = db / (count);
	printf("%f\n", fdb);
	float SNR = 10 * log10(500 * fdb / (alpha * alpha));
	
	printf("SNR = %f\n", SNR);
	
	FILE *mark = fopen("stamp.bin", "wb");
	fwrite(bits, sizeof(bool), width * height * 3, mark);
	fclose(mark);
}

int main(int argc, char **argv)
{		
	if(argc < 3)
	{
		printf("usage: %s image alpha\n", argv[0]);
		return 0;
	}
	
	Image image, tattoo;
	if(!image.loadFromFile(argv[1]))
	{
		printf("unable to load %s\n", argv[1]);
		return 0;
	}
	 
	apply_tattoo(image, tattoo, atoi(argv[2]));
	
	Texture tImage, tTattoo;
	tImage.loadFromImage(image);
	tTattoo.loadFromImage(tattoo);
	
	Sprite sImage, sTattoo;
	sImage.setTexture(tImage);
	sTattoo.setTexture(tTattoo);
	sTattoo.setPosition(image.getSize().x, 0);

	RenderWindow window(sf::VideoMode(image.getSize().x * 2, image.getSize().y), "Tattoo");
	window.setFramerateLimit(60);
	
    while(window.isOpen())
    {
		window.clear(Color(255, 255, 255));
		
		window.draw(sImage);
		window.draw(sTattoo);
		
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
	
	tattoo.saveToFile("tattooed.bmp");
	
    return EXIT_SUCCESS;
}
