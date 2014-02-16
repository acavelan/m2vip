#include <cmath>

#include "Target.hpp"

using namespace std;

double distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

Target::Target(Detector *detector, Matcher *matcher) :
	_detector(detector), _matcher(matcher)
{
}

std::vector<object> Target::setModel(unsigned char *image, int width, int height)
{
	_model = _detector->detect(image, width, height);
	return _model;
}

std::vector<object> Target::findModel(unsigned char *image, int width, int height)
{
	std::vector<object> objects = _detector->detect(image, width, height);
	return _matcher->match(_model, objects);
}

void recFindObject(double *phi, bool *flag, int width, int height, int x, int y, struct object &current)
{	
	flag[y * width + x] = true;
	
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
			current.vertices.push_back(vec2(x, y));
			
			if(x + 1 > 1 && y > 1 && !flag[y * width + (x+1)])
				recFindObject(phi, flag, width, height, x + 1, y, current);
			if(x > 1 && y + 1 > 1 && !flag[(y+1) * width + x])
				recFindObject(phi, flag, width, height, x, y + 1, current);
			if(x - 1 > 1 && y > 1 && !flag[y * width + (x-1)])
				recFindObject(phi, flag, width, height, x - 1, y, current);
			if(x > 1 && y - 1 > 1 && !flag[(y-1) * width + x])
				recFindObject(phi, flag, width, height, x, y - 1, current);
		}
	}
}

float getAngle(vec2 A, vec2 B, vec2 C)
{
	float a = abs(distance(A.x, A.y, B.x, B.y));
	float b = abs(distance(B.x, B.y, C.x, C.y));
	float c = abs(distance(C.x, C.y, A.x, A.y));
	
	return 180 - acos((a * a + b * b - c * c) / (2 * a * b)) * (360.0f / (2 * M_PI));
}

std::vector<object> ChanVeseDetector::detect(unsigned char *image, int width, int height)
{	
	double *phi = new double[width * height * sizeof(double)];
	bool *flag = new bool[width * height * sizeof(bool)];
	
	float radius = glm::min(width, height) / 4;
	
	// Init phi and inside
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			float d = distance(x, y, width/4, height/4);
			if(d > radius)
				phi[y * width + x] = -d;
			else
				phi[y * width + x] = d;
			
			flag[y * width + x] = false;
		}
	}
	
	// Iterate
	for(int i=0; i<10; i++)
	{
		int C1, C2;
		
		int insideSum = 0, outsideSum = 0;
		int insideCount = 0, outsideCount = 0;
		
		for(int y=0; y<height; y++)
		{
			for(int x=0; x<width; x++)
			{
				if(phi[y * width + x] < 0)
				{
					insideSum += image[y * width + x];
					insideCount++;
				}
				else
				{
					outsideSum += image[y * width + x];
					outsideCount++;
				}
			}
		}

		C1 = insideSum / insideCount;
		C2 = outsideSum / outsideCount;
		
		for(int y=0; y<height; y++)
		{
			for(int x=0; x<width; x++)
			{
				double PHI = phi[y * width + x];
				unsigned char I = image[y * width + x];
			
				phi[y * width + x] = PHI - 1.0f * (( -((I - C1) * (I - C1)) + ((I - C2) * (I - C2)) ));
			}
		}
	}
		
	std::vector<object> objects;
	
	for(int y=1; y<height-1; y++)
	{
		for(int x=1; x<width-1; x++)
		{
			if(phi[y * width + x] >= 0 && !flag[y * width + x])
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
					struct object current;
	
					recFindObject(phi, flag, width, height, x, y, current);
					
					objects.push_back(current);
				}
			}
			else
				flag[y * width + x] = true;
		}
	}
	
	int resolution = 250;
	
	// Elude
	for(unsigned int i=0; i<objects.size(); i++)
	{
		struct object newobject;
		
		float step = objects[i].vertices.size() / (float)resolution;
		
		for(int j=0; j<resolution; j++)
		{
			int count = 0;
			float x = 0.0f, y = 0.0f;
			for(int k=int(j * step); k<int((j+1) * step); k++)
			{
				if(k < (int)objects[i].vertices.size())
				{
					x += objects[i].vertices[k].x;
					y += objects[i].vertices[k].y;
					count++;
				}
			}
			
			if(count > 0)
				newobject.vertices.push_back(vec2(x/count, y/count));
		}
		
		objects[i] = newobject;
	}
	
	for(unsigned int i=0; i<objects.size(); i++)
	{
		struct object &o = objects[i];
		
		int angle = 0;
		int size = o.vertices.size();
		for(unsigned int j=1; j<o.vertices.size(); j++)
		{
			o.length += abs(distance(o.vertices[j-1].x, o.vertices[j-1].y, o.vertices[j].x, o.vertices[j].y));
			
			if(j+1 < o.vertices.size())
				angle = getAngle(o.vertices[j-1],o.vertices[j], o.vertices[j+1]);
			else
				angle = getAngle(o.vertices[j-1],o.vertices[j], o.vertices[0]);
			
			if(angle > 33)
				o.angles.push_back(angle);
		}
		
		o.length += abs(distance(o.vertices[0].x, o.vertices[0].y, o.vertices[size-1].x, o.vertices[size-1].y));
		
		angle = getAngle(o.vertices[size-1],o.vertices[0], o.vertices[1]);
		
		if(angle > 45)
			o.angles.push_back(angle);
		
		//printf("length[%d] = %f\n", i, o.length);
	}
	
	delete[] phi;
	delete[] flag;
	
	return objects;
}

std::vector<object> SimpleMatcher::match(const std::vector<object> &model, const std::vector<object> &list)
{
	std::vector<object> results;
	
	for(unsigned int i=0; i<list.size(); i++)
	{	
		/*if(list[i].angles.size() >= 4)
		{
			for(unsigned int j=0; j<model.size(); j++)
			{
				if(list[i].angles.size() < model[j].angles.size() + 5 || list[i].angles.size() > model[j].angles.size() - 5)
				{				
					float diff = abs(list[i].length - model[j].length);
				
					if(diff < 1.0f)					
						results.push_back(list[i]);
				}
			}
		}*/
        if(list[i].angles.size() >= 2)
		{
			for(unsigned int j=0; j<model.size(); j++)
			{
				if(list[i].angles.size() < model[j].angles.size() + 5 && list[i].angles.size() > model[j].angles.size() - 5)
				{				
						results.push_back(list[i]);
				}
			}
		}
	}
    
	return results;
}
