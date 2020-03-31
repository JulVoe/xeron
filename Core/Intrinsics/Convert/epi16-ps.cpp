#pragma once
#include "../intrin.cpp"

namespace intrin {
	namespace impl {
//    +=====================================================================================================+
//    |===============================================|epi32|===============================================|
//    +=====================================================================================================+
		/* There are some hackfloat possibilities (other than the onces below), but they are all slow
		   
		   Possibilities for int16_t->float using hack-floats: (all too slow)
		   1.: new=And old,0b011...1    | con=sign constant,old                            | unpack like for unsigned
		   2.: new=And old,0b011...1    | res=xor old,new         | con=xor res,constant   |             ""
		   3.: sgn=_bittestandreset old | sgnsh=shift sgn,31      | con=xor sgnsh,constant |             ""
		   
		   Possibilities for float->int16_t using hack-floats: (all too slow)
		   1.: use _mm_cvtpsx4_u32x4<SMALL> for 16 bit, transfer sign using _mm_sign_epi32, then use _mm_cvt2i32x4_i16x8.
		*/

		//Converts int16_t[8](in) to two float[4](lo, hi)
		//It first converts to two int32_t[4] and then to float
		ALWAYS_INLINE void _mm_cvt_i16x8_2psx4_1(__m128i in, __m128& lo, __m128& hi) {
			//1.: Convert to epi32
			__m128i lo32, hi32;
			_mm_cvti16x8_2i32x4(in, lo32, hi32);

			//2.: Convert to float
			lo = _mm_cvt_i32x4_psx4<16>(lo32);
			hi = _mm_cvt_i32x4_psx4<16>(hi32);
		}
		//Converts int16_t[8](in) to two float[4](lo, hi)
		//Note: Hackfloat version
		ALWAYS_INLINE void _mm_cvt_i16x8_2psx4_2(__m128i in, __m128& lo, __m128& hi) {
			//0.: Constants
			const __m128i exp      = _mm_sign_epi16(_mm_set1_epi16(0x4B00   ), in); //sign bit and exponent(23+127)
			const __m128  leading1 =                _mm_set1_ps   (8388608.f)     ; //2^23, leading 1 is implied

			//1.: Widen to 32bit
			const __m128i in_pos = _mm_sign_epi16(in, in);          //Make input positive. Can also be done with _mm_and_si128(_mm_set1_epi32(0b011...1))
			const __m128i in_lo  = _mm_unpacklo_epi16(in_pos, exp); //Hackfloat, ignores leading one                                     
			const __m128i in_hi  = _mm_unpackhi_epi16(in_pos, exp); //Hackfloat, ignores leading one

			//2.: "Convert" to float
			lo = _mm_sub_ps(_mm_castsi128_ps(in_lo), leading1); //Subtract, because leading one is implied
			hi = _mm_sub_ps(_mm_castsi128_ps(in_hi), leading1); //Subtract, because leading one is implied
		}
		//Converts two float[4](lo, hi) to int16_t[8]
		//First converts to two int32_t[4] using the specified rounding mode and then to int16_t
		template<ROUNDING_MODE round = PRECISE>
		ALWAYS_INLINE __m128i _mm_cvt_2psx4_i16x8_10(__m128 lo, __m128 hi) {
			//1.: Convert to epi32
			__m128i hi_epi32, lo_epi32;
			lo_epi32 = _mm_cvt_psx4_i32x4<16, round>(lo);
			hi_epi32 = _mm_cvt_psx4_i32x4<16, round>(hi);

			//2.: Convert to epi16
			return _mm_cvt_2i32x4_i16x8(lo_epi32, hi_epi32);
		}

//    +=====================================================================================================+
//    |===============================================|epu32|===============================================|
//    +=====================================================================================================+
		//Converts uint16_t[8](in) to two float[4](lo, hi)
		//It first converts to two uint32_t[4] and then to float
		ALWAYS_INLINE void _mm_cvt_u16x8_2psx4_10(__m128i in, __m128& lo, __m128& hi) {
			//1.: Convert to epu32
			__m128i lo32, hi32;
			_mm_cvt_u16x8_2u32x4(in, lo32, hi32);

			//2.: Convert to float
			lo = _mm_cvt_u32x4_psx4<16>(lo32);
			hi = _mm_cvt_u32x4_psx4<16>(hi32);
		}
		//Converts two float[4](lo, hi) to uint16_t[8]
		//First converts to two uint32_t[4] using the specified rounding mode and then to uint16_t
		template<ROUNDING_MODE round = PRECISE>
		ALWAYS_INLINE __m128i _mm_cvt_2psx4_u16x8_10(__m128 lo, __m128 hi) {

			//1.: Convert to epu32
			const __m128i lo_epu32 = _mm_cvt_psx4_u32x4<16, round>(lo);
			const __m128i hi_epu32 = _mm_cvt_psx4_u32x4<16, round>(hi);

			//2.: Convert to epu16
			return _mm_cvt2u32x4_u16x8(lo_epu32, hi_epu32);
		}

		//Converts uint16_t[8](in) to two float[4](lo, hi)
		//Note: It uses the pattern of _mm_cvt2u32x4_u16x8_1
		//Note: Hackfloats
		//Note: From https://stackoverflow.com/questions/9161807/sse-convert-short-integer-to-float
		ALWAYS_INLINE void _mm_cvt_u16x8_2psx4_20(__m128i in, __m128& lo, __m128& hi) {
			//0.: Prepare magic constants
			const __m128i exp      = _mm_set1_epi16(0x4B00   ); //sign bit and exponent(23+127)
			const __m128  leading1 = _mm_set1_ps   (8388608.f); //2^23, leading 1 is implied

			//1.: Widen to 32bit
			__m128i in_lo = _mm_unpacklo_epi16(in, exp); //Hackfloat, ignores leading one 
			__m128i in_hi = _mm_unpackhi_epi16(in, exp); //Hackfloat, ignores leading one 

			//2.: "Convert" to float
			lo = _mm_sub_ps(_mm_castsi128_ps(in_lo), magicFloat); //Subtract, because leading one is implied
			hi = _mm_sub_ps(_mm_castsi128_ps(in_hi), magicFloat); //Subtract, because leading one is implied
		}
		//Converts two float[4](lo, hi) to uint16_t[8]
		//As _mm_cvt_2psx4_u16x8_10 but has to use _mm_cvt2u32x4_u16x8_1, because its widening pattern was used
		template<int round = PRECISE>
		ALWAYS_INLINE __m128i _mm_cvt_2psx4_u16x8_20(__m128 lo, __m128 hi) {
			//0.: Check template parameters
			static_assert(round == FAST || round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_cvt_2psx4_u16x8_20 has to be either FAST(0), TRUNCATE(1) or PRECISE(2)!");

			//1.: Convert to epi32
			const __m128i lo_epi32 = _mm_cvt_psx4_i32x4<16, round>(lo);
			const __m128i hi_epi32 = _mm_cvt_psx4_i32x4<16, round>(hi);

			//2.: Convert to epu16
			return _mm_cvt2u32x4_u16x8_1(lo_epi32, hi_epi32);
		}

	}
}