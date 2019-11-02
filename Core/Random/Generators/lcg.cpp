#ifdef VS
#define ALIGN_PRE __declspec(align(16))
#define ALIGN_SUF
#else
#define ALIGN_PRE
#define ALIGN_SUF  __attribute__ ((aligned(16)))
#endif
#include<x86intrin.h>





static unsigned int g_seed;

inline void fast_srand(int seed)
{
	g_seed = seed;
}

inline int fastrand()
{
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}



//#define COMPATABILITY
ALIGN_PRE static __m128i cur_seed ALIGN_SUF;

void srand_sse(unsigned int seed)
{
	cur_seed = _mm_set_epi32(seed, seed + 1, seed, seed + 1);
}

inline void rand_sse(unsigned int* result)
{
	ALIGN_PRE __m128i cur_seed_split ALIGN_SUF;
	ALIGN_PRE __m128i multiplier     ALIGN_SUF;
	ALIGN_PRE __m128i adder          ALIGN_SUF;
	ALIGN_PRE __m128i mod_mask       ALIGN_SUF;
	ALIGN_PRE __m128i sra_mask       ALIGN_SUF;
	ALIGN_PRE __m128i sseresult      ALIGN_SUF;
	ALIGN_PRE static const unsigned int mult[4]   ALIGN_SUF = { 214013, 17405, 214013, 69069 };
	ALIGN_PRE static const unsigned int gadd[4]   ALIGN_SUF = { 2531011, 10395331, 13737667, 1 };
	ALIGN_PRE static const unsigned int mask[4]   ALIGN_SUF = { 0xFFFFFFFF, 0, 0xFFFFFFFF, 0 };
	ALIGN_PRE static const unsigned int masklo[4] ALIGN_SUF = { 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF };
	adder = _mm_load_si128((__m128i*) gadd);
	multiplier = _mm_load_si128((__m128i*) mult);
	mod_mask = _mm_load_si128((__m128i*) mask);
	sra_mask = _mm_load_si128((__m128i*) masklo);
	cur_seed_split = _mm_shuffle_epi32(cur_seed, _MM_SHUFFLE(2, 3, 0, 1));
	cur_seed = _mm_mul_epu32(cur_seed, multiplier);
	multiplier = _mm_shuffle_epi32(multiplier, _MM_SHUFFLE(2, 3, 0, 1));
	cur_seed_split = _mm_mul_epu32(cur_seed_split, multiplier);
	cur_seed = _mm_and_si128(cur_seed, mod_mask);
	cur_seed_split = _mm_and_si128(cur_seed_split, mod_mask);
	cur_seed_split = _mm_shuffle_epi32(cur_seed_split, _MM_SHUFFLE(2, 3, 0, 1));
	cur_seed = _mm_or_si128(cur_seed, cur_seed_split);
	cur_seed = _mm_add_epi32(cur_seed, adder);
#ifdef COMPATABILITY
	// Add the lines below if you wish to reduce your results to 16-bit vals...
	sseresult = _mm_srai_epi32(cur_seed, 16);
	sseresult = _mm_and_si128(sseresult, sra_mask);
	_mm_storeu_si128((__m128i*) result, sseresult);
	return;
#endif
	_mm_storeu_si128((__m128i*) result, cur_seed);
	return;
}
