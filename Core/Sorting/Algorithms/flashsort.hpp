#include "semi_static_sort.h"
#include <iterator>

// The flashsort algorithm is attributed to Karl-Dietrich Neubert
// The translation to C++ is provided by Clint Jed Casper
// Cleaning, rearranging, and conversion by Mark Randles
template <typename T>
void flashsort(T* begin, T* end) {

	if(begin == end) return;

	int length = end - begin;

	int m = (int)(0.1 * length);

	T min, max, maxIndex;
	auto p = std::minmax_element(begin,end);
	min = *p.first;
	max = *p.second;
	maxIndex = p.second-begin;
	if(max == min) return;

	T* L = (T*)calloc(m+1,sizeof(T));

	double c = (m-1.0)/(max-min);
	for(T* h=begin; h != end; ++h) {
    	++L[((int)(((*h)-min)*c))+1];
	}

	for(int K = 2; K <= m; K++) {
		L[K] = L[K] + L[K-1];
	}

	T temp = *(begin+maxIndex);
	*(begin+maxIndex) = *begin;
	*begin = temp;

	int j = 0;
	int K = m;
	int numMoves = 0;

	while(numMoves < length) {
		while(j >= L[K]) {
			j++;
			K = ((int)((*(begin+j) - min) * c)) + 1;
		}

		T evicted = *(begin+j);

		while(j < L[K])	{
			K = ((int)((evicted-min)*c))+1;

			int location = L[K] - 1;

			T temp = *(begin+location);
			*(begin+location) = evicted;
			evicted = temp;

			L[K] -= 1;

			numMoves++;
		}
	}

	for(K = m - 1; K >= 1; K--) {
		int classSize = L[K+1] - L[K];

		if(classSize > 256) {
			flashsort(begin+L[K],begin+L[K]+classSize);
		} else {
			semi_static_sort(begin+L[K],classSize);
		}
	}

	free(L);
}
