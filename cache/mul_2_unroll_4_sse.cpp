#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <immintrin.h>

#define SIZE 1024

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
    
    __m128 r1, r2, rtmp;
    for(int i=0; i<SIZE; ++i)
    {
        for(int k=0; k<SIZE; ++k)
        {
            r2 = _mm_set1_ps(m1[i*SIZE + k]);
            for(int j=0; j<SIZE; j+=4)
            {
                r1 = _mm_load_ps(&m2[k*SIZE+j]);
                r1 = r1 * r2;
                rtmp = _mm_load_ps(&m3[i*SIZE+j]);
                rtmp += r1;
                _mm_store_ps(&m3[i*SIZE+j], rtmp);
            }
        }
    }
    
    end = system_clock::now();
    
    int ms = duration_cast<milliseconds>(end-start).count();
    
    printf("time: %d ms\n", ms);
    
    /*printf("m1\n");
    display(m1);
    
    printf("m2\n");
    display(m2);
    
    printf("r\n");
    
    display(m3);*/
    
    delete[] m1;
    delete[] m2;
    delete[] m3;
    
    return 0;
}
