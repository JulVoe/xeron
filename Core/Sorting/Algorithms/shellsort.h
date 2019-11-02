#include "common.h"

template<class T>
void shellSort1(T* arr, size_t elemente) {
    int j;
    for (int gap = elemente>>1; gap > 0; gap>>=1) {
        for (int i = gap; i != elemente; ++i) {
            int temp = arr[i];
            for (j = i; j >= gap && temp < arr[j - gap]; j -= gap) {
            	arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

//Hier wird die richtige Schrittweise benötigt. Möglich sind O(n^{4/3}) und O(n*(log n)^2). Siehe Wikipedia / shellSort
template<class T, typename Comperator>
void shellSort2(T *sta, T* sto, int *schrittweiten, Comperator comp) {
  for (int sW : schrittweiten) {//Schrittweite
     for (T* p = sta+sW; p != sto; ++p){
        T e_z_E = *p; //Element zum einfügen
        T* e_S = p;       //Einfügestelle       war mal index
        while (e_S - sW >= sta && comp(*(e_S-sW),e_z_E) < 0) {
           *e_S = *(e_S-sW);
           e_S -= sW; // Sprung um schrittweite
        }
        *e_S = e_z_E;
     }

  }
}
