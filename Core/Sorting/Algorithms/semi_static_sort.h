#pragma once

#include "sortingnetworks.h"

void semi_static_sort(int *p, size_t n){
	switch(n){
	case 0:
		return;
	case 1:
		static_sort1<1>(p);
		return;
	case 2:
		static_sort1<2>(p);
		return;
	case 3:
		static_sort1<3>(p);
		return;
	case 4:
		static_sort1<4>(p);
		return;
	case 5:
		static_sort1<5>(p);
		return;
	case 6:
		static_sort1<6>(p);
		return;
	case 7:
		static_sort1<7>(p);
		return;
	case 8:
		static_sort1<8>(p);
		return;
	case 9:
		static_sort1<9>(p);
		return;
	case 10:
		static_sort1<10>(p);
		return;
	case 11:
		static_sort1<11>(p);
		return;
	case 12:
		static_sort1<12>(p);
		return;
	case 13:
		static_sort1<13>(p);
		return;
	case 14:
		static_sort1<14>(p);
		return;
	case 15:
		static_sort1<15>(p);
		return;
	case 16:
		static_sort1<16>(p);
		return;
	case 17:
		static_sort1<17>(p);
		return;
	case 18:
		static_sort1<18>(p);
		return;
	case 19:
		static_sort1<19>(p);
		return;
	case 20:
		static_sort1<20>(p);
		return;
	case 21:
		static_sort1<21>(p);
		return;
	case 22:
		static_sort1<22>(p);
		return;
	case 23:
		static_sort1<23>(p);
		return;
	case 24:
		static_sort1<24>(p);
		return;
	case 25:
		static_sort1<25>(p);
		return;
	case 26:
		static_sort1<26>(p);
		return;
	case 27:
		static_sort1<27>(p);
		return;
	case 28:
		static_sort1<28>(p);
		return;
	case 29:
		static_sort1<29>(p);
		return;
	case 30:
		static_sort1<30>(p);
		return;
	case 31:
		static_sort1<31>(p);
		return;
	case 32:
		static_sort1<32>(p);
		return;
	default:
		semi_static_sort(p,n>>1);
		semi_static_sort(p+(n>>1),n-(n>>1));
		std::inplace_merge(p,p+(n>>1),p+n);
		return;
	}
}
