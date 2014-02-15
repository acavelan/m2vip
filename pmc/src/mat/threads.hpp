#ifndef MAT_THREADS_HPP
#define MAT_THREADS_HPP

namespace mat
{
	void naive_omp(const float *m1, const float *m2, float *m3, int size);
	
	void cache_omp(const float *m1, const float *m2, float *m3, int size);
		
	void blocks_omp(const float *m1, const float *m2, float *m3, int size);
}

#endif
