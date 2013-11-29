#include <sys/time.h>
#include <cuda_runtime.h>

#include "utils.h"

__global__ void apply_grayscale(unsigned char *grayimg, const unsigned char *image, int width, int height)
{
	unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
	
	if(x < width && y < height)
	{
		const unsigned char R = image[(y * width + x)*3 + 0];
		const unsigned char G = image[(y * width + x)*3 + 1];
		const unsigned char B = image[(y * width + x)*3 + 2]; 

		unsigned char gray = (307 * R + 604 * G + 113 * B) >> 10;

		grayimg[y * width + x] = gray;
	}
}

__global__ void apply_sobel(const unsigned char *input, unsigned char *output, int width, int height)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	
	if(x > 1 && y > 1 && y < height - 1 && x < width -1)
	{
		#define PX(kx) (kx + x)
		#define PY(ky) width * (ky + y)
		
		int gx = 0, gy = 0;
		
		gx += input[PX(-1) + PY(-1)];
		gx += 2 * input[PX(-1) + PY(0)];
		gx += input[PX(-1) + PY(1)];
		gx -= input[PX(1) + PY(-1)];
		gx -= 2 * input[PX(1) + PY(0)];
		gx -= input[PX(1) + PY(1)];
		
		gy += input[PX(-1) + PY(-1)];
		gy += 2 * input[PX(0) + PY(-1)];
		gy += input[PX(1) + PY(-1)];
		gy -= input[PX(-1) + PY(1)];
		gy -= 2 * input[PX(0) + PY(1)];
		gy -= input[PX(1) + PY(1)];
		
		float grad = sqrt(float(gx * gx + gy * gy));
		output[y * width + x] = grad > 255 ? 255 : grad;
	}
}
	
int main(int argc, char **argv)
{
	struct timeval start, last, now, computation;
	
	gettimeofday(&start, 0);
	
	if(argc < 2)
	{
		printf("usage: %s image\n", argv[0]);
		return 0;
	}
	
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilEnable(IL_FILE_OVERWRITE);
	
	ILboolean result = ilLoadImage(argv[1]);

	if(!result)
	{
		ILenum err = ilGetError() ;
		printf("Failed to load %s\n", argv[1]);
		printf("Error: %s\n", ilGetString(err));
	}

	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
		
	ILuint width = ilGetInteger(IL_IMAGE_WIDTH);
	ILuint height = ilGetInteger(IL_IMAGE_HEIGHT);
	
	unsigned int size = width * height;
	
	ILubyte *image = ilGetData();
	
	gettimeofday(&now, 0);
	computation = last = now;
	
	printf("Image (%d * %d) loaded in %f\n", width, height, get_time(start, now));
	
	unsigned char *d_image = 0, *d_grayimg = 0;
	unsigned char *grayimg = (unsigned char*)malloc(size * sizeof(unsigned char));
	
	cudaMalloc((void**)&d_image, size * 3 * sizeof(unsigned char));
	cudaMalloc((void**)&d_grayimg, size * sizeof(unsigned char));
    
	cudaMemcpy(d_image, (unsigned char*)image, size * 3 * sizeof(unsigned char), cudaMemcpyHostToDevice);
	
	// Execution du kernel
	dim3 blockDim(16, 16, 1);
	dim3 gridDim((width + blockDim.x - 1)/ blockDim.x, (height + blockDim.y - 1) / blockDim.y, 1);
	
	apply_grayscale<<<gridDim, blockDim, 0>>>(d_grayimg, d_image, width, height);
	
	cudaFree(d_image);
	
	gettimeofday(&now, 0);
	printf("Gray image computed in %f\n", get_time(last, now));
	last = now;
	
	unsigned char *d_sobel = 0;
	unsigned char *sobel = (unsigned char*)malloc(size * sizeof(unsigned char));
	
	cudaMalloc((void**)&d_sobel, size * sizeof(unsigned char));
	
	apply_sobel<<<gridDim, blockDim, 0>>>(d_grayimg, d_sobel, width, height);
	
	cudaMemcpy(sobel, d_sobel, size * sizeof(unsigned char), cudaMemcpyDeviceToHost);
	
	cudaFree(d_sobel);
	cudaFree(d_grayimg);
	
	cudaThreadSynchronize();
	
	gettimeofday(&now, 0);
	printf("Sobel completed in %f\n", get_time(computation, now));
	last = now;
	
	cudaDeviceReset();
	
	// Save images	
	save_gray_image("sobel.jpg", sobel, width, height);

	free(sobel);
	
	gettimeofday(&now, 0);
	printf("Result saved in %f\n", get_time(last, now));
	printf("Total time %f\n", get_time(start, now));

	return 0;
}
