#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <chrono>

#define SIZE 4096

using namespace std::chrono;

int main(void)
{
    int *m1, *m2, *r;
    time_point<system_clock> start, end;
    
    m1 = new int[SIZE * SIZE];
    m2 = new int[SIZE * SIZE];
    r = new int[SIZE * SIZE];
    
    srand(time(0));
    
    for(int i=0; i<SIZE*SIZE; ++i)
    {
        m1[i] = rand() % 100;
        m2[i] = rand() % 100;
    }
    
    start = system_clock::now();
    
    for(int x=0; x<SIZE; ++x)
        for(int y=0; y<SIZE; ++y)
            r[y * SIZE + x] = m1[y * SIZE + x] + m2[y * SIZE + x];
    
    end = system_clock::now();
    
    int ms = duration_cast<milliseconds>(end-start).count();
    
    printf("time: %d ms\n", ms);
    
    delete[] m1;
    delete[] m2;
    delete[] r;
    
    return 0;
}

