#pragma once
#include "../intrin.cpp"

namespace intrin {
	namespace impl {
//    +=========================================================================================================+
//    |===============================================|pd->epi32|===============================================|
//    +=========================================================================================================+
		
		//Converts two double[2](in) to int32_t[4]
		ALWAYS_INLINE __m128i _mm_cvt_2pdx2_i32x4_1(__m128d lo, __m128d hi) {
			//1.: Convert
			const __m128i lo_epi32 = _mm_cvtpd_epi32(lo);                    //    0    |    0    | (int)lo | (int)lo
			const __m128i hi_epi32 = _mm_slli_si128(_mm_cvtpd_epi32(hi),64); // (int)hi | (int)hi |    0    |    0   

			//2.: Stitch together result
			return _mm_xor_si128(lo_epi32, hi_epi32);                        // (int)hi | (int)hi | (int)lo | (int)lo
		}
		//Converts two double[2](in) to int32_t[4]
		//Note: Same as _1, but with shuffle instead of shift
		ALWAYS_INLINE __m128i _mm_cvt_2pdx2_i32x4_2(__m128d lo, __m128d hi) {
			//1.: Convert																  //    0    |    0    | (int)lo | (int)lo
			const __m128i lo_epi32 = _mm_cvtpd_epi32(lo);								  // (int)hi | (int)hi |    0    |    0   
			const __m128i hi_epi32 = _mm_shuffle_epi32(_mm_cvtpd_epi32(hi), 0b10110001);

			//2.: Stitch together result												  // (int)hi | (int)hi | (int)lo | (int)lo
			return _mm_xor_si128(lo_epi32, hi_epi32);
		}

		//Converts two double[2](in) to int32_t[4]
		//Note: https://stackoverflow.com/questions/429632/how-to-speed-up-floating-point-to-integer-number-conversion
		//Note: http://stereopsis.com/sree/fpu2006.html
		ALWAYS_INLINE __m128i _mm_cvt_2pdx2_u32x4_2(__m128d lo, __m128d hi) {
			//0.: Constants
			const __m128d add = _mm_set1_pd((double)((1l << 51) + (1l << 52)));

			//1.: Round mantissa, shift into right place
			const __m128i lo_epi64 = _mm_shuffle_epi32(_mm_castpd_epi64(_mm_add_pd(lo, add)), 0b11011000); //  trash  |  trash  | (int)lo | (int)lo
			const __m128i hi_epi64 = _mm_shuffle_epi32(_mm_castpd_epi64(_mm_add_pd(hi, add)), 0b10001101); // (int)hi | (int)hi |  trash  |  trash

			//2.: Stitch together result
#if AVX >= 2
			return _mm_blend_epi32(lo_epi64, hi_epi64);													   // (int)hi | (int)hi | (int)lo | (int)lo
#else
			return _mm_blend_epi16(lo_epi64, hi_epi64, 0b11001100);										   // (int)hi | (int)hi | (int)lo | (int)lo
#endif
		}

#if 0 //Some alternatives but with wrong output-partition
		//Converts two double[2](in) to int32_t[4]
		//Note: https://stackoverflow.com/questions/429632/how-to-speed-up-floating-point-to-integer-number-conversion
		//Note: http://stereopsis.com/sree/fpu2006.html
		ALWAYS_INLINE __m128i _mm_cvt_2pdx2_u32x4_1(__m128d lo, __m128d hi) {
			//0.: Constants
			const __m128d add = _mm_set1_pd((double)((1l << 51) + (1l << 52)));

			//1.: Round mantissa, shift into right place
			const __m128i lo_epi64 = _mm_castpd_epi64(_mm_add_pd(lo, add));                     //  trash  | (int)lo |  trash  | (int)lo
			const __m128i hi_epi64 = _mm_slli_si128(_mm_castpd_epi64(_mm_add_pd(hi, add)), 32); // (int)hi |  trash  | (int)hi |    0

			//2.: Stitch together result
#if AVX >= 2
			return _mm_blend_epi32(lo_epi64, hi_epi64);											// (int)hi | (int)lo | (int)hi | (int)lo
#else
			return _mm_blend_epi16(lo_epi64, hi_epi64, 0b11001100);                             // (int)hi | (int)lo | (int)hi | (int)lo
#endif
		}
		//Converts two double[2](in) to int32_t[4]
		//Note: Same as _1, but with shuffle instead of shift
		ALWAYS_INLINE __m128i _mm_cvt_2pdx2_u32x4_2(__m128d lo, __m128d hi) {
			//0.: Constants
			const __m128d add = _mm_set1_pd((double)((1l << 51) + (1l << 52)));

			//1.: Round mantissa, shift into right place
			const __m128i lo_epi64 = _mm_castpd_epi64(_mm_add_pd(lo, add));								   //  trash  | (int)lo |  trash  | (int)lo
			const __m128i hi_epi64 = _mm_shuffle_epi32(_mm_castpd_epi64(_mm_add_pd(hi, add)), 0b10001101); // (int)hi |  trash  | (int)hi |    0

			//2.: Stitch together result
#if AVX >= 2
			return _mm_blend_epi32(lo_epi64, hi_epi64);													   // (int)hi | (int)lo | (int)hi | (int)lo
#else
			return _mm_blend_epi16(lo_epi64, hi_epi64, 0b11001100);										   // (int)hi | (int)lo | (int)hi | (int)lo
#endif
		}
#endif
//    +=========================================================================================================+
//    |===============================================|epi32->pd|===============================================|
//    +=========================================================================================================+
		//Converts int32_t[4](in) to two double[2]
		//Note: Hackfloat version
		//Note: in=|hi|hi|lo|lo|
		ALWAYS_INLINE void _mm_cvt_i32x4_2pdx2_1(__m128i in, __m128d& lo, __m128d& hi) {
			const __m128i exp      = _mm_sign(_mm_set1_epi32(1075ll<<52), in); //sign bit and exponent(1023+52)
			const __m128d leading1 =          _mm_set1_pd   (1ll   <<52)     ; //2^52, leading 1 is implied

			const __m128d lo_hackfloat = _mm_castepi64_pd(_mm_unpacklo_epi32(in, exp)); //Hackfloat, without implied leading one
			const __m128d hi_hackfloat = _mm_castepi64_pd(_mm_unpackhi_epi32(in, exp)); //Hackfloat, without implied leading one

			lo = _mm_sub_pd(lo_hackfloat, leading1);
			hi = _mm_sub_pd(hi_hackfloat, leading1);
		}
		//Converts int32_t[4](in) to two double[2]
		//Note: in=|hi|hi|lo|lo|
		ALWAYS_INLINE void _mm_cvt_i32x4_2pdx2_2(__m128i in, __m128d& lo, __m128d& hi) {
			lo = _mm_cvtepi32_pd(               in    );
			hi = _mm_cvtepi32_pd(_mm_srli_si128(in,64));
		}
		//Converts int32_t[4](in) to two double[2]
		//Note: in=|hi|hi|lo|lo|
		ALWAYS_INLINE void _mm_cvt_i32x4_2pdx2_3(__m128i in, __m128d& lo, __m128d& hi) {
			lo = _mm_cvtepi32_pd(                  in             );
			hi = _mm_cvtepi32_pd(_mm_shuffle_epi32(in, 0b01001110));
		}

		//Converts int32_t[4](in) to two double[2]
		//Note: Same as _mm_cvt_i32x4_2pdx2_1 but without _mm_sign
		//Note: in=|hi|hi|lo|lo|
		ALWAYS_INLINE void _mm_cvt_u32x4_2pdx2_1(__m128i in, __m128d& lo, __m128d& hi) {
			const __m128i exp      = _mm_set1_epi32(1075ll << 52); //sign bit and exponent(1023+52)
			const __m128d leading1 = _mm_set1_pd   (1ll    << 52); //2^52, leading 1 is implied

			const __m128d lo_hackfloat = _mm_castepi64_pd(_mm_unpacklo_epi32(in, exp)); //Hackfloat, without implied leading one
			const __m128d hi_hackfloat = _mm_castepi64_pd(_mm_unpackhi_epi32(in, exp)); //Hackfloat, without implied leading one

			lo = _mm_sub_pd(lo_hackfloat, leading1);
			hi = _mm_sub_pd(hi_hackfloat, leading1);
		}
	}
}