# Compiler
CXX=g++
CXXFLAGS=-std=c++11 -Wall -O3

# Options
BLOCK_SIZE=80
CHECK=false

all: dirs mat naive cache blocks
	
naive: dirs mat_naive
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/$@.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/$@.o $(CXXFLAGS) -DCHECK$(CHECK) -DMUL_FUNC=$@

blocks: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/$@.o $(CXXFLAGS) -DCHECK$(CHECK) -DMUL_FUNC=$@

mat: dirs
	$(CXX) -o obj/mat.o -c src/mat/mat.cpp $(CXXFLAGS)

mat_naive: dirs
	$(CXX) -o obj/naive.o -c src/mat/naive.cpp $(CXXFLAGS)

mat_cache: dirs
	$(CXX) -o obj/cache.o -c src/mat/cache.cpp $(CXXFLAGS)

mat_blocks: dirs
	$(CXX) -o obj/blocks.o -c src/mat/blocks.cpp $(CXXFLAGS) -DBLOCK_SIZE=$(BLOCK_SIZE)

dirs:
	mkdir -p bin obj

clean:
	rm -f obj/*.o
	rmdir obj

cleanall: clean
	rm -f bin/{naive,cache,blocks}
	rmdir bin