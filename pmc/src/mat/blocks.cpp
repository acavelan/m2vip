#include "blocks.hpp"
#include <algorithm>

#ifndef BLOCK_SIZE
#error "BLOCK_SIZE is not defined (use e.g -DBLOCK_SIZE=80, which is 2048Ko)"
#endif

namespace mat
{
    void blocks(const float *m1, const float *m2, float *m3, int size)
    {
        for(int bi=0; bi<size; bi += BLOCK_SIZE)
        {
            for(int bj=0; bj<size; bj += BLOCK_SIZE)
            {
                for(int bk=0; bk<size; bk += BLOCK_SIZE)
                {
                    int imax = std::min(bi+BLOCK_SIZE,size);
                    for(int i=bi; i<imax; ++i)
                    {
                        int kmax = std::min(bk+BLOCK_SIZE,size);
                        for(int k=bk; k<kmax; ++k)
                        { 
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; ++j)
                                m3[i*size+j] += m1[i*size+k] * m2[k*size+j];
                        }
                    }
                }
            }
        }
    }
}
