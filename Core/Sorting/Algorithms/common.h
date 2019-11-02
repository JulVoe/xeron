#pragma once

#include <iostream>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <windows.h>
#include <psapi.h>
#include <cassert>
#include <cmath>
#include <ctime>
#include <limits>
#include <vector>
#include <x86intrin.h>
#include <cstddef>
#include <functional>
#include <utility>
#include <cstdint>
#include <tuple>

#define FAST_SWAP

template<class T>
void printArray(T* p, size_t n){
	for(int i=0; i!= n; i++)
		std::cout<<*(p+i)<<"   ";
	std::cout<<std::endl;
}

template<class T>
void printArray(T* p, T* end){
	for(T* q = p; q!=end; ++q)
		std::cout<<*q<<"   ";
	std::cout<<std::endl;
}


#ifdef FAST_SWAP
template<class T>
void swap(T* p, T* q){ //Uses move-scematics
	T tmp = std::move(*p);
	*p = std::move(*q);
	*q = std::move(tmp);
}
#else

template<class T>
void swap(T* p, T* q){
	T tmp = *p;
	*p = *q;
	*q = tmp;
}
#endif

union intByteExtract{
	int i;
	uint8_t c[4];
};



int cur_mem_use(){
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	return pmc.WorkingSetSize;
}
