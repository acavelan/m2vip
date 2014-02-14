#ifndef MAT_BLOCKS_HPP
#define MAT_BLOCKS_HPP

namespace mat
{
	void blocks(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_1_4(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_1_8(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_1_16(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_2_4(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_2_8(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_2_16(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_sse_1_4(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_sse_1_8(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_sse_1_16(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_sse_2_4(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_sse_2_8(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_sse_2_16(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_sse_4_4(const float *m1, const float *m2, float *m3, int size);

	void blocks_unroll_sse_4_8(const float *m1, const float *m2, float *m3, int size);
}

#endif