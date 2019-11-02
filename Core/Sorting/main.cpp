#define NOMINMAX

#include "Algorithms/blocksort.h"
#include "Algorithms/bubblesort.h"
#include "Algorithms/common.h"
#include "Algorithms/countingsort.h"
#include "Algorithms/insertionsort.h"
#include "Algorithms/sortingnetworks.h"
#include "Algorithms/introsort.h"
#include "Algorithms/quicksort.h"
#include "Algorithms/radixsort.h"
#include "Algorithms/selectionsort.h"
#include "Algorithms/shellsort.h"
#include "Algorithms/skasort.h"
#include "Algorithms/skasort_fake.h"
#include "Algorithms/spreadsort.h"
#include "Algorithms/Verdgesort/vergesort.h"
#include "Algorithms/semi_static_sort.h"
#include "Algorithms/mysort.h"
#include "Algorithms/result.h"
#include "Algorithms/flashsort.hpp"
//#include "Algorithms/Timsort/timsort.h"

#include <iostream>
#include <cstdio>
#include <chrono>

#define SIZE 100000
#define MAX 250000
#define ROUNDS 1000


#define shuffel(x,y)\
	for(int* p=x; p!=y; ++p)\
		*p = abs(rand()%MAX);

#define vor_gluehen(f,t);\
	sta = std::chrono::high_resolution_clock::now();\
	for(int i=0; i!=t;++i){\
		f;\
		shuffel(a,e);\
	}\
	sto = std::chrono::high_resolution_clock::now();\
	std::cout<<"-------------------Warm-up took "<<std::chrono::duration<double>(sto-sta).count()<<"s ------------------"<<std::endl;


#define test_func(func,mes)\
		time_used = 0;\
		cur_mem = cur_mem_use();\
		for(int i=0; i!=ROUNDS;++i){\
			sta = std::chrono::high_resolution_clock::now();\
			func;\
			fwrite(a+(int)((e-a)*(double)((double)rand()/(double)RAND_MAX)),sizeof(int),1,f_d);\
			sto = std::chrono::high_resolution_clock::now();\
			time_used += std::chrono::duration<double>(sto-sta).count();\
			shuffel(a,e);\
		}\
		std::cout<<mes<<time_used<<"s and left "<<cur_mem_use()-cur_mem<<"bits on the ram"<<std::endl;

#define printArray(a,e);\
	for(int * p = a; p!=e; ++p)\
		std::cout<<*p<<std::endl;

#define test_algo(algo);\
	shuffel(a,e);\
	printArray(a,e);\
	std::cout<<"---------------------"<<std::endl;\
	algo;\
	printArray(a,e);


#define debug(func,mes);\
	shuffel(a,e);\
	func;\
	std::cout<<mes<<((std::is_sorted(a,e))?" is sorted.":" is not sorted!!!!!!!!!!")<<std::endl;

void benchmark(){
	srand(1337);
	FILE* f_d = fopen("anti-optimize.txt","wb");
	int a[SIZE];
	int *e = a+SIZE;
	shuffel(a,e);

	int cur_mem;
	double time_used;

	auto sta = std::chrono::high_resolution_clock::now();
	auto sto = std::chrono::high_resolution_clock::now();

	vor_gluehen(Wiki::Sort(a,e,std::less<int>()),2*ROUNDS);

	test_func(Wiki::Sort(a,e,std::less<int>()),"1. Blocksort-implementation took ");
	//test_func(bubbleSort1(a,e),"1. Bubblesort-implementation took ");
	//test_func(bubbleSort2(a,e),"2. Bubblesort-implementation took ");
	//test_func(bubbleSort3(a,e),"3. Bubblesort-implementation took ");
	test_func(countingsort1(a,e),"1. Countingsort-implementation took ");
	//test_func(insertion_sort2(a,e,std::less<int>()),"1. Insertionsort-implementation took ");
	test_func(pdqsort_branchless(a,e,std::less<int>()),"1. Introsort-implementation (pdq) took ");
	test_func(vergesort::vergesort(a,e,std::less<int>()),"Vergesort took ");
	//test_func(introsort2(a,e),"2. Introsort-implementation took ");
	//test_func(quickSort1(a,e),"1. Quicksort-implementation took ");                                       //Somehow does not sort
	//test_func(quickSort2(a,e),"2. Quicksort-implementation took ");                                       //Somehow doesn't work
	//test_func(quickSort3(a,e),"3. Quicksort-implementation took ");                                       //Somehow doesn't work
	//test_func(quicksort4(a,e),"4. Quicksort-implementation took ");                                       //Somehow doesn't work
	//test_func(quicksort5(a,SIZE),"5. Quicksort-implementation took ");
	//test_func(selectionsort1(a,e),"1. Selectionsort-implementation took ");                               //Somehow does not sort
	test_func(ska_sort(a,e),"1. Skasort-implementation took ");                                 //A modified American Flag sort implementation
	int buf[SIZE]; test_func(ska_sort_copy(a,e,buf),"2. Skasort-implementation took ");         //An Radix sort LSD with radix 256 and 4 iterations at once
	test_func(spreadsort1(a,e),"1. Spreadsort-implementation took ");
	test_func(flashsort(a,e),"1. Flashsort-implementation took ");
	//test_func(semi_static_sort(a,SIZE),"Semi-static-sort-implementation took ");
	//test_func(static_sort1<SIZE>(a),"1. Staticsort-Implementation took ");
	//static_sort2<SIZE> s;
	//test_func(s(a),"2. Staticsort-Implementation took ");
	test_func(fastest_sort_q(a,e),"My quicksort with semi-static base-case took ");
	test_func(fastest_sort_m(a,e),"My mergesort with semi-static base-case took ");
	test_func(fastest_sort_kx(a,e),"Kxsort with better traits and semi-static base-case took ");
	//test_func(fastest_sort_kx_2(a,e),"My second kxsort with semi-static base-case took ");
	test_func(radix_sort(a,e),"1. default kxsort with semi-static base-case took ");
	test_func(radix_sort_2(a,e),"2. default kxsort with semi-static base-case took ");
	test_func(radix_sort_4(a,e),"3. default kxsort with semi-static base-case took ");
	test_func(radix_sort_3(a,e,buf),"Out of place lsd radix sort took ");
	test_func(radix_sort_5(a,e,buf),"Out of place msd radix sort took ");                                    //Somehow does not sort
	test_func(fake::ska_sort_copy(a,e,buf),"Ska-sort fake took ");
	test_func(std::sort(a,e),"std::sort-implementation took ");
	//test_func(std::stable_sort(a,e),"std::stable_sort-implementation took ");

	//std::cout<<cur_mem_use()<<std::endl;
}

void test_algorithms(){
	srand(1337);
	int a[SIZE];
	int *e = a+SIZE;
	int buf[SIZE];
	shuffel(a,e);

	debug(Wiki::Sort(a,e,std::less<int>()),"1. Blocksort-implementation");
	debug(countingsort1(a,e),"1. Countingsort-implementation");
	debug(insertion_sort2(a,e,std::less<int>()),"1. Insertionsort-implementation");
	debug(pdqsort(a,e,std::less<int>()),"1. Introsort-implementation (pdq)");
	debug(introsort2(a,e),"2. Introsort-implementation");
	debug(quickSort1(a,e),"1. Quicksort-implementation");
	debug(quicksort5(a,SIZE),"5. Quicksort-implementation");
	debug(selectionsort1(a,e),"1. Selectionsort-implementation");
	debug(ska_sort(a,e),"1. Skasort-implementation");
	debug(ska_sort_copy(a,e,buf),"2. Skasort-implementation");
	debug(spreadsort1(a,e),"1. Spreadsort-implementation");
	debug(semi_static_sort(a,SIZE),"Semi-static-sort-implementation");
	debug(static_sort1<SIZE>(a),"1. Staticsort-Implementation");
	debug(fastest_sort_q(a,e),"My quicksort with semi-static base-case");
	debug(fastest_sort_m(a,e),"My mergesort with semi-static base-case");
	debug(fastest_sort_kx(a,e),"Kxsort with better traits and semi-static base-case");
	debug(radix_sort(a,e),"1. default kxsort with semi-static base-case");
	debug(radix_sort_2(a,e),"2. default kxsort with semi-static base-case");
	debug(radix_sort_4(a,e),"3. default kxsort with semi-static base-case");
	debug(radix_sort_3(a,e,buf),"Out of place lsd radix sort");
	debug(radix_sort_5(a,e,buf),"Out of place msd radix sort");
	debug(fake::ska_sort_copy(a,e,buf),"Ska-sort fake");
	debug(std::sort(a,e),"std::sort-implementation");
	debug(std::stable_sort(a,e),"std::stable_sort-implementation");
}


void reference(int * sta , int * sto){
	std::make_heap(sta,sto);
	std::sort_heap(sta,sto);
}

#include "C:/C++_Libs/gheap-sophisticated-gheap/gheap_cpp11.hpp"

void fast_heap_sort1(int * sta, int * sto)                                                                             //Bester Heapsort
{
	gheap<4, 1>::make_heap(sta, sto);
	gheap<4, 1>::sort_heap(sta, sto);
}

static size_t heapify(int *data, size_t n, size_t WIDTH, size_t root)
{
  assert(root < n);
  size_t count = 0;
  int val = data[root];
  size_t parent = root;
  size_t child = 0;
  assert(parent * WIDTH >= parent); // Overflow-Test
  while ( (child = parent * WIDTH + 1) < n )  // erstes Kind;
  {                                           // Abbruch am Ende des Heaps
    size_t w = n - child < WIDTH ?
      n - child : WIDTH;            // Anzahl der vorhandenen Geschwister

    count += w;

    size_t max = 0;
    for (size_t i = 1; i < w; ++i )  // größtes Kind suchen
      if (data[child+i] > data[child+max])
        max = i;
    child += max;

    if (data[child] <= val)        // kein Kind mehr größer als der
      break;                        //   zu versickernde Wert

    data[parent] = data[child];     // größtes Kind nach oben rücken
    parent = child;                 // in der Ebene darunter weitermachen
  }
  data[parent] = val;               // gesiebten Wert eintragen
  return count;
}

size_t fast_heap_sort2(int * data, size_t n, size_t WIDTH)
{
  assert(WIDTH > 1);
  if (n < 2)
    return 0;
  size_t m = (n + WIDTH - 2) / WIDTH;  // erstes Blatt im Baum
  int count = 0;                       // Zähler für Anzahl der Vergleiche

  assert(m > 0);
  for (size_t r = m; r > 0; --r)       // Teil 1: Konstruktion des Heaps
    count += heapify(data, n, WIDTH, r-1);
  for (size_t i = n - 1; i > 0; --i) { // Teil 2: eigentliche Sortierung
    int tmp = data[0];                 // Spitze des Heaps hinter den Heap in
    data[0] = data[i];
    data[i] = tmp;                     //   den sortierten Bereich verschieben
    count += heapify(data, i, WIDTH, 0);
  }
  return count;
}




int main(){
	benchmark();

	return 0;
}
