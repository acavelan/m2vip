# Compiler
CXX=g++
CXXFLAGS=-std=c++11 -O2 -march=native
INCLUDES=-Ieigen

# Options
CHECK=true

all: 
	$(CXX) -o eigen_multiply main.cpp $(INCLUDES) $(CXXFLAGS) -DCHECK=$(CHECK)

clean:
	rm -f *.o

cleanall: clean
	rm -f eigen_multiply
