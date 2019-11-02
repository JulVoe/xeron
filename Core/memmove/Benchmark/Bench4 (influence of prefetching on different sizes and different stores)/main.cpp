#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <x86intrin.h>
#include <chrono>
#include <windows.h>
#include "../../../SystemInfo/Makros.hpp"
#undef WINDOWS
#define WINDOWS

#ifdef WINDOWS
#include <windows.h>
#include <mmsystem.h>
#elif defined(LINUX)
#include <sys/time.h>
#include <unistd.h>
#else
#error it can only be compiled under windows or unix
#endif

#define SLEEP_TIME 100
#define RUNS /*0x10000000*/(2*256+129)* 256*4
#define MEASURE_TYPE microseconds
#include <vector>
#include <iostream>
std::vector<double> output[120]; //First 8 are benchmark, last 2 random_benchmark. First for are always algo1, then algo2

void sleepms(unsigned int millisec)
{
	Sleep(millisec);
}

#define ma(a,b) (((a)>(b))?(a):(b))
#define mi(a,b) (((a)<(b))?(a):(b))

template<typename ALG1, typename ALG2>
volatile void algo8_b(volatile char* RESTRICT sta, volatile char* RESTRICT sto, ALG1 l, ALG2 s, size_t li, size_t SIZE) {
	unsigned c_s = li * CACHE_LINE_SIZE;
	volatile char* end = sta + SIZE; 
	for (unsigned int i = 0; i != c_s; i += CACHE_LINE_SIZE) {
		_mm_prefetch((char*)sta + i, _MM_HINT_NTA); 
	}
	for (volatile char* p = sta, *d = sto; p != end; p += 256, d += 256) {
		__m256i ymm0 = l((__m256i*)(p + 0)); 
		__m256i ymm1 = l((__m256i*)(p + 32 ));
		__m256i ymm2 = l((__m256i*)(p + 64 ));
		__m256i ymm3 = l((__m256i*)(p + 96 ));
		__m256i ymm4 = l((__m256i*)(p + 128));
		__m256i ymm5 = l((__m256i*)(p + 160));
		__m256i ymm6 = l((__m256i*)(p + 192));
		__m256i ymm7 = l((__m256i*)(p + 224));
		_mm_prefetch((char*)p + c_s + 0  , _MM_HINT_NTA );
		_mm_prefetch((char*)p + c_s + 64 , _MM_HINT_NTA);
		_mm_prefetch((char*)p + c_s + 128, _MM_HINT_NTA);
		_mm_prefetch((char*)p + c_s + 192, _MM_HINT_NTA);
		s((__m256i*)(d + 0  ),ymm0);
		s((__m256i*)(d + 32 ),ymm1);
		s((__m256i*)(d + 64 ),ymm2);
		s((__m256i*)(d + 96 ),ymm3);
		s((__m256i*)(d + 128),ymm4);
		s((__m256i*)(d + 160),ymm5);
		s((__m256i*)(d + 192),ymm6);
		s((__m256i*)(d + 224),ymm7);
	}
}

//sta,sto mark the region with lenght SIZE, l and s are the store and load algorithms, n is the number of cache lines
//Prefetch before,unroll 4
template<typename ALG1, typename ALG2>
volatile void algo4_b(volatile char* RESTRICT sta, volatile char* RESTRICT sto, ALG1 l, ALG2 s, size_t li, size_t SIZE) {
	unsigned c_s = li * CACHE_LINE_SIZE;
	volatile char* end = sta + SIZE;
	for (unsigned int i = 0; i != c_s; i += CACHE_LINE_SIZE) {
		_mm_prefetch((char*)sta + i, _MM_HINT_NTA);
	}
	for (volatile char* p = sta, *d = sto; p != end; p += 128, d += 128) {
		__m256i ymm0 = l((__m256i*)(p + 0));
		__m256i ymm1 = l((__m256i*)(p + 32));
		__m256i ymm2 = l((__m256i*)(p + 64));
		__m256i ymm3 = l((__m256i*)(p + 96));
		_mm_prefetch((char*)p + c_s + 0, _MM_HINT_NTA);
		_mm_prefetch((char*)p + c_s + 64, _MM_HINT_NTA);
		s((__m256i*)(d + 0), ymm0);
		s((__m256i*)(d + 32), ymm1);
		s((__m256i*)(d + 64), ymm2);
		s((__m256i*)(d + 96), ymm3);
	}
}


//sta,sto mark the region with lenght SIZE, l and s are the store and load algorithms, n is the number of cache lines
//Prefetch before,unroll 2
template<typename ALG1, typename ALG2>
volatile void algo2_b(volatile char* RESTRICT sta, volatile char* RESTRICT sto, ALG1 l, ALG2 s, size_t li, size_t SIZE) {
	unsigned c_s = li * CACHE_LINE_SIZE;
	volatile char* end = sta + SIZE;
	for (unsigned int i = 0; i != c_s; i += CACHE_LINE_SIZE) {
		_mm_prefetch((char*)sta + i, _MM_HINT_NTA);
	}
	for (volatile char* p = sta, *d = (char*)sto; p != end; p += 64, d += 64) {
		__m256i ymm0 = l((__m256i*)(p + 0));
		__m256i ymm1 = l((__m256i*)(p + 32));
		_mm_prefetch((char*)p + c_s + 0, _MM_HINT_NTA);
		s((__m256i*)(d + 0), ymm0);
		s((__m256i*)(d + 32), ymm1);
	}
}


//sta,sto mark the region with lenght SIZE, l and s are the store and load algorithms, n is the number of cache lines, r the radius of preftech-forecast
//Don't Prefetch before,unroll 8
template<typename ALG1, typename ALG2>
volatile void algo8_n(volatile char* RESTRICT sta, volatile char* RESTRICT sto, ALG1 l, ALG2 s, size_t r, size_t SIZE) {
	volatile char* end = sta + SIZE;
	for (volatile char* p = sta, *d = sto; p != end; p += 256, d += 256) {
		__m256i ymm0 = l((__m256i*)(p + 0));
		__m256i ymm1 = l((__m256i*)(p + 32));
		__m256i ymm2 = l((__m256i*)(p + 64));
		__m256i ymm3 = l((__m256i*)(p + 96));
		__m256i ymm4 = l((__m256i*)(p + 128));
		__m256i ymm5 = l((__m256i*)(p + 160));
		__m256i ymm6 = l((__m256i*)(p + 192));
		__m256i ymm7 = l((__m256i*)(p + 224));
		_mm_prefetch((char*)p + r + 0, _MM_HINT_NTA);
		_mm_prefetch((char*)p + r + 64, _MM_HINT_NTA);
		_mm_prefetch((char*)p + r + 128, _MM_HINT_NTA);
		_mm_prefetch((char*)p + r + 192, _MM_HINT_NTA);
		s((__m256i*)(d + 0), ymm0);
		s((__m256i*)(d + 32), ymm1);
		s((__m256i*)(d + 64), ymm2);
		s((__m256i*)(d + 96), ymm3);
		s((__m256i*)(d + 128), ymm4);
		s((__m256i*)(d + 160), ymm5);
		s((__m256i*)(d + 192), ymm6);
		s((__m256i*)(d + 224), ymm7);
	}
}

//sta,sto mark the region with lenght SIZE, l and s are the store and load algorithms, n is the number of cache lines, r the radius of preftech-forecast
//Don't Prefetch before,unroll 4
template<typename ALG1, typename ALG2>
volatile void algo4_n(volatile char* RESTRICT sta, volatile char* RESTRICT sto, ALG1 l, ALG2 s, size_t r, size_t SIZE) {
	volatile char* end = sta + SIZE;
	for (volatile char* p = sta, *d = sto; p != end; p += 128, d += 128) {
		__m256i ymm0 = l((__m256i*)(p + 0));
		__m256i ymm1 = l((__m256i*)(p + 32));
		__m256i ymm2 = l((__m256i*)(p + 64));
		__m256i ymm3 = l((__m256i*)(p + 96));
		_mm_prefetch((char*)p + r + 0, _MM_HINT_NTA);
		_mm_prefetch((char*)p + r + 64, _MM_HINT_NTA);
		s((__m256i*)(d + 0), ymm0);
		s((__m256i*)(d + 32), ymm1);
		s((__m256i*)(d + 64), ymm2);
		s((__m256i*)(d + 96), ymm3);
	}
}

//sta,sto mark the region with lenght SIZE, l and s are the store and load algorithms, n is the number of cache lines, r the radius of preftech-forecast
//Don't Prefetch before,unroll 2
template<typename ALG1, typename ALG2>
volatile void algo2_n(volatile char* RESTRICT sta, volatile char* RESTRICT sto, ALG1 l, ALG2 s, size_t r, size_t SIZE) {
	volatile char* end = sta + SIZE;
	for (volatile char* p = sta, *d = sto; p != end; p += 64, d += 64) {
		__m256i ymm0 = l((__m256i*)(p + 0));
		__m256i ymm1 = l((__m256i*)(p + 32));
		_mm_prefetch((char*)p + r + 0, _MM_HINT_NTA);
		s((__m256i*)(d + 0), ymm0);
		s((__m256i*)(d + 32), ymm1);
	}
}

#define test(alg,index);\
	sleepms(SLEEP_TIME);\
	src += 64;\
	dst += 64;\
	__builtin___clear_cache ((char*)dst,(char*)src+size);\
	sta = std::chrono::high_resolution_clock::now();\
	for (k = times; k != 0; k--) {\
		asm volatile("" ::: "memory");\
		alg;\
		std::cout<<*(src+  (int)(((double)rand()/(double)RAND_MAX)*size));\
		asm volatile("" ::: "memory");\
	}\
	sto = std::chrono::high_resolution_clock::now();\
	output[index].push_back((256*std::chrono::duration_cast<std::chrono::MEASURE_TYPE>(sto - sta).count())/times);

#define bch1(al,add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_store_si256, 8, size), 0+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_store_si256, 8, size), 1+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_stream_si256, 8, size), 2+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_stream_si256, 8, size), 3+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_store_si256, 16, size), 4+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_store_si256, 16, size), 5+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_stream_si256, 16, size), 6+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_stream_si256, 16, size), 7+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_store_si256, 32, size), 8+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_store_si256, 32, size), 9+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_stream_si256, 32, size), 10+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_stream_si256, 32, size), 11+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_store_si256, 64, size), 12+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_store_si256, 64, size), 13+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_stream_si256, 64, size), 14+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_stream_si256, 64, size), 15+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_store_si256, 128, size), 16+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_store_si256, 128, size), 17+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_stream_si256, 128, size), 18+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_stream_si256, 128, size), 19+add);

#define bch2(al,add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_store_si256, 32, size), 0+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_store_si256, 32, size), 1+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_stream_si256, 32, size), 2+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_stream_si256, 32, size), 3+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_store_si256, 64, size), 4+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_store_si256, 64, size), 5+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_stream_si256, 64, size), 6+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_stream_si256, 64, size), 7+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_store_si256, 128, size), 8+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_store_si256, 128, size), 9+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_stream_si256, 128, size), 10+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_stream_si256, 128, size), 11+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_store_si256, 256, size), 12+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_store_si256, 256, size), 13+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_stream_si256, 256, size), 14+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_stream_si256, 256, size), 15+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_store_si256, 512, size), 16+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_store_si256, 512, size), 17+add);\
	test(al(src,dst,_mm256_loadu_si256, _mm256_stream_si256, 512, size), 18+add);\
	test(al(src,dst,_mm256_lddqu_si256, _mm256_stream_si256, 512, size), 19+add);

void benchmark(size_t size, int times)
{
	char * DATA1 = (char*)malloc(size + 128*64) + 64;
	char * DATA2 = (char*)malloc(size + 128*64) + 64;
	size_t LINEAR1 = mi( (size_t)DATA1 , (size_t)DATA2 );
	size_t LINEAR2 = ma( (size_t)DATA1, (size_t)DATA2);
	volatile char *dst = (char*)(((64 - (LINEAR1 & 63)) & 63) + LINEAR1);
	volatile char *src = (char*)(((64 - (LINEAR2 & 63)) & 63) + LINEAR2);
	int k;
	auto sta = std::chrono::high_resolution_clock::now();
	auto sto = std::chrono::high_resolution_clock::now();

	bch1(algo8_b,0);
	bch1(algo4_b,20);
	bch1(algo2_b,40);
	bch2(algo8_n,60);
	bch2(algo4_n,80);
	bch2(algo2_n,100);
	
	free(DATA1);
	free(DATA2);
}


int main(void)
{	
	for (int i = 256; i != 129*256; i+=256) {
		benchmark(i, RUNS/i);
		std::cout << (i>>8) << std::endl;
	}
	
	for (int i = 2*256*256; i != (2*256+129)* 256; i += 256) {
		benchmark(i, RUNS / i);
		std::cout << (i >> 8) << std::endl;
	}
	
#define OUTPUT_LINE(ind);\
	for(unsigned i=0; i!=output[ind].size(); ++i)\
		std::cout<<(double)((output[ind][i]*i)/RUNS)<<" ";\
	std::cout<<'\n';

	for (int index = 0; index != 120; index++) {
		OUTPUT_LINE(index);
	}
	
	return 0;
}