# Compiler
CXX=g++
CXXFLAGS=-std=c++11 -Wall -O2

# Options
BLOCK_SIZE=80
CHECK=false

all: dirs mat naive all_cache all_blocks

all_cache: cache all_cache_unroll all_cache_sse 
all_cache_unroll: cache_unroll_1_4 cache_unroll_1_8 cache_unroll_1_16 cache_unroll_2_4 cache_unroll_2_8 cache_unroll_2_16
all_cache_sse:

all_blocks: blocks all_blocks_unroll all_blocks_sse
all_blocks_unroll:
all_blocks_sse:

naive: dirs mat_naive
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/$@.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_1_4: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_1_8: dirs mat_cache 
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_1_16: dirs mat_cache 
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_2_4: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_2_8: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_2_16: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

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