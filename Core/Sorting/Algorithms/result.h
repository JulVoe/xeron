#pragma once

#include <iostream>
#include <algorithm>
#include <utility>

#define my_swap(a,b);\
	tmp = std::move(*(a));\
	*(a) = std::move(*(b));\
	*(b) = std::move(tmp);

#define cond_swap1(a,b);\
	if(*(a)>*(b)) std::iter_swap((a),(b));


#define cond_swap2(a,b);\
	s1 = ((unsigned)(*(a)-*(b)))>>31; /*Vorzeichen der Differenz*/ \
	s2 = s1 ^ 1; /*Negiert*/ \
	t = *(a)*s1+*(b)*s2;\
	*(b) = *(a)*s2+*(b)*s1;\
	*(a) = t;

#define cond_swap3(a,b);\
	t[0] = *(a); \
	t[1] = *(b);\
	s = ((unsigned)(*(a)-*(b)))>>31; /*Grab sign bit*/ \
	*(b) = t[s];\
	*(a) = t[1-s];

#define cond_swap4(a,b);\
    t[0] = *(b);\
	t[1] = *(a);\
	t[2] = *(b);\
	q=t;\
	q += (*(a)<*(b));\
	*(a) = *q;\
	*(b) = *(q+1);

/*
	__asm__(\
        "movl    (%[in_a]), %%eax\n\t"\
	    "movl    %%eax, -12(%%rsp)\n\t"\
        "movl    (%[in_b]), %%ecx\n\t"\
        "movl    %%ecx, -8(%%rsp)\n\t"\
        "movl    %%eax, -4(%%rsp)\n\t"\
        "xorl    %%edx, %%edx\n\t"\
        "cmpl    %%eax, %%ecx\n\t"\
        "setl    %%dl\n\t"\
        "movl    -12(%%rsp,%%rdx,4), %%eax\n\t"\
        "movl    %%eax, (%[out_b])\n\t"\
        "movl    -8(%%rsp,%%rdx,4), %%eax\n\t"\
        "movl    %%eax, (%[out_a])\n\t"\
        : [out_a] "=r"(a), [out_b] "=r"(b)\
        : [in_a] "r"(a), [in_b] "r"(b)\
        : "%eax","%rsp","%ecx","%edx","%dl"\
    );
*/

#define cond_swap5(a,b);\
    tmp = *(a);\
    *(a) = (tmp<*(b))?tmp:*(b);\
    *(b) = (tmp<*(b))?*(b):tmp;

/*
void y(int* a, int *b){
    __asm__(
        "movl    (%rdi), %eax\n\t"
        "movl    (%rsi), %ecx\n\t"
        "cmpl    %ecx, %eax\n\t"
        "cmovlel %eax, %ecx\n\t"
        "movl    %ecx, (%rdi)\n\t"
        "movl    (%rsi), %ecx\n\t"
        "cmovll  %ecx, %eax\n\t"
        "movl    %eax, (%rsi)\n\t"
    );
}

void z(int * a, int * b){
    __asm__(
        "movl    (%rdi), %eax\n\t"
        "cmpl    %eax, (%rsi)\n\t"
        "movl    %eax, %edx\n\t"
        "cmovle  (%rsi), %edx\n\t"
        "movl    %edx, (%rdi)\n\t"
        "cmovll  (%rsi), %eax\n\t"
        "movl    %eax, (%rsi)\n\t"
    );
}
*/






#define BASE_CASE_LIMIT 32

void msd_radix_inplace(int* sta, int*sto){

}

void lsd_radix_outofplace(int* sta, int* sto, int* buf){

}

void base_case(int* sta, int* sto){

}

void base_case(int* sta, size_t sto){

}




void result_sort(int* sta, int * sto){

}

void result_sort(int* sta, int* sto, int* buf){

}
