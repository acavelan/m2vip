CC=g++ -W -Wall -Ofast -s -m64 -march=native -fexcess-precision=fast
NVCC=nvcc
CFLAGS=-O2 -W -Wall
LDFLAGS=-lIL -lm
OBJ=utils.o

all: $(OBJ)
	$(NVCC) -o sobel sobel.cu $^ $(LDFLAGS) -O3
	$(NVCC) -o sobel_stream sobel_stream.cu $^ $(LDFLAGS) -O3
	$(NVCC) -o sobel_zerocopy sobel_zerocopy.cu $^ $(LDFLAGS) -O3
	$(NVCC) -o histo histo.cu $^ $(LDFLAGS) -arch sm_20 -O3
	$(NVCC) -o histo_atomic histo_atomic.cu $^ $(LDFLAGS) -arch sm_20 -O3
	$(NVCC) -o histo_stream histo_stream.cu $^ $(LDFLAGS) -arch sm_20 -O3
	$(NVCC) -o histo_zerocopy histo_zerocopy.cu $^ $(LDFLAGS) -arch sm_20 -O3

%.o: %.c
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -f *.o
