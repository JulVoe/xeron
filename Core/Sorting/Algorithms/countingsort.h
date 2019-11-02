#ifndef sajhjksjk
#define sajhjksjk

#include "common.h"

template<unsigned n>
void c_reduce(int* __restrict__ sta, int c){
    const __m256i co = _mm256_set1_epi32(c);
	__m256i m;
	unsigned v=0;
	for(; n-v>8; v+=8){
		m = _mm256_loadu_si256((__m256i *)(sta+v));
		_mm256_storeu_si256((__m256i *)(sta+v),_mm256_sub_epi32(m,co));
	}
	c_reduce<n%8>(sta+8,c);
};

template<>
void c_reduce<0>(int* __restrict__ sta, int c){return;};
template<>
void c_reduce<1>(int* __restrict__  sta, int c){ *sta -= c;return;};
template<>
void c_reduce<2>(int* __restrict__  sta, int c){ *sta -= c;*(sta+1)-=c;return;};
template<>
void c_reduce<3>(int* __restrict__  sta, int c){ *sta -= c;*(sta+1)-=c;*(sta+2)-=c;return;};
template<>
void c_reduce<4>(int* __restrict__  sta, int c){ *sta -= c;*(sta+1)-=c;*(sta+2)-=c;*(sta+3)-=c;return;};
template<>
void c_reduce<5>(int* __restrict__  sta, int c){ *sta -= c;*(sta+1)-=c;*(sta+2)-=c;*(sta+3)-=c;*(sta+4)-=c;return;};
template<>
void c_reduce<6>(int* __restrict__  sta, int c){ *sta -= c;*(sta+1)-=c;*(sta+2)-=c;*(sta+3)-=c;*(sta+4)-=c;*(sta+5)-=c;return;};
template<>
void c_reduce<7>(int* __restrict__  sta, int c){ *sta -= c;*(sta+1)-=c;*(sta+2)-=c;*(sta+3)-=c;*(sta+4)-=c;*(sta+5)-=c;*(sta+6)-=c;return;};



template<unsigned n>
void c_raise(int* __restrict__ sta, int c){
    const __m256i co = _mm256_set1_epi32(c);
	__m256i m;
	unsigned v=0;
	for(; n-v>8; v+=8){
		m = _mm256_loadu_si256((__m256i *)(sta+v));
		_mm256_storeu_si256((__m256i *)(sta+v),_mm256_add_epi32(m,co));
	}
	c_raise<n%8>(sta+8,c);
};

template<>
void c_raise<0>(int* __restrict__ sta, int c){return;};
template<>
void c_raise<1>(int* __restrict__  sta, int c){ *sta += c;return;};
template<>
void c_raise<2>(int* __restrict__  sta, int c){ *sta += c;*(sta+1)+=c;return;};
template<>
void c_raise<3>(int* __restrict__  sta, int c){ *sta += c;*(sta+1)+=c;*(sta+2)+=c;return;};
template<>
void c_raise<4>(int* __restrict__  sta, int c){ *sta += c;*(sta+1)+=c;*(sta+2)+=c;*(sta+3)+=c;return;};
template<>
void c_raise<5>(int* __restrict__  sta, int c){ *sta += c;*(sta+1)+=c;*(sta+2)+=c;*(sta+3)+=c;*(sta+4)+=c;return;};
template<>
void c_raise<6>(int* __restrict__  sta, int c){ *sta += c;*(sta+1)+=c;*(sta+2)+=c;*(sta+3)+=c;*(sta+4)+=c;*(sta+5)+=c;return;};
template<>
void c_raise<7>(int* __restrict__  sta, int c){ *sta += c;*(sta+1)+=c;*(sta+2)+=c;*(sta+3)+=c;*(sta+4)+=c;*(sta+5)+=c;*(sta+6)+=c;return;};

template<class T>
void countingsort1(T* sta, T* sto){
	auto m = std::minmax_element(sta,sto);
	//reduce(sta,sto,m.first);

	T* count = (T*)calloc(*m.second-*m.first+1,sizeof(T));
	T* p;
	for(p=sta; p!=sto; ++p)
		count[(*p)-*m.first]++;

	/*
	T* end = count+(m.last-m.first+1);
	for(p=count;p!=end;++p){
		while(*p)
			*(sta++) = *(p--);
	}
	*/

	p=count;  //This way there is no need to compute the end of count!
	while(sta!=sto){
		while(*p){
			*sta = (p-count)+*m.first;
			sta++;
			(*p)--;
		}
		p++;
	}

	free(count);
	return;
}

template<int s>
void countingsort2(int* sta, int* sto){
	auto m = std::minmax_element(sta,sto);

	c_reduce<s>(sta,*m.first);

	int count[*m.second-*m.first+1] = {0};
	int* p;
	for(p=sta; p!=sto; ++p)
		*(count+*p) ++;


	/*
	T* end = count+(m.last-m.first+1);
	for(p=count;p!=end;++p){
		while(*p)
			*(sta++) = (*p)--;
	}
	*/

	p=count;  //This way there is no need to compute the end of count!
	while(sta!=sto){
		while(*p)
			*(sta++) = (*p)--;
	}
	c_raise<s>(sto-s,*m.first); //If we don't want to use a new copy of sta, return m.first and use raise in the main programm!
	return;
}

#endif
