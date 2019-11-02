#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <x86intrin.h>
#include <chrono>
#include <thread>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/arithmetic/div.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/control/if.hpp>
#include "../../../SystemInfo/Makros.hpp"
#include "../../../Preprozessor/mult64.hpp"

#define Windows

#ifdef WINDOWS
#include <windows.h>
#include <mmsystem.h>
#elif defined(LINUX)
#include <sys/time.h>
#include <unistd.h>
#else
#error it can only be compiled under windows or unix
#endif

#define SLEEP_TIME 50
//Runs is alwas double of RUNS
#define RUNS /*0x10000000*/(2*256+129)* 256*4*16
#define MEASURE_TYPE microseconds
#include <vector>
#include <iostream>
std::vector<double> output[120];


#define CLEAR_CACHE(_sta_,_sto_,_size_); for(unsigned i=0; i!=_size_; i+=64){_mm_clflush(_sta_ + i);_mm_clflush(_sto_ + i);}memset(anti_cache,1,2*64*1024);

#define pprefetcher(z,i,data)\
    "prefetchnta    " BOOST_PP_STRINGIZE(PP_MUL_64(i))"(%[sta])\n"
#define loader(z,i,data)\
    #data" " BOOST_PP_STRINGIZE(BOOST_PP_MUL(32,i))"(%[sta],%%rcx), %%ymm"#i"\n"
#define storer(z,i,data)\
    #data" %%ymm"#i", " BOOST_PP_STRINGIZE(BOOST_PP_MUL(32,i))"(%[sto],%%rcx)\n"
#define sprefetcher(z,i,data)\
    "prefetchnta    " BOOST_PP_STRINGIZE(BOOST_PP_MUL(64,i))"(%%rax,%%rcx)\n"

//_l = vmovups/vlddqu, _s = vmovaps/vmovntdq
//_i is a jumplabel-counter and the result-index
//_p is the number of cache lines prefetched at the start
//_n is the number of loads and stores(how many times the loop is unrolled)
#define assembly(_sta,_sto,_SIZE, _l, _s, _i, _p, _n, _do_p);\
    __asm__ volatile(\
        BOOST_PP_REPEAT(_p, pprefetcher, 0)\
        "leaq " BOOST_PP_STRINGIZE(PP_MUL_64(_p)) "(%[sta]), %%rax\n"\
        "xorl    %%ecx, %%ecx\n"\
".loop_start"#_i"%=:\n"\
        BOOST_PP_REPEAT(_n,loader,_l)\
        BOOST_PP_IF(_do_p,BOOST_PP_REPEAT(BOOST_PP_DIV(_n,2),sprefetcher,0),"")\
        BOOST_PP_REPEAT(_n,storer,_s)\
        "addq    $" BOOST_PP_STRINGIZE(BOOST_PP_MUL(32,_n))", %%rcx\n"\
        "cmpq    %%rcx, %[SIZE]\n"\
        "jne     .loop_start"#_i"%=\n"\
        :\
        : [sta] "r" (_sta), [sto] "r" (_sto), [SIZE] "r" (_SIZE)\
        : "%rcx","%rax","%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7","memory","cc"\
    );

#define bench_assembly(_sta,_sto,_SIZE, _l, _s, _i, _p, _n,_runs, _do_p);\
	std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));\
	cs = std::max((size_t)64*1024,_SIZE);\
	_sta = (char*)malloc((16*cs+256) * sizeof(char)) + 256; \
	_sto = (char*)aligned_alloc(256, (16*cs+256) * sizeof(char)) + 256;\
	sto = std::chrono::high_resolution_clock::now(); \
	for (unsigned long long iter = 0; iter != _runs; iter++) {\
		assembly((_sta+cs*offset[iter&0b1111]), (_sto+cs*offset[(iter+3)&0b1111]), _SIZE, _l, _s, _i, _p, _n, _do_p); \
		CLEAR_CACHE((_sta+cs*offset[iter&0b1111]),(_sto+cs*offset[(iter+3)&0b1111]),cs);\
	}\
	sto = std::chrono::high_resolution_clock::now();\
	output[_i].push_back(((double)(std::chrono::duration_cast<std::chrono::MEASURE_TYPE>(sto - sta).count()) / _runs)/100.0);\
	free(_sta - 256); \
	free(_sto - 256); \

int main() {
	char* p;
	char* q;
	char* anti_cache = (char*)malloc(2 * 1024 * 64 * sizeof(char));
	unsigned offset[16] = {0,11,5,12,1,8,3,13,6,14,2,10,4,9,15,7};
	unsigned cs;
	auto sta = std::chrono::high_resolution_clock::now();
	auto sto = std::chrono::high_resolution_clock::now();

	for (size_t s = 64*1024; s != 64*1024+128*256; s += 256) {
		bench_assembly(p, q, s, vmovups, vmovaps, 0, 0, 8, RUNS / s, 0);
		bench_assembly(p, q, s, vlddqu, vmovaps, 1, 0, 8, RUNS / s, 0);
		bench_assembly(p, q, s, vmovups, vmovntdq, 2, 0, 8, RUNS / s, 0);
		bench_assembly(p, q, s, vlddqu, vmovntdq, 3, 0, 8, RUNS / s, 0);

		bench_assembly(p, q, s, vmovups, vmovaps, 4, 4, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovaps, 5, 4, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vmovups, vmovntdq, 6, 4, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovntdq, 7, 4, 8, RUNS / s, 1);

		bench_assembly(p, q, s, vmovups, vmovaps, 8, 8, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovaps, 9, 8, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vmovups, vmovntdq, 10, 8, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovntdq, 11, 8, 8, RUNS / s, 1);

		bench_assembly(p, q, s, vmovups, vmovaps, 12, 16, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovaps, 13, 16, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vmovups, vmovntdq, 14, 16, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovntdq, 15, 16, 8, RUNS / s, 1);

		bench_assembly(p, q, s, vmovups, vmovaps, 16, 32, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovaps, 17, 32, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vmovups, vmovntdq, 18, 32, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovntdq, 19, 32, 8, RUNS / s, 1);

		bench_assembly(p, q, s, vmovups, vmovaps, 20, 64, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovaps, 21, 64, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vmovups, vmovntdq, 22, 64, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovntdq, 23, 64, 8, RUNS / s, 1);

		bench_assembly(p, q, s, vmovups, vmovaps, 24, 128, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovaps, 25, 128, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vmovups, vmovntdq, 26, 128, 8, RUNS / s, 1);
		bench_assembly(p, q, s, vlddqu, vmovntdq, 27, 128, 8, RUNS / s, 1);
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


#if 0
/*
	When compiling this, both gcc and clang produce sh*t assembly, substituting memove, doing uneccessary stuff, super wierd urolling and so on.
	This is why I rewrote everything in inline-assembly to keep the compiler from fucking it all up.
	I used Boost-Preprocessor to simplify, now (almost) only one Makro is needed. The only problem is, that multiplication only goes up to 256, so I had to write my own
	Makro for multiplication with 64 up to 128*64.
*/


//========================================================
auto sta = std::chrono::high_resolution_clock::now();
for (int iter = 0; iter != 1e5; iter++) {
	asm volatile( //GCC
		"sall    $16, %[li]\n"           //li*CACHE_LINE_SIZE
		"je      .L21\n"                 //li=0 0=> Ende
		"xorl    %%eax, %%eax\n"         //i=0
		".L16:\n"
		"movl    %%eax, %%edx\n"         //off=i
		"addl    $65536, %%eax\n"        //i+=CACHE_LINE_SIZE
		"prefetchnta     (%[p],%%rdx)\n" //prefetch(p+off)
		"cmpl    %%eax, %[li]\n"         //if(i==li*CACHE_LINE_SIZE)
		"jne     .L16\n"                 //else jmp loopstart
		".L21:"
		:
	: [li] "r" (li), [p] "r" (p)
		: "%rax", "%rdx"
		);
}
auto sto = std::chrono::high_resolution_clock::now();
t1 = std::chrono::duration_cast<std::chrono::microseconds>(sto - sta).count();
//========================================================

//========================================================
sta = std::chrono::high_resolution_clock::now();
for (int iter = 0; iter != 1e5; iter++) {
	asm volatile( //CLANG
		"shll    $16, %[li]\n" //li*CACHE_SIZE
		"je      .LBB1_6\n"    //if(!li) return
		"addl    $-65536, %[li]\n" //(li-1)*CACHE_SIZE
		"movl    %[li], %%ecx\n" //ecx=(li-1)*CACHE_SIZE
		"shrl    $16, %%ecx\n"   //ecx=li-1
		"addl    $1, %%ecx\n"    //ecx=li
		"movl    %%ecx, %%eax\n"  //eax=ecx=li
		"andl    $7, %%eax\n"    //eax=li & 0b111
		"cmpl    $458752, %[li]\n" //li==8
		"jae     .LBB1_7\n"        //li>=8
		"xorl    %%ecx, %%ecx\n"     //ecx=0
		"testl   %%eax, %%eax\n"     //eax==0
		"jne     .LBB1_4\n"        //li=li & 0b111=eax!=0, li<8
		"jmp     .LBB1_6\n"        //if(!li) return
		".LBB1_7:\n"                       //Wenn li>=8
		"leaq    458752(%[p]), %%rdx\n" //rdx=p+6*CACHE_SIZE
		"movl    $1, %[li]\n"           //[li]=1
		"subl    %%ecx, %[li]\n"         //[li]=1-(p+6*CACHE_SIZE)
		"leal    (%%rax,%[li]), %[li]\n" //[li]=1-(p+6*CACHE_SIZE)+li & 0b111
		"addl    $-1, %[li]\n"           //[li]=li&0b111-(p+6*CACHE_SIZE)
		"xorl    %%ecx, %%ecx\n"          //ecx=0
		".LBB1_8:\n"
		"prefetchnta     -458752(%%rdx,%%rcx)\n" //p+0*CACHE_SIZE+rcx
		"prefetchnta     -393216(%%rdx,%%rcx)\n" //p+1*CACHE_SIZE+rcx
		"prefetchnta     -327680(%%rdx,%%rcx)\n" //p+2*CACHE_SIZE+rcx
		"prefetchnta     -262144(%%rdx,%%rcx)\n" //p+3*CACHE_SIZE+rcx
		"prefetchnta     -196608(%%rdx,%%rcx)\n" //p+4*CACHE_SIZE+rcx
		"prefetchnta     -131072(%%rdx,%%rcx)\n" //p+5*CACHE_SIZE+rcx
		"prefetchnta     -65536(%%rdx,%%rcx)\n" //p+6*CACHE_SIZE+rcx
		"prefetchnta     (%%rdx,%%rcx)\n"       //p+7*CACHE_SIZe+rcx
		"addq    $524288, %%rcx\n"              //rcx += 8*CACHE_SIZE
		"addl    $8, %[li]\n"   //[li]+=8
		"jne     .LBB1_8\n"     //while([li]!=0)
		"testl   %%eax, %%eax\n" //eax=li&0b111 ==0
		"je      .LBB1_6\n"      //End
		".LBB1_4:\n"     //Tail which could not be unrolled, li<8
		"addq    %%rcx, %[p]\n"
		".LBB1_5:\n"
		"prefetchnta     (%[p])\n"
		"addq    $65536, %[p]\n"
		"addl    $-1, %%eax\n"
		"jne     .LBB1_5\n"
		".LBB1_6:\n"
		:
	: [li] "r" (li), [p] "r" (p)
		: "%rax", "%rcx", "%rdx"
		);
}
sto = std::chrono::high_resolution_clock::now();
t2 = std::chrono::duration_cast<std::chrono::microseconds>(sto - sta).count();
//========================================================
std::cout << t1 << std::endl;

void sleepms(unsigned int millisec)
{
	usleep(millisec);
}

//#define max(a,b) (((a)>(b))?(a):(b))
//#define min(a,b) (((a)<(b))?(a):(b))

volatile void algo8_a(volatile char* RESTRICT sta, volatile char* RESTRICT sto, size_t li, size_t SIZE) {
	asm volatile("");
}

template<typename ALG1, typename ALG2>
volatile void algo8_b(volatile char* RESTRICT sta, volatile char* RESTRICT sto, ALG1 l, ALG2 s, size_t li, size_t SIZE) {
	unsigned c_s = li * CACHE_LINE_SIZE;
	volatile char* end = sta + SIZE;
	for (unsigned int i = 0; i != c_s; i += CACHE_LINE_SIZE) {
		_mm_prefetch((char*)sta + i, _MM_HINT_NTA);
	}
	for (volatile char* p = sta, *d = sto; p != end; p += 256, d += 256) {
		asm volatile("" ::: "memory");
		__m256i ymm0 = l((__m256i*)(p + 0));
		asm volatile("" ::: "memory");
		__m256i ymm1 = l((__m256i*)(p + 32));
		asm volatile("" ::: "memory");
		__m256i ymm2 = l((__m256i*)(p + 64));
		asm volatile("" ::: "memory");
		__m256i ymm3 = l((__m256i*)(p + 96));
		asm volatile("" ::: "memory");
		__m256i ymm4 = l((__m256i*)(p + 128));
		asm volatile("" ::: "memory");
		__m256i ymm5 = l((__m256i*)(p + 160));
		asm volatile("" ::: "memory");
		__m256i ymm6 = l((__m256i*)(p + 192));
		asm volatile("" ::: "memory");
		__m256i ymm7 = l((__m256i*)(p + 224));
		asm volatile("" ::: "memory");
		_mm_prefetch((char*)p + c_s + 0, _MM_HINT_NTA);
		_mm_prefetch((char*)p + c_s + 64, _MM_HINT_NTA);
		_mm_prefetch((char*)p + c_s + 128, _MM_HINT_NTA);
		_mm_prefetch((char*)p + c_s + 192, _MM_HINT_NTA);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 0), ymm0);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 32), ymm1);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 64), ymm2);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 96), ymm3);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 128), ymm4);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 160), ymm5);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 192), ymm6);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 224), ymm7);
		asm volatile("" ::: "memory");
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
		asm volatile("" ::: "memory");
		__m256i ymm0 = l((__m256i*)(p + 0));
		asm volatile("" ::: "memory");
		__m256i ymm1 = l((__m256i*)(p + 32));
		asm volatile("" ::: "memory");
		__m256i ymm2 = l((__m256i*)(p + 64));
		asm volatile("" ::: "memory");
		__m256i ymm3 = l((__m256i*)(p + 96));
		asm volatile("" ::: "memory");
		_mm_prefetch((char*)p + c_s + 0, _MM_HINT_NTA);
		_mm_prefetch((char*)p + c_s + 64, _MM_HINT_NTA);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 0), ymm0);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 32), ymm1);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 64), ymm2);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 96), ymm3);
		asm volatile("" ::: "memory");
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
		asm volatile("" ::: "memory");
		__m256i ymm0 = l((__m256i*)(p + 0));
		asm volatile("" ::: "memory");
		__m256i ymm1 = l((__m256i*)(p + 32));
		asm volatile("" ::: "memory");
		_mm_prefetch((char*)p + c_s + 0, _MM_HINT_NTA);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 0), ymm0);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 32), ymm1);
		asm volatile("" ::: "memory");
	}
}


//sta,sto mark the region with lenght SIZE, l and s are the store and load algorithms, n is the number of cache lines, r the radius of preftech-forecast
//Don't Prefetch before,unroll 8
template<typename ALG1, typename ALG2>
volatile void algo8_n(volatile char* RESTRICT sta, volatile char* RESTRICT sto, ALG1 l, ALG2 s, size_t r, size_t SIZE) {
	volatile char* end = sta + SIZE;
	for (volatile char* p = sta, *d = sto; p != end; p += 256, d += 256) {
		asm volatile("" ::: "memory");
		__m256i ymm0 = l((__m256i*)(p + 0));
		asm volatile("" ::: "memory");
		__m256i ymm1 = l((__m256i*)(p + 32));
		asm volatile("" ::: "memory");
		__m256i ymm2 = l((__m256i*)(p + 64));
		asm volatile("" ::: "memory");
		__m256i ymm3 = l((__m256i*)(p + 96));
		asm volatile("" ::: "memory");
		__m256i ymm4 = l((__m256i*)(p + 128));
		asm volatile("" ::: "memory");
		__m256i ymm5 = l((__m256i*)(p + 160));
		asm volatile("" ::: "memory");
		__m256i ymm6 = l((__m256i*)(p + 192));
		asm volatile("" ::: "memory");
		__m256i ymm7 = l((__m256i*)(p + 224));
		asm volatile("" ::: "memory");
		_mm_prefetch((char*)p + r + 0, _MM_HINT_NTA);
		_mm_prefetch((char*)p + r + 64, _MM_HINT_NTA);
		_mm_prefetch((char*)p + r + 128, _MM_HINT_NTA);
		_mm_prefetch((char*)p + r + 192, _MM_HINT_NTA);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 0), ymm0);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 32), ymm1);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 64), ymm2);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 96), ymm3);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 128), ymm4);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 160), ymm5);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 192), ymm6);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 224), ymm7);
		asm volatile("" ::: "memory");
	}
}

//sta,sto mark the region with lenght SIZE, l and s are the store and load algorithms, n is the number of cache lines, r the radius of preftech-forecast
//Don't Prefetch before,unroll 4
template<typename ALG1, typename ALG2>
volatile void algo4_n(volatile char* RESTRICT sta, volatile char* RESTRICT sto, ALG1 l, ALG2 s, size_t r, size_t SIZE) {
	volatile char* end = sta + SIZE;
	for (volatile char* p = sta, *d = sto; p != end; p += 128, d += 128) {
		asm volatile("" ::: "memory");
		__m256i ymm0 = l((__m256i*)(p + 0));
		asm volatile("" ::: "memory");
		__m256i ymm1 = l((__m256i*)(p + 32));
		asm volatile("" ::: "memory");
		__m256i ymm2 = l((__m256i*)(p + 64));
		asm volatile("" ::: "memory");
		__m256i ymm3 = l((__m256i*)(p + 96));
		asm volatile("" ::: "memory");
		_mm_prefetch((char*)p + r + 0, _MM_HINT_NTA);
		_mm_prefetch((char*)p + r + 64, _MM_HINT_NTA);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 0), ymm0);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 32), ymm1);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 64), ymm2);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 96), ymm3);
		asm volatile("" ::: "memory");
	}
}

//sta,sto mark the region with lenght SIZE, l and s are the store and load algorithms, n is the number of cache lines, r the radius of preftech-forecast
//Don't Prefetch before,unroll 2
template<typename ALG1, typename ALG2>
volatile void algo2_n(volatile char* RESTRICT sta, volatile char* RESTRICT sto, ALG1 l, ALG2 s, size_t r, size_t SIZE) {
	volatile char* end = sta + SIZE;
	for (volatile char* p = sta, *d = sto; p != end; p += 64, d += 64) {
		asm volatile("" ::: "memory");
		__m256i ymm0 = l((__m256i*)(p + 0));
		asm volatile("" ::: "memory");
		__m256i ymm1 = l((__m256i*)(p + 32));
		asm volatile("" ::: "memory");
		_mm_prefetch((char*)p + r + 0, _MM_HINT_NTA);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 0), ymm0);
		asm volatile("" ::: "memory");
		s((__m256i*)(d + 32), ymm1);
		asm volatile("" ::: "memory");
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
	char * DATA1 = (char*)malloc(size + 128 * 64) + 64;
	char * DATA2 = (char*)malloc(size + 128 * 64) + 64;
	size_t LINEAR1 = std::min((size_t)DATA1, (size_t)DATA2);
	size_t LINEAR2 = std::max((size_t)DATA1, (size_t)DATA2);
	volatile char *dst = (char*)(((64 - (LINEAR1 & 63)) & 63) + LINEAR1);
	volatile char *src = (char*)(((64 - (LINEAR2 & 63)) & 63) + LINEAR2);
	int k;
	auto sta = std::chrono::high_resolution_clock::now();
	auto sto = std::chrono::high_resolution_clock::now();

	bch1(algo8_b, 0);
	bch1(algo4_b, 20);
	bch1(algo2_b, 40);
	bch2(algo8_n, 60);
	bch2(algo4_n, 80);
	bch2(algo2_n, 100);

	free(DATA1);
	free(DATA2);
}


int main(void)
{
	for (int i = 256; i != 129 * 256; i += 256) {
		benchmark(i, RUNS / i);
		std::cout << (i >> 8) << std::endl;
	}

	for (int i = 2 * 256 * 256; i != (2 * 256 + 129) * 256; i += 256) {
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
#endif