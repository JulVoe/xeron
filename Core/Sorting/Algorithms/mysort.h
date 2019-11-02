#include "common.h"
#include "radixsort.h"
#include "semi_static_sort.h"

#undef cond_swap
#define cond_swap(a,b); if(*(a)>*(b)) swap((a),(b));

__attribute__((always_inline)) inline int med_three_pivot(int * sta, int * sto){
	int* mid = ((sto-sta)/2)+sta; //Equivalent to (sta+sto)/2, but avoids overflow
	cond_swap(mid,(sto-1));
	cond_swap(sta,(sto-1));
	cond_swap(sta,mid);
	return *mid;
}

__attribute__((always_inline)) inline int* quick_partition(int* sta, int* sto, const int pivot) { //Hoares Methode: (n/6)-(1/3) swaps on average stat dem naiven (n/2)-(1/2)
    while(true) {
        while(*sta < pivot) ++sta;
        while(*sto > pivot) --sto;
        if (sta < sto) swap(sta,sto);
        else return sto;
    }
}

void fastest_sort_q(int *sta, int * sto){
	ptrdiff_t n = sto-sta;
	if(n<=32){
		semi_static_sort(sta,n);
		return;
	}

	int* mid = ((sto-sta)/2)+sta; //Equivalent to (sta+sto)/2, but avoids overflow
	cond_swap(mid,(sto-1));
	cond_swap(sta,(sto-1));
	cond_swap(sta,mid);

	int *i=sta+1,*j=sto-1; //Hoares Methode: (n/6)-(1/3) swaps on average stat dem naiven (n/2)-(1/2)
	while(true) {
		do{++i;}while(*i < *mid);
		do{--j;}while(*j > *mid);
	    if (i < j) swap(i,j);
	    else goto brk;
	}

	brk:
	fastest_sort_q(sta,j);
	fastest_sort_q(j,sto);
};

void fastest_sort_m(int *sta, int * sto){
	ptrdiff_t n = sto-sta;
	if(n<=32){
		semi_static_sort(sta,n);
		return;
	}

	int* mid = ((sto-sta)/2)+sta;
	fastest_sort_m(sta,mid);
	fastest_sort_m(mid,sto);
	std::inplace_merge(sta,mid,sto);
};

//-------------------------------------------------
struct RadixTraitsInt{
	static const int nBytes = 4;
	__attribute__((always_inline)) int kth_byte(const int x, int k){
		intByteExtract iBc;
		iBc.i = x;
		return iBc.c[k];
	}
	bool compare(const int x, const int y){return x<y;}
};

inline void fastest_sort_kx(int * sta, int * sto){
	RadixTraitsInt i;
	radix_sort_core_<int*,int,RadixTraitsInt,3>(sta,sto,i);
}
