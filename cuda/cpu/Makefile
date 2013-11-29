CC=g++
CFLAGS=-W -Wall -Ofast -s -m64 -march=native -fexcess-precision=fast -fopenmp

LDFLAGS=-lIL -lm
OBJ=utils.o

all: $(OBJ)
	$(CC) -o gray gray.c $^ $(LDFLAGS) $(CFLAGS)
	$(CC) -o histo histo.c $^ $(LDFLAGS) $(CFLAGS)
	$(CC) -o sobel sobel.c $^ $(LDFLAGS) $(CFLAGS)

%.o: %.c
	$(CC) -c $^

clean:
	rm -f *.o
