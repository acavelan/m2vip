#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdio>
#include <cmath>
#include <string.h>

#include "filter.hpp"

using namespace sf;

// Coquelicot
/*
#define POINTS 20
#define SPACING 5
#define PADDING 1
#define CEIL 0.99f

#define CINT 0.04f
#define CEXT 0.1f*/

// Default
/*
#define POINTS 48
#define SPACING 10
#define PADDING 1
#define CEIL 0.5f

#define CINT 0.02f
#define CEXT 0.02f*/

#define POINTS 128
#define SPACING 15
#define PADDING 1
#define CEIL 0.2f

#define CINT 0.0001f
#define CEXT 0.03f

#define IT_MUL 50

Vector2f normalize(const Vector2f& source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return Vector2f(source.x / length, source.y / length);
    else
        return source;
}

void filter_sobel(const Image &origin, Image &output)
{	
	int width = origin.getSize().x;
	int height = origin.getSize().y;
	
	unsigned char *input = new unsigned char[width * height * 3];
	unsigned char *gray = new unsigned char[width * height];
	unsigned char *sobel = new unsigned char[width * height];
	
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

	apply_sobel(gray, sobel, width, height);
	
	memset(sobel + (height - 1) * width, 0, width * sizeof(unsigned char));
	
	for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
			output.setPixel(x, y, Color(sobel[y*width+x], sobel[y*width+x], sobel[y*width+x]));
	
	delete[] gray;
	delete[] sobel;
	delete[] input;
}

class Snake
{
	public:
		
		std::vector<Vector2f> points;
		
		int width, height;
	 
		Vector2f *gvf;
		float *gradient;
		float *distances;
		
		Snake()
		{
			gvf = 0;
		}
		
		~Snake()
		{
			if(distances) delete[] distances;
			if(gradient) delete[] gradient;
			if(gvf) delete[] gvf;
		}
		
		void build(const Vector2f &position, int radius, int count)
		{
			for(double angle=0; angle <= 2 * M_PI; angle += (2*M_PI) / count)
			{
				float x = position.x + radius * cos(angle);
				float y = position.y + radius * sin(angle);
				points.push_back(Vector2f(x, y));
			}
		}
		
		double distance(double x1, double y1, double x2, double y2)
		{
			return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
		}
		
		void getDistance(const float *gradient, float *distances, int gx, int gy)
		{		
			const unsigned int g = gy * width + gx;
			distances[g] = 0.0f;
			
			for(int y=gy-SPACING*PADDING; y<gy+SPACING*PADDING; y+=PADDING)
			{
				for(int x=gx-SPACING*PADDING; x<gx+SPACING*PADDING; x+=PADDING)
				{
					if(x > 0 && x < width && y > 0 && y < height && (x != gx || y != gy))
					{
						if(gradient[y * width + x] > CEIL)
						{
							float d = distance(gx, gy, x, y);
							if(d < 0.0f) d = -d;
							distances[g] += gradient[y * width + x] / d;
						}
					}
				}
			}
		}
		
		void getGradient(const float *distances, Vector2f *gvf, int gx, int gy)
		{		
			int vx = 0, vy = 0;
			float max = 0.0f;
			
			for(int y=gy-4; y<gy+4; y++)
			{
				for(int x=gx-4; x<gx+4; x++)
				{
					if(x > 0 && x < width && y > 0 && y < height)
					{
						if(distances[y * width + x] > max)
						{
							max = distances[y * width + x];
							vx = (gx - x) * distances[gy * width + gx];
							vy = (gy - y) * distances[gy * width + gx];
						}
					}
				}
			}
			
			//printf("%d, %d\n", vx, vy);
			
			gvf[gy * width + gx] = Vector2f(vx, vy);
		}
		
		void setGradient(const Image &image)
		{
			width = image.getSize().x;
			height = image.getSize().y;
			
			distances = new float[width * height];
			gradient = new float[width * height];
			gvf = new Vector2f[width * height];
			
			for(int y=0; y<height; y++)
				for(int x=0; x<width; x++)
					gradient[y * width + x] = image.getPixel(x, y).r / 255.0f;
			
			for(int y=0; y<height; y++)
				for(int x=0; x<width; x++)
					getDistance(gradient, distances, x, y);
			
			for(int y=0; y<height; y++)
				for(int x=0; x<width; x++)
					getGradient(distances, gvf, x, y);
		}
		
		void update()
		{
			int count = points.size();
			
			std::vector<Vector2f> dist;
			std::vector<Vector2f> newpoints;
			
			for(int i=0; i<count; i++)
			{
				Vector2f prev = Vector2f(0.0f, 0.0f), next = Vector2f(0.0f, 0.0f);
				for(int j=0; j<count; j++)
				{
					prev += points.at((i-j+count) % count) / (float)(j+1);
					next += points.at((i+j+count) % count) / (float)(j+1);
				}
				
				Vector2f d = prev - next;
				
				dist.push_back(d);
			}
			
			for(int i=0; i<count; i++)
			{
				Vector2f prev = Vector2f(0.0f, 0.0f), next = Vector2f(0.0f, 0.0f);
				for(int j=0; j<count; j++)
				{
					prev += dist.at((i-j+count) % count) / (float)(j+1);
					next += dist.at((i+j+count) % count) / (float)(j+1);
				}
				Vector2f &cur = points.at(i);
				
				Vector2f fint = prev - next;
				
				Vector2f fext = Vector2f(0.0f, 0.0f);
				if(cur.x > 1 && cur.x < width - 1 && cur.y > 1 && cur.y < height - 1)
				{
					fext = -fint;
					fext = normalize(fext);
					float fd1 = (float)distances[(int)(cur.y + fext.y) * width + (int)(cur.x + fext.x)];
					float fd2 = (float)distances[(int)(cur.y - fext.y) * width + (int)(cur.x - fext.x)];
					
					float fd = fd1;
					if(fd2 > fd1) fd = -fd2;
					
					fext.x = fext.x * fd;
					fext.y = fext.y * fd;
					
					//fext = gvf[(int)cur.y * width + (int)cur.x];
				}
				
				//printf("%f, %f\n", fext.x, fext.y);
				
				//Vector2f newpoint = cur + fint * CINT - fext * CEXT;
				Vector2f newpoint = cur + fint * CINT + fext * CEXT;
				
				newpoints.push_back(newpoint);
			}
			
			points = newpoints;
		}
};

int main(int argc, char **argv)
{
	if(argc < 2)
		printf("usage: %s image\n", argv[0]);
	
	Image image, gradient; 
	if(!image.loadFromFile(argv[1]))
	{
		printf("unable to load %s\n", argv[1]);
		return 0;
	}
	
	gradient = image;
	
	filter_sobel(image, gradient);
	
	Texture tImage, tGrad;
	tImage.loadFromImage(image);
	tGrad.loadFromImage(gradient);
	
	Sprite sImage, sGrad;
	sImage.setTexture(tImage);
	sGrad.setTexture(tGrad);
	sGrad.setPosition(image.getSize().x, 0);
	
	Snake snake;
	snake.setGradient(gradient);
	snake.build(Vector2f(image.getSize().x / 2, image.getSize().y / 2), image.getSize().x/2, POINTS);
	
	RenderWindow window(sf::VideoMode(image.getSize().x * 2, image.getSize().y), "Snake");
	window.setFramerateLimit(60);
	
    while(window.isOpen())
    {
		window.clear();
		
		for(int i=0; i<IT_MUL; i++)
			snake.update();
		
		window.draw(sImage);
		window.draw(sGrad);
		
		std::vector<Vertex> vertices;
		for(unsigned int i=0; i<snake.points.size(); i++)
			vertices.push_back(Vertex(snake.points.at(i)));
		vertices.push_back(Vertex(snake.points.at(0)));

		window.draw(&vertices[0], snake.points.size() + 1, LinesStrip);
		
		for(unsigned int i=0; i<snake.points.size(); i++)
		{
			CircleShape circle(2);
			circle.setFillColor(Color(255, 0, 0));
			circle.setPosition(Vector2f(snake.points.at(i).x - 2, snake.points.at(i).y - 2));
			window.draw(circle);
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
