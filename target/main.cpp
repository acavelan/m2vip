#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdio>
#include <cmath>
#include <string.h>

#include "Target.hpp"

using namespace sf;
using namespace std;

void find(const Image &model, const Image &image, std::vector<object> &base, std::vector<object> &result)
{	
	int mwidth = model.getSize().x;
	int mheight = model.getSize().y;
	
	unsigned char *char_model = new unsigned char[mwidth * mheight];
	
	for(int y=0; y<mheight; y++)
	{
		for(int x=0; x<mwidth; x++)
		{
			unsigned char R, G, B, gray;
			
			R = model.getPixel(x, y).r;
			G = model.getPixel(x, y).g;
			B = model.getPixel(x, y).b;
			
			gray = (307 * R + 604 * G + 113 * B) >> 10;
			
			char_model[y * mwidth + x] = gray;
		}
	}
	
	int iwidth = image.getSize().x;
	int iheight = image.getSize().y;
	
	unsigned char *char_image = new unsigned char[iwidth * iheight];
	
	for(int y=0; y<iheight; y++)
	{
		for(int x=0; x<iwidth; x++)
		{
			unsigned char R, G, B, gray;

			R = image.getPixel(x, y).r;
			G = image.getPixel(x, y).g;
			B = image.getPixel(x, y).b;
			
			gray = (307 * R + 604 * G + 113 * B) >> 10;
			
			char_image[y * iwidth + x] = gray;
		}
	}
	
	Target target(new ChanVeseDetector(), new SimpleMatcher());
	base = target.setModel(char_model, mwidth, mheight);
	result = target.findModel(char_image, iwidth, iheight);
	
	delete[] char_model;
	delete[] char_image;
}

int main(int argc, char **argv)
{
	if(argc < 3)
		printf("usage: %s model image\n", argv[0]);
	
	Image model, image; 
	if(!model.loadFromFile(argv[1]))
	{
		printf("unable to load %s\n", argv[1]);
		return 0;
	}
	
	if(!image.loadFromFile(argv[2]))
	{
		printf("unable to load %s\n", argv[2]);
		return 0;
	}
	
	std::vector<object> base, results;
	
	find(model, image, base, results);
	
	Texture tModel, tImage;
	tModel.loadFromImage(model);
	tImage.loadFromImage(image);
	
	Sprite sModel, sImage;
	sModel.setTexture(tModel);
	sImage.setTexture(tImage);

	int sx = tModel.getSize().x;
	int sy = tModel.getSize().y;
	
	float mratio = glm::min((float)128 / sx, (float)512 / sy);
	
	int mw = sx * mratio;
	int my = sy * mratio;
	
	sModel.setScale(mratio, mratio);
	
	sx = tImage.getSize().x;
	sy = tImage.getSize().y;
	
	float iratio = glm::min((float)1152 / sx, (float)512 / sy);
	
	int iw = sx * iratio;
	int iy = sy * iratio;
	
	sImage.setScale(iratio, iratio);
	sImage.setPosition(mw, 0);
	
	RenderWindow window(sf::VideoMode(mw + iw, glm::max(my, iy)), "Target lock");
	window.setFramerateLimit(60);
	
    while(window.isOpen())
    {
		window.clear();
		
		window.draw(sModel);
		window.draw(sImage);
		
		// Draw base
		for(unsigned int i=0; i<base.size(); i++)
		{
			for(unsigned int j=0; j<base[i].vertices.size(); j++)
			{
				CircleShape circle(2);
				circle.setFillColor(Color(255, 0, 0));
				circle.setPosition(Vector2f(base[i].vertices[j].x * mratio - 1, base[i].vertices[j].y * mratio - 1));
				window.draw(circle);
			}
		}
		
		// Draw result
		for(unsigned int i=0; i<results.size(); i++)
		{
			for(unsigned int j=0; j<results[i].vertices.size(); j++)
			{
				CircleShape circle(2);
				circle.setFillColor(Color(255, 0, 0));
				circle.setPosition(Vector2f(128 + results[i].vertices[j].x * iratio - 1, results[i].vertices[j].y * iratio - 1));
				window.draw(circle);
			}
		}
		
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
