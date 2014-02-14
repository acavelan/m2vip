#include "mat.hpp"

#include <ctime>
#include <cstdlib>

namespace mat
{
	void zeros(float *m, int size)
	{
		for(int i=0; i<size*size; ++i)
			m[i] = 0;
	}

	void random(float *m, int size)
	{
		srand(time(0));

		for(int i=0; i<size*size; ++i)
			m[i] = rand() % 100;
	}

	float* read(const char *filename, int &size);

	void write(const char *filename, const float *m, int size);
}