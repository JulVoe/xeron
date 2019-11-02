#include <x86intrin.h>
#include <chrono>
#include <iostream>
#include <Windows.h>

#define RUNS 50
#define SIZE 64*1024*64*64
#define bench1(algo1,algo2,var);\
    sta = std::chrono::high_resolution_clock::now();\
	for(int t=0; t!=RUNS; ++t){\
		for (unsigned i = 0; i != SIZE; i += 256) { \
			__m256i ymm0 = algo1((__m256i*)(src + i)); \
			__m256i ymm1 = algo1((__m256i*)(src + i + 32)); \
			__m256i ymm2 = algo1((__m256i*)(src + i + 64)); \
			__m256i ymm3 = algo1((__m256i*)(src + i + 96)); \
			__m256i ymm4 = algo1((__m256i*)(src + i + 128)); \
			__m256i ymm5 = algo1((__m256i*)(src + i + 160)); \
			__m256i ymm6 = algo1((__m256i*)(src + i + 192)); \
			__m256i ymm7 = algo1((__m256i*)(src + i + 224)); \
			algo2((__m256i*)(dst + i), ymm0); \
			algo2((__m256i*)(dst + i + 32), ymm1); \
			algo2((__m256i*)(dst + i + 64), ymm2); \
			algo2((__m256i*)(dst + i + 96), ymm3); \
			algo2((__m256i*)(dst + i + 128), ymm4); \
			algo2((__m256i*)(dst + i + 160), ymm5); \
			algo2((__m256i*)(dst + i + 192), ymm6); \
			algo2((__m256i*)(dst + i + 224), ymm7); \
			sto = std::chrono::high_resolution_clock::now();\
		}\
	}\
	sto = std::chrono::high_resolution_clock::now(); \
	var = std::chrono::duration_cast<std::chrono::microseconds>(sto - sta).count();
#define bench2(algo1,algo2,var);\
    sta = std::chrono::high_resolution_clock::now();\
	for(int t=0; t!=RUNS; ++t){\
		for (unsigned i = 0; i != SIZE; i += 256) { \
			__m256i ymm0 = algo1((__m256i*)(src + i)); \
			algo2((__m256i*)(dst + i), ymm0); \
			__m256i ymm1 = algo1((__m256i*)(src + i + 32)); \
			algo2((__m256i*)(dst + i + 32), ymm1); \
			__m256i ymm2 = algo1((__m256i*)(src + i + 64)); \
			algo2((__m256i*)(dst + i + 64), ymm2); \
			__m256i ymm3 = algo1((__m256i*)(src + i + 96)); \
			algo2((__m256i*)(dst + i + 96), ymm3); \
			__m256i ymm4 = algo1((__m256i*)(src + i + 128)); \
			algo2((__m256i*)(dst + i + 128), ymm4); \
			__m256i ymm5 = algo1((__m256i*)(src + i + 160)); \
			algo2((__m256i*)(dst + i + 160), ymm5); \
			__m256i ymm6 = algo1((__m256i*)(src + i + 192)); \
			algo2((__m256i*)(dst + i + 192), ymm6); \
			__m256i ymm7 = algo1((__m256i*)(src + i + 224)); \
			algo2((__m256i*)(dst + i + 224), ymm7); \
			sto = std::chrono::high_resolution_clock::now();\
		}\
	}\
	sto = std::chrono::high_resolution_clock::now(); \
	var = std::chrono::duration_cast<std::chrono::microseconds>(sto - sta).count();
#define bench3(algo1,algo2,var);\
    sta = std::chrono::high_resolution_clock::now();\
	for(int t=0; t!=RUNS; ++t){\
		for (unsigned i = 0; i != SIZE; i += 256) { \
			__m256i ymm0 = algo1((__m256i*)(src + i)); \
			algo2((__m256i*)(dst + i), ymm0); \
			ymm0 = algo1((__m256i*)(src + i + 32)); \
			algo2((__m256i*)(dst + i + 32), ymm0); \
			ymm0 = algo1((__m256i*)(src + i + 64)); \
			algo2((__m256i*)(dst + i + 64), ymm0); \
			ymm0 = algo1((__m256i*)(src + i + 96)); \
			algo2((__m256i*)(dst + i + 96), ymm0); \
			ymm0 = algo1((__m256i*)(src + i + 128)); \
			algo2((__m256i*)(dst + i + 128), ymm0); \
			ymm0 = algo1((__m256i*)(src + i + 160)); \
			algo2((__m256i*)(dst + i + 160), ymm0); \
			ymm0 = algo1((__m256i*)(src + i + 192)); \
			algo2((__m256i*)(dst + i + 192), ymm0); \
			ymm0 = algo1((__m256i*)(src + i + 224)); \
			algo2((__m256i*)(dst + i + 224), ymm0); \
			sto = std::chrono::high_resolution_clock::now();\
		}\
	}\
	sto = std::chrono::high_resolution_clock::now(); \
	var = std::chrono::duration_cast<std::chrono::microseconds>(sto - sta).count();
#define flush();\
	for(int i=0; i!=SIZE; i+=256){\
		_mm_clflush(src + i);\
		_mm_clflush(dst + i);\
	}\
	Sleep(100);


int main() {
	char* src = (char*)aligned_alloc(256, SIZE * sizeof(char)); //Greater than L1 cache
	char* dst = (char*)aligned_alloc(256, SIZE * sizeof(char)); //Greater than L1 cache
	auto sta = std::chrono::high_resolution_clock::now();
	auto sto = std::chrono::high_resolution_clock::now();
	long long unsigned t1, t2, t3, t4;
	for (int ra = 0; ra != 4; ra++) {
		bench1(_mm256_load_si256, _mm256_store_si256, t1);
		flush();
		bench1(_mm256_load_si256, _mm256_store_si256, t2);
		flush();
		bench1(_mm256_load_si256, _mm256_stream_si256, t3);
		flush();
		bench1(_mm256_stream_load_si256, _mm256_stream_si256, t4);
		std::cout << t1 << " " << t2 << " " << t3 << " " << t4 << std::endl;

		flush();
		bench2(_mm256_load_si256, _mm256_store_si256, t1);
		flush();
		bench2(_mm256_stream_load_si256, _mm256_store_si256, t2);
		flush();
		bench2(_mm256_load_si256, _mm256_stream_si256, t3);
		flush();
		bench2(_mm256_stream_load_si256, _mm256_stream_si256, t4);
		std::cout << t1 << " " << t2 << " " << t3 << " " << t4 << std::endl;

		flush();
		bench3(_mm256_load_si256, _mm256_store_si256, t1);
		flush();
		bench3(_mm256_stream_load_si256, _mm256_store_si256, t2);
		flush();
		bench3(_mm256_load_si256, _mm256_stream_si256, t3);
		flush();
		bench3(_mm256_stream_load_si256, _mm256_stream_si256, t4);
		std::cout << t1 << " " << t2 << " " << t3 << " " << t4 << "\n--------------------------------------\n" << std::endl;
	}
	return 0;
}