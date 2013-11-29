BIN=img
CFLAGS=-O2 -W -Wall -Wextra
LDFLAGS=-lm -lsfml-system -lsfml-window -lsfml-graphics -fopenmp
CXX=g++

all:
	$(CXX) $(CFLAGS) -o stamp stamp.cpp $(LDFLAGS)
	$(CXX) $(CFLAGS) -c fft_utils.cpp
	$(CXX) $(CFLAGS) -o stampfft stampfft.cpp fft_utils.o $(LDFLAGS) -I. -Iunsupported -lfftw3
	$(CXX) $(CFLAGS) -o stampfft_check stampfft_check.cpp fft_utils.o $(LDFLAGS) -I. -Iunsupported -lfftw3
	$(CXX) $(CFLAGS) -o stamp_check stamp_check.cpp $(LDFLAGS)

