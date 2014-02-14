#ifndef MAT_HPP
#define MAT_HPP

namespace mat
{
	void zeros(float *m, int size);

	void random(float *m, int size);

	float* read(const char *filename, int &size);

	void write(const char *filename, float *m, int size);

	void mul(const float *m1, const float *m2, float *m3, int size);
}

#endif