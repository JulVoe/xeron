#include "../../apex_memmove.cpp"
#include "../../FastMemcpy_Avx.hpp"
#include "../../jvmemcpy.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <chrono>
#include <windows.h>

#ifdef WINDOWS
#include <windows.h>
#include <mmsystem.h>
#elif defined(LINUX)
#include <sys/time.h>
#include <unistd.h>
#else
#error it can only be compiled under windows or unix
#endif


#define algo1 apex::kryptonite
#define algo2 apex::tiberium

void sleepms(unsigned int millisec)
{
	Sleep(millisec);
}

#define SLEEP_TIME 75
#define RUNS 0x10000000
#define MEASURE_TYPE nanoseconds
#ifndef TEXT_OUTPUT
#include <vector>
#include <iostream>
std::vector<double> output[10]; //First 8 are benchmark, last 2 random_benchmark. First for are always algo1, then algo2
#endif

void benchmark(int dstalign, int srcalign, size_t size, int times)
{
	char *DATA1 = (char*)malloc(size + 64);
	char *DATA2 = (char*)malloc(size + 64);
	size_t LINEAR1 = ((size_t)DATA1);
	size_t LINEAR2 = ((size_t)DATA2);
	char *ALIGN1 = (char*)(((64 - (LINEAR1 & 63)) & 63) + LINEAR1);
	char *ALIGN2 = (char*)(((64 - (LINEAR2 & 63)) & 63) + LINEAR2);
	char *dst = (dstalign) ? ALIGN1 : (ALIGN1 + 1);
	char *src = (srcalign) ? ALIGN2 : (ALIGN2 + 3);
	int k;

	sleepms(SLEEP_TIME);
	auto sta = std::chrono::high_resolution_clock::now();
	for (k = times; k > 0; k--) {
		algo1(dst, src, size);
	}
	auto sto = std::chrono::high_resolution_clock::now();
	output[dstalign + 2 * srcalign].push_back(std::chrono::duration_cast<std::chrono::MEASURE_TYPE>(sto-sta).count());
	sleepms(SLEEP_TIME);
	sta = std::chrono::high_resolution_clock::now();
	for (k = times; k > 0; k--) {
		algo2(dst, src, size);
	}
	sto = std::chrono::high_resolution_clock::now();
	output[dstalign + 2 * srcalign + 4].push_back(std::chrono::duration_cast<std::chrono::MEASURE_TYPE>(sto - sta).count());

	free(DATA1);
	free(DATA2);
}


void bench(int copysize, int times)
{
	benchmark(1, 1, copysize, times);
	benchmark(1, 0, copysize, times);
	benchmark(0, 1, copysize, times);
	benchmark(0, 0, copysize, times);
}


void random_bench(int maxsize, unsigned times)
{
	static char A[11 * 1024 * 1024 + 2];
	static char B[11 * 1024 * 1024 + 2];
	static int random_offsets[0x10000];
	static int random_sizes[0x8000];
	unsigned int i, p1, p2;
	for (i = 0; i < 0x10000; i++) {	// generate random offsets
		random_offsets[i] = rand() % (10 * 1024 * 1024 + 1);
	}
	for (i = 0; i < 0x8000; i++) {	// generate random sizes
		random_sizes[i] = 1 + rand() % maxsize;
	}
	sleepms(SLEEP_TIME);
	auto sta = std::chrono::high_resolution_clock::now();
	for (p1 = 0, p2 = 0, i = 0; i < times; i++) {
		int offset1 = random_offsets[(p1++) & 0xffff];
		int offset2 = random_offsets[(p1++) & 0xffff];
		int size = random_sizes[(p2++) & 0x7fff];
		algo1(A + offset1, B + offset2, size);
	}
	auto sto = std::chrono::high_resolution_clock::now();
	output[8].push_back(std::chrono::duration_cast<std::chrono::MEASURE_TYPE>(sto - sta).count());
	sleepms(SLEEP_TIME);
	sta = std::chrono::high_resolution_clock::now();
	for (p1 = 0, p2 = 0, i = 0; i < times; i++) {
		int offset1 = random_offsets[(p1++) & 0xffff];
		int offset2 = random_offsets[(p1++) & 0xffff];
		int size = random_sizes[(p2++) & 0x7fff];
		algo2(A + offset1, B + offset2, size);
	}
	sto = std::chrono::high_resolution_clock::now();
	output[9].push_back(std::chrono::duration_cast<std::chrono::MEASURE_TYPE>(sto - sta).count());
}

int main(void)
{
	/*
	bench(32, 0x2000000);
	bench(64, 0x2000000);
	bench(512, 0x1000000);
	bench(1024, 0x800000);
	bench(4096, 0x100000);
	bench(8192, 0x80000);
	bench(1024 * 1024 * 1, 0x1000);
	bench(1024 * 1024 * 4, 0x400);
	bench(1024 * 1024 * 8, 0x200);

	random_bench(2048, 16000000);
	random_bench(4096, 8000000);
	random_bench(8192, 4000000);
	random_bench(16384, 2000000);
	*/

	for (int i = 1; i != 513; ++i) {
		bench(i, RUNS/i);
		std::cout << i << std::endl;
	}
	bench(‭0x100000‬, 0x100); /*1024*1024*/
	bench(0x800000, 0x40);
	bench(0x400000, 0x20);
	
	for (int i = 1; i != 513; ++i) {
		random_bench(i, RUNS/i);
		std::cout << i << std::endl;
	}
	bench(‭0x100000‬, 0x100); /*1024*1024*/
	bench(0x800000, 0x40);
	bench(0x400000, 0x20);

#define OUTPUT_LINE(ind);\
	for(unsigned i=0; i!=output[ind].size(); ++i)\
		std::cout<<(double)((output[ind][i]*i)/RUNS)<<" ";\
	std::cout<<'\n';

	OUTPUT_LINE(0);
	OUTPUT_LINE(1);
	OUTPUT_LINE(2);
	OUTPUT_LINE(3);
	OUTPUT_LINE(4);
	OUTPUT_LINE(5);
	OUTPUT_LINE(6);
	OUTPUT_LINE(7);
	OUTPUT_LINE(8);
	OUTPUT_LINE(9);

	//getchar();
	return 0;
}