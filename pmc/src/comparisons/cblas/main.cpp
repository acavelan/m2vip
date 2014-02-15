#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cmath>

extern "C"
{
   #include <cblas.h>
}

#include "mat/mat.hpp"

// Error margin with float
#define EPSILON 0.00001

using namespace std::chrono;

int main(int argc, char **argv)
{
    int size;
    float *m1, *m2, *m3;
    time_point<system_clock> start, end;

    if(argc < 2)
    {
        printf("usage: %s size\n", argv[0]);
        return 0;
    }

    size = atoi(argv[1]);
    
    posix_memalign((void**)&m1, 16, size * size * sizeof(float));
    posix_memalign((void**)&m2, 16, size * size * sizeof(float));
    posix_memalign((void**)&m3, 16, size * size * sizeof(float));
    
    srand(time(0));
    
    mat::random(m1, size);
    mat::random(m2, size);
    //mat::zeros(m3, size);

    start = system_clock::now();
	
	// using blas
    cblas_sgemm(
		// RowMajor, as is, as is
		CblasRowMajor, CblasNoTrans, CblasNoTrans,
		// rows of A, columns of B, columns of A
		size, size, size, 
		// alpha, B, A, first dim of A, B, first dim of B
		1.0, m1, size, m2, size, 
		// beta (if beta == 0, C will be initialized), C, first dim of C
		0.0, m3, size
	);
        
    end = system_clock::now();
    
    int ms = duration_cast<milliseconds>(end-start).count();
    
    printf("%d\n", ms);

    #ifdef CHECK
        #if CHECK==true
            float *check = new float[size*size];

            for(int i=0; i<size; ++i)
                for(int k=0; k<size; ++k)
                    for(int j=0; j<size; ++j)
                        check[i * size + j] += m1[i * size + k] * m2[k * size + j];
            
            for(int i=0; i<size; i++)
                for(int j=0; j<size; j++)
                    if(fabs(check[i * size + j] - m3[i * size + j]) > EPSILON)
                        printf("Error[%d,%d] : %8.0f != %8.0f\n", i, j, check[i * size + j], m3[i * size + j]);

            delete[] check;
        #endif
    #endif
    
    delete[] m1;
    delete[] m2;
    delete[] m3;
    
    return 0;
}
