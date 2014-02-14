#include "cache.hpp"

namespace mat
{
	void cache(const float *m1, const float *m2, float *m3, int size)
	{
	    for(int i=0; i<size; ++i)
	        for(int k=0; k<size; ++k)
	            for(int j=0; j<size; ++j)
	            	m3[i*size+j] += m1[i*size + k] * m2[k*size+j];
	}

	void cache_unroll_1_4(const float *m1, const float *m2, float *m3, int size)
	{
		for(int i=0; i<size; ++i)
	        for(int k=0; k<size; ++k)
	            for(int j=0; j<size; j+=4)
	            {
	                m3[i * size + j] += m1[i * size + k] * m2[k * size + j];
	                m3[i * size + j+1] += m1[i * size + k] * m2[k * size + j+1];
	                m3[i * size + j+2] += m1[i * size + k] * m2[k * size + j+2];
	                m3[i * size + j+3] += m1[i * size + k] * m2[k * size + j+3];
	            }
	}

	void cache_unroll_1_8(const float *m1, const float *m2, float *m3, int size)
	{
		for(int i=0; i<size; ++i)
	        for(int k=0; k<size; ++k)
	            for(int j=0; j<size; j+=8)
	            {
	            	m3[i*size+j] += m1[i*size + k] * m2[k*size+j];
	            	m3[i*size+j+1] += m1[i*size + k] * m2[k*size+j+1];
	            	m3[i*size+j+2] += m1[i*size + k] * m2[k*size+j+2];
	            	m3[i*size+j+3] += m1[i*size + k] * m2[k*size+j+3];
	            	m3[i*size+j+4] += m1[i*size + k] * m2[k*size+j+4];
	            	m3[i*size+j+5] += m1[i*size + k] * m2[k*size+j+5];
	            	m3[i*size+j+6] += m1[i*size + k] * m2[k*size+j+6];
	            	m3[i*size+j+7] += m1[i*size + k] * m2[k*size+j+7];
	            }
	}

	void cache_unroll_1_16(const float *m1, const float *m2, float *m3, int size)
	{
		for(int i=0; i<size; ++i)
	        for(int k=0; k<size; ++k)
	            for(int j=0; j<size; j+=16)
	            {
	            	m3[i*size+j] += m1[i*size + k] * m2[k*size+j];
	            	m3[i*size+j+1] += m1[i*size + k] * m2[k*size+j+1];
	            	m3[i*size+j+2] += m1[i*size + k] * m2[k*size+j+2];
	            	m3[i*size+j+3] += m1[i*size + k] * m2[k*size+j+3];
	            	m3[i*size+j+4] += m1[i*size + k] * m2[k*size+j+4];
	            	m3[i*size+j+5] += m1[i*size + k] * m2[k*size+j+5];
	            	m3[i*size+j+6] += m1[i*size + k] * m2[k*size+j+6];
	            	m3[i*size+j+7] += m1[i*size + k] * m2[k*size+j+7];
	            	m3[i*size+j+8] += m1[i*size + k] * m2[k*size+j+8];
	            	m3[i*size+j+9] += m1[i*size + k] * m2[k*size+j+9];
	            	m3[i*size+j+10] += m1[i*size + k] * m2[k*size+j+10];
	            	m3[i*size+j+11] += m1[i*size + k] * m2[k*size+j+11];
	            	m3[i*size+j+12] += m1[i*size + k] * m2[k*size+j+12];
	            	m3[i*size+j+13] += m1[i*size + k] * m2[k*size+j+13];
	            	m3[i*size+j+14] += m1[i*size + k] * m2[k*size+j+14];
	            	m3[i*size+j+15] += m1[i*size + k] * m2[k*size+j+15];
	            }
	}

	void cache_unroll_2_4(const float *m1, const float *m2, float *m3, int size)
	{
		for(int i=0; i<size; ++i)
	        for(int k=0; k<size; k+=2)
	            for(int j=0; j<size; j+=4)
	            {
	                m3[i * size + j] += m1[i * size + k] * m2[k * size + j];
	                m3[i * size + j+1] += m1[i * size + k] * m2[k * size + j+1];
	                m3[i * size + j+2] += m1[i * size + k] * m2[k * size + j+2];
	                m3[i * size + j+3] += m1[i * size + k] * m2[k * size + j+3];

	                m3[i * size + j] += m1[i * size + k+1] * m2[(k+1) * size + j];
	                m3[i * size + j+1] += m1[i * size + k+1] * m2[(k+1) * size + j+1];
	                m3[i * size + j+2] += m1[i * size + k+1] * m2[(k+1) * size + j+2];
	                m3[i * size + j+3] += m1[i * size + k+1] * m2[(k+1) * size + j+3];
	            }
	}

	void cache_unroll_2_8(const float *m1, const float *m2, float *m3, int size)
	{
		for(int i=0; i<size; ++i)
	        for(int k=0; k<size; k+=2)
	            for(int j=0; j<size; j+=8)
	            {
	                m3[i*size+j] += m1[i*size + k] * m2[k*size+j];
	            	m3[i*size+j+1] += m1[i*size + k] * m2[k*size+j+1];
	            	m3[i*size+j+2] += m1[i*size + k] * m2[k*size+j+2];
	            	m3[i*size+j+3] += m1[i*size + k] * m2[k*size+j+3];
	            	m3[i*size+j+4] += m1[i*size + k] * m2[k*size+j+4];
	            	m3[i*size+j+5] += m1[i*size + k] * m2[k*size+j+5];
	            	m3[i*size+j+6] += m1[i*size + k] * m2[k*size+j+6];
	            	m3[i*size+j+7] += m1[i*size + k] * m2[k*size+j+7];
	            	m3[i*size+j+8] += m1[i*size + k] * m2[k*size+j+8];
	            	m3[i*size+j+9] += m1[i*size + k] * m2[k*size+j+9];
	            	m3[i*size+j+10] += m1[i*size + k] * m2[k*size+j+10];
	            	m3[i*size+j+11] += m1[i*size + k] * m2[k*size+j+11];
	            	m3[i*size+j+12] += m1[i*size + k] * m2[k*size+j+12];
	            	m3[i*size+j+13] += m1[i*size + k] * m2[k*size+j+13];
	            	m3[i*size+j+14] += m1[i*size + k] * m2[k*size+j+14];
	            	m3[i*size+j+15] += m1[i*size + k] * m2[k*size+j+15];

	                m3[i*size+j] += m1[i*size + k+1] * m2[(k+1)*size+j];
	            	m3[i*size+j+1] += m1[i*size + k+1] * m2[(k+1)*size+j+1];
	            	m3[i*size+j+2] += m1[i*size + k+1] * m2[(k+1)*size+j+2];
	            	m3[i*size+j+3] += m1[i*size + k+1] * m2[(k+1)*size+j+3];
	            	m3[i*size+j+4] += m1[i*size + k+1] * m2[(k+1)*size+j+4];
	            	m3[i*size+j+5] += m1[i*size + k+1] * m2[(k+1)*size+j+5];
	            	m3[i*size+j+6] += m1[i*size + k+1] * m2[(k+1)*size+j+6];
	            	m3[i*size+j+7] += m1[i*size + k+1] * m2[(k+1)*size+j+7];
	            	m3[i*size+j+8] += m1[i*size + k+1] * m2[(k+1)*size+j+8];
	            	m3[i*size+j+9] += m1[i*size + k+1] * m2[(k+1)*size+j+9];
	            	m3[i*size+j+10] += m1[i*size + k+1] * m2[(k+1)*size+j+10];
	            	m3[i*size+j+11] += m1[i*size + k+1] * m2[(k+1)*size+j+11];
	            	m3[i*size+j+12] += m1[i*size + k+1] * m2[(k+1)*size+j+12];
	            	m3[i*size+j+13] += m1[i*size + k+1] * m2[(k+1)*size+j+13];
	            	m3[i*size+j+14] += m1[i*size + k+1] * m2[(k+1)*size+j+14];
	            	m3[i*size+j+15] += m1[i*size + k+1] * m2[(k+1)*size+j+15];
	            }
	}

	void cache_unroll_2_16(const float *m1, const float *m2, float *m3, int size)
	{
		for(int i=0; i<size; ++i)
	        for(int k=0; k<size; k+=2)
	            for(int j=0; j<size; j+=8)
	            {
	                m3[i*size+j] += m1[i*size + k] * m2[k*size+j];
	            	m3[i*size+j+1] += m1[i*size + k] * m2[k*size+j+1];
	            	m3[i*size+j+2] += m1[i*size + k] * m2[k*size+j+2];
	            	m3[i*size+j+3] += m1[i*size + k] * m2[k*size+j+3];
	            	m3[i*size+j+4] += m1[i*size + k] * m2[k*size+j+4];
	            	m3[i*size+j+5] += m1[i*size + k] * m2[k*size+j+5];
	            	m3[i*size+j+6] += m1[i*size + k] * m2[k*size+j+6];
	            	m3[i*size+j+7] += m1[i*size + k] * m2[k*size+j+7];

	                m3[i*size+j] += m1[i*size + k+1] * m2[(k+1)*size+j];
	            	m3[i*size+j+1] += m1[i*size + k+1] * m2[(k+1)*size+j+1];
	            	m3[i*size+j+2] += m1[i*size + k+1] * m2[(k+1)*size+j+2];
	            	m3[i*size+j+3] += m1[i*size + k+1] * m2[(k+1)*size+j+3];
	            	m3[i*size+j+4] += m1[i*size + k+1] * m2[(k+1)*size+j+4];
	            	m3[i*size+j+5] += m1[i*size + k+1] * m2[(k+1)*size+j+5];
	            	m3[i*size+j+6] += m1[i*size + k+1] * m2[(k+1)*size+j+6];
	            	m3[i*size+j+7] += m1[i*size + k+1] * m2[(k+1)*size+j+7];
	            }
	}

	void cache_unroll_sse_1_4(const float *m1, const float *m2, float *m3, int size)
	{

	}

	void cache_unroll_sse_1_8(const float *m1, const float *m2, float *m3, int size)
	{

	}

	void cache_unroll_sse_1_16(const float *m1, const float *m2, float *m3, int size)
	{

	}

	void cache_unroll_sse_2_4(const float *m1, const float *m2, float *m3, int size)
	{

	}

	void cache_unroll_sse_2_8(const float *m1, const float *m2, float *m3, int size)
	{

	}

	void cache_unroll_sse_2_16(const float *m1, const float *m2, float *m3, int size)
	{

	}
}
