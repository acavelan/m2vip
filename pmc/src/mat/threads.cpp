#include "threads.hpp"

#include <algorithm>
#include <immintrin.h>

#ifndef BLOCK_SIZE
#error "BLOCK_SIZE is not defined (use e.g -DBLOCK_SIZE=80, which is 2048Ko)"
#endif

using std::min;

namespace mat
{
	void naive_omp(const float *m1, const float *m2, float *m3, int size)
	{
		#pragma omp parallel for
		for(int i=0; i<size; ++i)
	        for(int j=0; j<size; ++j)
	            for(int k=0; k<size; ++k)
	            	m3[i*size+j] += m1[i*size + k] * m2[k*size+j];
	}
		
	void cache_omp(const float *m1, const float *m2, float *m3, int size)
	{
		#pragma omp parallel for
		for(int i=0; i<size; ++i)
	    {
			__m128 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13;
	        for(int k=0; k<size; k+=4)
	        {
	            r0 = _mm_set1_ps(m1[i*size + k]);
	            r1 = _mm_set1_ps(m1[i*size + k + 1]);
	            r2 = _mm_set1_ps(m1[i*size + k + 2]);
	            r3 = _mm_set1_ps(m1[i*size + k + 3]);
	            for(int j=0; j<size; j+=8)
	            {
	                r4 = _mm_load_ps(&m2[k*size+j]);
	                r5 = _mm_load_ps(&m2[k*size+j+4]);
                    r6 = _mm_load_ps(&m2[(k+1)*size+j]);
                    r7 = _mm_load_ps(&m2[(k+1)*size+j+4]);
                    r8 = _mm_load_ps(&m2[(k+2)*size+j]);
                    r9 = _mm_load_ps(&m2[(k+2)*size+j+4]);
                    r10 = _mm_load_ps(&m2[(k+3)*size+j]);
                    r11 = _mm_load_ps(&m2[(k+3)*size+j+4]);
                    r12 = _mm_load_ps(&m3[i*size+j]);
                    r13 = _mm_load_ps(&m3[i*size+j+4]);
                    r12 += r0*r4 + r1*r6 + r2*r8 + r3*r10;
                    r13 += r0*r5 + r1*r7 + r2*r9 + r3*r11;
                    _mm_store_ps(&m3[i*size+j], r12);
                    _mm_store_ps(&m3[i*size+j+4], r13);
	            }
	        }
	    }
	}
	
	void blocks_omp(const float *m1, const float *m2, float *m3, int size)
	{
		#pragma omp parallel for
        for(int bi=0; bi<size; bi += BLOCK_SIZE)
        {
			__m128 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13;
            for(int bj=0; bj<size; bj += BLOCK_SIZE)
            {
                for(int bk=0; bk<size; bk += BLOCK_SIZE)
                {
                    int imax = std::min(bi+BLOCK_SIZE,size);
                    for(int i=bi; i<imax; ++i)
                    {
                        int kmax = std::min(bk+BLOCK_SIZE,size);
                        for(int k=bk; k<kmax; k+=4)
                        { 
                            r0 = _mm_set1_ps(m1[i*size + k]);
                            r1 = _mm_set1_ps(m1[i*size + k + 1]);
                            r2 = _mm_set1_ps(m1[i*size + k + 2]);
                            r3 = _mm_set1_ps(m1[i*size + k + 3]);
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=8)
                            {
                                r4 = _mm_load_ps(&m2[k*size+j]);
                                r5 = _mm_load_ps(&m2[k*size+j+4]);
                                r6 = _mm_load_ps(&m2[(k+1)*size+j]);
                                r7 = _mm_load_ps(&m2[(k+1)*size+j+4]);
                                r8 = _mm_load_ps(&m2[(k+2)*size+j]);
                                r9 = _mm_load_ps(&m2[(k+2)*size+j+4]);
                                r10 = _mm_load_ps(&m2[(k+3)*size+j]);
                                r11 = _mm_load_ps(&m2[(k+3)*size+j+4]);
                                r12 = _mm_load_ps(&m3[i*size+j]);
                                r13 = _mm_load_ps(&m3[i*size+j+4]);
                                r12 += r0*r4 + r1*r6 + r2*r8 + r3*r10;
                                r13 += r0*r5 + r1*r7 + r2*r9 + r3*r11;
                                _mm_store_ps(&m3[i*size+j], r12);
                                _mm_store_ps(&m3[i*size+j+4], r13);
                            }
                        }
                    }
                }
            }
        }
	}
}
