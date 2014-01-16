#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <chrono>

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
    
    m1 = new float[SIZE * SIZE];
    m2 = new float[SIZE * SIZE];
    m3 = new float[SIZE * SIZE];
    
    srand(time(0));
    
    for(int i=0; i<SIZE*SIZE; ++i)
    {
        m1[i] = rand() % 100;
        m2[i] = rand() % 100;
    }
    
    start = system_clock::now();
    
    init(m3);
    
    for(int i=0; i<SIZE; ++i)
    {
        for(int k=0; k<SIZE; ++k)
        {
            for(int j=0; j<SIZE; j+=4)
            {
                m3[i * SIZE + j] += m1[i * SIZE + k] * m2[k * SIZE + j];
                m3[i * SIZE + j+1] += m1[i * SIZE + k] * m2[k * SIZE + j+1];
                m3[i * SIZE + j+2] += m1[i * SIZE + k] * m2[k * SIZE + j+2];
                m3[i * SIZE + j+3] += m1[i * SIZE + k] * m2[k * SIZE + j+3];
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
    
    display(r);*/
    
    delete[] m1;
    delete[] m2;
    delete[] m3;
    
    return 0;
}
