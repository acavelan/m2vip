#include "blocks.hpp"
#include <algorithm>

#ifndef BLOCK_SIZE
#error "BLOCK_SIZE is not defined (use e.g -DBLOCK_SIZE=80, which is 2048Ko)"
#endif

using std::min;

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

    void blocks_unroll_1_4(const float *m1, const float *m2, float *m3, int size)
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
                            for(int j=bj; j<jmax; j+=4)
                            {
                                m3[i * size + j] += m1[i * size + k] * m2[k * size + j];
                                m3[i * size + j+1] += m1[i * size + k] * m2[k * size + j+1];
                                m3[i * size + j+2] += m1[i * size + k] * m2[k * size + j+2];
                                m3[i * size + j+3] += m1[i * size + k] * m2[k * size + j+3];
                            }
                        }
                    }
                }
            }
        }
    }

    void blocks_unroll_1_8(const float *m1, const float *m2, float *m3, int size)
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
                            for(int j=bj; j<jmax; j+=8)
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
                    }
                }
            }
        }
    }

    void blocks_unroll_1_16(const float *m1, const float *m2, float *m3, int size)
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
                            for(int j=bj; j<jmax; j+=16)
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
                    }
                }
            }
        }
    }

    void blocks_unroll_2_4(const float *m1, const float *m2, float *m3, int size)
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
                        for(int k=bk; k<kmax; k+=2)
                        { 
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=4)
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
                    }
                }
            }
        }
    }

    void blocks_unroll_2_8(const float *m1, const float *m2, float *m3, int size)
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
                        for(int k=bk; k<kmax; k+=2)
                        { 
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=8)
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
                    }
                }
            }
        }
    }

    void blocks_unroll_2_16(const float *m1, const float *m2, float *m3, int size)
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
                        for(int k=bk; k<kmax; k+=2)
                        { 
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=16)
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
                    }
                }
            }
        }
    }

    void blocks_unroll_sse_1_4(const float *m1, const float *m2, float *m3, int size)
    {
        __m128 r0, r1, r2;
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
                            r0 = _mm_set1_ps(m1[i*size + k]);
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=4)
                            {
                                r1 = _mm_load_ps(&m2[k*size+j]);
                                r2 = _mm_load_ps(&m3[i*size+j]);
                                r2 += r0 * r1;
                                _mm_store_ps(&m3[i*size+j], r2);
                            }
                        }
                    }
                }
            }
        }
    }

    void blocks_unroll_sse_1_8(const float *m1, const float *m2, float *m3, int size)
    {
        __m128 r0, r1, r2, r3, r4;
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
                            r0 = _mm_set1_ps(m1[i*size + k]);
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=8)
                            {
                                r1 = _mm_load_ps(&m2[k*size+j]);
                                r2 = _mm_load_ps(&m2[k*size+j+4]);
                                r3 = _mm_load_ps(&m3[i*size+j]);
                                r4 = _mm_load_ps(&m3[i*size+j+4]);
                                r3 += r0 * r1;
                                r4 += r0 * r2;
                                _mm_store_ps(&m3[i*size+j], r3);
                                _mm_store_ps(&m3[i*size+j+4], r4);
                            }
                        }
                    }
                }
            }
        }
    }

    void blocks_unroll_sse_1_16(const float *m1, const float *m2, float *m3, int size)
    {
        __m128 r0, r1, r2, r3, r4, r5, r6, r7, r8;
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
                            r0 = _mm_set1_ps(m1[i*size + k]);
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=16)
                            {
                                r1 = _mm_load_ps(&m2[k*size+j]);
                                r2 = _mm_load_ps(&m2[k*size+j+4]);
                                r3 = _mm_load_ps(&m2[k*size+j+8]);
                                r4 = _mm_load_ps(&m2[k*size+j+12]);
                                r5 = _mm_load_ps(&m3[i*size+j]);
                                r6 = _mm_load_ps(&m3[i*size+j+4]);
                                r7 = _mm_load_ps(&m3[i*size+j+8]);
                                r8 = _mm_load_ps(&m3[i*size+j+12]);
                                r5 += r0 * r1;
                                r6 += r0 * r2;
                                r7 += r0 * r3;
                                r8 += r0 * r4;
                                _mm_store_ps(&m3[i*size+j], r5);
                                _mm_store_ps(&m3[i*size+j+4], r6);
                                _mm_store_ps(&m3[i*size+j+8], r7);
                                _mm_store_ps(&m3[i*size+j+12], r8);
                            }
                        }
                    }
                }
            }
        }
    }

    void blocks_unroll_sse_2_4(const float *m1, const float *m2, float *m3, int size)
    {
        __m128 r0, r1, r2, r3, r4;
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
                        for(int k=bk; k<kmax; k+=2)
                        { 
                            r0 = _mm_set1_ps(m1[i*size + k]);
                            r1 = _mm_set1_ps(m1[i*size + k + 1]);
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=4)
                            {
                                r2 = _mm_load_ps(&m2[k*size+j]);
                                r3 = _mm_load_ps(&m2[(k+1)*size+j]);
                                r4 = _mm_load_ps(&m3[i*size+j]);
                                r4 += r0 * r2 + r1 * r3;
                                _mm_store_ps(&m3[i*size+j], r4);
                            }
                        }
                    }
                }
            }
        }
    }

    void blocks_unroll_sse_2_8(const float *m1, const float *m2, float *m3, int size)
    {
        __m128 r0, r1, r2, r3, r4, r5, r6, r7;
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
                        for(int k=bk; k<kmax; k+=2)
                        { 
                            r0 = _mm_set1_ps(m1[i*size + k]);
                            r1 = _mm_set1_ps(m1[i*size + k + 1]);
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=8)
                            {
                                r2 = _mm_load_ps(&m2[k*size+j]);
                                r3 = _mm_load_ps(&m2[k*size+j+4]);
                                r4 = _mm_load_ps(&m2[(k+1)*size+j]);
                                r5 = _mm_load_ps(&m2[(k+1)*size+j+4]);
                                r6 = _mm_load_ps(&m3[i*size+j]);
                                r7 = _mm_load_ps(&m3[i*size+j+4]);
                                r6 += r0 * r2 + r1 * r4;
                                r7 += r0 * r3 + r1 * r5;
                                _mm_store_ps(&m3[i*size+j], r6);
                                _mm_store_ps(&m3[i*size+j+4], r7);
                            }
                        }
                    }
                }
            }
        }
    }

    void blocks_unroll_sse_2_16(const float *m1, const float *m2, float *m3, int size)
    {
        __m128 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13;
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
                        for(int k=bk; k<kmax; k+=2)
                        { 
                            r0 = _mm_set1_ps(m1[i*size + k]);
                            r1 = _mm_set1_ps(m1[i*size + k + 1]);
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=16)
                            {
                                r2 = _mm_load_ps(&m2[k*size+j]);
                                r3 = _mm_load_ps(&m2[k*size+j+4]);
                                r4 = _mm_load_ps(&m2[k*size+j+8]);
                                r5 = _mm_load_ps(&m2[k*size+j+12]);
                                r6 = _mm_load_ps(&m2[(k+1)*size+j]);
                                r7 = _mm_load_ps(&m2[(k+1)*size+j+4]);
                                r8 = _mm_load_ps(&m2[(k+1)*size+j+8]);
                                r9 = _mm_load_ps(&m2[(k+1)*size+j+12]);
                                r10 = _mm_load_ps(&m3[i*size+j]);
                                r11 = _mm_load_ps(&m3[i*size+j+4]);
                                r12 = _mm_load_ps(&m3[i*size+j+8]);
                                r13 = _mm_load_ps(&m3[i*size+j+12]);
                                r10 += r0 * r2 + r1 * r6;
                                r11 += r0 * r3 + r1 * r7;
                                r12 += r0 * r4 + r1 * r8;
                                r13 += r0 * r5 + r1 * r9;
                                _mm_store_ps(&m3[i*size+j], r10);
                                _mm_store_ps(&m3[i*size+j+4], r11);
                                _mm_store_ps(&m3[i*size+j+8], r12);
                                _mm_store_ps(&m3[i*size+j+12], r13);
                            }
                        }
                    }
                }
            }
        }
    }

    void blocks_unroll_sse_4_4(const float *m1, const float *m2, float *m3, int size)
    {
        __m128 r0, r1, r2, r3, r4, r5, r6, r7, r8;
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
                        for(int k=bk; k<kmax; k+=4)
                        { 
                            r0 = _mm_set1_ps(m1[i*size + k]);
                            r1 = _mm_set1_ps(m1[i*size + k + 1]);
                            r2 = _mm_set1_ps(m1[i*size + k + 2]);
                            r3 = _mm_set1_ps(m1[i*size + k + 3]);
                            int jmax = std::min(bj+BLOCK_SIZE,size);
                            for(int j=bj; j<jmax; j+=4)
                            {
                                r4 = _mm_load_ps(&m2[k*size+j]);
                                r5 = _mm_load_ps(&m2[(k+1)*size+j]);
                                r6 = _mm_load_ps(&m2[(k+2)*size+j]);
                                r7 = _mm_load_ps(&m2[(k+3)*size+j]);
                                r8 = _mm_load_ps(&m3[i*size+j]);
                                r8 += r0*r4 + r1*r5 + r2*r6 + r3*r7;
                                _mm_store_ps(&m3[i*size+j], r8);
                            }
                        }
                    }
                }
            }
        }
    }

    void blocks_unroll_sse_4_8(const float *m1, const float *m2, float *m3, int size)
    {
        __m128 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13;
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
