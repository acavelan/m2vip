#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <immintrin.h>

void display(float *a)
{
    printf("%.1f, %.1f, %.1f, %.1f\n", a[0], a[1], a[2], a[3]);
}

int main(void)
{
    alignas(16) float a0[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float a1[4] = {5.0f, 6.0f, 7.0f, 8.0f};
    alignas(16) float a2[4];
    
    display(a0);
    display(a1);

    __m128 r0, r1;
    
    r0 = _mm_load_ps(a0);
    r1 = _mm_load_ps(a1);

    printf("shuffle(a0, a1) :1, 0, 3, 2\n");
    
    r0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 0, 3, 2));
    
    _mm_store_ps(a2, r0);
    
    display(a2);
    
    return 0;
}
