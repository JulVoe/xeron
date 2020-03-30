#pragma once
#include "../intrin.cpp"

namespace intrin {
	namespace impl {
		//    +========================================================================================================+
		//    |===============================================|epi32-pd|===============================================|
		//    +========================================================================================================+
		//Noch Hackfloat version because of sign bit. dvtpd_epi32 will be faster

		ALWAYS_INLINE __m128i _mm_cvt_2pdx2_i32x4_1(__m128d lo, __m128d hi) {
			const __m128i lo_epi32 = _mm_cvtpd_epi32(lo);
			const __m128i hi_epi32 = _mm_slli_si128(_mm_cvtpd_epi32(hi),64);

			return _mm_xor_si128(lo_epi32, hi_epi32);
		}
		ALWAYS_INLINE __m128i _mm_cvt_2pdx2_i32x4_2(__m128d lo, __m128d hi) {
			const __m128i lo_epi32 = _mm_cvtpd_epi32(lo);
			const __m128i hi_epi32 = _mm_shuffle_epi32(_mm_cvtpd_epi32(hi), 0b10110001);

			return _mm_xor_si128(lo_epi32, hi_epi32);
		}


		//https://stackoverflow.com/questions/429632/how-to-speed-up-floating-point-to-integer-number-conversion
		//http://stereopsis.com/sree/fpu2006.html
		ALWAYS_INLINE __m128i _mm_cvt_2pdx2_u32x4_1(__m128d lo, __m128d hi) {
			//0.: Constants
			const __m128d add = _mm_set1_pd((double)((1l << 51) + (1l << 52)));

			const __m128i lo_epi64 = _mm_castpd_epi64(_mm_add_pd(lo, add));
			const __m128i hi_epi64 = _mm_slli_si128(_mm_castpd_epi64(_mm_add_pd(hi, add)), 32);

#if AVX >= 2
			return _mm_blend_epi32(lo_epi64, hi_epi64);
#else
			return _mm_blend_epi16(lo_epi64, hi_epi64, 0b11001100);
#endif
		}
		ALWAYS_INLINE __m128i _mm_cvt_2pdx2_u32x4_2(__m128d lo, __m128d hi) {
			//0.: Constants
			const __m128d add = _mm_set1_pd((double)((1l << 51) + (1l << 52)));

			const __m128i lo_epi64 = _mm_castpd_epi64(_mm_add_pd(lo, add));
			const __m128i hi_epi64 = _mm_shuffle_epi32(_mm_castpd_epi64(_mm_add_pd(hi, add)), 0b10110001);

#if AVX >= 2
			return _mm_blend_epi32(lo_epi64, hi_epi64);
#else
			return _mm_blend_epi16(lo_epi64, hi_epi64, 0b11001100);
#endif
		}




		ALWAYS_INLINE void _mm_cvt_i32x4_2pdx2_1(__m128i in, __m128d& lo, __m128d& hi) {
			const __m128i exp = _mm_sign(_mm_set1_epi32(1075<<20), in); //1023+52
			const __m128d magicDouble = _mm_set1_ps(1d<<52); //2^52

			const __m128d lo_hackfloat = _mm_castepi64_pd(_mm_unpacklo_epi32(in, exp));
			const __m128d hi_hackfloat = _mm_castepi64_pd(_mm_unpackhi_epi32(in, exp));

			lo = _mm_sub_pd(lo_hackfloat, magicDouble);
			hi = _mm_sub_pd(hi_hackfloat, magicDouble);
		}
		ALWAYS_INLINE void _mm_cvt_i32x4_2pdx2_2(__m128i in, __m128d& lo, __m128d& hi) {
			lo = _mm_cvtepi32_pd(in);
			hi = _mm_cvtepi32_pd(_mm_srli_si128(in,64));
		}
		ALWAYS_INLINE void _mm_cvt_i32x4_2pdx2_2(__m128i in, __m128d& lo, __m128d& hi) {
			lo = _mm_cvtepi32_pd(in);
			hi = _mm_cvtepi32_pd(_mm_shuffle_epi32(in, 0b01001110));
		}

		ALWAYS_INLINE void _mm_cvt_u32x4_2pdx2_1(__m128i in, __m128d& lo, __m128d& hi) {
			const __m128i exp = _mm_set1_epi32(1075 << 20); //1023+52
			const __m128d magicDouble = _mm_set1_ps(1d << 52); //2^52

			const __m128d lo_hackfloat = _mm_castepi64_pd(_mm_unpacklo_epi32(in, exp));
			const __m128d hi_hackfloat = _mm_castepi64_pd(_mm_unpackhi_epi32(in, exp));

			lo = _mm_sub_pd(lo_hackfloat, magicDouble);
			hi = _mm_sub_pd(hi_hackfloat, magicDouble);
		}
	}
}