#ifndef TARGET_H
#define TARGET_H

#include "glm/glm.hpp"
#include <vector>

using namespace glm;

struct object
{
	std::vector<vec2> vertices;
	std::vector<float> angles;
	float length;
};

class Detector
{
	public:
		
		virtual ~Detector(){}
		
		virtual std::vector<object> detect(unsigned char *image, int width, int height) = 0;
		
};

class ChanVeseDetector : public Detector
{
	public:
		
		std::vector<object> detect(unsigned char *image, int width, int height);
		
};

class Matcher
{
	public:
		
		virtual ~Matcher(){}
		
		virtual std::vector<object> match(const std::vector<object> &model, const std::vector<object> &list) = 0;
		
};

class SimpleMatcher : public Matcher
{
	public:
		
		std::vector<object> match(const std::vector<object> &model, const std::vector<object> &list);
};

class Target
{
	public:
	
		Target(Detector *detector, Matcher *matcher);
		
		std::vector<object> setModel(unsigned char *image, int width, int height);
		
		std::vector<object> findModel(unsigned char *image, int width, int height);
	
	private:
		
		Detector *_detector;
		Matcher *_matcher;
		
		std::vector<object> _model;
		
};

#endif
