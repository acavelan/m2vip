#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <Eigen/Dense>

// Error margin with float
#define EPSILON 0.001

using namespace Eigen;
using namespace std::chrono;

int main(int argc, char **argv)
{
	int size;
    MatrixXf m1, m2, m3;
    time_point<system_clock> start, end;

    if(argc < 2)
    {
        printf("usage: %s size\n", argv[0]);
        return 0;
    }

    size = atoi(argv[1]);
    
    srand(time(0));
    
    // Avoid rand() between 1 and -1
    // But damn ! Eigen is still pretty fast ...
    //m1 = MatrixXf::Random(size, size);
    //m2 = MatrixXf::Random(size, size);
    
    m1.resize(size, size);
    m2.resize(size, size);
    
    for(int i=0; i<size; ++i)
    {
		for(int j=0; j<size; ++j)
		{
			m1(i, j) = rand() % 100;
			m2(i, j) = rand() % 100;
		}
	}
	
    m3 = MatrixXf::Zero(size, size);
    
    start = system_clock::now();
	
	m3 = m1 * m2;
        
    end = system_clock::now();
    
    int ms = duration_cast<milliseconds>(end-start).count();
    
    printf("%d\n", ms);

    #ifdef CHECK
        #if CHECK==true
            float *check = new float[size*size];

            for(int i=0; i<size; ++i)
                for(int k=0; k<size; ++k)
                    for(int j=0; j<size; ++j)
                        check[i * size + j] += m1(i, k) * m2(k, j);
            
            for(int i=0; i<size; i++)
                for(int j=0; j<size; j++)
                    if(fabs(check[i * size + j] - m3(i, j)) > EPSILON)
                        printf("Error[%d,%d] : %8.0f != %8.0f\n", i, j, check[i * size + j], m3(i, j));

            delete[] check;
        #endif
    #endif
    
    return 0;
}
