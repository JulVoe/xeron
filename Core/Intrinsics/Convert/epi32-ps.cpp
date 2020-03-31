#pragma once
#include "../intrin.cpp"

namespace intrin {
	namespace impl {
		//    +========================================================================================================+
		//    |===============================================|epi32-ps|===============================================|
		//    +========================================================================================================+
		//Converts int32_t[4](v) to float[4]
		ALWAYS_INLINE __m128 _mm_cvt_i32x4_psx4_1(const __m128i in) {
			return _mm_cvtepi32_ps(in);
		}
		//Converts float[4](in) to int32_t[4]
		template<int round = PRECISE>
		ALWAYS_INLINE __m128i _mm_cvt_psx4_i32x4_1(const __m128 in) {
			//0.: Check template parameters
			static_assert(round == FAST || round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_cvt_psx4_i32x4_1 has to be either FAST(0), TRUNCATE(1) or PRECISE(2)!");

			//1.: Convert
			if constexpr (round == TRUNCATE)
				return _mm_cvttps_epi32(in);
			else
				return _mm_cvtps_epi32(in);
		}

		
#if 0	
		//Here are some hack-float versions, but they are all was to slow:

		//Converts float[4](in) to int32_t[4]
		//https://stackoverflow.com/questions/78619/what-is-the-fastest-way-to-convert-float-to-int-on-x86
		//Note: Slow as shit
		ALWAYS_INLINE __m128i _mm_cvt_psx4_i32x4_precise_small_1(const __m128 in) {
			//0.: Set constants
			const __m128  m1 = _mm_set1_ps((3 << 22));
			const __m128i m2 = _mm_set1_epi32((1 << 23) - 1);
			const __m128  m3 = _mm_set1_ps((1 << 22));

			//1.: Conversion to epi32
			return _mm_sub_epi32(_mm_and_si128(_mm_castps_si128(_mm_add_ps(in, m1)), m2), m3);
		}
		//Converts float[4](in) to int32_t[4]
		ALWAYS_INLINE __m128i _mm_cvt_psx4_i32x4_precise_small_2(const __m128 in) {
			//0.: Set constants
			const __m128  m1 = _mm_set1_ps((1 << 23));
			const __m128i m2 = _mm_set1_epi32((1 << 23) - 1);

			//1.: Conversion to epi32
			return  _mm_sign_epi32(_mm_and_si128(_mm_castps_si128(_mm_add_ps(in, m1)), m2), in);
		}
		//Convert int32_t[4](in) to float[4]
		ALWAYS_INLINE __m128 _mm_cvt_i32x4_psx4_precise_small_1(const __m128i in) {
			//0.: Set constants
			const __m128i exp = _mm_set1_epi32(0x4B000000); //Exponent of 23+127
			const __m128  rnd = _mm_set1_ps(8388608.0f); //2^23

			//1.: Conversion
			const __m128  flt = _mm_castsi128_ps(_mm_xor_si128(in, _mm_sign_epi32(exp, in)));
			return _mm_sub_ps(flt, rnd);
		}
#endif	

		//    +========================================================================================================+
		//    |===============================================|epu32-ps|===============================================|
		//    +========================================================================================================+

		//Convert uint32_t[4](in) to float[4]
		ALWAYS_INLINE __m128 _mm_cvt_u32x4_psx4_precise_23bit_1(const __m128i in) {
			//0.: Set constants
			const __m128i exp = _mm_set1_epi32(0x4B000000); //Exponent of 23+127
			const __m128  rnd = _mm_set1_ps(8388608.0f); //2^23

			//1.: Conversion
			const __m128  flt = _mm_castsi128_ps(_mm_xor_si128(in, _exp));
			return _mm_sub_ps(flt, rnd);
		}

		//Converts uint32_t[4](in) to float[4]
		//Cuts of last binary digit + 0.75ULP
		ALWAYS_INLINE _m128 _mm_cvt_u32x4_psx4_fast_32bit_1(__m128i in) {
			const __m128 half = _mm_cvtepi32_ps(_mm_srl_epi32(in, 1));
			return _mm_add_ps(half, half);
		}
		//Converts uint32_t[4](in) to float[4]
		//0.5ULP
		//https://stackoverflow.com/questions/34066228/how-to-perform-uint32-float-conversion-with-sse
		ALWAYS_INLINE _m128 _mm_cvt_u32x4_psx4_precise_32bit_1(__m128i in) {
			//0.: Constants
			const __m128i msk_lo = _mm_set1_epi32(0xFFFF);
			const __m128  cnst65536f = _mm_set1_ps(65536.0f);

			//1.: Convert
			const __m128i v_lo = _mm_and_si128(in, msk_lo);
			const __m128i v_hi = _mm_srli_epi32(in, 16);
			const __m128  v_lo_flt = _mm_cvtepi32_ps(v_lo);
			__m128  v_hi_flt = _mm_cvtepi32_ps(v_hi);
			v_hi_flt = _mm_mul_ps(cnst65536f, v_hi_flt);
			return _mm_add_ps(v_hi_flt, v_lo_flt);
		}
		//Converts uint32_t[4](v) to float[4]
		//0.75ULP
		//https://stackoverflow.com/questions/34066228/how-to-perform-uint32-float-conversion-with-sse
		ALWAYS_INLINE __m128 _mm_cvt_u32x4_psx4_truncate_l32bit_1(const __m128i v)
		{
			const __m128i v2 = _mm_srli_epi32(v, 1);     // v2 = v / 2
			const __m128i v1 = _mm_sub_epi32(v, v2);     // v1 = v - (v / 2)
			const __m128 v2f = _mm_cvtepi32_ps(v2);
			const __m128 v1f = _mm_cvtepi32_ps(v1);
			return _mm_add_ps(v2f, v1f);
		}
		//Converts uint32_t[4](v) to float[4]
		//0.75ULP
		//https://stackoverflow.com/questions/34066228/how-to-perform-uint32-float-conversion-with-sse
		ALWAYS_INLINE __m128 _mm_cvt_u32x4_psx4_truncate_32bit_1(const __m128i v)
		{
			const __m128i v2 = _mm_srli_epi32(v, 1);                 // v2 = v / 2
			const __m128i v1 = _mm_and_si128(v, _mm_set1_epi32(1));  // v1 = v & 1
			const __m128 v2f = _mm_cvtepi32_ps(v2);
			const __m128 v1f = _mm_cvtepi32_ps(v1);
			return _mm_add_ps(_mm_add_ps(v2f, v2f), v1f);      // return 2 * v2 + v1
		}
		//Converts uint32_t[4](v) to float[4]
		//0.75ULP
		//https://stackoverflow.com/questions/34066228/how-to-perform-uint32-float-conversion-with-sse
		ALWAYS_INLINE __m128 _mm_cvt_u32x4_psx4_truncate_32bit_2(const __m128i v)
		{
			//0.: Constants
			const __m128i msk0 = _mm_set1_epi32(0x7FFFFFFF);
			const __m128i cnst2_31 = _mm_set1_epi32(0x4F000000);

			//1.: Convert
			const __m128i msk1 = _mm_srai_epi32(v, 31);
			const __m128i v_low = _mm_and_si128(msk0, v);
			const __m128  v_lowf = _mm_cvtepi32_ps(v_low);
			const __m128  v_highf = _mm_castsi128_ps(_mm_and_si128(msk1, cnst2_31));
			const __m128  v_sum = _mm_add_ps(v_lowf, v_highf);
			return v_sum;
		}

		template<int round = TRUNCATE, int size = HUGE>
		ALWAYS_INLINE __m128 _mm_cvt_u32x4_psx4(__m128i in) {
			static_assert(round == FAST || round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_cvt_u32x4_psx4 has to be either FAST(0), TRUNCATE(1) or PRECISE(2)!");
			static_assert(size == SMALL || size == MEDIUM || size == BIG || size == HUGE, "_mm_cvt_u32x4_psx4 only supports size SMALL(0), MEDIUM(1), BIG(2) or HUGE(3)!");
#ifdef AVX512		
			return _mm_cvtepu32_ps(in);
#else
			UNREACHABLE();
#endif		
		}
#ifndef AVX512
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<FAST, 23 >(in) { return _mm_cvt_u32x4_psx4_precise_small(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<FAST, 32>(in) { return _mm_cvt_u32x4_psx4_fast_huge(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<FAST, BIG   >(in) { return _mm_cvt_u32x4_psx4_fast_huge(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<FAST, HUGE  >(in) { return _mm_cvt_u32x4_psx4_fast_huge(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<TRUNCATE, SMALL >(in) { return _mm_cvt_u32x4_psx4_precise_small(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<TRUNCATE, MEDIUM>(in) { return _mm_cvt_u32x4_psx4_truncate_big(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<TRUNCATE, BIG   >(in) { return _mm_cvt_u32x4_psx4_truncate_big(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<TRUNCATE, HUGE  >(in) { return _mm_cvt_u32x4_psx4_truncate_huge(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<PRECISE, SMALL >(in) { return _mm_cvt_u32x4_psx4_precise_small(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<PRECISE, MEDIUM>(in) { return _mm_cvt_u32x4_psx4_precise_huge(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<PRECISE, BIG   >(in) { return _mm_cvt_u32x4_psx4_precise_huge(in); }
		ALWAYS_INLINE template<> __m128 _mm_cvt_u32x4_psx4<PRECISE, HUGE  >(in) { return _mm_cvt_u32x4_psx4_precise_huge(in); }
#endif

		//Converts float[4](in) to uint32_t[4]
			//http://stereopsis.com/FPU.html
			//https://stackoverflow.com/questions/78619/what-is-the-fastest-way-to-convert-float-to-int-on-x86	
		ALWAYS_INLINE __m128i _mm_cvt_psx4_u32x4_precise_small_1(__m128 in) {
			//0.: Constants
			const __m128  m1 = _mm_set1_ps((1 << 23));
			const __m128i m2 = _mm_set1_epi32((1 << 23) - 1);

			//1.: Convert
			const __m128 in_floored = _mm_add_ps(a, m1);
			return _mm_and_si128(_mm_castps_si128(in_floored), m2);
		}
		//Converts float[4](in) to uint32_t[4]
		template<int round = FAST>
		ALWAYS_INLINE __m128i _mm_cvt_psx4_u32x4_medium_1(__m128 in) {
			//0.: Check template parameters
			static_assert(round == FAST || round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_cvt_psx4_u32x4_medium has to be either FAST(0), TRUNCATE(1) or PRECISE(2)!");

			//1.: Convert
			if constexpr (round == TRUNCATE)
				return _mm_cvtps_epi32(in);
			else
				return _mm_cvttps_epi32(in);
		}
		//Converts float[4](in) to uint32_t[4]
		ALWAYS_INLINE __m128i _mm_cvt_psx4_u32x4_fast_huge(__m128 in) {
			//0.: Set up constants
			const __m128i div2 = _mm_set1_epi32(1 << 23); //Subtract one from exponent of float

			__m128 in_half = _mm_castsi128_ps(_mm_sub_epi32(_mm_castps_si128(in), div2));
			__m128i in_half_epu32 = _mm_cvt_psx4_u32x4_medium<FAST>(in_half);
			return _mm_slli_epi32(in_half_epu32, 1);
		}
		//Converts float[4](in) to uint32_t[4]
		template<int round>
		ALWAYS_INLINE __m128i _mm_cvt_psx4_u32x4_huge(__m128 in) {
			//0.: Check template parameters
			static_assert(round == FAST || round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_cvt_psx4_u32x4_precise_huge has to be either FAST(0), TRUNCATE(1) or PRECISE(2)!");

			//1.: Set up constants
			const __m128  sub_f = _mm_set1_ps((float)(1 << 31));
			const __m128i add_i = _mm_set1_epi32(1 << 31);

			//2.: Convert
			const __m128  in_float_i32 = _mm_sub_ps(in, sub_f);
			const __m128i in_i32 = _mm_cvt_psx4_u32x4_medium<round>(in_float_i32);
			return _mm_add_epi32(in_i32, add_i);
		}

		template<int precision = PRECISE, int size = HUGE>
		ALWAS_INLINE _mm_cvt_psx4_u32x4(__m128 in) {
			//0.: Check template parameters
			static_assert(precision == FAST || precision == PRECISE || precision == TRUNCATE, "The rounding-mode for _mm_cvt_psx4_u32x4 has to be either FAST(0), TRUNCATE(1) or PRECISE(2)!");
			static_assert(size == SMALL || size == MEDIUM || size == BIG || size == HUGE, "_mm_cvt_psx4_u32x4 only supports size SMALL(0), MEDIUM(1), BIG(2) or HUGE(3)!");
#ifdef AVX512		
			return _mm_cvtps_epu32(in);
#else
			UNREACHABLE();
#endif		
		}
#ifndef AVX512
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<FAST, SMALL >(in) { return _mm_cvt_psx4_u32x4_precise_small(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<FAST, MEDIUM>(in) { return _mm_cvt_psx4_u32x4_medium(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<FAST, BIG   >(in) { return _mm_cvt_psx4_u32x4_fast_huge(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<FAST, HUGE  >(in) { return _mm_cvt_psx4_u32x4_fast_huge(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<TRUNCATE, SMALL >(in) { return _mm_cvt_psx4_u32x4_precise_small(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<TRUNCATE, MEDIUM>(in) { return _mm_cvt_psx4_u32x4_medium(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<TRUNCATE, BIG   >(in) { return _mm_cvt_psx4_u32x4_huge(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<TRUNCATE, HUGE  >(in) { return _mm_cvt_psx4_u32x4_huge(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<PRECISE, SMALL >(in) { return _mm_cvt_psx4_u32x4_precise_small(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<PRECISE, MEDIUM>(in) { return _mm_cvt_psx4_u32x4_medium(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<PRECISE, BIG   >(in) { return _mm_cvt_psx4_u32x4_huge(in); }
		template<> ALWAYS_INLINE _mm_cvt_psx4_u32x4<PRECISE, HUGE  >(in) { return _mm_cvt_psx4_u32x4_huge(in); }
#endif
	}

}