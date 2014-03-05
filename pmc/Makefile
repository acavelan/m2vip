# Compiler
CXX=g++
CXXFLAGS=-std=c++11 -Wall -O2 -march=native

# Options
BLOCK_SIZE=128
CHECK=false

# Benchmark
MATSIZE=2048
REPEAT=10

all: naive all_cache all_blocks all_omp

all_cache: dirs mat cache all_cache_unroll all_cache_sse
all_cache_unroll: dirs mat cache_unroll_1_4 cache_unroll_1_8 cache_unroll_1_16 cache_unroll_2_4 cache_unroll_2_8 cache_unroll_2_16
all_cache_sse: dirs mat cache_unroll_sse_1_4 cache_unroll_sse_1_8 cache_unroll_sse_1_16 cache_unroll_sse_2_4 cache_unroll_sse_2_8 cache_unroll_sse_2_16 cache_unroll_sse_4_4 cache_unroll_sse_4_8

all_blocks: dirs mat blocks all_blocks_unroll all_blocks_sse
all_blocks_unroll: dirs mat blocks_unroll_1_4 blocks_unroll_1_8 blocks_unroll_1_16 blocks_unroll_2_4 blocks_unroll_2_8 blocks_unroll_2_16
all_blocks_sse: dirs mat blocks_unroll_sse_1_4 blocks_unroll_sse_1_8 blocks_unroll_sse_1_16 blocks_unroll_sse_2_4 blocks_unroll_sse_2_8 blocks_unroll_sse_2_16 blocks_unroll_sse_4_4 blocks_unroll_sse_4_8

all_omp: dirs mat naive_omp cache_omp blocks_omp

benchmark: clean_benchmarks benchmark_naive benchmark_cache benchmark_blocks
benchmark_naive:
	@for p in bin/naive*; do \
		echo "Benchmarking $$p for size=$(MATSIZE) ($(REPEAT) times) ..."; \
		for i in {0..$(REPEAT)}; do \
			./$$p $(MATSIZE) >> $$p.times; \
		done; \
	done

benchmark_cache:
	@for p in bin/cache*; do \
		echo "Benchmarking $$p for size=$(MATSIZE) ($(REPEAT) times) ..."; \
		for i in {0..$(REPEAT)}; do \
			./$$p $(MATSIZE) >> $$p.times; \
		done; \
	done

benchmark_blocks:
	@for p in bin/blocks*; do \
		echo "Benchmarking $$p for size=$(MATSIZE) ($(REPEAT) times) ..."; \
		for i in {0..$(REPEAT)}; do \
			./$$p $(MATSIZE) >> $$p.times; \
		done; \
	done

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

cache_unroll_sse_1_4: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_sse_1_8: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_sse_1_16: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_sse_2_4: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_sse_2_8: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_sse_2_16: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_sse_4_4: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

cache_unroll_sse_4_8: dirs mat_cache
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/cache.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_1_4: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_1_8: dirs mat_blocks 
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_1_16: dirs mat_blocks 
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_2_4: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_2_8: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_2_16: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_sse_1_4: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_sse_1_8: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_sse_1_16: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_sse_2_4: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_sse_2_8: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_sse_2_16: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_sse_4_4: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

blocks_unroll_sse_4_8: dirs mat_blocks
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/blocks.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@

naive_omp: dirs mat_threads
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/threads.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@ -fopenmp

cache_omp: dirs mat_threads
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/threads.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@ -fopenmp

blocks_omp: dirs mat_threads
	$(CXX) -o bin/$@ src/main.cpp obj/mat.o obj/threads.o $(CXXFLAGS) -DCHECK=$(CHECK) -DMUL_FUNC=$@ -fopenmp

mat: dirs
	$(CXX) -o obj/mat.o -c src/mat/mat.cpp $(CXXFLAGS)

mat_naive: dirs mat
	$(CXX) -o obj/naive.o -c src/mat/naive.cpp $(CXXFLAGS)

mat_cache: dirs mat
	$(CXX) -o obj/cache.o -c src/mat/cache.cpp $(CXXFLAGS)

mat_blocks: dirs mat
	$(CXX) -o obj/blocks.o -c src/mat/blocks.cpp $(CXXFLAGS) -DBLOCK_SIZE=$(BLOCK_SIZE)
	
mat_threads: dirs mat
	$(CXX) -o obj/threads.o -c src/mat/threads.cpp $(CXXFLAGS) -DBLOCK_SIZE=$(BLOCK_SIZE) -fopenmp
	
dirs:
	mkdir -p bin obj

clean:
	rm -f obj/*.o
	rmdir obj

clean_benchmarks:
	rm -f bin/*.times

cleanall: clean clean_benchmarks
	rm -f bin/{naive*,cache*,blocks*}
	rmdir bin
