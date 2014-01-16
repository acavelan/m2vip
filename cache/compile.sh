#!/bin/bash

# -ftree-vectorizer-verbose=2

CXX=g++
CXXFLAGS="-Wall -s -std=c++11"

$CXX -o add add.cpp $CXXFLAGS -O3
$CXX -o add_2 add_2.cpp $CXXFLAGS -O3

$CXX -o mul mul.cpp $CXXFLAGS -O3
$CXX -o mul_2 mul_2.cpp $CXXFLAGS -O3

$CXX -o mul_2_unroll_4 mul_2_unroll_4.cpp $CXXFLAGS -O2
$CXX -o mul_2_unroll_4_sse mul_2_unroll_4_sse.cpp $CXXFLAGS -O2

$CXX -o mul_2_unroll_4_sse mul_2_unroll_4_sse.cpp $CXXFLAGS $SSEFLAGS -O2
$CXX -o mul_2_unroll_4_2_sse mul_2_unroll_4_2_sse.cpp $CXXFLAGS $SSEFLAGS -O2

$CXX -o mul_2_unroll_4_sse_threads mul_2_unroll_4_sse_threads.cpp $CXXFLAGS $SSEFLAGS -O2
