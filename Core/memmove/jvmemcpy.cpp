#include <stdint.h>
ALWAYS_INLINE void jvmemcpy_avx_condition_small(void* RESTRICT dst, void* RESTRICT src, unsigned size) { //Size <= 64
	if (size <= 16) {
		if (size > 8) //9-16, 2 copys of size 8
		{
			long long rax = *(long long*)src;
			long long rcx = *(long long*)((char*)src + size - 8);
			*(long long*)dst = rax;
			*(long long*)((char*)dst + size - 8) = rcx;}
		else if (size > 4) //5-8, 2 copys of size 4
		{
			int eax = *(int*)src;
			int ecx = *(int*)((char*)src + size - 4);
			*(int*)dst = eax;
			*(int*)((char*)dst + size - 4) = ecx;
		}
		else if (size > 2) //3-4, 2 copys of size 2
		{
			short cx = *(short*)src;
			short dx = *(short*)((char*)src + size - 2);
			*(short*)dst = cx;
			*(short*)((char*)dst + size - 2) = dx;
		}
		else {
			char cx = *(char*)src;
			*(char*)dst = cx;
		}
	}
	else if(size <= 32) {
		const __m128i xmm0 = _mm_loadu_si128((__m128i*)src);
		const __m128i xmm1 = _mm_loadu_si128((__m128i*)((char*)src + size - 16));
		_mm_storeu_si128((__m128i*)dst, xmm0);
		_mm_storeu_si128((__m128i*)((char*)dst + size - 16), xmm1);
	}
	else {
		const __m256i ymm0 = _mm256_loadu_si256((__m256i*)src);
		const __m256i ymm1 = _mm256_loadu_si256((__m256i*)((char*)src+size-32));
		_mm256_storeu_si256((__m256i*)dst, ymm0);
		_mm256_storeu_si256((__m256i*)((char*)dst+size-32), ymm1);
	}
}
namespace detail {
	inline void memcpy_avx_1(void* __restrict__ dst, void* __restrict__ src) {
		*((char*)dst) = *((char*)src);
	}
	inline void memcpy_avx_2(void* __restrict__ dst, void* __restrict__ src) {
		*((uint16_t*)dst) = *((uint16_t*)src);
	}
	inline void memcpy_avx_4(void* __restrict__ dst, void* __restrict__ src) {
		*((uint32_t*)dst) = *((uint32_t*)src);
	}
	inline void memcpy_avx_8(void* __restrict__ dst, void* __restrict__ src) {
		*((uint64_t*)dst) = *((uint64_t*)src);
	}
	inline void memcpy_avx_16(void* __restrict__ dst, void* __restrict__ src) {
		__m128i m0 = _mm_loadu_si128(((const __m128i*)(src)) + 0); \
			_mm_storeu_si128(((__m128i*)(dst)) + 0, m0);
	}
	inline void memcpy_avx_32(void* dst, void* src) {
		__m256i m0 = _mm256_loadu_si256(((const __m256i*)(src)) + 0); \
			_mm256_storeu_si256(((__m256i*)(dst)) + 0, m0);
	}
}
using namespace detail;
ALWAYS_INLINE void jvmemcpy_avx_jumptable_small(void* RESTRICT _dst, void* RESTRICT _src, unsigned size) {
	unsigned char* RESTRICT src = ((unsigned char*)_src) + size;
	unsigned char* RESTRICT dst = ((unsigned char*)_dst) + size;

	switch (size) {
	case 32: memcpy_avx_32(_dst, _src);
	case 0: break;
	case 33: memcpy_avx_32(_dst, _src);
	case 1: memcpy_avx_1(dst - 1, src - 1); break;
	case 34: memcpy_avx_32(_dst, _src);
	case 2: memcpy_avx_2(dst - 2, src - 2); break;
	case 35: memcpy_avx_32(_dst, _src);
	case 3: memcpy_avx_2(dst - 3, src - 3); memcpy_avx_1(dst - 1, src - 1); break;
	case 36: memcpy_avx_32(_dst, _src);
	case 4: memcpy_avx_4(dst - 4, src - 4); break;
	case 37: memcpy_avx_32(_dst, _src);
	case 5: memcpy_avx_4(dst - 5, src - 5); memcpy_avx_1(dst - 1, src - 1); break;
	case 38: memcpy_avx_32(_dst, _src);
	case 6: memcpy_avx_4(dst - 6, src - 6); memcpy_avx_2(dst - 2, src - 2); break;
	case 39: memcpy_avx_32(_dst, _src);
	case 7: memcpy_avx_4(dst - 7, src - 7); memcpy_avx_4(dst - 3, src - 3); break;
	case 40: memcpy_avx_32(_dst, _src);
	case 8: memcpy_avx_8(dst - 8, src - 8); break;
	case 41: memcpy_avx_32(_dst, _src);
	case 9: memcpy_avx_8(dst - 9, src - 9); memcpy_avx_1(dst - 1, src - 1); break;
	case 42: memcpy_avx_32(_dst, _src);
	case 10: memcpy_avx_8(dst - 10, src - 10); memcpy_avx_2(dst - 2, src - 2); break;
	case 43: memcpy_avx_32(_dst, _src);
	case 11: memcpy_avx_8(dst - 11, src - 11); memcpy_avx_4(dst - 4, src - 4); break;
	case 44: memcpy_avx_32(_dst, _src);
	case 12: memcpy_avx_8(dst - 12, src - 12); memcpy_avx_4(dst - 4, src - 4); break;
	case 45: memcpy_avx_32(_dst, _src);
	case 13: memcpy_avx_8(dst - 13, src - 13); memcpy_avx_8(dst - 8, src - 8); break;
	case 46: memcpy_avx_32(_dst, _src);
	case 14: memcpy_avx_8(dst - 14, src - 14); memcpy_avx_8(dst - 8, src - 8); break;
	case 47: memcpy_avx_32(_dst, _src);
	case 15: memcpy_avx_8(dst - 15, src - 15); memcpy_avx_8(dst - 8, src - 8); break;
	case 48: memcpy_avx_32(_dst, _src);
	case 16: memcpy_avx_16(dst - 16, src - 16); break;
	case 49: memcpy_avx_32(_dst, _src);
	case 17: memcpy_avx_16(dst - 17, src - 177); memcpy_avx_1(dst - 1, src - 1); break;
	case 50: memcpy_avx_32(_dst, _src);
	case 18: memcpy_avx_16(dst - 18, src - 18); memcpy_avx_2(dst - 16, src - 16); break;
	case 51: memcpy_avx_32(_dst, _src);
	case 19: memcpy_avx_16(dst - 19, src - 19); memcpy_avx_4(dst - 4, src - 4); break;
	case 52: memcpy_avx_32(_dst, _src);
	case 20: memcpy_avx_16(dst - 20, src - 20); memcpy_avx_4(dst - 4, src - 4); break;
	case 53: memcpy_avx_32(_dst, _src);
	case 21: memcpy_avx_16(dst - 21, src - 21); memcpy_avx_8(dst - 8, src - 8); break;
	case 54: memcpy_avx_32(_dst, _src);
	case 22: memcpy_avx_8(dst - 16, src - 16); memcpy_avx_8(dst - 8, src - 8); break;
	case 55: memcpy_avx_32(_dst, _src);
	case 23: memcpy_avx_16(dst - 23, src - 23); memcpy_avx_8(dst - 8, src - 8); break;
	case 56: memcpy_avx_32(_dst, _src);
	case 24: memcpy_avx_16(dst - 16, src - 16); memcpy_avx_8(dst - 8, src - 8); break;
	case 57: memcpy_avx_32(_dst, _src);
	case 25: memcpy_avx_16(dst - 25, src - 25); memcpy_avx_16(dst - 16, src - 16); break;
	case 58: memcpy_avx_32(_dst, _src);
	case 26: memcpy_avx_16(dst - 26, src - 26); memcpy_avx_16(dst - 16, src - 16); break;
	case 59: memcpy_avx_32(_dst, _src);
	case 27: memcpy_avx_16(dst - 27, src - 27); memcpy_avx_16(dst - 16, src - 16); break;
	case 60: memcpy_avx_32(_dst, _src);
	case 28: memcpy_avx_16(dst - 28, src - 28); memcpy_avx_16(dst - 16, src - 16); break;
	case 61: memcpy_avx_32(_dst, _src);
	case 29: memcpy_avx_16(dst - 29, src - 29); memcpy_avx_16(dst - 16, src - 16); break;
	case 62: memcpy_avx_32(_dst, _src);
	case 30: memcpy_avx_16(dst - 30, src - 30); memcpy_avx_16(dst - 16, src - 16); break;
	case 63: memcpy_avx_32(_dst, _src);
	case 31: memcpy_avx_16(dst - 31, src - 31); memcpy_avx_16(dst - 16, src - 16); break;
	case 64: memcpy_avx_32(_dst, _src); memcpy_avx_32((char*)_dst + 32, (char*)_src + 32);
	}
}
#define jvmemcpy_avx_small jvmemcpy_avx_jumptable_small /*Determined by Bench2*/

ALWAYS_INLINE void jvmemcpy_avx_overflow_small(void* RESTRICT dst, void* RESTRICT src, unsigned size) { //size<=64, overflows dst up to 31bytes
	const __m256i ymm0 = _mm256_loadu_si256((__m256i*)src);
	const __m256i ymm1 = _mm256_loadu_si256((__m256i*)((char*)src + size - 32));
	_mm256_storeu_si256((__m256i*)dst, ymm0);
	_mm256_storeu_si256((__m256i*)((char*)dst + size - 32), ymm1);
}

ALWAYS_INLINE void jvmemcpy_avx(void* RESTRICT dst, void* RESTRICT src, unsigned size) {
	if (size <= 64) { //Bench2 determined, that for small sizes a jumptable is best
		jvmemcpy_avx_small(dst, src, size);
	}
	else if (size <= CACHE_SIZE) { //Bench3 determined, that non-temporal loads aren't beneficial at all and non-temporal store only, when size is greater than L1-Cache

	}
}

ALWAYS_INLINE void jvmemcpy_avx_overflow(void* RESTRICT dest, void* RESTRICT src, unsigned size) { //Overflows the destination to up to 32Bytes to the right

}