#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <immintrin.h>

#define SIZE 8

using namespace std::chrono;

void display(float *m)
{
    for(int y=0; y<SIZE; ++y)
    {
        for(int x=0; x<SIZE; ++x)
        {
            printf("%8.1f", m[y * SIZE + x]);
        }
        printf("\n");
    }
}

void init(float *m)
{
    for(int i=0; i<SIZE * SIZE; ++i)
        m[i] = 0.0f;
}

int main(void)
{
    float *m1, *m2, *m3;
    time_point<system_clock> start, end;
    
    posix_memalign((void**)&m1, 16, SIZE * SIZE * sizeof(float));
    posix_memalign((void**)&m2, 16, SIZE * SIZE * sizeof(float));
    posix_memalign((void**)&m3, 16, SIZE * SIZE * sizeof(float));
    
    srand(time(0));
    
    for(int i=0; i<SIZE*SIZE; ++i)
    {
        m1[i] = rand() % 5;
        m2[i] = rand() % 5;
    }
    
    start = system_clock::now();
    
    init(m3);
    
    __m128 r0, r1, r2, r3, r4, r5, r6, r7, r8;
    for(int i=0; i<SIZE; ++i)
    {
        for(int k=0; k<SIZE; k+=4)
        {
            r0 = _mm_set1_ps(m1[i*SIZE + k]);
            r1 = _mm_set1_ps(m1[i*SIZE + k + 1]);
            r2 = _mm_set1_ps(m1[i*SIZE + k + 2]);
            r3 = _mm_set1_ps(m1[i*SIZE + k + 3]);
            for(int j=0; j<SIZE; j+=4)
            {
                r4 = _mm_load_ps(&m2[k*SIZE+j]);
                r5 = _mm_load_ps(&m2[k*SIZE+j]);
                r6 = _mm_load_ps(&m2[(k+1)*SIZE+j]);
                r7 = _mm_load_ps(&m2[(k+2)*SIZE+j]);
                r8 = _mm_load_ps(&m2[(k+3)*SIZE+j]);
                
                r4 *= r0;
                r5 *= r1;
                r6 *= r2;
                r7 *= r3;
                
                _mm_store_ps(&m3[i*SIZE+j], r8+r4+r5+r6+r7);
            }
        }
    }
    
    end = system_clock::now();
    
    int ms = duration_cast<milliseconds>(end-start).count();
    
    printf("time: %d ms\n", ms);
    
    printf("m1\n");
    display(m1);
    
    printf("m2\n");
    display(m2);
    
    printf("r\n");
    
    display(m3);
    
    delete[] m1;
    delete[] m2;
    delete[] m3;
    
    return 0;
}

/*for (int i = 0; i < size; ++i)
    {
        for (int k = 0; k < size; k+=4)
        {
            auto r0 = _mm_set1_ps(a(i,k  ));
            auto r1 = _mm_set1_ps(a(i,k+1));
            auto r2 = _mm_set1_ps(a(i,k+2));
            auto r3 = _mm_set1_ps(a(i,k+3));

            for (int j = 0; j < size; j+= 4)
            {
                auto r4 = _mm_load_ps(&b(k,  j));
                auto r5 = _mm_load_ps(&b(k+1,j));
                auto r6 = _mm_load_ps(&b(k+2,j));
                auto r7 = _mm_load_ps(&b(k+3,j));

                auto r8 = _mm_load_ps(&e(i,j));

                r4 = r4 * r0;
                r5 = r5 * r1;
                r6 = r6 * r2;
                r7 = r7 * r3;

                _mm_store_ps(&e(i,j), r4+r5+r6+r7+r8);
            }
        }
    }
    * */
