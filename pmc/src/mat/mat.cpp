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

	void mul(const float *m1, const float *m2, float *m3, int size)
	{
	    for(int i=0; i<size; ++i)
	        for(int k=0; k<size; ++k)
	            for(int j=0; j<size; ++j)
	            	m3[i*size+j] += m1[i*size + k] * m2[k*size+j];
	}
}