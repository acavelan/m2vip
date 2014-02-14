#include "naive.hpp"

namespace mat
{
	void naive(const float *m1, const float *m2, float *m3, int size)
	{
	    for(int i=0; i<size; ++i)
	        for(int j=0; j<size; ++j)
	            for(int k=0; k<size; ++k)
	            	m3[i*size+j] += m1[i*size + k] * m2[k*size+j];
	}
}
