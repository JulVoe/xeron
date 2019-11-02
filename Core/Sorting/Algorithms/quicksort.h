#ifndef udhasuiHZUIHSGLDG
#define udhasuiHZUIHSGLDG

#include "sortingnetworks.h"
#include "common.h"

/*
 * Reine quicksort-algorithmen
 */


template<class iterator>
void quickSort1(iterator begin, iterator end)
{
    if (end - begin > 1)
    {
        auto lpivot = begin + (end - begin) / 2;
        auto rpivot = lpivot;

        auto pValue = *lpivot;

        auto left_it = lpivot - 1;
        auto right_it = rpivot + 1;

        auto lValue = *left_it;
        auto rValue = *right_it;

        bool isGreater = false;
        bool isLess = false;

        while (left_it != begin-1 || right_it != end)
        {
            if (lValue >= pValue)
            {
                if (lValue == pValue)
                {
                    lpivot--;
                    swap(lpivot, left_it);
                }
                else
                    isGreater = true;
            }

            if (rValue <= pValue)
            {
                if (rValue == pValue)
                {
                    rpivot++;
                    swap(rpivot, right_it);
                }
                else
                    isLess = true;
            }
            if (isGreater && isLess)
            {
            	swap(left_it, right_it);
            }
            else if (isGreater)
            {
                if (left_it != lpivot - 1)
                	swap(left_it, lpivot - 1);
                swap(rpivot - 1, lpivot - 1);
                swap(rpivot, rpivot - 1);
                lpivot--;
                rpivot--;
            }
            else if (isLess)
            {
                if (right_it != rpivot + 1)
                	swap(right_it, rpivot + 1);
                swap(lpivot + 1, rpivot + 1);
                swap(lpivot, lpivot + 1);
                lpivot++;
                rpivot++;
            }

            if (left_it != begin - 1)
                left_it--;
            if (right_it != end)
                right_it++;

            lValue = *left_it;
            rValue = *right_it;

            isGreater = false;
            isLess = false;
        }

        quickSort1(begin, lpivot);
        quickSort1(rpivot + 1, end);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------

int* MedianOfThreePartition(int *a, int* b) {
    int x=*a;
    int y=*((a-b)/2+b);
    int z=*b;
    int *i=a;
    int *j=b;
    if (((y>x) && (y<z)) || ((y>z) && (y<x)) ) x=y;
    else if (((z>x) && (z<y)) || ((z>y) && (z<x)) ) x=z;
    while (1) {
        do  {j--;} while (*j > x);
        do  {i++;} while (*i < x);
        if  (i < j) swap(i,j);
        else return j+1;
    }
}

template<class T>
void quickSort2(T *start,T *end) {
    T *q;
    if (end-start<2) return;
    q=MedianOfThreePartition(start,end);
    quickSort2(start,q);
    quickSort2(q,end);
}

template<class T>
T* RandomAndMedianPartition(T *p, T *r) {
    T t=( (rand()%(*r-*p) )>>1) + *p + (*r-*p)/4;
    T x=*(p+t);
    T y=*(p+t+1);
    T z=*(p+t-1);
    T* i=p;
    T* j=r-1;

    if (((y>x) && (y<z)) || ((y>z) && (y<x)) ) x=y;
    else if (((z>x) && (z<y)) || ((z>y) && (z<x)) ) x=z;
    while (1) {
        do  {j--;} while (*j > x);
        do  {i++;} while (*i < x);
        if  (i < j) swap(i,j);
        else return j+1;
    }
}

template<class T>
void quickSort3(T *start,T *end) {
    T *q;
    if (end-start<2) return;
    q=RandomAndMedianPartition(start,end);
    quickSort3(start,q);
    quickSort3(q,end);
}

//------------------------------------------------------------------------------------------------------------------

#define cond_swap(a,b) if(*(a)>*(b)) swap((a),(b));

template<class T>
T* partition(T* sta, T* sto){
	T* mid = ((sto-sta)/2)+sta; //Equivalent to (sta+sto)/2, but avoids overflow
	cond_swap(mid,(sto-1));
	cond_swap(sta,(sto-1));
	cond_swap(sta,mid);

	T* ret = sta;
	for(T* p=sta;p!=sto;++p){
		if(*p<=*mid){
			swap(p,ret);
			++ret;
		}
	}
	return ret;
}

template<class T>
void quicksort4(T* sta, T* end){
	switch(end-sta){
	case 0:
		return;
	case 1:
		return;
	case 2:
		static_sort1<2>(sta);
		break;
	case 3:
		static_sort1<3>(sta);
		break;
	case 4:
		static_sort1<4>(sta);
		break;
	case 5:
		static_sort1<5>(sta);
		break;
	case 6:
		static_sort1<6>(sta);
		break;
	case 7:
		static_sort1<7>(sta);
		break;
	case 8:
		static_sort1<8>(sta);
		break;
	case 9:
		static_sort1<9>(sta);
		break;
	case 10:
		static_sort1<10>(sta);
		break;
	case 11:
		static_sort1<11>(sta);
		break;
	case 12:
		static_sort1<12>(sta);
		break;
	case 13:
		static_sort1<13>(sta);
		break;
	case 14:
		static_sort1<14>(sta);
		break;
	case 15:
		static_sort1<15>(sta);
		break;
	case 16:
		static_sort1<16>(sta);
		break;
	default:
		break;
	}

	T *pivot = partition(sta,end);
//	std::cout<<pivot-sta<<std::endl;
	quicksort4(sta,pivot);
	quicksort4(pivot,end);
}

//------------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
void quicksort5(T * const array, const int N)
{
    // Return if array size is 1 or less
    if (N <= 1)
    {
        return;
    }
    else if (N == 2)
    {
        // If two elements in the array, sort them manually and return
        if (array[0] > array[1])
        {
            std::swap(array[0], array[1]);
        }
        return;
    }

    // Optimization: Choose median of first, middle and last element as the pivot
    int indices_tmp[3] = {0, (N/2)-1, N-1};
    cond_swap(array+1, array+2);
    cond_swap(array, array+2);
    cond_swap(array, array+1);


    const int pivot = N-1;
    // Place pivot element at its location (end of array)
    std::swap(array[indices_tmp[1]], array[pivot]);

    // Indices of left and right elements to swap:
    //      array[i0] < array[pivot] and array[i1] > array[pivot]
    int i0 = 0;
    int i1 = pivot-1;
    // Loop until break statement reached
    while (true)
    {
        // Store previous values so we know where to start looking for the next
        // possible elements to swap
        const int prev_i0 = i0;
        const int prev_i1 = i1;
        // Reset indices. Later, we detect if these values were changed.
        i0 = -1;
        i1 = -1;

        // Find the first element that is larger than the pivot
        for (int i = prev_i0 ; i < pivot ; i++)
        {
            if (array[i] > array[pivot])
            {
                i0 = i;
                break;
            }
        }

        // Find the last element that is smaller than the pivot
        for (int i = prev_i1 ; i > i0 ; i--)
        {
            if (array[i] < array[pivot])
            {
                i1 = i;
                break;
            }
        }

        if (i0 == -1 and i1 == -1)
        {
            // If both i0 and i1 are -1, then the whole array is equal to a single
            // value. In that case, we return.
            return;
        }
        else if (i0 == -1)
        {
            // All elements are either smaller or equal to the pivot.
            // Keep the pivot at the end of the array.
            i0 = N-1;
            i1 = N;
            break;
        }
        else if (i1 == -1)
        {
            // All elements are either larger or equal to the pivot
            // Place pivot at the beginning.
            i1 = i0 + 1;
            break;
        }
        else
        {
            // Element at i0 is larger than the pivot and element at i1
            // is smaller; swap them.
            std::swap(array[i0], array[i1]);
            i0++;
            i1--;
        }
    }

    // Swap the pivot with the first element of the large values
    if (i0 != pivot)
    {
        std::swap(array[i0], array[pivot]);
    }

    // Recursively call function on the two subarrays
    quicksort5(array, i0);
    quicksort5(array+i0+1, N-i1);
}
#undef cond_swap
#endif
