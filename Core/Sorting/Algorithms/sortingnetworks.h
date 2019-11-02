#ifndef jdkhsajkdhajkshdjkhasj
#define jdkhsajkdhajkshdjkhasj

#include "result.h"

#define sort_net2(x, y) if (comp((y), (x))) std::swap((x), (y))

template<typename T, typename Comparator>
void sort2(T &a, T &b, Comparator comp) {
    sort_net2(a, b);
};

template<typename T, typename Comparator>
void sort3(T &a, T &b, T &c, Comparator comp) {
    sort_net2(b, c);
    sort_net2(a, c);
    sort_net2(a, b);
};

template<typename T, typename Comparator>
void sort4(T &a, T &b, T &c, T &d, Comparator comp) {
    sort_net2(a, b);
    sort_net2(c, d);
    sort_net2(a, c);
    sort_net2(b, d);
    sort_net2(b, c);
};

template<typename T, typename Comparator>
void sort5(T &a, T &b, T &c, T &d, T &e, Comparator comp) {
    sort_net2(a, b);
    sort_net2(d, e);
    sort_net2(c, e);
    sort_net2(c, d);
    sort_net2(b, e);
    sort_net2(a, d);
    sort_net2(a, c);
    sort_net2(b, d);
    sort_net2(b, c);
};

template<typename T, typename Comparator>
void sort6(T &a, T &b, T &c, T &d, T &e, T &f, Comparator comp) {
    sort_net2(b, c);
    sort_net2(e, f);
    sort_net2(a, c);
    sort_net2(d, f);
    sort_net2(a, b);
    sort_net2(d, e);
    sort_net2(c, f);
    sort_net2(a, d);
    sort_net2(b, e);
    sort_net2(c, e);
    sort_net2(b, d);
    sort_net2(c, d);
};

#undef sort_net2

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define cond_swap(a,b)            cond_swap5(a,b);
#define iter_swap_if(a,b,c,d)     cond_swap5(a,b);

template<size_t n>
void static_sort1(int* p){
	static_sort1<n/2>(p);
	static_sort1<n-(n/2)>(p+(n/2));
	std::inplace_merge(p,p+(n/2),p+n);
	return;
};

template<>
void static_sort1<1>(int* p){
	return;
};

template<>
void static_sort1<2>(int* p){
	int tmp;
	cond_swap(p,p+1);
	return;
};

template<>
void static_sort1<3>(int* p){
	int tmp;
	cond_swap(p+1, p+2);
	cond_swap(p, p+2);
	cond_swap(p, p+1);
	return;
};

template<>
void static_sort1<4>(int* p){
	int tmp;
	cond_swap(p  , p+1);
	cond_swap(p+2, p+3);
	cond_swap(p  , p+2);
	cond_swap(p+1, p+3);
	cond_swap(p+1, p+2);

	return;
};

template<>
void static_sort1<5>(int* p){
	int tmp;
	cond_swap(p  , p+1);
	cond_swap(p+3, p+4);
	cond_swap(p+2, p+4);
	cond_swap(p+2, p+3);
	cond_swap(p  , p+3);
	cond_swap(p  , p+2);
	cond_swap(p+1, p+4);
	cond_swap(p+1, p+3);
	cond_swap(p+1, p+2);

	return;
};

template<>
void static_sort1<6>(int* p){
	int tmp;
	cond_swap(p+1, p+2);
	cond_swap(p  , p+2);
	cond_swap(p  , p+1);
	cond_swap(p+4, p+5);
	cond_swap(p+3, p+5);
	cond_swap(p+3, p+4);
	cond_swap(p  , p+3);
	cond_swap(p+1, p+4);
	cond_swap(p+2, p+5);
	cond_swap(p+2, p+4);
	cond_swap(p+1, p+3);
	cond_swap(p+2, p+3);

	return;
};

template<>
void static_sort1<7>(int* p){
	int tmp;
	cond_swap(p+1, p+2);
	cond_swap(p  , p+2);
	cond_swap(p  , p+1);
	cond_swap(p+3, p+4);
	cond_swap(p+5, p+6);
	cond_swap(p+3, p+5);
	cond_swap(p+4, p+6);
	cond_swap(p+4, p+5);
	cond_swap(p  , p+4);
	cond_swap(p  , p+3);
	cond_swap(p+1, p+5);
	cond_swap(p+2, p+6);
	cond_swap(p+2, p+5);
	cond_swap(p+1, p+3);
	cond_swap(p+2, p+4);
	cond_swap(p+2, p+3);

	return;
};

template<>
void static_sort1<8>(int* p){
	int tmp;
	cond_swap(p  , p+1);
	cond_swap(p+2, p+3);
	cond_swap(p  , p+2);
	cond_swap(p+1, p+3);
	cond_swap(p+1, p+2);
	cond_swap(p+4, p+5);
	cond_swap(p+6, p+7);
	cond_swap(p+4, p+6);
	cond_swap(p+5, p+7);
	cond_swap(p+5, p+6);
	cond_swap(p  , p+4);
	cond_swap(p+1, p+5);
	cond_swap(p+1, p+4);
	cond_swap(p+2, p+6);
	cond_swap(p+3, p+7);
	cond_swap(p+3, p+6);
	cond_swap(p+2, p+4);
	cond_swap(p+3, p+5);
	cond_swap(p+3, p+4);

	return;
};

template<>
void static_sort1<9>(int* p){
	int tmp;
	cond_swap(p  , p+1);
	cond_swap(p+3, p+4);
	cond_swap(p+6, p+7);
	cond_swap(p+1, p+2);
	cond_swap(p+4, p+5);
	cond_swap(p+7, p+8);
	cond_swap(p  , p+1);
	cond_swap(p+3, p+4);
	cond_swap(p+6, p+7);
	cond_swap(p  , p+3);
	cond_swap(p+3, p+6);
	cond_swap(p  , p+3);
	cond_swap(p+1, p+4);
	cond_swap(p+4, p+7);
	cond_swap(p+1, p+4);
	cond_swap(p+2, p+5);
	cond_swap(p+5, p+8);
	cond_swap(p+2, p+5);
	cond_swap(p+1, p+3);
	cond_swap(p+5, p+7);
	cond_swap(p+2, p+6);
	cond_swap(p+4, p+6);
	cond_swap(p+2, p+4);
	cond_swap(p+2, p+3);
	cond_swap(p+5, p+6);

	return;
};

template<>
void static_sort1<10>(int* p){
	int tmp;
	cond_swap(p+4, p+9);
	cond_swap(p+3, p+8);
	cond_swap(p+2, p+7);
	cond_swap(p+1, p+6);
	cond_swap(p  , p+5);
	cond_swap(p+1, p+4);
	cond_swap(p+6, p+9);
	cond_swap(p  , p+3);
	cond_swap(p+5, p+8);
	cond_swap(p  , p+2);
	cond_swap(p+3, p+6);
	cond_swap(p+7, p+9);
	cond_swap(p  , p+1);
	cond_swap(p+2, p+4);
	cond_swap(p+5, p+7);
	cond_swap(p+8, p+9);
	cond_swap(p+1, p+2);
	cond_swap(p+4, p+6);
	cond_swap(p+7, p+8);
	cond_swap(p+3, p+5);
	cond_swap(p+2, p+5);
	cond_swap(p+6, p+8);
	cond_swap(p+1, p+3);
	cond_swap(p+4, p+7);
	cond_swap(p+2, p+3);
	cond_swap(p+6, p+7);
	cond_swap(p+3, p+4);
	cond_swap(p+5, p+6);
	cond_swap(p+4, p+5);

	return;
};

template<>
void static_sort1<11>(int* p){
	int tmp;
	cond_swap(p  , p+1);
	cond_swap(p+2, p+3);
	cond_swap(p+4, p+5);
	cond_swap(p+6, p+7);
	cond_swap(p+8, p+9);
	cond_swap(p+1, p+3);
	cond_swap(p+5, p+7);
	cond_swap(p  , p+2);
	cond_swap(p+4, p+6);
	cond_swap(p+8, p+10);
	cond_swap(p+1, p+2);
	cond_swap(p+5, p+6);
	cond_swap(p+9, p+10);
	cond_swap(p+1, p+5);
	cond_swap(p+6, p+10);
	cond_swap(p+5, p+9);
	cond_swap(p+2, p+6);
	cond_swap(p+1, p+5);
	cond_swap(p+6, p+10);
	cond_swap(p  , p+4);
	cond_swap(p+3, p+7);
	cond_swap(p+4, p+8);
	cond_swap(p  , p+4);
	cond_swap(p+1, p+4);
	cond_swap(p+7, p+10);
	cond_swap(p+3, p+8);
	cond_swap(p+2, p+3);
	cond_swap(p+8, p+9);
	cond_swap(p+2, p+4);
	cond_swap(p+7, p+9);
	cond_swap(p+3, p+5);
	cond_swap(p+6, p+8);
	cond_swap(p+3, p+4);
	cond_swap(p+5, p+6);
	cond_swap(p+7, p+8);

	return;
};

template<>
void static_sort1<12>(int* p){
	int tmp;
	cond_swap(p  , p+1);
	cond_swap(p+2, p+3);
	cond_swap(p+4, p+5);
	cond_swap(p+6, p+7);
	cond_swap(p+8, p+9);
	cond_swap(p+10,p+11);
	cond_swap(p+1, p+3);
	cond_swap(p+5, p+7);
	cond_swap(p+9, p+11);
	cond_swap(p  , p+2);
	cond_swap(p+4, p+6);
	cond_swap(p+8, p+10);
	cond_swap(p+1, p+2);
	cond_swap(p+5, p+6);
	cond_swap(p+9, p+10);
	cond_swap(p+1, p+5);
	cond_swap(p+6, p+10);
	cond_swap(p+5, p+9);
	cond_swap(p+2, p+6);
	cond_swap(p+1, p+5);
	cond_swap(p+6, p+10);
	cond_swap(p  , p+4);
	cond_swap(p+7, p+11);
	cond_swap(p+3, p+7);
	cond_swap(p+4, p+8);
	cond_swap(p  , p+4);
	cond_swap(p+7, p+11);
	cond_swap(p+1, p+4);
	cond_swap(p+7, p+10);
	cond_swap(p+3, p+8);
	cond_swap(p+2, p+3);
	cond_swap(p+8, p+9);
	cond_swap(p+2, p+4);
	cond_swap(p+7, p+9);
	cond_swap(p+3, p+5);
	cond_swap(p+6, p+8);
	cond_swap(p+3, p+4);
	cond_swap(p+5, p+6);
	cond_swap(p+7, p+8);

	return;
};

template<>
void static_sort1<13>(int* p){
	int tmp;
	cond_swap(p+ 1, p+7);
	cond_swap(p+9 , p+11);
	cond_swap(p+3 , p+4);
	cond_swap(p+5 , p+8);
	cond_swap(p   , p+12);
	cond_swap(p+2 , p+6);
	cond_swap(p   , p+1);
	cond_swap(p+2 , p+3);
	cond_swap(p+4 , p+6);
	cond_swap(p+8 , p+11);
	cond_swap(p+7 , p+12);
	cond_swap(p+5 , p+9);
	cond_swap(p   , p+2);
	cond_swap(p+3 , p+7);
	cond_swap(p+10, p+11);
	cond_swap(p+1 , p+4);
	cond_swap(p+6 , p+12);
	cond_swap(p+7 , p+8);
	cond_swap(p+11, p+12);
	cond_swap(p+4 , p+9);
	cond_swap(p+6 , p+10);
	cond_swap(p+3 , p+4);
	cond_swap(p+5 , p+6);
	cond_swap(p+8 , p+9);
	cond_swap(p+10, p+11);
	cond_swap(p+1 , p+7);
	cond_swap(p+2 , p+6);
	cond_swap(p+9 , p+11);
	cond_swap(p+1 , p+3);
	cond_swap(p+4 , p+7);
	cond_swap(p+8 , p+10);
	cond_swap(p   , p+5);
	cond_swap(p+2 , p+5);
	cond_swap(p+6 , p+8);
	cond_swap(p+9 , p+10);
	cond_swap(p+1 , p+2);
	cond_swap(p+3 , p+5);
	cond_swap(p+7 , p+8);
	cond_swap(p+4 , p+6);
	cond_swap(p+2 , p+3);
	cond_swap(p+4 , p+5);
	cond_swap(p+6 , p+7);
	cond_swap(p+8 , p+9);
	cond_swap(p+3 , p+4);
	cond_swap(p+5 , p+6);

	return;
};

template<>
void static_sort1<14>(int* p){
	int tmp;
	cond_swap(p   , p+1);
	cond_swap(p+2 , p+3);
	cond_swap(p+4 , p+5);
	cond_swap(p+6 , p+7);
	cond_swap(p+8 , p+9);
	cond_swap(p+10, p+11);
	cond_swap(p+12, p+13);
	cond_swap(p   , p+2);
	cond_swap(p+4 , p+6);
	cond_swap(p+8 , p+10);
	cond_swap(p+1 , p+3);
	cond_swap(p+5 , p+7);
	cond_swap(p+9 , p+11);
	cond_swap(p   , p+4);
	cond_swap(p+8 , p+12);
	cond_swap(p+1 , p+5);
	cond_swap(p+9 , p+13);
	cond_swap(p+2 , p+6);
	cond_swap(p+3 , p+7);
	cond_swap(p   , p+8);
	cond_swap(p+1 , p+9);
	cond_swap(p+2 , p+10);
	cond_swap(p+3 , p+11);
	cond_swap(p+4 , p+12);
	cond_swap(p+5 , p+13);
	cond_swap(p+5 , p+10);
	cond_swap(p+6 , p+9);
	cond_swap(p+3 , p+12);
	cond_swap(p+7 , p+11);
	cond_swap(p+1 , p+2);
	cond_swap(p+4 , p+8);
	cond_swap(p+1 , p+4);
	cond_swap(p+7 , p+13);
	cond_swap(p+2 , p+8);
	cond_swap(p+2 , p+4);
	cond_swap(p+5 , p+6);
	cond_swap(p+9 , p+10);
	cond_swap(p+11, p+13);
	cond_swap(p+3 , p+8);
	cond_swap(p+7 , p+12);
	cond_swap(p+6 , p+8);
	cond_swap(p+10, p+12);
	cond_swap(p+3 , p+5);
	cond_swap(p+7 , p+9);
	cond_swap(p+3 , p+4);
	cond_swap(p+5 , p+6);
	cond_swap(p+7 , p+8);
	cond_swap(p+9 , p+10);
	cond_swap(p+11, p+12);
	cond_swap(p+6 , p+7);
	cond_swap(p+8 , p+9);

	return;
};

template<>
void static_sort1<15>(int* p){
	int tmp;
	cond_swap(p   , p+1);
	cond_swap(p+2 , p+3);
	cond_swap(p+4 , p+5);
	cond_swap(p+6 , p+7);
	cond_swap(p+8 , p+9);
	cond_swap(p+10, p+11);
	cond_swap(p+12, p+13);
	cond_swap(p   , p+2);
	cond_swap(p+4 , p+6);
	cond_swap(p+8 , p+10);
	cond_swap(p+12, p+14);
	cond_swap(p+1 , p+3);
	cond_swap(p+5 , p+7);
	cond_swap(p+9 , p+11);
	cond_swap(p   , p+4);
	cond_swap(p+8 , p+12);
	cond_swap(p+1 , p+5);
	cond_swap(p+9 , p+13);
	cond_swap(p+2 , p+6);
	cond_swap(p+10, p+14);
	cond_swap(p+3 , p+7);
	cond_swap(p   , p+8);
	cond_swap(p+1 , p+9);
	cond_swap(p+2 , p+10);
	cond_swap(p+3 , p+11);
	cond_swap(p+4 , p+12);
	cond_swap(p+5 , p+13);
	cond_swap(p+6 , p+14);
	cond_swap(p+5 , p+10);
	cond_swap(p+6 , p+9);
	cond_swap(p+3 , p+12);
	cond_swap(p+13, p+14);
	cond_swap(p+7 , p+11);
	cond_swap(p+1 , p+2);
	cond_swap(p+4 , p+8);
	cond_swap(p+1 , p+4);
	cond_swap(p+7 , p+13);
	cond_swap(p+2 , p+8);
	cond_swap(p+11, p+14);
	cond_swap(p+2 , p+4);
	cond_swap(p+5 , p+6);
	cond_swap(p+9 , p+10);
	cond_swap(p+11, p+13);
	cond_swap(p+3 , p+8);
	cond_swap(p+7 , p+12);
	cond_swap(p+6 , p+8);
	cond_swap(p+10, p+12);
	cond_swap(p+3 , p+5);
	cond_swap(p+7 , p+9);
	cond_swap(p+3 , p+4);
	cond_swap(p+5 , p+6);
	cond_swap(p+7 , p+8);
	cond_swap(p+9 , p+10);
	cond_swap(p+11, p+12);
	cond_swap(p+6 , p+7);
	cond_swap(p+8 , p+9);

	return;
};

template<>
void static_sort1<16>(int* p){
	int tmp;
	cond_swap(p   , p+1);
//	std::cout<<1<<std::endl;
	cond_swap(p+2 , p+3);
//	std::cout<<2<<std::endl;
	cond_swap(p+4 , p+5);
//	std::cout<<3<<std::endl;
	cond_swap(p+6 , p+7);
//	std::cout<<4<<std::endl;
	cond_swap(p+8 , p+9);
//	std::cout<<5<<std::endl;
	cond_swap(p+10, p+11);
//	std::cout<<6<<std::endl;
	cond_swap(p+12, p+13);
//	std::cout<<7<<std::endl;
	cond_swap(p+14, p+15);
//	std::cout<<8<<std::endl;
	cond_swap(p   , p+2);
//	std::cout<<9<<std::endl;
	cond_swap(p+4 , p+6);
//	std::cout<<9<<std::endl;
	cond_swap(p+8 , p+10);
//	std::cout<<10<<std::endl;
	cond_swap(p+12, p+14);
//	std::cout<<11<<std::endl;
	cond_swap(p+1 , p+3);
//	std::cout<<12<<std::endl;
	cond_swap(p+5 , p+7);
//	std::cout<<13<<std::endl;
	cond_swap(p+9 , p+11);
//	std::cout<<14<<std::endl;
	cond_swap(p+13, p+15);
//	std::cout<<15<<std::endl;
	cond_swap(p   , p+4);
//	std::cout<<16<<std::endl;
	cond_swap(p+8 , p+12);
//	std::cout<<17<<std::endl;
	cond_swap(p+1 , p+5);
//	std::cout<<18<<std::endl;
	cond_swap(p+9 , p+13);
//	std::cout<<19<<std::endl;
	cond_swap(p+2 , p+6);
//	std::cout<<20<<std::endl;
	cond_swap(p+10, p+14);
//	std::cout<<21<<std::endl;
	cond_swap(p+3 , p+7);
//	std::cout<<22<<std::endl;
	cond_swap(p+11, p+15);
//	std::cout<<23<<std::endl;
	cond_swap(p   , p+8);
//	std::cout<<24<<std::endl;
	cond_swap(p+1 , p+9);
//	std::cout<<25<<std::endl;
	cond_swap(p+2 , p+10);
//	std::cout<<26<<std::endl;
	cond_swap(p+3 , p+11);
//	std::cout<<27<<std::endl;
	cond_swap(p+4 , p+12);
//	std::cout<<28<<std::endl;
	cond_swap(p+5 , p+13);
//	std::cout<<29<<std::endl;
	cond_swap(p+6 , p+14);
//	std::cout<<30<<std::endl;
	cond_swap(p+7 , p+15);
//	std::cout<<31<<std::endl;
	cond_swap(p+5 , p+10);
//	std::cout<<32<<std::endl;
	cond_swap(p+6 , p+9);
//	std::cout<<33<<std::endl;
	cond_swap(p+3 , p+12);
//	std::cout<<34<<std::endl;
	cond_swap(p+13, p+14);
//	std::cout<<35<<std::endl;
	cond_swap(p+7 , p+11);
//	std::cout<<36<<std::endl;
	cond_swap(p+1 , p+2);
//	std::cout<<37<<std::endl;
	cond_swap(p+4 , p+8);
//	std::cout<<38<<std::endl;
	cond_swap(p+1 , p+4);
//	std::cout<<39<<std::endl;
	cond_swap(p+7 , p+13);
//	std::cout<<40<<std::endl;
	cond_swap(p+2 , p+8);
//	std::cout<<41<<std::endl;
	cond_swap(p+11, p+14);
//	std::cout<<42<<std::endl;
	cond_swap(p+2 , p+4);
//	std::cout<<43<<std::endl;
	cond_swap(p+5 , p+6);
//	std::cout<<44<<std::endl;
	cond_swap(p+9 , p+10);
//	std::cout<<45<<std::endl;
	cond_swap(p+11, p+13);
//	std::cout<<46<<std::endl;
	cond_swap(p+3 , p+8);
//	std::cout<<47<<std::endl;
	cond_swap(p+7 , p+12);
//	std::cout<<48<<std::endl;
	cond_swap(p+6 , p+8);
//	std::cout<<49<<std::endl;
	cond_swap(p+10, p+12);
//	std::cout<<50<<std::endl;
	cond_swap(p+3 , p+5);
//	std::cout<<51<<std::endl;
	cond_swap(p+7 , p+9);
//	std::cout<<52<<std::endl;
	cond_swap(p+3 , p+4);
//	std::cout<<53<<std::endl;
	cond_swap(p+5 , p+6);
//	std::cout<<54<<std::endl;
	cond_swap(p+7 , p+8);
//	std::cout<<55<<std::endl;
	cond_swap(p+9 , p+10);
//	std::cout<<56<<std::endl;
	cond_swap(p+11, p+12);
//	std::cout<<57<<std::endl;
	cond_swap(p+6 , p+7);
//	std::cout<<58<<std::endl;
	cond_swap(p+8 , p+9);
//	std::cout<<59<<std::endl;

	return;
};

//------------------------------------- Now copeyd from c++14 sorting library

template<>
void static_sort1<17>(int* first){
	int tmp;
	iter_swap_if(first + 5u, first + 11u, compare, projection);
    iter_swap_if(first + 4u, first + 9u, compare, projection);
    iter_swap_if(first + 7u, first + 12u, compare, projection);
    iter_swap_if(first + 0u, first + 14u, compare, projection);
    iter_swap_if(first + 2u, first + 16u, compare, projection);
    iter_swap_if(first + 1u, first + 15u, compare, projection);
    iter_swap_if(first + 3u, first + 8u, compare, projection);
    iter_swap_if(first + 6u, first + 13u, compare, projection);
    iter_swap_if(first + 3u, first + 10u, compare, projection);
    iter_swap_if(first + 8u, first + 13u, compare, projection);
    iter_swap_if(first + 4u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 0u, first + 2u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 1u, first + 6u, compare, projection);
    iter_swap_if(first + 10u, first + 15u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 0u, first + 4u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 1u, first + 3u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 2u, first + 9u, compare, projection);
    iter_swap_if(first + 7u, first + 14u, compare, projection);
    iter_swap_if(first + 5u, first + 10u, compare, projection);
    iter_swap_if(first + 6u, first + 11u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 8u, first + 14u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 10u, first + 14u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 1u, first + 8u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 1u, first + 4u, compare, projection);
    iter_swap_if(first + 2u, first + 5u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 12u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 3u, first + 10u, compare, projection);
    iter_swap_if(first + 3u, first + 8u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
};

template<>
void static_sort1<18>(int* first){
	int tmp;
    iter_swap_if(first + 4u, first + 12u, compare, projection);
    iter_swap_if(first + 5u, first + 13u, compare, projection);
    iter_swap_if(first + 0u, first + 7u, compare, projection);
    iter_swap_if(first + 10u, first + 17u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 1u, first + 16u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 11u, first + 15u, compare, projection);
    iter_swap_if(first + 1u, first + 9u, compare, projection);
    iter_swap_if(first + 8u, first + 16u, compare, projection);
    iter_swap_if(first + 4u, first + 10u, compare, projection);
    iter_swap_if(first + 7u, first + 13u, compare, projection);
    iter_swap_if(first + 3u, first + 12u, compare, projection);
    iter_swap_if(first + 5u, first + 14u, compare, projection);
    iter_swap_if(first + 0u, first + 2u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 1u, first + 4u, compare, projection);
    iter_swap_if(first + 13u, first + 16u, compare, projection);
    iter_swap_if(first + 0u, first + 5u, compare, projection);
    iter_swap_if(first + 12u, first + 17u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 3u, first + 7u, compare, projection);
    iter_swap_if(first + 10u, first + 14u, compare, projection);
    iter_swap_if(first + 6u, first + 9u, compare, projection);
    iter_swap_if(first + 8u, first + 11u, compare, projection);
    iter_swap_if(first + 2u, first + 15u, compare, projection);
    iter_swap_if(first + 3u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 14u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 1u, first + 4u, compare, projection);
    iter_swap_if(first + 13u, first + 16u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 11u, first + 15u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 5u, first + 11u, compare, projection);
    iter_swap_if(first + 6u, first + 12u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 10u, first + 13u, compare, projection);
    iter_swap_if(first + 4u, first + 7u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 6u, first + 9u, compare, projection);
    iter_swap_if(first + 8u, first + 11u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
};

template<>
void static_sort1<19>(int* first){
	int tmp;
    iter_swap_if(first + 0u, first + 12u, compare, projection);
    iter_swap_if(first + 1u, first + 4u, compare, projection);
    iter_swap_if(first + 2u, first + 8u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 17u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 9u, first + 14u, compare, projection);
    iter_swap_if(first + 10u, first + 13u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 0u, first + 2u, compare, projection);
    iter_swap_if(first + 1u, first + 7u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 4u, first + 11u, compare, projection);
    iter_swap_if(first + 5u, first + 17u, compare, projection);
    iter_swap_if(first + 8u, first + 12u, compare, projection);
    iter_swap_if(first + 10u, first + 15u, compare, projection);
    iter_swap_if(first + 13u, first + 16u, compare, projection);
    iter_swap_if(first + 14u, first + 18u, compare, projection);
    iter_swap_if(first + 3u, first + 10u, compare, projection);
    iter_swap_if(first + 4u, first + 14u, compare, projection);
    iter_swap_if(first + 5u, first + 15u, compare, projection);
    iter_swap_if(first + 6u, first + 13u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 17u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 0u, first + 7u, compare, projection);
    iter_swap_if(first + 1u, first + 10u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 9u, first + 15u, compare, projection);
    iter_swap_if(first + 11u, first + 16u, compare, projection);
    iter_swap_if(first + 12u, first + 17u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 0u, first + 3u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 1u, first + 8u, compare, projection);
    iter_swap_if(first + 2u, first + 9u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 15u, compare, projection);
    iter_swap_if(first + 7u, first + 13u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 18u, compare, projection);
    iter_swap_if(first + 1u, first + 3u, compare, projection);
    iter_swap_if(first + 2u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 9u, compare, projection);
    iter_swap_if(first + 7u, first + 12u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 4u, first + 8u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 10u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
};

template<>
void static_sort1<20>(int* first){
	int tmp;
    iter_swap_if(first + 0u, first + 3u, compare, projection);
    iter_swap_if(first + 1u, first + 7u, compare, projection);
    iter_swap_if(first + 2u, first + 5u, compare, projection);
    iter_swap_if(first + 4u, first + 8u, compare, projection);
    iter_swap_if(first + 6u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 13u, compare, projection);
    iter_swap_if(first + 11u, first + 15u, compare, projection);
    iter_swap_if(first + 12u, first + 18u, compare, projection);
    iter_swap_if(first + 14u, first + 17u, compare, projection);
    iter_swap_if(first + 16u, first + 19u, compare, projection);
    iter_swap_if(first + 0u, first + 14u, compare, projection);
    iter_swap_if(first + 1u, first + 11u, compare, projection);
    iter_swap_if(first + 2u, first + 16u, compare, projection);
    iter_swap_if(first + 3u, first + 17u, compare, projection);
    iter_swap_if(first + 4u, first + 12u, compare, projection);
    iter_swap_if(first + 5u, first + 19u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 7u, first + 15u, compare, projection);
    iter_swap_if(first + 8u, first + 18u, compare, projection);
    iter_swap_if(first + 9u, first + 13u, compare, projection);
    iter_swap_if(first + 0u, first + 4u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 8u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 11u, first + 16u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 19u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 1u, first + 6u, compare, projection);
    iter_swap_if(first + 2u, first + 12u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 4u, first + 11u, compare, projection);
    iter_swap_if(first + 7u, first + 17u, compare, projection);
    iter_swap_if(first + 8u, first + 15u, compare, projection);
    iter_swap_if(first + 13u, first + 18u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 18u, first + 19u, compare, projection);
    iter_swap_if(first + 1u, first + 6u, compare, projection);
    iter_swap_if(first + 5u, first + 9u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 8u, first + 12u, compare, projection);
    iter_swap_if(first + 10u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 18u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 4u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 15u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 1u, first + 3u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 13u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
};

template<>
void static_sort1<21>(int* first){
	int tmp;
    iter_swap_if(first + 0u, first + 20u, compare, projection);
    iter_swap_if(first + 1u, first + 19u, compare, projection);
    iter_swap_if(first + 2u, first + 18u, compare, projection);
    iter_swap_if(first + 3u, first + 17u, compare, projection);
    iter_swap_if(first + 4u, first + 16u, compare, projection);
    iter_swap_if(first + 5u, first + 15u, compare, projection);
    iter_swap_if(first + 6u, first + 14u, compare, projection);
    iter_swap_if(first + 7u, first + 13u, compare, projection);
    iter_swap_if(first + 8u, first + 12u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 0u, first + 3u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 4u, first + 8u, compare, projection);
    iter_swap_if(first + 5u, first + 9u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 15u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 13u, first + 19u, compare, projection);
    iter_swap_if(first + 14u, first + 18u, compare, projection);
    iter_swap_if(first + 17u, first + 20u, compare, projection);
    iter_swap_if(first + 1u, first + 7u, compare, projection);
    iter_swap_if(first + 2u, first + 13u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 8u, first + 11u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 10u, first + 19u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 0u, first + 4u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 7u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 15u, first + 19u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 1u, first + 4u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 5u, first + 14u, compare, projection);
    iter_swap_if(first + 6u, first + 9u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 17u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 4u, first + 10u, compare, projection);
    iter_swap_if(first + 8u, first + 13u, compare, projection);
    iter_swap_if(first + 11u, first + 15u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 19u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 4u, first + 7u, compare, projection);
    iter_swap_if(first + 5u, first + 15u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 16u, compare, projection);
    iter_swap_if(first + 10u, first + 13u, compare, projection);
    iter_swap_if(first + 12u, first + 17u, compare, projection);
    iter_swap_if(first + 14u, first + 19u, compare, projection);
    iter_swap_if(first + 1u, first + 4u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 3u, first + 7u, compare, projection);
    iter_swap_if(first + 5u, first + 9u, compare, projection);
    iter_swap_if(first + 8u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 17u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 18u, first + 19u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
};

template<>
void static_sort1<22>(int* first){
	int tmp;
	iter_swap_if(first + 0u, first + 11u, compare, projection);
	            iter_swap_if(first + 1u, first + 12u, compare, projection);
	            iter_swap_if(first + 2u, first + 13u, compare, projection);
	            iter_swap_if(first + 3u, first + 14u, compare, projection);
	            iter_swap_if(first + 4u, first + 15u, compare, projection);
	            iter_swap_if(first + 5u, first + 16u, compare, projection);
	            iter_swap_if(first + 6u, first + 17u, compare, projection);
	            iter_swap_if(first + 7u, first + 18u, compare, projection);
	            iter_swap_if(first + 8u, first + 19u, compare, projection);
	            iter_swap_if(first + 9u, first + 20u, compare, projection);
	            iter_swap_if(first + 10u, first + 21u, compare, projection);
	            iter_swap_if(first + 0u, first + 3u, compare, projection);
	            iter_swap_if(first + 1u, first + 5u, compare, projection);
	            iter_swap_if(first + 2u, first + 4u, compare, projection);
	            iter_swap_if(first + 6u, first + 8u, compare, projection);
	            iter_swap_if(first + 7u, first + 10u, compare, projection);
	            iter_swap_if(first + 11u, first + 14u, compare, projection);
	            iter_swap_if(first + 13u, first + 15u, compare, projection);
	            iter_swap_if(first + 16u, first + 20u, compare, projection);
	            iter_swap_if(first + 17u, first + 19u, compare, projection);
	            iter_swap_if(first + 18u, first + 21u, compare, projection);
	            iter_swap_if(first + 0u, first + 6u, compare, projection);
	            iter_swap_if(first + 1u, first + 9u, compare, projection);
	            iter_swap_if(first + 2u, first + 7u, compare, projection);
	            iter_swap_if(first + 3u, first + 8u, compare, projection);
	            iter_swap_if(first + 4u, first + 10u, compare, projection);
	            iter_swap_if(first + 11u, first + 17u, compare, projection);
	            iter_swap_if(first + 12u, first + 20u, compare, projection);
	            iter_swap_if(first + 13u, first + 18u, compare, projection);
	            iter_swap_if(first + 14u, first + 19u, compare, projection);
	            iter_swap_if(first + 15u, first + 21u, compare, projection);
	            iter_swap_if(first + 1u, first + 2u, compare, projection);
	            iter_swap_if(first + 4u, first + 11u, compare, projection);
	            iter_swap_if(first + 5u, first + 9u, compare, projection);
	            iter_swap_if(first + 6u, first + 7u, compare, projection);
	            iter_swap_if(first + 10u, first + 17u, compare, projection);
	            iter_swap_if(first + 12u, first + 16u, compare, projection);
	            iter_swap_if(first + 14u, first + 15u, compare, projection);
	            iter_swap_if(first + 19u, first + 20u, compare, projection);
	            iter_swap_if(first + 0u, first + 2u, compare, projection);
	            iter_swap_if(first + 5u, first + 7u, compare, projection);
	            iter_swap_if(first + 9u, first + 10u, compare, projection);
	            iter_swap_if(first + 11u, first + 12u, compare, projection);
	            iter_swap_if(first + 14u, first + 16u, compare, projection);
	            iter_swap_if(first + 19u, first + 21u, compare, projection);
	            iter_swap_if(first + 0u, first + 1u, compare, projection);
	            iter_swap_if(first + 2u, first + 14u, compare, projection);
	            iter_swap_if(first + 3u, first + 9u, compare, projection);
	            iter_swap_if(first + 4u, first + 5u, compare, projection);
	            iter_swap_if(first + 7u, first + 19u, compare, projection);
	            iter_swap_if(first + 8u, first + 10u, compare, projection);
	            iter_swap_if(first + 11u, first + 13u, compare, projection);
	            iter_swap_if(first + 12u, first + 18u, compare, projection);
	            iter_swap_if(first + 16u, first + 17u, compare, projection);
	            iter_swap_if(first + 20u, first + 21u, compare, projection);
	            iter_swap_if(first + 1u, first + 4u, compare, projection);
	            iter_swap_if(first + 2u, first + 6u, compare, projection);
	            iter_swap_if(first + 3u, first + 11u, compare, projection);
	            iter_swap_if(first + 8u, first + 12u, compare, projection);
	            iter_swap_if(first + 9u, first + 13u, compare, projection);
	            iter_swap_if(first + 10u, first + 18u, compare, projection);
	            iter_swap_if(first + 15u, first + 19u, compare, projection);
	            iter_swap_if(first + 17u, first + 20u, compare, projection);
	            iter_swap_if(first + 2u, first + 3u, compare, projection);
	            iter_swap_if(first + 5u, first + 6u, compare, projection);
	            iter_swap_if(first + 7u, first + 13u, compare, projection);
	            iter_swap_if(first + 8u, first + 14u, compare, projection);
	            iter_swap_if(first + 9u, first + 11u, compare, projection);
	            iter_swap_if(first + 10u, first + 12u, compare, projection);
	            iter_swap_if(first + 15u, first + 16u, compare, projection);
	            iter_swap_if(first + 18u, first + 19u, compare, projection);
	            iter_swap_if(first + 1u, first + 2u, compare, projection);
	            iter_swap_if(first + 3u, first + 4u, compare, projection);
	            iter_swap_if(first + 5u, first + 9u, compare, projection);
	            iter_swap_if(first + 7u, first + 14u, compare, projection);
	            iter_swap_if(first + 8u, first + 11u, compare, projection);
	            iter_swap_if(first + 10u, first + 13u, compare, projection);
	            iter_swap_if(first + 12u, first + 16u, compare, projection);
	            iter_swap_if(first + 17u, first + 18u, compare, projection);
	            iter_swap_if(first + 19u, first + 20u, compare, projection);
	            iter_swap_if(first + 2u, first + 3u, compare, projection);
	            iter_swap_if(first + 6u, first + 8u, compare, projection);
	            iter_swap_if(first + 13u, first + 15u, compare, projection);
	            iter_swap_if(first + 18u, first + 19u, compare, projection);
	            iter_swap_if(first + 4u, first + 6u, compare, projection);
	            iter_swap_if(first + 8u, first + 11u, compare, projection);
	            iter_swap_if(first + 10u, first + 13u, compare, projection);
	            iter_swap_if(first + 15u, first + 17u, compare, projection);
	            iter_swap_if(first + 4u, first + 5u, compare, projection);
	            iter_swap_if(first + 7u, first + 10u, compare, projection);
	            iter_swap_if(first + 11u, first + 14u, compare, projection);
	            iter_swap_if(first + 16u, first + 17u, compare, projection);
	            iter_swap_if(first + 3u, first + 4u, compare, projection);
	            iter_swap_if(first + 7u, first + 9u, compare, projection);
	            iter_swap_if(first + 10u, first + 11u, compare, projection);
	            iter_swap_if(first + 12u, first + 14u, compare, projection);
	            iter_swap_if(first + 17u, first + 18u, compare, projection);
	            iter_swap_if(first + 6u, first + 7u, compare, projection);
	            iter_swap_if(first + 8u, first + 9u, compare, projection);
	            iter_swap_if(first + 12u, first + 13u, compare, projection);
	            iter_swap_if(first + 14u, first + 15u, compare, projection);
	            iter_swap_if(first + 5u, first + 6u, compare, projection);
	            iter_swap_if(first + 7u, first + 8u, compare, projection);
	            iter_swap_if(first + 9u, first + 10u, compare, projection);
	            iter_swap_if(first + 11u, first + 12u, compare, projection);
	            iter_swap_if(first + 13u, first + 14u, compare, projection);
	            iter_swap_if(first + 15u, first + 16u, compare, projection);
};

template<>
void static_sort1<23>(int* first){
	int tmp;
    iter_swap_if(first + 0u, first + 2u, compare, projection);
    iter_swap_if(first + 1u, first + 8u, compare, projection);
    iter_swap_if(first + 3u, first + 18u, compare, projection);
    iter_swap_if(first + 4u, first + 17u, compare, projection);
    iter_swap_if(first + 5u, first + 20u, compare, projection);
    iter_swap_if(first + 6u, first + 19u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 22u, compare, projection);
    iter_swap_if(first + 0u, first + 3u, compare, projection);
    iter_swap_if(first + 1u, first + 15u, compare, projection);
    iter_swap_if(first + 2u, first + 18u, compare, projection);
    iter_swap_if(first + 4u, first + 12u, compare, projection);
    iter_swap_if(first + 5u, first + 21u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 7u, first + 14u, compare, projection);
    iter_swap_if(first + 8u, first + 22u, compare, projection);
    iter_swap_if(first + 9u, first + 16u, compare, projection);
    iter_swap_if(first + 11u, first + 19u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 0u, first + 4u, compare, projection);
    iter_swap_if(first + 1u, first + 7u, compare, projection);
    iter_swap_if(first + 2u, first + 13u, compare, projection);
    iter_swap_if(first + 3u, first + 12u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 14u, compare, projection);
    iter_swap_if(first + 9u, first + 15u, compare, projection);
    iter_swap_if(first + 10u, first + 21u, compare, projection);
    iter_swap_if(first + 11u, first + 20u, compare, projection);
    iter_swap_if(first + 16u, first + 22u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 0u, first + 5u, compare, projection);
    iter_swap_if(first + 2u, first + 11u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 4u, first + 10u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 12u, first + 21u, compare, projection);
    iter_swap_if(first + 13u, first + 19u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 20u, compare, projection);
    iter_swap_if(first + 18u, first + 22u, compare, projection);
    iter_swap_if(first + 1u, first + 5u, compare, projection);
    iter_swap_if(first + 2u, first + 8u, compare, projection);
    iter_swap_if(first + 3u, first + 9u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 14u, first + 20u, compare, projection);
    iter_swap_if(first + 15u, first + 21u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 1u, first + 3u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 13u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 18u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 19u, first + 21u, compare, projection);
    iter_swap_if(first + 20u, first + 22u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 12u, compare, projection);
    iter_swap_if(first + 7u, first + 13u, compare, projection);
    iter_swap_if(first + 10u, first + 16u, compare, projection);
    iter_swap_if(first + 11u, first + 17u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 18u, first + 19u, compare, projection);
    iter_swap_if(first + 20u, first + 21u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 18u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 11u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 12u, first + 15u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
};

template<>
void static_sort1<24>(int* first){
	int tmp;
    iter_swap_if(first + 0u, first + 2u, compare, projection);
    iter_swap_if(first + 1u, first + 8u, compare, projection);
    iter_swap_if(first + 3u, first + 18u, compare, projection);
    iter_swap_if(first + 4u, first + 17u, compare, projection);
    iter_swap_if(first + 5u, first + 20u, compare, projection);
    iter_swap_if(first + 6u, first + 19u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 22u, compare, projection);
    iter_swap_if(first + 21u, first + 23u, compare, projection);
    iter_swap_if(first + 0u, first + 3u, compare, projection);
    iter_swap_if(first + 1u, first + 15u, compare, projection);
    iter_swap_if(first + 2u, first + 18u, compare, projection);
    iter_swap_if(first + 4u, first + 12u, compare, projection);
    iter_swap_if(first + 5u, first + 21u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 7u, first + 14u, compare, projection);
    iter_swap_if(first + 8u, first + 22u, compare, projection);
    iter_swap_if(first + 9u, first + 16u, compare, projection);
    iter_swap_if(first + 11u, first + 19u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 20u, first + 23u, compare, projection);
    iter_swap_if(first + 0u, first + 4u, compare, projection);
    iter_swap_if(first + 1u, first + 7u, compare, projection);
    iter_swap_if(first + 2u, first + 13u, compare, projection);
    iter_swap_if(first + 3u, first + 12u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 14u, compare, projection);
    iter_swap_if(first + 9u, first + 15u, compare, projection);
    iter_swap_if(first + 10u, first + 21u, compare, projection);
    iter_swap_if(first + 11u, first + 20u, compare, projection);
    iter_swap_if(first + 16u, first + 22u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 23u, compare, projection);
    iter_swap_if(first + 0u, first + 5u, compare, projection);
    iter_swap_if(first + 2u, first + 11u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 4u, first + 10u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 12u, first + 21u, compare, projection);
    iter_swap_if(first + 13u, first + 19u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 20u, compare, projection);
    iter_swap_if(first + 18u, first + 23u, compare, projection);
    iter_swap_if(first + 1u, first + 5u, compare, projection);
    iter_swap_if(first + 2u, first + 8u, compare, projection);
    iter_swap_if(first + 3u, first + 9u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 14u, first + 20u, compare, projection);
    iter_swap_if(first + 15u, first + 21u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 18u, first + 22u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 22u, first + 23u, compare, projection);
    iter_swap_if(first + 1u, first + 3u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 13u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 18u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 19u, first + 21u, compare, projection);
    iter_swap_if(first + 20u, first + 22u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 12u, compare, projection);
    iter_swap_if(first + 7u, first + 13u, compare, projection);
    iter_swap_if(first + 10u, first + 16u, compare, projection);
    iter_swap_if(first + 11u, first + 17u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 18u, first + 19u, compare, projection);
    iter_swap_if(first + 20u, first + 21u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 18u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 11u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 12u, first + 15u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
};

template<>
void static_sort1<25>(int* first){
	int tmp;
    iter_swap_if(first + 2u, first + 14u, compare, projection);
    iter_swap_if(first + 18u, first + 22u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 16u, compare, projection);
    iter_swap_if(first + 0u, first + 24u, compare, projection);
    iter_swap_if(first + 4u, first + 12u, compare, projection);
    iter_swap_if(first + 0u, first + 2u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 12u, compare, projection);
    iter_swap_if(first + 16u, first + 22u, compare, projection);
    iter_swap_if(first + 14u, first + 24u, compare, projection);
    iter_swap_if(first + 10u, first + 18u, compare, projection);
    iter_swap_if(first + 0u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 14u, compare, projection);
    iter_swap_if(first + 20u, first + 22u, compare, projection);
    iter_swap_if(first + 2u, first + 8u, compare, projection);
    iter_swap_if(first + 12u, first + 24u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 22u, first + 24u, compare, projection);
    iter_swap_if(first + 8u, first + 18u, compare, projection);
    iter_swap_if(first + 12u, first + 20u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 20u, first + 22u, compare, projection);
    iter_swap_if(first + 2u, first + 14u, compare, projection);
    iter_swap_if(first + 4u, first + 12u, compare, projection);
    iter_swap_if(first + 18u, first + 22u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 20u, compare, projection);
    iter_swap_if(first + 0u, first + 10u, compare, projection);
    iter_swap_if(first + 4u, first + 10u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 8u, first + 12u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 1u, first + 3u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 21u, first + 23u, compare, projection);
    iter_swap_if(first + 3u, first + 7u, compare, projection);
    iter_swap_if(first + 11u, first + 15u, compare, projection);
    iter_swap_if(first + 19u, first + 23u, compare, projection);
    iter_swap_if(first + 1u, first + 5u, compare, projection);
    iter_swap_if(first + 9u, first + 13u, compare, projection);
    iter_swap_if(first + 17u, first + 21u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 19u, first + 21u, compare, projection);
    iter_swap_if(first + 3u, first + 11u, compare, projection);
    iter_swap_if(first + 13u, first + 21u, compare, projection);
    iter_swap_if(first + 11u, first + 19u, compare, projection);
    iter_swap_if(first + 5u, first + 13u, compare, projection);
    iter_swap_if(first + 3u, first + 11u, compare, projection);
    iter_swap_if(first + 13u, first + 21u, compare, projection);
    iter_swap_if(first + 1u, first + 9u, compare, projection);
    iter_swap_if(first + 15u, first + 23u, compare, projection);
    iter_swap_if(first + 7u, first + 15u, compare, projection);
    iter_swap_if(first + 9u, first + 17u, compare, projection);
    iter_swap_if(first + 1u, first + 9u, compare, projection);
    iter_swap_if(first + 15u, first + 23u, compare, projection);
    iter_swap_if(first + 3u, first + 9u, compare, projection);
    iter_swap_if(first + 15u, first + 21u, compare, projection);
    iter_swap_if(first + 7u, first + 17u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 5u, first + 9u, compare, projection);
    iter_swap_if(first + 15u, first + 19u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 18u, first + 19u, compare, projection);
    iter_swap_if(first + 20u, first + 21u, compare, projection);
    iter_swap_if(first + 22u, first + 23u, compare, projection);
    iter_swap_if(first + 1u, first + 16u, compare, projection);
    iter_swap_if(first + 3u, first + 18u, compare, projection);
    iter_swap_if(first + 5u, first + 20u, compare, projection);
    iter_swap_if(first + 7u, first + 22u, compare, projection);
    iter_swap_if(first + 9u, first + 24u, compare, projection);
    iter_swap_if(first + 1u, first + 8u, compare, projection);
    iter_swap_if(first + 3u, first + 10u, compare, projection);
    iter_swap_if(first + 5u, first + 12u, compare, projection);
    iter_swap_if(first + 7u, first + 14u, compare, projection);
    iter_swap_if(first + 9u, first + 16u, compare, projection);
    iter_swap_if(first + 11u, first + 18u, compare, projection);
    iter_swap_if(first + 13u, first + 20u, compare, projection);
    iter_swap_if(first + 15u, first + 22u, compare, projection);
    iter_swap_if(first + 17u, first + 24u, compare, projection);
    iter_swap_if(first + 1u, first + 4u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 18u, compare, projection);
    iter_swap_if(first + 17u, first + 20u, compare, projection);
    iter_swap_if(first + 19u, first + 22u, compare, projection);
    iter_swap_if(first + 21u, first + 24u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 23u, first + 24u, compare, projection);
};

template<>
void static_sort1<26>(int* first){
	int tmp;
    iter_swap_if(first + 2u, first + 14u, compare, projection);
    iter_swap_if(first + 18u, first + 22u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 16u, compare, projection);
    iter_swap_if(first + 0u, first + 24u, compare, projection);
    iter_swap_if(first + 4u, first + 12u, compare, projection);
    iter_swap_if(first + 0u, first + 2u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 12u, compare, projection);
    iter_swap_if(first + 16u, first + 22u, compare, projection);
    iter_swap_if(first + 14u, first + 24u, compare, projection);
    iter_swap_if(first + 10u, first + 18u, compare, projection);
    iter_swap_if(first + 0u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 14u, compare, projection);
    iter_swap_if(first + 20u, first + 22u, compare, projection);
    iter_swap_if(first + 2u, first + 8u, compare, projection);
    iter_swap_if(first + 12u, first + 24u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 22u, first + 24u, compare, projection);
    iter_swap_if(first + 8u, first + 18u, compare, projection);
    iter_swap_if(first + 12u, first + 20u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 20u, first + 22u, compare, projection);
    iter_swap_if(first + 2u, first + 14u, compare, projection);
    iter_swap_if(first + 4u, first + 12u, compare, projection);
    iter_swap_if(first + 18u, first + 22u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 20u, compare, projection);
    iter_swap_if(first + 0u, first + 10u, compare, projection);
    iter_swap_if(first + 4u, first + 10u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 8u, first + 12u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 3u, first + 15u, compare, projection);
    iter_swap_if(first + 19u, first + 23u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 17u, compare, projection);
    iter_swap_if(first + 1u, first + 25u, compare, projection);
    iter_swap_if(first + 5u, first + 13u, compare, projection);
    iter_swap_if(first + 1u, first + 3u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 13u, compare, projection);
    iter_swap_if(first + 17u, first + 23u, compare, projection);
    iter_swap_if(first + 15u, first + 25u, compare, projection);
    iter_swap_if(first + 11u, first + 19u, compare, projection);
    iter_swap_if(first + 1u, first + 5u, compare, projection);
    iter_swap_if(first + 7u, first + 15u, compare, projection);
    iter_swap_if(first + 21u, first + 23u, compare, projection);
    iter_swap_if(first + 3u, first + 9u, compare, projection);
    iter_swap_if(first + 13u, first + 25u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 23u, first + 25u, compare, projection);
    iter_swap_if(first + 9u, first + 19u, compare, projection);
    iter_swap_if(first + 13u, first + 21u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 21u, first + 23u, compare, projection);
    iter_swap_if(first + 3u, first + 15u, compare, projection);
    iter_swap_if(first + 5u, first + 13u, compare, projection);
    iter_swap_if(first + 19u, first + 23u, compare, projection);
    iter_swap_if(first + 3u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 21u, compare, projection);
    iter_swap_if(first + 1u, first + 11u, compare, projection);
    iter_swap_if(first + 5u, first + 11u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 19u, first + 21u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 9u, first + 13u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 18u, first + 19u, compare, projection);
    iter_swap_if(first + 20u, first + 21u, compare, projection);
    iter_swap_if(first + 22u, first + 23u, compare, projection);
    iter_swap_if(first + 24u, first + 25u, compare, projection);
    iter_swap_if(first + 1u, first + 16u, compare, projection);
    iter_swap_if(first + 3u, first + 18u, compare, projection);
    iter_swap_if(first + 5u, first + 20u, compare, projection);
    iter_swap_if(first + 7u, first + 22u, compare, projection);
    iter_swap_if(first + 9u, first + 24u, compare, projection);
    iter_swap_if(first + 1u, first + 8u, compare, projection);
    iter_swap_if(first + 3u, first + 10u, compare, projection);
    iter_swap_if(first + 5u, first + 12u, compare, projection);
    iter_swap_if(first + 7u, first + 14u, compare, projection);
    iter_swap_if(first + 9u, first + 16u, compare, projection);
    iter_swap_if(first + 11u, first + 18u, compare, projection);
    iter_swap_if(first + 13u, first + 20u, compare, projection);
    iter_swap_if(first + 15u, first + 22u, compare, projection);
    iter_swap_if(first + 17u, first + 24u, compare, projection);
    iter_swap_if(first + 1u, first + 4u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 18u, compare, projection);
    iter_swap_if(first + 17u, first + 20u, compare, projection);
    iter_swap_if(first + 19u, first + 22u, compare, projection);
    iter_swap_if(first + 21u, first + 24u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 23u, first + 24u, compare, projection);
};


template<>
void static_sort1<27>(int* first){
	int tmp;
    iter_swap_if(first + 0u, first + 2u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 20u, first + 22u, compare, projection);
    iter_swap_if(first + 24u, first + 26u, compare, projection);
    iter_swap_if(first + 0u, first + 4u, compare, projection);
    iter_swap_if(first + 8u, first + 12u, compare, projection);
    iter_swap_if(first + 16u, first + 20u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 10u, first + 14u, compare, projection);
    iter_swap_if(first + 18u, first + 22u, compare, projection);
    iter_swap_if(first + 0u, first + 8u, compare, projection);
    iter_swap_if(first + 16u, first + 24u, compare, projection);
    iter_swap_if(first + 2u, first + 10u, compare, projection);
    iter_swap_if(first + 18u, first + 26u, compare, projection);
    iter_swap_if(first + 4u, first + 12u, compare, projection);
    iter_swap_if(first + 6u, first + 14u, compare, projection);
    iter_swap_if(first + 0u, first + 16u, compare, projection);
    iter_swap_if(first + 2u, first + 18u, compare, projection);
    iter_swap_if(first + 4u, first + 20u, compare, projection);
    iter_swap_if(first + 6u, first + 22u, compare, projection);
    iter_swap_if(first + 8u, first + 24u, compare, projection);
    iter_swap_if(first + 10u, first + 26u, compare, projection);
    iter_swap_if(first + 10u, first + 20u, compare, projection);
    iter_swap_if(first + 12u, first + 18u, compare, projection);
    iter_swap_if(first + 6u, first + 24u, compare, projection);
    iter_swap_if(first + 14u, first + 22u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 8u, first + 16u, compare, projection);
    iter_swap_if(first + 2u, first + 8u, compare, projection);
    iter_swap_if(first + 14u, first + 26u, compare, projection);
    iter_swap_if(first + 4u, first + 16u, compare, projection);
    iter_swap_if(first + 4u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 22u, first + 26u, compare, projection);
    iter_swap_if(first + 6u, first + 16u, compare, projection);
    iter_swap_if(first + 14u, first + 24u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 20u, first + 24u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 14u, first + 18u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 22u, first + 24u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 3u, first + 15u, compare, projection);
    iter_swap_if(first + 19u, first + 23u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 17u, compare, projection);
    iter_swap_if(first + 1u, first + 25u, compare, projection);
    iter_swap_if(first + 5u, first + 13u, compare, projection);
    iter_swap_if(first + 1u, first + 3u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 13u, compare, projection);
    iter_swap_if(first + 17u, first + 23u, compare, projection);
    iter_swap_if(first + 15u, first + 25u, compare, projection);
    iter_swap_if(first + 11u, first + 19u, compare, projection);
    iter_swap_if(first + 1u, first + 5u, compare, projection);
    iter_swap_if(first + 7u, first + 15u, compare, projection);
    iter_swap_if(first + 21u, first + 23u, compare, projection);
    iter_swap_if(first + 3u, first + 9u, compare, projection);
    iter_swap_if(first + 13u, first + 25u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 23u, first + 25u, compare, projection);
    iter_swap_if(first + 9u, first + 19u, compare, projection);
    iter_swap_if(first + 13u, first + 21u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 21u, first + 23u, compare, projection);
    iter_swap_if(first + 3u, first + 15u, compare, projection);
    iter_swap_if(first + 5u, first + 13u, compare, projection);
    iter_swap_if(first + 19u, first + 23u, compare, projection);
    iter_swap_if(first + 3u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 21u, compare, projection);
    iter_swap_if(first + 1u, first + 11u, compare, projection);
    iter_swap_if(first + 5u, first + 11u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 19u, first + 21u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 9u, first + 13u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 18u, first + 19u, compare, projection);
    iter_swap_if(first + 20u, first + 21u, compare, projection);
    iter_swap_if(first + 22u, first + 23u, compare, projection);
    iter_swap_if(first + 24u, first + 25u, compare, projection);
    iter_swap_if(first + 1u, first + 16u, compare, projection);
    iter_swap_if(first + 3u, first + 18u, compare, projection);
    iter_swap_if(first + 5u, first + 20u, compare, projection);
    iter_swap_if(first + 7u, first + 22u, compare, projection);
    iter_swap_if(first + 9u, first + 24u, compare, projection);
    iter_swap_if(first + 11u, first + 26u, compare, projection);
    iter_swap_if(first + 1u, first + 8u, compare, projection);
    iter_swap_if(first + 3u, first + 10u, compare, projection);
    iter_swap_if(first + 5u, first + 12u, compare, projection);
    iter_swap_if(first + 7u, first + 14u, compare, projection);
    iter_swap_if(first + 9u, first + 16u, compare, projection);
    iter_swap_if(first + 11u, first + 18u, compare, projection);
    iter_swap_if(first + 13u, first + 20u, compare, projection);
    iter_swap_if(first + 15u, first + 22u, compare, projection);
    iter_swap_if(first + 17u, first + 24u, compare, projection);
    iter_swap_if(first + 19u, first + 26u, compare, projection);
    iter_swap_if(first + 1u, first + 4u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 18u, compare, projection);
    iter_swap_if(first + 17u, first + 20u, compare, projection);
    iter_swap_if(first + 19u, first + 22u, compare, projection);
    iter_swap_if(first + 21u, first + 24u, compare, projection);
    iter_swap_if(first + 23u, first + 26u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 23u, first + 24u, compare, projection);
    iter_swap_if(first + 25u, first + 26u, compare, projection);
};

template<>
void static_sort1<28>(int* first){
	int tmp;
    iter_swap_if(first + 0u, first + 9u, compare, projection);
    iter_swap_if(first + 1u, first + 20u, compare, projection);
    iter_swap_if(first + 2u, first + 21u, compare, projection);
    iter_swap_if(first + 3u, first + 22u, compare, projection);
    iter_swap_if(first + 4u, first + 19u, compare, projection);
    iter_swap_if(first + 5u, first + 24u, compare, projection);
    iter_swap_if(first + 6u, first + 25u, compare, projection);
    iter_swap_if(first + 7u, first + 26u, compare, projection);
    iter_swap_if(first + 8u, first + 23u, compare, projection);
    iter_swap_if(first + 10u, first + 15u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 12u, first + 17u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 18u, first + 27u, compare, projection);
    iter_swap_if(first + 0u, first + 18u, compare, projection);
    iter_swap_if(first + 1u, first + 7u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 4u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 27u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 19u, first + 23u, compare, projection);
    iter_swap_if(first + 20u, first + 26u, compare, projection);
    iter_swap_if(first + 21u, first + 25u, compare, projection);
    iter_swap_if(first + 22u, first + 24u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 19u, compare, projection);
    iter_swap_if(first + 6u, first + 20u, compare, projection);
    iter_swap_if(first + 7u, first + 21u, compare, projection);
    iter_swap_if(first + 8u, first + 22u, compare, projection);
    iter_swap_if(first + 9u, first + 18u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 23u, first + 24u, compare, projection);
    iter_swap_if(first + 25u, first + 26u, compare, projection);
    iter_swap_if(first + 0u, first + 3u, compare, projection);
    iter_swap_if(first + 1u, first + 10u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 26u, compare, projection);
    iter_swap_if(first + 19u, first + 22u, compare, projection);
    iter_swap_if(first + 20u, first + 21u, compare, projection);
    iter_swap_if(first + 24u, first + 27u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 2u, first + 7u, compare, projection);
    iter_swap_if(first + 3u, first + 10u, compare, projection);
    iter_swap_if(first + 4u, first + 8u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 24u, compare, projection);
    iter_swap_if(first + 19u, first + 23u, compare, projection);
    iter_swap_if(first + 20u, first + 25u, compare, projection);
    iter_swap_if(first + 26u, first + 27u, compare, projection);
    iter_swap_if(first + 1u, first + 3u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 7u, first + 19u, compare, projection);
    iter_swap_if(first + 8u, first + 20u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 21u, first + 25u, compare, projection);
    iter_swap_if(first + 22u, first + 23u, compare, projection);
    iter_swap_if(first + 24u, first + 26u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 12u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 10u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 15u, first + 22u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 23u, first + 25u, compare, projection);
    iter_swap_if(first + 2u, first + 9u, compare, projection);
    iter_swap_if(first + 4u, first + 11u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 13u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 14u, first + 20u, compare, projection);
    iter_swap_if(first + 16u, first + 23u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 18u, first + 25u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 16u, compare, projection);
    iter_swap_if(first + 4u, first + 9u, compare, projection);
    iter_swap_if(first + 6u, first + 12u, compare, projection);
    iter_swap_if(first + 10u, first + 14u, compare, projection);
    iter_swap_if(first + 11u, first + 24u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 15u, first + 21u, compare, projection);
    iter_swap_if(first + 18u, first + 23u, compare, projection);
    iter_swap_if(first + 25u, first + 26u, compare, projection);
    iter_swap_if(first + 2u, first + 8u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 4u, first + 7u, compare, projection);
    iter_swap_if(first + 6u, first + 16u, compare, projection);
    iter_swap_if(first + 9u, first + 15u, compare, projection);
    iter_swap_if(first + 11u, first + 21u, compare, projection);
    iter_swap_if(first + 12u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 25u, compare, projection);
    iter_swap_if(first + 20u, first + 23u, compare, projection);
    iter_swap_if(first + 22u, first + 24u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 15u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 19u, first + 22u, compare, projection);
    iter_swap_if(first + 24u, first + 25u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 19u, first + 21u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 18u, first + 19u, compare, projection);
    iter_swap_if(first + 20u, first + 21u, compare, projection);
    iter_swap_if(first + 22u, first + 23u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 23u, first + 24u, compare, projection);
};

template<>
void static_sort1<29>(int* first){
	int tmp;
    iter_swap_if(first + 0u, first + 16u, compare, projection);
    iter_swap_if(first + 8u, first + 24u, compare, projection);
    iter_swap_if(first + 8u, first + 16u, compare, projection);
    iter_swap_if(first + 4u, first + 20u, compare, projection);
    iter_swap_if(first + 12u, first + 28u, compare, projection);
    iter_swap_if(first + 12u, first + 20u, compare, projection);
    iter_swap_if(first + 4u, first + 8u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 20u, first + 24u, compare, projection);
    iter_swap_if(first + 2u, first + 18u, compare, projection);
    iter_swap_if(first + 10u, first + 26u, compare, projection);
    iter_swap_if(first + 10u, first + 18u, compare, projection);
    iter_swap_if(first + 6u, first + 22u, compare, projection);
    iter_swap_if(first + 14u, first + 22u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 14u, first + 18u, compare, projection);
    iter_swap_if(first + 22u, first + 26u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 22u, first + 24u, compare, projection);
    iter_swap_if(first + 26u, first + 28u, compare, projection);
    iter_swap_if(first + 1u, first + 17u, compare, projection);
    iter_swap_if(first + 9u, first + 25u, compare, projection);
    iter_swap_if(first + 9u, first + 17u, compare, projection);
    iter_swap_if(first + 5u, first + 21u, compare, projection);
    iter_swap_if(first + 13u, first + 21u, compare, projection);
    iter_swap_if(first + 5u, first + 9u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 21u, first + 25u, compare, projection);
    iter_swap_if(first + 3u, first + 19u, compare, projection);
    iter_swap_if(first + 11u, first + 27u, compare, projection);
    iter_swap_if(first + 11u, first + 19u, compare, projection);
    iter_swap_if(first + 7u, first + 23u, compare, projection);
    iter_swap_if(first + 15u, first + 23u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 15u, first + 19u, compare, projection);
    iter_swap_if(first + 23u, first + 27u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 19u, first + 21u, compare, projection);
    iter_swap_if(first + 23u, first + 25u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 23u, first + 24u, compare, projection);
    iter_swap_if(first + 25u, first + 26u, compare, projection);
    iter_swap_if(first + 27u, first + 28u, compare, projection);
};

template<>
void static_sort1<30>(int* first){
	int tmp;
    iter_swap_if(first + 0u, first + 2u, compare, projection);
    iter_swap_if(first + 4u, first + 6u, compare, projection);
    iter_swap_if(first + 8u, first + 10u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 20u, first + 22u, compare, projection);
    iter_swap_if(first + 24u, first + 26u, compare, projection);
    iter_swap_if(first + 0u, first + 4u, compare, projection);
    iter_swap_if(first + 8u, first + 12u, compare, projection);
    iter_swap_if(first + 16u, first + 20u, compare, projection);
    iter_swap_if(first + 24u, first + 28u, compare, projection);
    iter_swap_if(first + 2u, first + 6u, compare, projection);
    iter_swap_if(first + 10u, first + 14u, compare, projection);
    iter_swap_if(first + 18u, first + 22u, compare, projection);
    iter_swap_if(first + 0u, first + 8u, compare, projection);
    iter_swap_if(first + 16u, first + 24u, compare, projection);
    iter_swap_if(first + 2u, first + 10u, compare, projection);
    iter_swap_if(first + 18u, first + 26u, compare, projection);
    iter_swap_if(first + 4u, first + 12u, compare, projection);
    iter_swap_if(first + 20u, first + 28u, compare, projection);
    iter_swap_if(first + 6u, first + 14u, compare, projection);
    iter_swap_if(first + 0u, first + 16u, compare, projection);
    iter_swap_if(first + 2u, first + 18u, compare, projection);
    iter_swap_if(first + 4u, first + 20u, compare, projection);
    iter_swap_if(first + 6u, first + 22u, compare, projection);
    iter_swap_if(first + 8u, first + 24u, compare, projection);
    iter_swap_if(first + 10u, first + 26u, compare, projection);
    iter_swap_if(first + 12u, first + 28u, compare, projection);
    iter_swap_if(first + 10u, first + 20u, compare, projection);
    iter_swap_if(first + 12u, first + 18u, compare, projection);
    iter_swap_if(first + 6u, first + 24u, compare, projection);
    iter_swap_if(first + 26u, first + 28u, compare, projection);
    iter_swap_if(first + 14u, first + 22u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 8u, first + 16u, compare, projection);
    iter_swap_if(first + 2u, first + 8u, compare, projection);
    iter_swap_if(first + 14u, first + 26u, compare, projection);
    iter_swap_if(first + 4u, first + 16u, compare, projection);
    iter_swap_if(first + 22u, first + 28u, compare, projection);
    iter_swap_if(first + 4u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 22u, first + 26u, compare, projection);
    iter_swap_if(first + 6u, first + 16u, compare, projection);
    iter_swap_if(first + 14u, first + 24u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 20u, first + 24u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 14u, first + 18u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 22u, first + 24u, compare, projection);
    iter_swap_if(first + 12u, first + 14u, compare, projection);
    iter_swap_if(first + 16u, first + 18u, compare, projection);
    iter_swap_if(first + 1u, first + 3u, compare, projection);
    iter_swap_if(first + 5u, first + 7u, compare, projection);
    iter_swap_if(first + 9u, first + 11u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 21u, first + 23u, compare, projection);
    iter_swap_if(first + 25u, first + 27u, compare, projection);
    iter_swap_if(first + 1u, first + 5u, compare, projection);
    iter_swap_if(first + 9u, first + 13u, compare, projection);
    iter_swap_if(first + 17u, first + 21u, compare, projection);
    iter_swap_if(first + 25u, first + 29u, compare, projection);
    iter_swap_if(first + 3u, first + 7u, compare, projection);
    iter_swap_if(first + 11u, first + 15u, compare, projection);
    iter_swap_if(first + 19u, first + 23u, compare, projection);
    iter_swap_if(first + 1u, first + 9u, compare, projection);
    iter_swap_if(first + 17u, first + 25u, compare, projection);
    iter_swap_if(first + 3u, first + 11u, compare, projection);
    iter_swap_if(first + 19u, first + 27u, compare, projection);
    iter_swap_if(first + 5u, first + 13u, compare, projection);
    iter_swap_if(first + 21u, first + 29u, compare, projection);
    iter_swap_if(first + 7u, first + 15u, compare, projection);
    iter_swap_if(first + 1u, first + 17u, compare, projection);
    iter_swap_if(first + 3u, first + 19u, compare, projection);
    iter_swap_if(first + 5u, first + 21u, compare, projection);
    iter_swap_if(first + 7u, first + 23u, compare, projection);
    iter_swap_if(first + 9u, first + 25u, compare, projection);
    iter_swap_if(first + 11u, first + 27u, compare, projection);
    iter_swap_if(first + 13u, first + 29u, compare, projection);
    iter_swap_if(first + 11u, first + 21u, compare, projection);
    iter_swap_if(first + 13u, first + 19u, compare, projection);
    iter_swap_if(first + 7u, first + 25u, compare, projection);
    iter_swap_if(first + 27u, first + 29u, compare, projection);
    iter_swap_if(first + 15u, first + 23u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 9u, first + 17u, compare, projection);
    iter_swap_if(first + 3u, first + 9u, compare, projection);
    iter_swap_if(first + 15u, first + 27u, compare, projection);
    iter_swap_if(first + 5u, first + 17u, compare, projection);
    iter_swap_if(first + 23u, first + 29u, compare, projection);
    iter_swap_if(first + 5u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 19u, first + 21u, compare, projection);
    iter_swap_if(first + 23u, first + 27u, compare, projection);
    iter_swap_if(first + 7u, first + 17u, compare, projection);
    iter_swap_if(first + 15u, first + 25u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 21u, first + 25u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 15u, first + 19u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 19u, first + 21u, compare, projection);
    iter_swap_if(first + 23u, first + 25u, compare, projection);
    iter_swap_if(first + 13u, first + 15u, compare, projection);
    iter_swap_if(first + 17u, first + 19u, compare, projection);
    iter_swap_if(first + 0u, first + 1u, compare, projection);
    iter_swap_if(first + 2u, first + 3u, compare, projection);
    iter_swap_if(first + 4u, first + 5u, compare, projection);
    iter_swap_if(first + 6u, first + 7u, compare, projection);
    iter_swap_if(first + 8u, first + 9u, compare, projection);
    iter_swap_if(first + 10u, first + 11u, compare, projection);
    iter_swap_if(first + 12u, first + 13u, compare, projection);
    iter_swap_if(first + 14u, first + 15u, compare, projection);
    iter_swap_if(first + 16u, first + 17u, compare, projection);
    iter_swap_if(first + 18u, first + 19u, compare, projection);
    iter_swap_if(first + 20u, first + 21u, compare, projection);
    iter_swap_if(first + 22u, first + 23u, compare, projection);
    iter_swap_if(first + 24u, first + 25u, compare, projection);
    iter_swap_if(first + 26u, first + 27u, compare, projection);
    iter_swap_if(first + 28u, first + 29u, compare, projection);
    iter_swap_if(first + 1u, first + 16u, compare, projection);
    iter_swap_if(first + 3u, first + 18u, compare, projection);
    iter_swap_if(first + 5u, first + 20u, compare, projection);
    iter_swap_if(first + 7u, first + 22u, compare, projection);
    iter_swap_if(first + 9u, first + 24u, compare, projection);
    iter_swap_if(first + 11u, first + 26u, compare, projection);
    iter_swap_if(first + 13u, first + 28u, compare, projection);
    iter_swap_if(first + 1u, first + 8u, compare, projection);
    iter_swap_if(first + 3u, first + 10u, compare, projection);
    iter_swap_if(first + 5u, first + 12u, compare, projection);
    iter_swap_if(first + 7u, first + 14u, compare, projection);
    iter_swap_if(first + 9u, first + 16u, compare, projection);
    iter_swap_if(first + 11u, first + 18u, compare, projection);
    iter_swap_if(first + 13u, first + 20u, compare, projection);
    iter_swap_if(first + 15u, first + 22u, compare, projection);
    iter_swap_if(first + 17u, first + 24u, compare, projection);
    iter_swap_if(first + 19u, first + 26u, compare, projection);
    iter_swap_if(first + 21u, first + 28u, compare, projection);
    iter_swap_if(first + 1u, first + 4u, compare, projection);
    iter_swap_if(first + 3u, first + 6u, compare, projection);
    iter_swap_if(first + 5u, first + 8u, compare, projection);
    iter_swap_if(first + 7u, first + 10u, compare, projection);
    iter_swap_if(first + 9u, first + 12u, compare, projection);
    iter_swap_if(first + 11u, first + 14u, compare, projection);
    iter_swap_if(first + 13u, first + 16u, compare, projection);
    iter_swap_if(first + 15u, first + 18u, compare, projection);
    iter_swap_if(first + 17u, first + 20u, compare, projection);
    iter_swap_if(first + 19u, first + 22u, compare, projection);
    iter_swap_if(first + 21u, first + 24u, compare, projection);
    iter_swap_if(first + 23u, first + 26u, compare, projection);
    iter_swap_if(first + 25u, first + 28u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 23u, first + 24u, compare, projection);
    iter_swap_if(first + 25u, first + 26u, compare, projection);
    iter_swap_if(first + 27u, first + 28u, compare, projection);
};

template<>
void static_sort1<31>(int* first){
	int tmp;
    static_sort1<16>(first);
    static_sort1<15>(first+16);

    iter_swap_if(first + 0u, first + 16u, compare, projection);
    iter_swap_if(first + 8u, first + 24u, compare, projection);
    iter_swap_if(first + 8u, first + 16u, compare, projection);
    iter_swap_if(first + 4u, first + 20u, compare, projection);
    iter_swap_if(first + 12u, first + 28u, compare, projection);
    iter_swap_if(first + 12u, first + 20u, compare, projection);
    iter_swap_if(first + 4u, first + 8u, compare, projection);
    iter_swap_if(first + 12u, first + 16u, compare, projection);
    iter_swap_if(first + 20u, first + 24u, compare, projection);
    iter_swap_if(first + 2u, first + 18u, compare, projection);
    iter_swap_if(first + 10u, first + 26u, compare, projection);
    iter_swap_if(first + 10u, first + 18u, compare, projection);
    iter_swap_if(first + 6u, first + 22u, compare, projection);
    iter_swap_if(first + 14u, first + 30u, compare, projection);
    iter_swap_if(first + 14u, first + 22u, compare, projection);
    iter_swap_if(first + 6u, first + 10u, compare, projection);
    iter_swap_if(first + 14u, first + 18u, compare, projection);
    iter_swap_if(first + 22u, first + 26u, compare, projection);
    iter_swap_if(first + 2u, first + 4u, compare, projection);
    iter_swap_if(first + 6u, first + 8u, compare, projection);
    iter_swap_if(first + 10u, first + 12u, compare, projection);
    iter_swap_if(first + 14u, first + 16u, compare, projection);
    iter_swap_if(first + 18u, first + 20u, compare, projection);
    iter_swap_if(first + 22u, first + 24u, compare, projection);
    iter_swap_if(first + 26u, first + 28u, compare, projection);
    iter_swap_if(first + 1u, first + 17u, compare, projection);
    iter_swap_if(first + 9u, first + 25u, compare, projection);
    iter_swap_if(first + 9u, first + 17u, compare, projection);
    iter_swap_if(first + 5u, first + 21u, compare, projection);
    iter_swap_if(first + 13u, first + 29u, compare, projection);
    iter_swap_if(first + 13u, first + 21u, compare, projection);
    iter_swap_if(first + 5u, first + 9u, compare, projection);
    iter_swap_if(first + 13u, first + 17u, compare, projection);
    iter_swap_if(first + 21u, first + 25u, compare, projection);
    iter_swap_if(first + 3u, first + 19u, compare, projection);
    iter_swap_if(first + 11u, first + 27u, compare, projection);
    iter_swap_if(first + 11u, first + 19u, compare, projection);
    iter_swap_if(first + 7u, first + 23u, compare, projection);
    iter_swap_if(first + 15u, first + 23u, compare, projection);
    iter_swap_if(first + 7u, first + 11u, compare, projection);
    iter_swap_if(first + 15u, first + 19u, compare, projection);
    iter_swap_if(first + 23u, first + 27u, compare, projection);
    iter_swap_if(first + 3u, first + 5u, compare, projection);
    iter_swap_if(first + 7u, first + 9u, compare, projection);
    iter_swap_if(first + 11u, first + 13u, compare, projection);
    iter_swap_if(first + 15u, first + 17u, compare, projection);
    iter_swap_if(first + 19u, first + 21u, compare, projection);
    iter_swap_if(first + 23u, first + 25u, compare, projection);
    iter_swap_if(first + 27u, first + 29u, compare, projection);
    iter_swap_if(first + 1u, first + 2u, compare, projection);
    iter_swap_if(first + 3u, first + 4u, compare, projection);
    iter_swap_if(first + 5u, first + 6u, compare, projection);
    iter_swap_if(first + 7u, first + 8u, compare, projection);
    iter_swap_if(first + 9u, first + 10u, compare, projection);
    iter_swap_if(first + 11u, first + 12u, compare, projection);
    iter_swap_if(first + 13u, first + 14u, compare, projection);
    iter_swap_if(first + 15u, first + 16u, compare, projection);
    iter_swap_if(first + 17u, first + 18u, compare, projection);
    iter_swap_if(first + 19u, first + 20u, compare, projection);
    iter_swap_if(first + 21u, first + 22u, compare, projection);
    iter_swap_if(first + 23u, first + 24u, compare, projection);
    iter_swap_if(first + 25u, first + 26u, compare, projection);
    iter_swap_if(first + 27u, first + 28u, compare, projection);
    iter_swap_if(first + 29u, first + 30u, compare, projection);
};

template<>
void static_sort1<32>(int* first){
	int tmp;
    static_sort1<16>(first);
    static_sort1<16>(first+16);

    cond_swap(first + 0u, first + 16u);
    cond_swap(first + 8u, first + 24u);
    cond_swap(first + 8u, first + 16u);
    cond_swap(first + 4u, first + 20u);
    cond_swap(first + 12u, first + 28u);
    cond_swap(first + 12u, first + 20u);
    cond_swap(first + 4u, first + 8u);
    cond_swap(first + 12u, first + 16u);
    cond_swap(first + 20u, first + 24u);
    cond_swap(first + 2u, first + 18u);
    cond_swap(first + 10u, first + 26u);
    cond_swap(first + 10u, first + 18u);
    cond_swap(first + 6u, first + 22u);
    cond_swap(first + 14u, first + 30u);
    cond_swap(first + 14u, first + 22u);
    cond_swap(first + 6u, first + 10u);
    cond_swap(first + 14u, first + 18u);
    cond_swap(first + 22u, first + 26u);
    cond_swap(first + 2u, first + 4u);
    cond_swap(first + 6u, first + 8u);
    cond_swap(first + 10u, first + 12u);
    cond_swap(first + 14u, first + 16u);
    cond_swap(first + 18u, first + 20u);
    cond_swap(first + 22u, first + 24u);
    cond_swap(first + 26u, first + 28u);
    cond_swap(first + 1u, first + 17u);
    cond_swap(first + 9u, first + 25u);
    cond_swap(first + 9u, first + 17u);
    cond_swap(first + 5u, first + 21u);
    cond_swap(first + 13u, first + 29u);
    cond_swap(first + 13u, first + 21u);
    cond_swap(first + 5u, first + 9u);
    cond_swap(first + 13u, first + 17u);
    cond_swap(first + 21u, first + 25u);
    cond_swap(first + 3u, first + 19u);
    cond_swap(first + 11u, first + 27u);
    cond_swap(first + 11u, first + 19u);
    cond_swap(first + 7u, first + 23u);
    cond_swap(first + 15u, first + 31u);
    cond_swap(first + 15u, first + 23u);
    cond_swap(first + 7u, first + 11u);
    cond_swap(first + 15u, first + 19u);
    cond_swap(first + 23u, first + 27u);
    cond_swap(first + 3u, first + 5u);
    cond_swap(first + 7u, first + 9u);
    cond_swap(first + 11u, first + 13u);
    cond_swap(first + 15u, first + 17u);
    cond_swap(first + 19u, first + 21u);
    cond_swap(first + 23u, first + 25u);
    cond_swap(first + 27u, first + 29u);
    cond_swap(first + 1u, first + 2u);
    cond_swap(first + 3u, first + 4u);
    cond_swap(first + 5u, first + 6u);
    cond_swap(first + 7u, first + 8u);
    cond_swap(first + 9u, first + 10u);
    cond_swap(first + 11u, first + 12u);
    cond_swap(first + 13u, first + 14u);
    cond_swap(first + 15u, first + 16u);
    cond_swap(first + 17u, first + 18u);
    cond_swap(first + 19u, first + 20u);
    cond_swap(first + 21u, first + 22u);
    cond_swap(first + 23u, first + 24u);
    cond_swap(first + 25u, first + 26u);
    cond_swap(first + 27u, first + 28u);
    cond_swap(first + 29u, first + 30u);

	/*
	__asm__(
			"pushq   %%rbp                    \n\t"
			"pushq   %%r15                    \n\t"
			"pushq   %%r14                    \n\t"
			"pushq   %%r13                    \n\t"
			"pushq   %%r12                    \n\t"
			"pushq   %%rbx                    \n\t"
			"movl    64(%[rdi]), %%ecx        \n\t"
			"movl    (%[rdi]), %%edx          \n\t"
			"movl    8(%[rdi]), %%r12d        \n\t"
			"cmpl    %%ecx, %edx              \n\t"
			"movl    %%ecx, %%esi              \n\t"
			"cmovlel %edx, %%esi              \n\t"
			"movl    %%esi, (%[rdi])          \n\t"
			"cmovll  %%ecx, %%edx              \n\t"
			"movl    32(%[rdi]), %%ebx        \n\t"
			"movl    96(%[rdi]), %%ecx        \n\t"
			"cmpl    %%ecx, %%ebx              \n\t"
			"movl    %%ecx, %%ebp              \n\t"
			"cmovlel %%ebx, %%ebp              \n\t"
			"cmovll  %%ecx, %%ebx              \n\t"
			"cmpl    %%edx, %%ebp              \n\t"
			"movl    %%edx, %%esi              \n\t"
			"cmovlel %%ebp, %%esi              \n\t"
			"cmovll  %edx, %%ebp              \n\t"
			"movl    16(%[rdi]), %%edx        \n\t"
			"movl    80(%[rdi]), %%ecx        \n\t"
			"cmpl    %%ecx, %edx              \n\t"
			"movl    %%ecx, %%r15d             \n\t"
			"cmovlel %%edx, %%r15d             \n\t"
			"cmovll  %%ecx, %%edx              \n\t"
			"movl    48(%[rdi]), %%eax        \n\t"
			"movl    112(%[rdi]), %%ecx       \n\t"
			"cmpl    %%ecx, %%eax              \n\t"
			"movl    %%ecx, %%r13d             \n\t"
			"cmovlel %%eax, %%r13d             \n\t"
			"cmovll  %%ecx, %%eax              \n\t"
			"movl    %eax, -64(%%rsp)         \n\t"
			"cmpl    %%edx, %%r13d             \n\t"
			"movl    %%edx, %%r14d             \n\t"
			"cmovlel %r13d, %%r14d            \n\t"
			"cmovll  %%edx, %%r13d             \n\t"
			"cmpl    %%esi, %%r15d             \n\t"
			"movl    %%esi, %%r11d             \n\t"
			"cmovlel %%r15d, %%r11d            \n\t"
			"cmovll  %%esi, %%r15d             \n\t"
			"cmpl    %%ebp, %%r14d             \n\t"
			"movl    %%ebp, %%r10d             \n\t"
			"cmovlel %r14d, %%r10d            \n\t"
			"cmovll  %%ebp, %%r14d             \n\t"
			"cmpl    %%ebx, %%r13d             \n\t"
			"movl    %%ebx, %%eax              \n\t"
			"cmovlel %%r13d, %%eax             \n\t"
			"movl    %%eax, -68(%%rsp)         \n\t"
			"cmovll  %%ebx, %%r13d             \n\t"
			"movl    72(%[rdi]), %%ecx        \n\t"
			"cmpl    %%ecx, %%r12d             \n\t"
			"movl    %%ecx, %%r9d              \n\t"
			"cmovlel %%r12d, %%r9d             \n\t"
			"cmovll  %%ecx, %%r12d             \n\t"
			"movl    40(%[rdi]), %%ebx        \n\t"
			"movl    104(%[rdi]), %%ecx       \n\t"
			"cmpl    %%ecx, %%ebx              \n\t"
			"movl    %%ecx, %%ebp              \n\t"
			"cmovlel %%ebx, %%ebp              \n\t"
			"cmovll  %%ecx, %%ebx              \n\t"
			"cmpl    %%r12d, %%ebp             \n\t"
			"movl    %%r12d, %%ecx             \n\t"
			"cmovlel %%ebp, %%ecx              \n\t"
			"cmovll  %%r12d, %%ebp             \n\t"
			"movl    24(%[rdi]), %%edx        \n\t"
			"movl    88(%[rdi]), %%esi        \n\t"
			"cmpl    %%esi, %edx              \n\t"
			"movl    %%esi, %%r8d              \n\t"
			"cmovlel %%edx, %%r8d              \n\t"
			"cmovll  %%esi, %%edx              \n\t"
			"movl    56(%[rdi]), %%eax        \n\t"
			"movl    120(%[rdi]), %%r12d      \n\t"
			"cmpl    %%r12d, %%eax             \n\t"
			"movl    %%r12d, %%esi             \n\t"
			"cmovlel %%eax, %%esi              \n\t"
			"cmovll  %%r12d, %eax             \n\t"
			"movl    %%eax, -48(%%rsp)         \n\t"
			"cmpl    %%edx, %%esi              \n\t"
			"movl    %%edx, %%r12d             \n\t"
			"cmovlel %%esi, %%r12d             \n\t"
			"cmovll  %%edx, %%esi              \n\t"
			"cmpl    %%ecx, %%r8d              \n\t"
			"movl    %%ecx, %%eax              \n\t"
			"cmovlel %%r8d, %%eax              \n\t"
			"cmovll  %%ecx, %%r8d              \n\t"
			"cmpl    %%ebp, %%r12d             \n\t"
			"movl    %%ebp, %%edx              \n\t"
			"cmovlel %%r12d, %%edx             \n\t"
			"cmovll  %%ebp, %%r12d             \n\t"
			"cmpl    %%ebx, %%esi              \n\t"
			"movl    %%ebx, %%ecx              \n\t"
			"cmovlel %%esi, %%ecx              \n\t"
			"cmovll  %%ebx, %%esi              \n\t"
			"cmpl    %%r11d, %%r9d             \n\t"
			"movl    %%r11d, %%ebx             \n\t"
			"cmovlel %%r9d, %%ebx              \n\t"
			"movl    %%ebx, -60(%%rsp)         \n\t"
			"cmovll  %%r11d, %%r9d             \n\t"
			"movl    %%r9d, -56(%%rsp)         \n\t"
			"cmpl    %%r15d, %%eax             \n\t"
			"movl    %%r15d, %%ebp             \n\t"
			"cmovlel %%eax, %%ebp              \n\t"
			"movl    %%ebp, -52(%%rsp)         \n\t"
			"cmovll  %%r15d, %%eax             \n\t"
			"movl    %%eax, %%r15d             \n\t"
			"movl    %%r15d, -36(%%rsp)        \n\t"
			"cmpl    %%r10d, %%r8d             \n\t"
			"movl    %%r10d, %%eax             \n\t"
			"cmovlel %%r8d, %%eax              \n\t"
			"movl    %%eax, %%r11d             \n\t"
			"movl    %%r11d, -32(%%rsp)        \n\t"
			"cmovll  %%r10d, %%r8d             \n\t"
			"movl    %%r8d, %%r10d             \n\t"
			"movl    %%r10d, -28(%%rsp)        \n\t"
			"cmpl    %%r14d, %%edx             \n\t"
			"movl    %%r14d, %%eax             \n\t"
			"cmovlel %%edx, %%eax              \n\t"
			"movl    %%eax, %%r9d              \n\t"
			"movl    %%r9d, -24(%%rsp)         \n\t"
			"cmovll  %%r14d, %%edx             \n\t"
			"movl    %%edx, %%r8d              \n\t"
			"movl    %%r8d, -20(%%rsp)         \n\t"
			"movl    -68(%%rsp), %%edx         \n\t"
			"cmpl    %%edx, %%r12d             \n\t"
			"movl    %%edx, %%eax              \n\t"
			"cmovlel %%r12d, %%eax             \n\t"
			"movl    %%eax, %%ebx              \n\t"
			"movl    %%ebx, -16(%%rsp)         \n\t"
			"cmovll  %%edx, %%r12d             \n\t"
			"movl    %%r12d, -12(%%rsp)        \n\t"
			"cmpl    %%r13d, %%ecx             \n\t"
			"movl    %%r13d, %%eax             \n\t"
			"cmovlel %%ecx, %%eax              \n\t"
			"movl    %%eax, %%ebp              \n\t"
			"movl    %%ebp, -8(%%rsp)          \n\t"
			"cmovll  %%r13d, %%ecx             \n\t"
			"movl    %%ecx, %%edx              \n\t"
			"movl    %%edx, -4(%%rsp)          \n\t"
			"movl    -64(%%rsp), %%eax         \n\t"
			"cmpl    %%eax, %%esi              \n\t"
			"movl    %%eax, %%ecx              \n\t"
			"cmovlel %%esi, %%ecx              \n\t"
			"movl    %%ecx, -68(%%rsp)         \n\t"
			"cmovll  %%eax, %%esi              \n\t"
			"movl    %%esi, -64(%%rsp)         \n\t"
			"movl    -48(%%rsp), %%eax         \n\t"
			"movl    %%eax, 120(%[rdi])       \n\t"
			"movl    -60(%%rsp), %%eax         \n\t"
			"movl    %%eax, 8(%[rdi])         \n\t"
			"movl    -56(%%rsp), %%eax         \n\t"
			"movl    %%eax, 16(%[rdi])        \n\t"
			"movl    -52(%%rsp), %%eax         \n\t"
			"movl    %%eax, 24(%[rdi])        \n\t"
			"movl    %%r15d, 32(%[rdi])       \n\t"
			"movl    %%r11d, 40(%[rdi])       \n\t"
			"movl    %%r10d, 48(%[rdi])       \n\t"
			"movl    %%r9d, 56(%[rdi])        \n\t"
			"movl    %%r8d, 64(%[rdi])        \n\t"
			"movl    %%ebx, 72(%[rdi])        \n\t"
			"movl    %%r12d, 80(%[rdi])       \n\t"
			"movl    %%ebp, 88(%[rdi])        \n\t"
			"movl    %%edx, 96(%[rdi])        \n\t"
			"movl    %%ecx, 104(%[rdi])       \n\t"
			"movl    %%esi, 112(%[rdi])       \n\t"
			"movl    4(%[rdi]), %%edx         \n\t"
			"movl    68(%[rdi]), %%eax        \n\t"
			"cmpl    %%eax, %%edx              \n\t"
			"movl    %%eax, %%ecx              \n\t"
			"cmovlel %%edx, %%ecx              \n\t"
			"movl    %%ecx, -40(%%rsp)         \n\t"
			"cmovll  %%eax, %%edx              \n\t"
			"movl    %%ecx, 4(%[rdi])         \n\t"
			"movl    36(%[rdi]), %%r10d       \n\t"
			"movl    100(%[rdi]), %%esi       \n\t"
			"cmpl    %%esi, %%r10d             \n\t"
			"movl    %%esi, %%eax              \n\t"
			"cmovlel %%r10d, %%eax             \n\t"
			"cmovll  %%esi, %%r10d             \n\t"
			"cmpl    %%edx, %%eax              \n\t"
			"movl    %%edx, %%r8d              \n\t"
			"cmovlel %%eax, %%r8d              \n\t"
			"cmovll  %%edx, %eax              \n\t"
			"movl    20(%[rdi]), %%ebx        \n\t"
			"movl    84(%[rdi]), %edx        \n\t"
			"cmpl    %%edx, %%ebx              \n\t"
			"movl    %%edx, %%r9d              \n\t"
			"cmovlel %%ebx, %%r9d              \n\t"
			"cmovll  %%edx, %%ebx              \n\t"
			"movl    52(%[rdi]), %%esi        \n\t"
			"movl    116(%[rdi]), %%ebp       \n\t"
			"cmpl    %%ebp, %%esi              \n\t"
			"movl    %%ebp, %%edx              \n\t"
			"cmovlel %%esi, %%edx              \n\t"
			"cmovll  %%ebp, %%esi              \n\t"
			"movl    %%esi, -44(%%rsp)         \n\t"
			"cmpl    %%ebx, %%edx              \n\t"
			"movl    %%ebx, %%esi              \n\t"
			"cmovlel %%edx, %%esi              \n\t"
			"cmovll  %%ebx, %%edx              \n\t"
			"cmpl    %%r8d, %%r9d              \n\t"
			"movl    %%r8d, %%ebp              \n\t"
			"cmovlel %%r9d, %%ebp              \n\t"
			"cmovll  %%r8d, %r9d              \n\t"
			"movl    %%r9d, %%r15d             \n\t"
			"cmpl    %%eax, %%esi              \n\t"
			"movl    %%eax, %%ecx              \n\t"
			"cmovlel %%esi, %%ecx              \n\t"
			"movl    %%ecx, -80(%%rsp)         \n\t"
			"cmovll  %%eax, %%esi              \n\t"
			"movl    %%esi, -76(%%rsp)         \n\t"
			"cmpl    %%r10d, %%edx             \n\t"
			"movl    %%r10d, %%ecx             \n\t"
			"cmovlel %%edx, %%ecx              \n\t"
			"movl    %%ecx, -72(%%rsp)         \n\t"
			"cmovll  %%r10d, %%edx             \n\t"
			"movl    12(%[rdi]), %%eax        \n\t"
			"movl    76(%[rdi]), %%ecx        \n\t"
			"cmpl    %%ecx, %%eax              \n\t"
			"movl    %%ecx, %%r10d             \n\t"
			"cmovlel %%eax, %%r10d             \n\t"
			"cmovll  %%ecx, %%eax              \n\t"
			"movl    44(%[rdi]), %%r14d       \n\t"
			"movl    108(%[rdi]), %%ecx       \n\t"
			"cmpl    %%ecx, %%r14d             \n\t"
			"movl    %%ecx, %%r12d             \n\t"
			"cmovlel %%r14d, %%r12d            \n\t"
			"cmovll  %%ecx, %%r14d             \n\t"
			"cmpl    %%eax, %%r12d             \n\t"
			"movl    %%eax, %%r13d             \n\t"
			"cmovlel %%r12d, %%r13d            \n\t"
			"cmovll  %%eax, %%r12d             \n\t"
			"movl    28(%[rdi]), %%ecx        \n\t"
			"movl    92(%[rdi]), %%esi        \n\t"
			"cmpl    %%esi, %%ecx              \n\t"
			"movl    %%esi, %%r9d              \n\t"
			"cmovlel %%ecx, %%r9d              \n\t"
			"cmovll  %%esi, %%ecx              \n\t"
			"movl    60(%[rdi]), %%ebx        \n\t"
			"movl    124(%[rdi]), %%r8d       \n\t"
			"cmpl    %%r8d, %%ebx              \n\t"
			"movl    %%r8d, %%r11d             \n\t"
			"cmovlel %%ebx, %%r11d             \n\t"
			"cmovll  %%r8d, %%ebx              \n\t"
			"movl    %%ebx, 124(%[rdi])       \n\t"
			"cmpl    %%ecx, %%r11d             \n\t"
			"movl    %%ecx, %%r8d              \n\t"
			"cmovlel %%r11d, %%r8d             \n\t"
			"cmovll  %%ecx, %%r11d             \n\t"
			"cmpl    %%r13d, %%r9d             \n\t"
			"movl    %%r13d, %%esi             \n\t"
			"cmovlel %%r9d, %%esi              \n\t"
			"cmovll  %%r13d, %%r9d             \n\t"
			"cmpl    %%r12d, %%r8d             \n\t"
			"movl    %%r12d, %%ebx             \n\t"
			"cmovlel %%r8d, %%ebx              \n\t"
			"cmovll  %%r12d, %%r8d             \n\t"
			"cmpl    %%r14d, %%r11d            \n\t"
			"movl    %%r14d, %%r13d            \n\t"
			"cmovlel %%r11d, %%r13d            \n\t"
			"cmovll  %%r14d, %%r11d            \n\t"
			"cmpl    %%ebp, %%r10d             \n\t"
			"movl    %%ebp, %%r14d             \n\t"
			"cmovlel %%r10d, %%r14d            \n\t"
			"cmovll  %%ebp, %%r10d             \n\t"
			"movl    %%r15d, %%eax             \n\t"
			"cmpl    %%eax, %%esi              \n\t"
			"cmovlel %%esi, %%r15d             \n\t"
			"cmovll  %%eax, %%esi              \n\t"
			"movl    -80(%%rsp), %%eax         \n\t"
			"cmpl    %%eax, %%r9d              \n\t"
			"movl    %%eax, %%ebp              \n\t"
			"cmovlel %%r9d, %%ebp              \n\t"
			"cmovll  %%eax, %%r9d              \n\t"
			"movl    -76(%%rsp), %%eax         \n\t"
			"cmpl    %%eax, %%ebx              \n\t"
			"movl    %%eax, %%r12d             \n\t"
			"cmovlel %%ebx, %%r12d             \n\t"
			"cmovll  %%eax, %%ebx              \n\t"
			"movl    -72(%%rsp), %%eax         \n\t"
			"cmpl    %%eax, %%r8d              \n\t"
			"movl    %%eax, %%ecx              \n\t"
			"cmovlel %%r8d, %%ecx              \n\t"
			"movl    %%ecx, -80(%%rsp)         \n\t"
			"cmovll  %%eax, %%r8d              \n\t"
			"cmpl    %%edx, %%r13d             \n\t"
			"movl    %%edx, %%eax              \n\t"
			"cmovlel %%r13d, %%eax             \n\t"
			"movl    %%eax, -76(%%rsp)         \n\t"
			"cmovll  %%edx, %%r13d             \n\t"
			"movl    -44(%%rsp), %%eax         \n\t"
			"cmpl    %%eax, %%r11d             \n\t"
			"movl    %%eax, %%ecx              \n\t"
			"cmovlel %%r11d, %%ecx             \n\t"
			"movl    %%ecx, -72(%%rsp)         \n\t"
			"cmovll  %%eax, %%r11d             \n\t"
			"movl    -60(%%rsp), %%eax         \n\t"
			"movl    -40(%%rsp), %%ecx         \n\t"
			"cmpl    %%eax, %%ecx              \n\t"
			"movl    %%eax, %%edx              \n\t"
			"cmovlel %%ecx, %%edx              \n\t"
			"movl    %%edx, 4(%[rdi])         \n\t"
			"cmovll  %%eax, %%ecx              \n\t"
			"movl    %%ecx, 8(%[rdi])         \n\t"
			"movl    -56(%%rsp), %%eax         \n\t"
			"cmpl    %%eax, %%r14d             \n\t"
			"movl    %%eax, %%edx              \n\t"
			"cmovlel %%r14d, %%edx             \n\t"
			"movl    %%edx, 12(%[rdi])        \n\t"
			"cmovll  %%eax, %%r14d             \n\t"
			"movl    %%r14d, 16(%[rdi])       \n\t"
			"movl    -52(%%rsp), %%r14d        \n\t"
			"cmpl    %%r14d, %%r10d            \n\t"
			"movl    %%r14d, %%edx             \n\t"
			"cmovlel %%r10d, %%edx             \n\t"
			"movl    %%edx, 20(%[rdi])        \n\t"
			"cmovll  %%r14d, %%r10d            \n\t"
			"movl    %r10d, 24(%[rdi])       \n\t"
			"movl    -36(%%rsp), %%r10d        \n\t"
			"cmpl    %%r10d, %%r15d            \n\t"
			"movl    %%r10d, %%edx             \n\t"
			"cmovlel %%r15d, %%edx             \n\t"
			"movl    %%edx, 28(%[rdi])        \n\t"
			"cmovll  %%r10d, %%r15d            \n\t"
			"movl    %%r15d, 32(%[rdi])       \n\t"
			"movl    -32(%%rsp), %r10d        \n\t"
			"cmpl    %%r10d, %%esi             \n\t"
			"movl    %%r10d, %%edx             \n\t"
			"cmovlel %%esi, %%edx              \n\t"
			"movl    %%edx, 36(%[rdi])        \n\t"
			"cmovll  %%r10d, %%esi             \n\t"
			"movl    %%esi, 40(%[rdi])        \n\t"
			"movl    -28(%%rsp), %%edx         \n\t"
			"cmpl    %%edx, %%ebp              \n\t"
			"movl    %%edx, %%ecx              \n\t"
			"cmovlel %%ebp, %%ecx              \n\t"
			"movl    %%ecx, 44(%[rdi])        \n\t"
			"cmovll  %%edx, %%ebp              \n\t"
			"movl    %%ebp, 48(%[rdi])        \n\t"
			"movl    -24(%%rsp), %%edx         \n\t"
			"cmpl    %%edx, %%r9d              \n\t"
			"movl    %%edx, %%ecx              \n\t"
			"cmovlel %%r9d, %%ecx              \n\t"
			"movl    %%ecx, 52(%[rdi])        \n\t"
			"cmovll  %%edx, %%r9d              \n\t"
			"movl    %%r9d, 56(%[rdi])        \n\t"
			"movl    -20(%%rsp), %%ecx         \n\t"
			"cmpl    %%ecx, %%r12d             \n\t"
			"movl    %%ecx, %%eax              \n\t"
			"cmovlel %%r12d, %%eax             \n\t"
			"movl    %%eax, 60(%[rdi])        \n\t"
			"cmovll  %%ecx, %%r12d             \n\t"
			"movl    %%r12d, 64(%[rdi])       \n\t"
			"movl    -16(%%rsp), %%ecx         \n\t"
			"cmpl    %%ecx, %%ebx              \n\t"
			"movl    %%ecx, %%%eax              \n\t"
			"cmovlel %%ebx, %%eax              \n\t"
			"movl    %%eax, 68(%[rdi])        \n\t"
			"cmovll  %%ecx, %%ebx              \n\t"
			"movl    %%ebx, 72(%[rdi])        \n\t"
			"movl    -12(%%rsp), %%ecx         \n\t"
			"movl    -80(%%rsp), %%edx         \n\t"
			"cmpl    %%ecx, %%edx              \n\t"
			"movl    %%ecx, %%eax              \n\t"
			"cmovlel %%edx, %%eax              \n\t"
			"movl    %%eax, 76(%[rdi])        \n\t"
			"cmovll  %%ecx, %%edx              \n\t"
			"movl    %%edx, 80(%[rdi])        \n\t"
			"movl    -8(%%rsp), %%ecx          \n\t"
			"cmpl    %%ecx, %%r8d              \n\t"
			"movl    %%ecx, %%eax              \n\t"
			"cmovlel %%r8d, %%eax              \n\t"
			"movl    %%eax, 84(%[rdi])        \n\t"
			"cmovll  %%ecx, %%r8d              \n\t"
			"movl    %%r8d, 88(%[rdi])        \n\t"
			"movl    -4(%%rsp), %%ecx          \n\t"
			"movl    -76(%%rsp), %%edx         \n\t"
			"cmpl    %%ecx, %%edx              \n\t"
			"movl    %%ecx, %%eax              \n\t"
			"cmovlel %%edx, %%eax              \n\t"
			"movl    %%eax, 92(%[rdi])        \n\t"
			"cmovll  %%ecx, %%edx              \n\t"
			"movl    %%edx, 96(%[rdi])        \n\t"
			"movl    -68(%%rsp), %%ecx         \n\t"
			"cmpl    %%ecx, %%r13d             \n\t"
			"movl    %%ecx, %%eax              \n\t"
			"cmovlel %%r13d, %%eax             \n\t"
			"movl    %%eax, 100(%[rdi])       \n\t"
			"cmovll  %%ecx, %%r13d             \n\t"
			"movl    %%r13d, 104(%[rdi])      \n\t"
			"movl    -64(%%rsp), %%ecx         \n\t"
			"movl    -72(%%rsp), %%edx         \n\t"
			"cmpl    %%ecx, %%edx              \n\t"
			"movl    %%ecx, %%eax              \n\t"
			"cmovlel %%edx, %%eax              \n\t"
			"movl    %%eax, 108(%[rdi])       \n\t"
			"cmovll  %%ecx, %%edx              \n\t"
			"movl    %%edx, 112(%[rdi])       \n\t"
			"movl    -48(%%rsp), %%ecx         \n\t"
			"cmpl    %%ecx, %%r11d             \n\t"
			"movl    %%ecx, %%eax              \n\t"
			"cmovlel %%r11d, %%eax             \n\t"
			"movl    %%eax, 116(%[rdi])       \n\t"
			"cmovll  %%ecx, %%r11d             \n\t"
			"movl    %%r11d, 120(%[rdi])      \n\t"
			"popq    %%rbx                    \n\t"
			"popq    %%r12                    \n\t"
			"popq    %%r13                    \n\t"
			"popq    %%r14                    \n\t"
			"popq    %%r15                    \n\t"
			"popq    %%rbp                    \n\t"
			:[rdi] "+r"(first)
			:
			:"%eax","%ebx","%ecx","%edx","%esi","%ebp","%r8","%r8d","%r9d","r10d","%r11","%r11d","%r12","%r12d","%r13","%r13d","%r14","%r15","%rbp","%rsp"
	);
	*/
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template <unsigned NumElements, class Compare = void>
class static_sort2
{
	template <class A, class C> struct Swap
	{
		template <class T> inline void s(T &v0, T &v1)
		{
			T t = Compare()(v0, v1) ? v0 : v1; // Min
			v1 = Compare()(v0, v1) ? v1 : v0; // Max
			v0 = t;
		}

		inline Swap(A &a, const int &i0, const int &i1) { s(a[i0], a[i1]); }
	};

	template <class A> struct Swap <A, void>
	{
		template <class T> inline void s(T &v0, T &v1)
		{
			// Explicitly code out the Min and Max to nudge the compiler
			// to generate branchless code.
			T t = v0 < v1 ? v0 : v1; // Min
			v1 = v0 < v1 ? v1 : v0; // Max
			v0 = t;
		}

		inline Swap(A &a, const int &i0, const int &i1) { s(a[i0], a[i1]); }
	};

	template <class A, class C, int I, int J, int X, int Y> struct PB
	{
		inline PB(A &a)
		{
			enum { L = X >> 1, M = (X & 1 ? Y : Y + 1) >> 1, IAddL = I + L, XSubL = X - L };
			PB<A, C, I, J, L, M> p0(a);
			PB<A, C, IAddL, J + M, XSubL, Y - M> p1(a);
			PB<A, C, IAddL, J, XSubL, M> p2(a);
		}
	};

	template <class A, class C, int I, int J> struct PB <A, C, I, J, 1, 1>
	{
		inline PB(A &a) { Swap<A, C> s(a, I - 1, J - 1); }
	};

	template <class A, class C, int I, int J> struct PB <A, C, I, J, 1, 2>
	{
		inline PB(A &a) { Swap<A, C> s0(a, I - 1, J); Swap<A, C> s1(a, I - 1, J - 1); }
	};

	template <class A, class C, int I, int J> struct PB <A, C, I, J, 2, 1>
	{
		inline PB(A &a) { Swap<A, C> s0(a, I - 1, J - 1); Swap<A, C> s1(a, I, J - 1); }
	};

	template <class A, class C, int I, int M, bool Stop = false> struct PS
	{
		inline PS(A &a)
		{
			enum { L = M >> 1, IAddL = I + L, MSubL = M - L};
			PS<A, C, I, L, (L <= 1)> ps0(a);
			PS<A, C, IAddL, MSubL, (MSubL <= 1)> ps1(a);
			PB<A, C, I, IAddL, L, MSubL> pb(a);
		}
	};

	template <class A, class C, int I, int M> struct PS <A, C, I, M, true>
	{
		inline PS(A &a) {}
	};

public:
	/**
	 * Sorts the array/container arr.
	 * \param  arr  The array/container to be sorted.
	 */
	template <class Container> inline void operator() (Container &arr) const
	{
		PS<Container, Compare, 1, NumElements, (NumElements <= 1)> ps(arr);
	};

	/**
	 * Sorts the array arr.
	 * \param  arr  The array to be sorted.
	 */
	template <class T> inline void operator() (T *arr) const
	{
		PS<T*, Compare, 1, NumElements, (NumElements <= 1)> ps(arr);
	};
};
#endif
