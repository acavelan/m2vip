# Compiler
CXX=g++
CXXFLAGS=-std=c++11 -Wall -O2 -march=native
INCLUDES=-I../../

# Options
CHECK=true

all: 
	$(CXX) -o cblas_multiply main.cpp ../../mat/mat.cpp $(INCLUDES) $(CXXFLAGS) -DCHECK=$(CHECK)

clean:
	rm -f *.o

cleanall: clean
	rm -f cblas_multiply
