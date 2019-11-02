#include <cassert>                   //For static_assert
#include <x86intrin.h>               //For SSE and AVX
#include "../SystemInfo/Makros.hpp"  //For compiler detection, VS makro
#ifdef VS
#include "msinttypes/stdint.h"
#define ALIGN_PRE __declspec(align(16))
#define ALIGN_SUF
#else
#include <stdint.h>
#define ALIGN_PRE
#define ALIGN_SUF  __attribute__ ((aligned(16)))
#endif



//PCG                 https://github.com/lemire/simdpcg
//LCG                 https://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor
//xoshiro256+         http://prng.di.unimi.it/
//xorshift            https://stackoverflow.com/questions/24001930/avx-sse-version-of-xorshift128,https://github.com/lemire/SIMDxorshift

namespace jlib {
	namespace random {

		enum {LCG, PCG, XOSHIRO, XORSHIFT};

		template<short N>
		union int32_vec {
			__m256i vec;
			int32_t arr[N];
		};

		template<short algo>
		class random_32 {
			static_assert(algo == 0, "Unknown algorithm for random_32"); //Will fail when generated
		};

#ifndef AVX2
		template<>
		class random_32<LCG> { //https://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor
		private:
			ALIGN_PRE int32_vec<8> seed ALIGN_SUF;
			unsigned ind;
			ALIGN_PRE __m128i       multiplier ALIGN_SUF = _mm_load_si128((__m128i*) (unsigned int[]) { 214013, 17405, 214013, 69069 });
			ALIGN_PRE __m128i const adder      ALIGN_SUF = _mm_load_si128((__m128i*) (unsigned int[]) { 2531011, 10395331, 13737667, 1 });
			ALIGN_PRE __m128i const mod_mask   ALIGN_SUF = _mm_load_si128((__m128i*) (unsigned int[]) { 0xFFFFFFFF, 0, 0xFFFFFFFF, 0 });
			ALIGN_PRE __m128i const sra_mask   ALIGN_SUF = _mm_load_si128((__m128i*) (unsigned int[]) { 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF });

		public:

			random_32(unsigned int seed) {
				srand(seed);
			}
			random_32() {
				srand(0x82c77445);
			}

			inline void srand(unsigned int _seed) {
				seed.vec = _mm_set_epi32(_seed, _seed + 1, _seed, _seed + 1);
				ind = 0; //Freshly generated
			}

			inline int32_t rand()
			{
				if (ind == 0) {
					ALIGN_PRE __m128i seed_split ALIGN_SUF = _mm_shuffle_epi32(seed.vec, _MM_SHUFFLE(2, 3, 0, 1));

					multiplier = _mm_shuffle_epi32(multiplier, _MM_SHUFFLE(2, 3, 0, 1));
					seed_split = _mm_mul_epu32(seed_split, multiplier);
					seed.vec = _mm_and_si128(seed.vec, mod_mask);
					seed_split = _mm_and_si128(seed_split, mod_mask);
					seed_split = _mm_shuffle_epi32(seed_split, _MM_SHUFFLE(2, 3, 0, 1));
					seed.vec = _mm_or_si128(seed.vec, seed_split);
					seed.vec = _mm_add_epi32(seed.vec, adder);
					ind = 8;
				}

				return seed.arr[--ind];
			}
		};
#else
		template<>
		class random_32<LCG> { //https://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor
		private:

			ALIGN_PRE int32_vec seed ALIGN_SUF;
			unsigned ind;
			ALIGN_PRE __m256i       multiplier ALIGN_SUF = _mm256_load_si256((__m256i*) (unsigned int[]) { 214013, 17405, 214013, 69069, 897623, 487376, 315429, 215467 });
			ALIGN_PRE __m256i const adder      ALIGN_SUF = _mm256_load_si256((__m256i*) (unsigned int[]) { 2531011, 10395331, 13737667, 1, 7481637, 3784279, 634789, 547585 });
			ALIGN_PRE __m256i const mod_mask   ALIGN_SUF = _mm256_load_si256((__m256i*) (unsigned int[]) { 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0});
			ALIGN_PRE __m256i const sra_mask   ALIGN_SUF = _mm256_load_si256((__m256i*) (unsigned int[]) { 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF});

		public:

			random_32(unsigned int seed) {
				srand(seed);
			}
			random_32() {
				srand(0x82c77445);
			}

			inline void srand(unsigned int _seed) {
				seed.vec = _mm256_set_epi32(_seed, _seed + 1, _seed, _seed-1, _seed, _seed+1, _seed, _seed-1);
				ind = 0; //Freshly generated
			}

			inline int32_t rand()
			{
				if (ind == 0) {
					ALIGN_PRE __m256i seed_split ALIGN_SUF = _mm256_shuffle_epi32(seed.vec, _MM_SHUFFLE(2, 3, 0, 1));

					multiplier = _mm256_shuffle_epi32(multiplier, _MM_SHUFFLE(2, 3, 0, 1));
					seed_split = _mm256_mul_epu32(seed_split, multiplier);
					seed.vec   = _mm256_and_si256(seed.vec, mod_mask);
					seed_split = _mm256_and_si256(seed_split, mod_mask);
					seed_split = _mm256_shuffle_epi32(seed_split, _MM_SHUFFLE(2, 3, 0, 1));
					seed.vec   = _mm256_or_si256(seed.vec, seed_split);
					seed.vec   = _mm256_add_epi32(seed.vec, adder);
					ind = 8;
				}

				return seed.arr[--ind];
			}
		};
#endif




		template<>
		class random_32<PCG> { //A PCG traditionally always has a state double the size of the output, but this would almost half it's performance...
		private:
			__m256i state;
			__m256i inc;   
			__m256i mult = _mm256_set1_epi64x(0x5851f42d4c957f2dull);
			int32_vec<4> out;
	
			inline __m256i hacked_mm256_rorv_epi32(__m256i x, __m256i r) { //Rotates each uint32_t right by r bits
				//return (x<<(32-r)) | (x>>r);
				return _mm256_or_si256(_mm256_sllv_epi32(x, _mm256_sub_epi32(_mm256_set1_epi32(32), r)),_mm256_srlv_epi32(x, r));
			}

		public:
			/*LCG state-advance+permutation
			Permutations are: 
				- fixed shift
				- random shift
				- xor-shift
				- rotation
			*/


			inline int32_t rand() { //Xorfixedshift + Rotate
				__m256i xorshifted = _mm256_srli_epi64(state, 18); //xorshifted = ((state>>18)^state). Usually, an additional >>27 would be used at the end
				xorshifted = _mm256_xor_si256(xorshifted, state)
				//xorshifted = _mm256_srli_epi64(xorshifted, 27);
				__m256i rot = _mm256_srli_epi64(state, 59);
				
				state = _mm256_mul_epi64(state, mult); //LCG state advance
				state = _mm256_add_epi64(state, inc);
	
				out.vec = hacked_mm256_rorv_epi32(xorshifted, rot);
			}

			inline int32_t rand() {//Xorrandshift
				state = _mm256_mul_epi64(state,mul);
				state = _mm256_add_epi64(state,inc);

				__m256i ret = state 
			}

			
		};	
	}
}