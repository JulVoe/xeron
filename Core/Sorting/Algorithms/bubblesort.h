#include "common.h"

template<class T>
void bubbleSort1(T *arr, T* end)
{
   bool swapped;
   for (unsigned i = 1; i != end-arr; i++)
   {
     swapped = false;
     for (T* j = arr; j != end-i; j++)
     {
        if (*j > *(j+1))
        {
           swap(j, j+1);
           swapped = true;
        }
     }

     if (swapped == false)
        return;
   }
}

template<class T>
void bubbleSort2(T* sta, T* sto){
	while(sta+1!=sto){
		T* new_sto = sta+1;
		for(T* p = sta; p!=sto-1; ++p){
			if(*p > *(p+1)){
				swap(p,p+1);
				new_sto = p+1;
			}
		}
		sto = new_sto;
	}
}

template<class T>
void bubbleSort3(T* sta, T* sto){
	while(sto!=sta){
		for(T* p=sta; p!=sto-1; p++){
			if(*p>*(p+1)) swap(p,p+1);
		}
		sto--;
	}
}
