//Actually this is cocktailshaker sort, which is selectionsort but we find minimum AND MAXIMUM in every pass, effectivly halfing the number of passes!

#include "common.h"

template<class T>
void selectionsort1(T* sta, T* sto){
	while(sto-sta>1){
		auto t = std::minmax_element(sta,sto);
		swap(sta,t.first);
		swap(sto-1,t.second);
		sta++;
		sto--;
	}
	return;
}
