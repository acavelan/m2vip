#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdio>
#include <cmath>
#include <string.h>

#define DELTA 0.1f
#define SIZE_DIV 4

using namespace sf;
using namespace std;

void apply_grayscale(const unsigned char *input, unsigned char *output, int width, int height)
{	
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			unsigned char R = input[y * width * 3 + x + 0];
			unsigned char G = input[y * width * 3 + x + 1];
			unsigned char B = input[y * width * 3 + x + 2];
			
			unsigned char gray = (307 * R + 604 * G + 113 * B) >> 10;
			
			output[y*width+x] = gray;
		}
	}
}

Vector2f normalize(const Vector2f& source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return Vector2f(source.x / length, source.y / length);
    else
        return source;
}

double distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
		
void filter_gray(const Image &origin, Image &output)
{	
	int width = origin.getSize().x;
	int height = origin.getSize().y;
	
	unsigned char *input = new unsigned char[width * height * 3];
	unsigned char *gray = new unsigned char[width * height];
	
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
	
	for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
			output.setPixel(x, y, Color(gray[y*width+x], gray[y*width+x], gray[y*width+x]));
	
	delete[] gray;
	delete[] input;
}

class ChanVese
{
	public:
		
		int width, height;
	 
		double *phi;
		unsigned char *gray;
		vector<Vector2f> shape;
		
		ChanVese()
		{
			phi = 0;
			gray = 0;
		}
		
		~ChanVese()
		{
			if(phi) delete[] phi;
			if(gray) delete[] gray;
		}
		
		void init(const Image &image, const Vector2f &center, int radius)
		{
			width = image.getSize().x;
			height = image.getSize().y;
			
			gray = new unsigned char[width * height * sizeof(unsigned char)];
			phi = new double[width * height * sizeof(double)];
			
			// Init gray
			for(int y=0; y<height; y++)
				for(int x=0; x<width; x++)
					gray[y * width + x] = image.getPixel(x, y).r;
			
			// Init phi and inside
			for(int y=0; y<height; y++)
			{
				for(int x=0; x<width; x++)
				{
					float d = distance(x, y, center.x, center.y);
					if(d > radius)
						phi[y * width + x] = -d;
					else
						phi[y * width + x] = d;
				}
			}
			
			createShape();			
		}
		
		void createShape()
		{
			shape.clear();
			
			#pragma omp parallel for
			for(int y=1; y<height-1; y++)
			{
				for(int x=1; x<width-1; x++)
				{
					if(phi[y * width + x] >= 0)
					{
						if(	phi[(y-1) * width + x - 1] < 0 ||
							phi[(y-1) * width + x] < 0 ||
							phi[(y-1) * width + x + 1] < 0 ||
							phi[(y) * width + x - 1] < 0 ||
							phi[(y) * width + x + 1] < 0 ||
							phi[(y+1) * width + x - 1] < 0 ||
							phi[(y+1) * width + x] < 0 ||
							phi[(y+1) * width + x + 1] < 0)
						{
							#pragma omp critical
							shape.push_back(Vector2f(x, y));
						}
					}
				}
			}
		}
		
		void initPhi()
		{
			if(shape.size() > 0)
			{
				#pragma omp parallel for
				for(int y=0; y<height; y++)
				{
					for(int x=0; x<width; x++)
					{
						bool stop = false;
						float dmin = distance(x, y, shape.at(0).x, shape.at(0).y);
						for(unsigned int i=1; i<shape.size() && !stop; i++)
						{
							float d = distance(x, y, shape.at(i).x, shape.at(i).y);
							if(d < dmin)
								dmin = d;
							else
								stop = true;
						}
						if(phi[y * width + x] < 0)
							phi[y * width + x] = -dmin;
						else
							phi[y * width + x] = dmin;
					}
				}
			}
		}
		
		void getAverage(int &C1, int &C2)
		{
			int insideSum = 0, outsideSum = 0;
			int insideCount = 0, outsideCount = 0;
			
			#pragma omp parallel for
			for(int y=0; y<height; y++)
			{
				for(int x=0; x<width; x++)
				{
					if(phi[y * width + x] < 0)
					{
						insideSum += gray[y * width + x];
						insideCount++;
					}
					else
					{
						outsideSum += gray[y * width + x];
						outsideCount++;
					}
				}
			}
			
			C1 = insideSum / insideCount;
			C2 = outsideSum / outsideCount;
		}
		
		void update()
		{
			int C1, C2;
			
			getAverage(C1, C2);
			
			#pragma omp parallel for
			for(int y=0; y<height; y++)
			{
				for(int x=0; x<width; x++)
				{
					double PHI = phi[y * width + x];
					unsigned char I = gray[y * width + x];
				
					phi[y * width + x] = PHI - DELTA * (( -((I - C1) * (I - C1)) + ((I - C2) * (I - C2)) ));
				}
			}
			
			createShape();
		}
};

int main(int argc, char **argv)
{
	if(argc < 2)
		printf("usage: %s image\n", argv[0]);
	
	Image image, gray; 
	if(!image.loadFromFile(argv[1]))
	{
		printf("unable to load %s\n", argv[1]);
		return 0;
	}
	
	gray = image;
	
	filter_gray(image, gray);
	
	Texture tImage, tGrad;
	tImage.loadFromImage(image);
	tGrad.loadFromImage(gray);
	
	Sprite sImage, sGrad;
	sImage.setTexture(tImage);
	sGrad.setTexture(tGrad);
	sGrad.setPosition(image.getSize().x, 0);
	
	ChanVese cv;
	cv.init(gray, Vector2f(image.getSize().x / 2, image.getSize().y / 2), image.getSize().x/SIZE_DIV);
	
	RenderWindow window(sf::VideoMode(image.getSize().x * 2, image.getSize().y), "ChanVese");
	window.setFramerateLimit(60);
	
    while(window.isOpen())
    {
		window.clear();
		
		window.draw(sImage);
		window.draw(sGrad);
		
		for(unsigned int i=0; i<cv.shape.size(); i++)
		{
			CircleShape circle(1);
			circle.setFillColor(Color(255, 0, 0));
			circle.setPosition(Vector2f(cv.shape.at(i).x - 1, cv.shape.at(i).y - 1));
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
        
        cv.update();
    }

    return EXIT_SUCCESS;
}
