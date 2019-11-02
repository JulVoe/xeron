#pragma once

#include "common.h"
#include "semi_static_sort.h"

//================================================================================Inplace===============================================================================================

static const int kRadixBits = 8;
static const size_t kInsertSortThreshold = 64;
static const int kRadixMask = (1 << kRadixBits) - 1;
static const int kRadixBin = 1 << kRadixBits;

template <class T>
struct RadixTraitsUnsigned {
    static const int nBytes = sizeof(T);
    int kth_byte (const T &x, int k) { return (x >> (kRadixBits * k)) & kRadixMask; }
    bool compare(const T &x, const T &y) { return x < y; }
};

template<class T>
struct RadixTraitsSigned {
    static const int nBytes = sizeof(T);
    static const T kMSB = T(0x80) << ((sizeof(T) - 1) * 8);
    int kth_byte (const T &x, int k) {
        return ((x ^ kMSB) >> (kRadixBits * k)) & kRadixMask;
    }
    bool compare(const T &x, const T &y) { return x < y; }
};

template <class RandomIt, class ValueType, class RadixTraits>
inline void insert_sort_core_(RandomIt s, RandomIt e, RadixTraits radix_traits)
{
    for (RandomIt i = s + 1; i < e; ++i) {
        if (radix_traits.compare(*i, *(i - 1))) {
            RandomIt j;
            ValueType tmp = *i;
            *i = *(i - 1);
            for (j = i - 1; j > s && radix_traits.compare(tmp, *(j - 1)); --j) {
                *j = *(j - 1);
            }
            *j = tmp;
        }
    }
}

template <class RandomIt, class ValueType, class RadixTraits, int kWhichByte>
inline void radix_sort_core_(RandomIt s, RandomIt e, RadixTraits radix_traits)
{
    RandomIt last_[kRadixBin + 1];
    RandomIt *last = last_ + 1;
    size_t count[kRadixBin] = {0};

    for (RandomIt i = s; i < e; ++i) {
        ++count[radix_traits.kth_byte(*i, kWhichByte)];
    }

    last_[0] = last_[1] = s;

    for (int i = 1; i < kRadixBin; ++i) {
        last[i] = last[i-1] + count[i-1];
    }

    for (int i = 0; i < kRadixBin; ++i) {
        RandomIt end = last[i-1] + count[i];
        if (end == e) { last[i] = e; break; }
        while (last[i] != end) {
            ValueType swapper = *last[i];
            int tag = radix_traits.kth_byte(swapper, kWhichByte);
            if (tag != i) {
                do {
                    std::swap(swapper, *last[tag]++);
                } while ((tag = radix_traits.kth_byte(swapper, kWhichByte)) != i);
                *last[i] = swapper;
            }
            ++last[i];
        }
    }

    if (kWhichByte > 0) {
        for (int i = 0; i < kRadixBin; ++i) {
            if (count[i] > kInsertSortThreshold) {
                radix_sort_core_<RandomIt, ValueType, RadixTraits,
                                  (kWhichByte > 0 ? (kWhichByte - 1) : 0)>
                                  (last[i-1], last[i], radix_traits);
            } else if (count[i] > 1) {
            	semi_static_sort(last[i-1], last[i]-last[i-1]);
            }
        }
    }
}

template <class RandomIt, class ValueType, class RadixTraits>
inline void radix_sort_entry_(RandomIt s, RandomIt e, ValueType*,
                              RadixTraits radix_traits)
{
    if (e - s <= (int)kInsertSortThreshold)
        insert_sort_core_<RandomIt, ValueType, RadixTraits>(s, e, radix_traits);
    else
        radix_sort_core_<RandomIt, ValueType, RadixTraits, RadixTraits::nBytes - 1>(s, e, radix_traits);
}

template <class RandomIt, class ValueType>
inline void radix_sort_entry_(RandomIt s, RandomIt e, ValueType *)
{
    if (ValueType(-1) > ValueType(0)) {
        radix_sort_entry_(s, e, (ValueType*)(0), RadixTraitsUnsigned<ValueType>());
    } else {
        radix_sort_entry_(s, e, (ValueType*)(0), RadixTraitsSigned<ValueType>());
    }
}


template <class RandomIt, class RadixTraits>
inline void radix_sort(RandomIt s, RandomIt e, RadixTraits radix_traits)
{
    typename std::iterator_traits<RandomIt>::value_type *dummy(0);
    radix_sort_entry_(s, e, dummy, radix_traits);
}

template <class RandomIt>
inline void radix_sort(RandomIt s, RandomIt e)                                                             //An MSD Radix sort, but with radix of 256.  American-flag-sort ->in-place
{
    typename std::iterator_traits<RandomIt>::value_type *dummy(0);
    radix_sort_entry_(s, e, dummy);
}

//-----------------------------

template <int kWhichByte>
inline void radix_sort_2_core(int* s, int* e) //Still an MSD Radix sort with radix 256, but inproved byte extraction and base case. American-flag-sort ->in-place
{
    int* last_[257];
    int* *last = last_ + 1;
    size_t count[256] = {0};
    intByteExtract iBc;

    for (int* i = s; i < e; ++i) {
    	iBc.i = *i;
        ++count[iBc.c[kWhichByte]];
    }

    last_[0] = last_[1] = s;

    for (int i = 1; i != 256; ++i) {
        last[i] = last[i-1] + count[i-1];
    }

    for (int i = 0; i != 256; ++i) {
        int* end = last[i-1] + count[i]; //Da last[i-1] voll ist, ist dies der ausgangs-anfang des Buckets
        if (end == e) { last[i] = e; break; }
        while (last[i] != end) {
            int swapper = *last[i];
            iBc.i = swapper;
            int tag = iBc.c[kWhichByte];
            if (tag != i) {
                do {
                    std::swap(swapper, *last[tag]++);
                    iBc.i = swapper;
                } while ((tag = iBc.c[kWhichByte]) != i);
                *last[i] = swapper;
            }
            ++last[i];
        }
    }

	if(kWhichByte>0){
		for (int i = 0; i != 256; ++i) {
			if (count[i] > 128) {
				radix_sort_2_core<(kWhichByte>0)?kWhichByte-1:0>(last[i-1], last[i]);
			} else{
				semi_static_sort(last[i-1], last[i]-last[i-1]);
			}
		}
	}
}

inline void radix_sort_2(int * sta, int * sto){
	radix_sort_2_core<3>(sta,sto);
}


template <int kWhichByte>
inline void radix_sort_4_core(int* s, int* e) //Still an MSD Radix sort with radix 256, but inproved byte extraction and base case. American-flag-sort ->in-place
{
	if(kWhichByte<0){
		return;
	}
    int* last_[257];
    int* *last = last_ + 1;
    size_t count[256] = {0};
    intByteExtract iBc;

    for (int* i = s; i < e; ++i) {
    	iBc.i = *i;
        ++count[iBc.c[kWhichByte]];
    }

    last_[0] = last_[1] = s;

    for (int i = 1; i != 256; ++i) {
        last[i] = last_[i] + count[i-1];
    }

    for (int i = 0; i != 256; ++i) {
        int* end = last[i-1] + count[i]; //Da last[i-1] voll ist, ist dies der ausgangs-anfang des Buckets
        if (end == e) { last[i] = e; break; }
        while (last[i] != end) {
            iBc.i = *last[i];
            int tag = iBc.c[kWhichByte];
            if (tag != i) {
                do {
                    swap(last[i], last[tag]++);
                    iBc.i = *last[i];
                } while ((tag = iBc.c[kWhichByte]) != i);
            }
            ++last[i];
        }
    }

    for (int i = 0; i != 256; ++i) {
        if (count[i] > 128) {
        	radix_sort_2_core<(kWhichByte>0)?kWhichByte-1:-1>(last[i-1], last[i]);
        } else{
           	semi_static_sort(last[i-1], last[i]-last[i-1]);
        }
    }
}

inline void radix_sort_4(int * sta, int * sto){
	radix_sort_2_core<3>(sta,sto);
}

//================================================================================Out of place==========================================================

inline void radix_sort_3_core(int* s, int* e, int* buf) //LSD Sort copy
{
    int* offset[256];
    size_t count[256] = {0};
    intByteExtract iBc;
    int *buf_e=buf+(e-s);

    for (int* i = s; i < e; ++i) {
    	iBc.i = *i;
        ++count[iBc.c[0]];
    }

    offset[0] = buf;
    for (int i = 0; i != 255; ++i){
        offset[i+1] = offset[i] + count[i];
    }


    for (int* p = s; p != e; ++p){
    	iBc.i=*p;
    	*offset[iBc.c[0]]=*p;
    	++offset[iBc.c[0]];
    }

    //This was the first iteration

    std::fill(count,count+256,0);
    for (int* i = s; i < e; ++i) {
       	iBc.i = *i;
       	++count[iBc.c[1]];
    }

    offset[0] = s;
    for (int i = 0; i != 255; ++i){
    	offset[i+1] = offset[i] + count[i];
    }

    for (int* p = buf; p != buf_e; ++p){
    	iBc.i=*p;
    	*offset[iBc.c[1]]=*p;
    	++offset[iBc.c[1]];
    }

    //This was the second iteration

    std::fill(count,count+256,0);
    for (int* i = s; i < e; ++i) {
    	iBc.i = *i;
        ++count[iBc.c[2]];
    }

    offset[0] = buf;
    for (int i = 0; i != 255; ++i){
        offset[i+1] = offset[i] + count[i];
    }


    for (int* p = s; p != e; ++p){
    	iBc.i=*p;
    	*offset[iBc.c[2]]=*p;
    	++offset[iBc.c[2]];
    }

    //This was the third iteration

    std::fill(count,count+256,0);
    for (int* i = s; i < e; ++i) {
       	iBc.i = *i;
       	++count[iBc.c[3]];
    }

    offset[0] = s;
    for (int i = 0; i != 255; ++i){
    	offset[i+1] = offset[i] + count[i];
    }

    for (int* p = buf; p != buf_e; ++p){
    	iBc.i=*p;
    	*offset[iBc.c[3]]=*p;
    	++offset[iBc.c[3]];
    }

    //This was the last iteration
}

inline void radix_sort_3(int * sta, int * sto, int * buf){
	radix_sort_3_core(sta,sto,buf);
}

template<int Byte>
void radix_sort_5_core(int * sta, int * sto, int * buf){
	if(Byte<0){
		return;
	}

	int *offset[256];
	size_t count[256] = {0};
	intByteExtract iBc;

	for(int *p=sta;p!=sto;++p){
		iBc.i=*p;
		++count[iBc.c[Byte]];
	}

	*offset = buf;
	for(int i=0;i!=255;++i)
		offset[i+1]=offset[i]+count[i];

	for(int *p=sta;p!=sto;++p){
		iBc.i=*p;
		*offset[iBc.c[Byte]]=*p;
		++offset[iBc.c[Byte]];

	}

	if(Byte>0){
		if(count[0]>128)
			radix_sort_5_core<(Byte>0)?Byte-1:-1>(buf,offset[0],sta);
		else
			semi_static_sort(buf,count[0]);

		for(int i=1;i!=256;++i){
			if(count[i]>128)
				radix_sort_5_core<(Byte>0)?Byte-1:-1>(offset[i-1],offset[i],sta);
			else
				semi_static_sort(offset[i-1],count[i]);
		}
	}
}

__attribute__((always_inline)) inline void radix_sort_5(int *sta, int *sto, int *buf){
	radix_sort_5_core<3>(sta, sto, buf);
}
