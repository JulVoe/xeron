#include "../SystemInfo/Makros.hpp"

namespace intrin {

/*******************************
 *            Enums            *
 *******************************/
	
//Integer division. Don't mess with the values or error messages will be wrong
enum { FAST = 0, TRUNCATE = 1, PRECISE = 2 }; //PRECISE=highest precision possible, FAST=not as precise, TRUNCATE=same behaviour as sisd integer division.
enum { SMALL = 0, BIG = 1 }; //BIG=input can be as big as possible, SMALL=input has to be <=2^24
	
namespace impl {
	//Converts int16_t[8] (in) to two int32_t[4] (out1, out2)
	ALWAYS_INLINE void _mm_widen_epi16_10(const __m128i in, __m128i& out1, __m128i& out2) {
		const __m128i sign = _mm_srai_epi16(in, 16);
		out1 = _mm_unpackhi_epi16(in, sign);
		out2 = _mm_unpacklo_epi16(in, sign);
	}
	//Converts uint16_t[8] (in) to two uint32_t[4] (out1, out2)
	ALWAYS_INLINE void _mm_widen_epu16_10(const __m128i in, __m128i& out1, __m128i& out2) {
		out1 = _mm_unpackhi_epi16(in, _mm_setzero_si128());
		out2 = _mm_unpacklo_epi16(in, _mm_setzero_si128());
	}
	//Converts two int32_t[4] (in_lo, in_hi) to int16_t[8] using saturation
	ALWAYS_INLINE __m128i _mm_compress_epi32_10(const __m128i in_lo, const __m128i in_hi) {
		return _mm_packs_epi32(in_lo, in_hi);
	}
	//Converts two int32_t[4] (in_lo, in_hi) to uint16_t[8] using saturation
	//Note: also converts 2xuint32_t[4] to uint16_t[8] correct, if highest bit of elements isn't set
	//	If that happens, output will be zero and not uint16_t::max()
	ALWAYS_INLINE __m128i _mm_compress_epu32_10(const __m128i in_lo, const __m128i in_hi) {
		return _mm_packus_epi32(in_lo, in_hi);
	}
	
	//Converts int16_t[8] (in) to two int32_t[4] (out1, out2) 
	ALWAYS_INLINE void _mm_widen_epi16_20(const __m128i in, __m128i& out1, __m128i& out2) {
		out1 = _mm_srai_epi32(in, 16);
		const __m128i out2_tmp = _mm_slli_epi32(in, 16);
		out2 = _mm_srai_epi32(out2_tmp, 16);
	}
	//Converts uint16_t[8] (in) to two uint32_t[4] (out1, out2)
	ALWAYS_INLINE void _mm_widen_epu16_20(const __m128i in, __m128i& out1, __m128i& out2) {
		out1 = _mm_srli_epi32(in, 16);
		out2 = _mm_and_si128(in, _mm_set1_epi32(0xFF));
	}
	//Converts two [u]int32_t[4] (in_lo, in_hi) to [u]int16_t[8] using truncation.
	ALWAYS_INLINE __m128i _mm_compress_epi32_20(const __m128i in_lo, const __m128i in_hi) {
		const __m128i hi_epi32_shift = _mm_slli_epi32(in_hi, 16);
#if SSE >= 41
		return _mm_blend_epi16(in_lo, hi_epi32_shift, 0xAA);
#else
		const __m128i lo_mask = _mm_set1_epi32(0xFFFF);
		const __m128i lo_epi32_mask = _mm_and_si128(in_lo, lo_mask);
		return _mm_or_si128(hi_epi32_shift, lo_epi32_mask);
#endif
	}
	//Converts two int32_t[4] (in_lo, in_hi) to int16_t[8]
	//If SSE4.1 is available, truncation is used, otherwiese saturation. 
	ALWAYS_INLINE __m128i _mm_compress_epi32_21(const __m128i in_lo, const __m128i in_hi) {
#if SSE >= 41
		const __m128i hi_epi32_shift = _mm_slli_epi32(in_hi, 16);
		return _mm_blend_epi16(in_lo, hi_epi32_shift, 0xAA);
#else
	    return _mm_shuffle_epi8(_mm_compress_epi32_10(in_lo,in_hi),_mm_setr_epi8(0,1,8,9,2,3,10,11,4,5,12,13,6,7,14,15));
#endif
	}
	//Converts two uint32_t[4] (in_lo, in_hi) to uint16_t[8]
	//If SSE4.1 is available, truncation is used, otherwiese saturation.
	//Note: If SSE4.1 is not available, no highest bit of the input should be set or the output will be 0 (see _mm_compress_epu32_10)
	ALWAYS_INLINE __m128i _mm_compress_epi32_21(const __m128i in_lo, const __m128i in_hi) {
#if SSE >= 41
		const __m128i hi_epi32_shift = _mm_slli_epi32(in_hi, 16);
		return _mm_blend_epi16(in_lo, hi_epi32_shift, 0xAA);
#else
	    return _mm_shuffle_epi8(_mm_compress_epu32_10(in_lo,in_hi),_mm_setr_epi8(0,1,8,9,2,3,10,11,4,5,12,13,6,7,14,15));
#endif
	}
	
	//Converts int32_t[8] to int16_t[8] using saturation
	ALWAYS_INLINE __m128i _mm256_compress_epi32(__m256i in){
		_mm_compress_epi32_10(_mm256_extractf128_si256(in, 0), _mm256_extractf128_si256(in, 1))
	}
	//Converts uint32_t[8] (in) to uint16_t[8] using saturation
	//Note: If the highest bit of on element of in is set, the corresponding returned element will be 0
	//	(see _mm_compress_epi32_10)
	ALWAYS_INLINE __m128i _mm256_compress_epu32(__m256i in){
		_mm_compress_epi32_10(_mm256_extractf128_si256(in, 0), _mm256_extractf128_si256(in, 1))
	}


	//https://stackoverflow.com/questions/31555260/fast-vectorized-rsqrt-and-reciprocal-with-sse-avx-depending-on-precision
	//Approximates the inverse of float[4] (in) using intrinsic and n passes of newton iterations
	template<unsigned n>
	ALWAYS_INLINE __m128 _mm_rcp_ps(const __m128 in) {
		const __m128  two = _mm_set1_ps(2.00000051757f);
		__m128 ret = _mm_rcp_ps(in);

		for (unsigned i = 0; i < n; i++) {
#if FMA
			const __m128 ret_mult = _mm_fnmadd_ps(in, ret, two);
#else
			const __m128i lo_mask = _mm_set1_epi32(0xFFFF);
			const __m128 ret_mult_tmp = _mm_mul_ps(in, ret);
			const __m128 ret_mult = _mm_sub_ps(two, ret_mult_tmp);
#endif
			ret = _mm_mul_ps(ret, ret_mult);
		}
		return ret;
	}
	//Approximates the inverse of float[8] (in) using intrinsic and n passes of newton iterations
	template<unsigned n>
	ALWAYS_INLINE __m256 _mm256_rcp_ps(const __m256 in) {
		const __m256  two = _mm256_set1_ps(2.0f);
		__m256 ret = _mm256_rcp_ps(in);

		for (unsigned i = 0; i < n; i++) {
#if FMA
			const __m256 ret_mult = _mm256_fnmadd_ps(in, ret, two);
#else
			const __m256 ret_mult_tmp = _mm256_mul_ps(in, ret);
			const __m256 ret_mult = _mm256_sub_ps(two, ret_mult_tmp);
#endif
			ret = _mm256_mul_ps(ret, ret_mult);
		}
		return ret;
	}
//--------------------------------------_mm_div_ep[i,u]32--------------------------------------//
	/* https://stackoverflow.com/questions/50597764/convert-signed-short-to-float-in-c-simd
	 * https://stackoverflow.com/questions/9161807/sse-convert-short-integer-to-float
	 * https://stackoverflow.com/questions/41228180/how-can-i-convert-a-vector-of-float-to-short-int-using-avx-instructions
	 * https://stackoverflow.com/questions/16822757/sse-integer-division
	*/
	//Divides a_epi16 by b_epi16 elementwise
	//Note: Correctness has to be verified
	ALWAYS_INLINE __m128i _mm_div_epi16_rcp(const __m128i &a_epi16, const __m128i &b_epi16) {
#if AVX >= 2
		//1.: Convert to epi32
		const __m256i a = _mm256_cvtepi16_epi32(a_epi16);
		const __m256i b = _mm256_cvtepi16_epi32(b_epi16);

		//2.: Convert to float
		const __m256 a_float = _mm256_cvtepi32_ps(a);
		const __m256 b_float = _mm256_cvtepi32_ps(b);

		//3.: Calculate reciprocal             (b_rcp = 1/b_float)
		const __m256 b_rcp = _mm256_rcp_ps<1>(b_float);

		//4.: Divide
		const __m256 c_float = _mm256_mul_ps(a_float, b_rcp);

		//5.: Convert back to epi32
		const __m256i c_epi32 = _mm256_cvttps_epi32(c_float);

		//6.: Convert back to epi16
		return _mm256_compress_epi32(c_epi32);

#else		
		//1.: Convert to epi32
		__m128i a_hi_epi32;
		__m128i a_lo_epi32;
		__m128i b_hi_epi32;
		__m128i b_lo_epi32;
		_mm_widen_epi16(a_epi16, a_hi_epi32, a_lo_epi32);
		_mm_widen_epi16(b_epi16, b_hi_epi32, b_lo_epi32);

		//2.: Convert to float
		const __m128 a_hi = _mm_cvtepi32_ps(a_hi_epi32);
		const __m128 a_lo = _mm_cvtepi32_ps(a_lo_epi32);
		const __m128 b_hi = _mm_cvtepi32_ps(b_hi_epi32);
		const __m128 b_lo = _mm_cvtepi32_ps(b_lo_epi32);

		//3.: Calculate reciprocal
		const __m128 b_hi_rcp = _mm_rcp_ps<1>(b_hi);
		const __m128 b_lo_rcp = _mm_rcp_ps<1>(b_lo);

		//4.: Divide
		const __m128 hi = _mm_mul_ps(a_hi, b_hi_rcp);
		const __m128 lo = _mm_mul_ps(a_lo, b_lo_rcp);

		//5.: Convert back to epi32
		const __m128i hi_epi32 = _mm_cvttps_epi32(hi);
		const __m128i lo_epi32 = _mm_cvttps_epi32(lo);

		//6.: Convert back to epi16
		return _mm_compress_epi32(lo_epi32, hi_epi32);
#endif
	}
	//Divides a_epi16 by b_epi16 elementwise
	//Note: Result will always be correct
	ALWAYS_INLINE __m128i _mm_div_epi16_div(const __m128i &a_epi16, const __m128i &b_epi16) {
#if AVX >= 2
		//1.: Convert to epi32
		const __m256i a = _mm256_cvtepi16_epi32(a_epi16);
		const __m256i b = _mm256_cvtepi16_epi32(b_epi16);

		//2.: Convert to float
		const __m256 a_float = _mm256_cvtepi32_ps(a);
		const __m256 b_float = _mm256_cvtepi32_ps(b);

		//3.: Divide
		const __m256 c_float = _mm256_div_ps(a_float, b_float);

		//4.: Convert back to epi32
		const __m256i c_epi32 = _mm256_cvttps_epi32(c_float);

		//5.: Convert back to epi16
		return _mm256_compress_epi32(c_epi32);
#else		
		//1.: Convert to epi32
		__m128i a_hi_epi32;
		__m128i a_lo_epi32;
		__m128i b_hi_epi32;
		__m128i b_lo_epi32;
		_mm_widen_epi16(a_epi16, a_hi_epi32, a_lo_epi32);
		_mm_widen_epi16(b_epi16, b_hi_epi32, b_lo_epi32);

		//2.: Convert to float
		const __m128 a_hi = _mm_cvtepi32_ps(a_hi_epi32);
		const __m128 a_lo = _mm_cvtepi32_ps(a_lo_epi32);
		const __m128 b_hi = _mm_cvtepi32_ps(b_hi_epi32);
		const __m128 b_lo = _mm_cvtepi32_ps(b_lo_epi32);

		//3.: Divide
		const __m128 hi = _mm_div_ps(a_hi, b_hi);
		const __m128 lo = _mm_div_ps(a_lo, b_lo);

		//4.: Convert back to epi32
		const __m128i hi_epi32 = _mm_cvttps_epi32(hi);
		const __m128i lo_epi32 = _mm_cvttps_epi32(lo);

		//5.: Convert back to epi16
		return _mm_compress_epi32(lo_epi32, hi_epi32);
#endif
	}
	//Divides a_epu16 by b_epu16 elementwise
	//Note: Correctness has to be verified
	ALWAYS_INLINE __m128i _mm_div_epu16_rcp(const __m128i &a_epu16, const __m128i &b_epu16) { 
#if AVX >= 2
		//1.: Convert to epi32
		const __m256i a = _mm256_cvtepu16_epi32(a_epu16);
		const __m256i b = _mm256_cvtepu16_epi32(b_epu16);

		//2.: Convert to float
		const __m256 a_float = _mm256_cvtepi32_ps(a);
		const __m256 b_float = _mm256_cvtepi32_ps(b);

		//3.: Calculate reciprocal             (b_rcp = 1/b_float)
		const __m256 b_rcp = _mm256_rcp_ps<1>(b_float);

		//4.: Divide
		const __m256 c_float = _mm256_mul_ps(a_float, b_rcp);

		//5.: Convert back to epi32
		const __m256i c_epi32 = _mm256_cvttps_epi32(c_float);

		//6.: Convert back to epu16
		return _mm256_compress_epu32(c_epi32);

#else		
		//1.: Convert to epi32
		__m128i a_hi_epi32;
		__m128i a_lo_epi32;
		__m128i b_hi_epi32;
		__m128i b_lo_epi32;
		_mm_widen_epu16(a_epu16, a_hi_epi32, a_lo_epi32);
		_mm_widen_epu16(b_epu16, b_hi_epi32, b_lo_epi32);

		//2.: Convert to float
		const __m128 a_hi = _mm_cvtepi32_ps(a_hi_epi32);
		const __m128 a_lo = _mm_cvtepi32_ps(a_lo_epi32);
		const __m128 b_hi = _mm_cvtepi32_ps(b_hi_epi32);
		const __m128 b_lo = _mm_cvtepi32_ps(b_lo_epi32);

		//3.: Calculate reciprocal
		const __m128 b_hi_rcp = _mm_rcp_ps<1>(b_hi);
		const __m128 b_lo_rcp = _mm_rcp_ps<1>(b_lo);

		//4.: Divide
		const __m128 hi = _mm_mul_ps(a_hi, b_hi_rcp);
		const __m128 lo = _mm_mul_ps(a_lo, b_lo_rcp);

		//5.: Convert back to epi32
		const __m128i hi_epi32 = _mm_cvttps_epi32(hi);
		const __m128i lo_epi32 = _mm_cvttps_epi32(lo);

		//6.: Convert back to epu16
		return _mm_compress_epu32(lo_epi32, hi_epi32);
#endif
	}
	//Divides a_epu16 by b_epu16 elementwise
	//Note: Will always be correct
	ALWAYS_INLINE __m128i _mm_div_epu16_div(const __m128i &a_epu16, const __m128i &b_epu16) {
#if AVX >= 2
		//1.: Convert to epi32
		const __m256i a = _mm256_cvtepu16_epi32(a_epu16);
		const __m256i b = _mm256_cvtepu16_epi32(b_epu16);

		//2.: Convert to float
		const __m256 a_float = _mm256_cvtepi32_ps(a);
		const __m256 b_float = _mm256_cvtepi32_ps(b);

		//3.: Divide
		const __m256 c_float = _mm256_div_ps(a_float, b_float);

		//4.: Convert back to epi32
		const __m256i c_epi32 = _mm256_cvttps_epi32(c_float);

		//5.: Convert back to epu16
		return _mm256_compress_epu32(c_epi32);

#else		
		//1.: Convert to epi32
		__m128i a_hi_epi32;
		__m128i a_lo_epi32;
		__m128i b_hi_epi32;
		__m128i b_lo_epi32;
		_mm_widen_epu16(a_epu16, a_hi_epi32, a_lo_epi32);
		_mm_widen_epu16(b_epu16, b_hi_epi32, b_lo_epi32);

		//2.: Convert to float
		const __m128 a_hi = _mm_cvtepi32_ps(a_hi_epi32);
		const __m128 a_lo = _mm_cvtepi32_ps(a_lo_epi32);
		const __m128 b_hi = _mm_cvtepi32_ps(b_hi_epi32);
		const __m128 b_lo = _mm_cvtepi32_ps(b_lo_epi32);

		//3.: Divide
		const __m128 hi = _mm_div_ps(a_hi, b_hi);
		const __m128 lo = _mm_div_ps(a_lo, b_lo);

		//4.: Convert back to epi32
		const __m128i hi_epi32 = _mm_cvttps_epi32(hi);
		const __m128i lo_epi32 = _mm_cvttps_epi32(lo);

		//5.: Convert back to epu16
		return _mm_compress_epu32(lo_epi32, hi_epi32);
#endif
	}
	
//--------------------------------------_mm_div_epi32--------------------------------------//
	//Divides a by b elementwise.
	//_mm_idiv_epi32_split with the extra assumption that a<=2^24. Very fast. Precise für a<=2^24. From there on inprecise for small b.
	template<int round, typename T=__m128i>
	ALWAYS_INLINE __m128i _mm_idiv_epi32_small(__m128i a, __m128i b) {
		//Are the template parameters valid?
		static_assert(round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_idiv_epi32_small has to be either TRUNCATE(1) or PRECISE(2)!");
		static_assert(typeid(T) == typeid(__m128i) || typeid(T) == typeid(__m128), "_mm_idiv_epi32_small can only return either __m128i or __m128!");

		//Computation
		const __m128 fa = _mm_cvtepi32_ps(a);
		const __m128 fb = _mm_cvtepi32_ps(b);
		const __m128 fr = _mm_div_ps(fa, fb);

		//Return in the right form
		if constexpr (typeid(T) == typeid(__m128i)) {
			if constexpr (round == PRECISE)
				return _mm_cvtps_epi32(fr);
			else if constexpr (round == TRUNCATE)
				return _mm_cvttps_epi32(fr);
		}
		else if constexpr (typeid(T) == typeid(__m128)) {
			return fr;
		}
		else {
			UNREACHABLE();
		}
	}
	//Divides a by b elementwise.
	//Very fast. Precise für a<=2^24. From there on inprecise for small b. Uses _mm_rcp_ps with multiplication instead of division
	template<int round, typename T = __m128i>
	ALWAYS_INLINE __m128i _mm_idiv_epi32_small_rcp(__m128i a, __m128i b) {
		//Are the template parameters valid?
		static_assert(round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_idiv_epi32_small_rcp has to be either TRUNCATE(1) or PRECISE(2)!");
		static_assert(typeid(T) == typeid(__m128i) || typeid(T) == typeid(__m128), "_mm_idiv_epi32_small_rcp can only return either __m128i or __m128!");

		//Computation
		const __m128 fa = _mm_cvtepi32_ps(a);
		const __m128 fb = _mm_cvtepi32_ps(b);

		const __m128 rcp = _mm_rcp_ps<0>(fb);
		const __m128 fr = _mm_mul_ps(fa, rcp);

		//Return in the right form
		if constexpr (typeid(T) == typeid(__m128i)) {
			if constexpr (round == PRECISE)
				return _mm_cvtps_epi32(fr);
			else if constexpr (round == TRUNCATE)
				return _mm_cvttps_epi32(fr);
		}
		else if constexpr (typeid(T) == typeid(__m128)) {
			return fr;
		}
		else {
			UNREACHABLE();
		}
	}
	//Divides a by b elementwise. It doesn't matter wether a nd b are signed or unsigned.
	//Converts a and b to 256-double vectors and divides them. Very precise.
	template<int round, typename T = __m128i>
	ALWAYS_INLINE __m128i _mm_idiv_epi32_avx(__m128i a, __m128i b) {
		//Are the template parameters valid?
		static_assert(round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_idiv_epi32_avx has to be either TRUNCATE(1) or PRECISE(2)!");
		static_assert(typeid(T) == typeid(__m128i) || typeid(T) == typeid(__m128), "_mm_idiv_epi32_avx can only return either __m128i or __m128!");

		//Computation
		const __m256d da = _mm256_cvtepi32_pd(a);
		const __m256d db = _mm256_cvtepi32_pd(b);
		const __m256d dr = _mm256_div_pd(da, db);

		//Return in the right form
		if constexpr (typeid(T) == typeid(__m128i)) {
			if constexpr (round == PRECISE)
				return _mm256_cvtpd_epi32(dr);
			else if constexpr (round == TRUNCATE)
				return _mm256_cvttpd_epi32(dr);
		}
		else if constexpr (typeid(T) == typeid(__m128)) {
			return _mm256_cvtpd_ps(dr);
		}
		else {
			UNREACHABLE();
		}
	}
	//Divides a by b elementwise.
	//Splits a up into two parts which will fit into a float without loss of accuracy. Divides them and add the result
	template<int round, typename T = __m128i>
	ALWAYS_INLINE __m128i _mm_idiv_epi32_split(__m128i a, __m128i b) {
		//Are the template parameters valid?
		static_assert(round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_idiv_epi32_split has to be either TRUNCATE(1) or PRECISE(2)!");
		static_assert(typeid(T) == typeid(__m128i) || typeid(T) == typeid(__m128), "_mm_idiv_epi32_split can only return either __m128i or __m128!");

		//Computation
		const __m128 ha = _mm_cvtepi32_ps(_mm_srli_epi32(a, 24));//High 8 bits of a
		const __m128 la = _mm_cvtepi32_ps(_mm_and_si128(a, _mm_set1_epi32((1 << 24) - 1)));//Lower 24 bits of a, will fit in mantissa of float
		const __m128 fb = _mm_cvtepi32_ps(b);//If has more than 24 bits, some error will occur(at most 2^-23+2^-24). This is ok, because if b is that huge, the answer is at most 2^8 and the error won't show

		const __m128 hr = _mm_mul_ps(_mm_div_ps(ha, fb), _mm_set1_ps((float)(1 << 24)));//Divide and shift back up(upper 8 bits, shift left by 24)
		const __m128 fr = _mm_add_ps(_mm_div_ps(la, fb), hr);//fr=a/b=(2^24*ha+la)/b=2^24*(ha/b)+la/b

		//Return in the right form
		if constexpr (typeid(T) == typeid(__m128i)) {
			if constexpr (round == PRECISE)
				return _mm_cvtps_epi32(fr);
			else if constexpr (round == TRUNCATE)
				return _mm_cvttps_epi32(fr);
		}
		else if constexpr (typeid(T) == typeid(__m128)) {
			return fr;
		}
		else {
			UNREACHABLE();
		}
	}

	//Vector-equivalent of "return (T)a/(T)b;"
	template<int precision = PRECISE, int size = BIG, typename T = __m128i>
	ALWAYS_INLINE T _mm_idiv_epi32(__m128i a, __m128i b) {
		static_assert(precision == PRECISE || precision == FAST || precision == TRUNCATE, "_mm_idiv_epi32 only supports precision FAST(0), TRUNCATE(1) and PRECISE(2)!");
		static_assert(size == BIG || size == SMALL, "_mm_idiv_epi32 unterstützt nur size 0 oder 1!");
		static_assert(typeid(T) == typeid(__m128i) || typeid(T) == typeid(__m128), "_mm_idiv_epi32 can only return either __m128i or __m128!");
		UNREACHABLE();
	}
	ALWAYS_INLINE template<> __m128i _mm_idiv_epi32<PRECISE,  BIG  , __m128i>(__m128i a, __m128i b) { return _mm_idiv_epi32_precise_int  <PRECISE,  __m128i>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epi32<PRECISE,  SMALL, __m128i>(__m128i a, __m128i b) { return _mm_idiv_epi32_small        <PRECISE,  __m128i>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epi32<FAST,     BIG  , __m128i>(__m128i a, __m128i b) { return _mm_idiv_epi32_small_rcp    <PRECISE,  __m128i>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epi32<FAST,     SMALL, __m128i>(__m128i a, __m128i b) { return _mm_idiv_epi32_small_rcp    <PRECISE,  __m128i>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epi32<TRUNCATE, BIG  , __m128i>(__m128i a, __m128i b) { return _mm_idiv_epi32_precise_int  <TRUNCATE, __m128i>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epi32<TRUNCATE, SMALL, __m128i>(__m128i a, __m128i b) { return _mm_idiv_epi32_small        <TRUNCATE, __m128i>(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epi32<PRECISE,  BIG,   __m128 >(__m128i a, __m128i b) { return _mm_idiv_epi32_precise_float<PRECISE,  __m128 >(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epi32<PRECISE,  SMALL, __m128 >(__m128i a, __m128i b) { return _mm_idiv_epi32_small        <PRECISE,  __m128 >(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epi32<FAST,     BIG,   __m128 >(__m128i a, __m128i b) { return _mm_idiv_epi32_small_rcp    <PRECISE,  __m128 >(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epi32<FAST,     SMALL, __m128 >(__m128i a, __m128i b) { return _mm_idiv_epi32_small_rcp    <PRECISE,  __m128 >(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epi32<TRUNCATE, BIG,   __m128 >(__m128i a, __m128i b) { return _mm_idiv_epi32_precise_float<TRUNCATE, __m128 >(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epi32<TRUNCATE, SMALL, __m128 >(__m128i a, __m128i b) { return _mm_idiv_epi32_small        <TRUNCATE, __m128 >(a, b); }
	
	
	//https://stackoverflow.com/questions/9161807/sse-convert-short-integer-to-float
}
namespace detail {
/*
 * Types which will be introduced:
 *	- vec_[u]int16x[8,16]
 *	- vec_[u]int32x[4,8]
 *	- vec_[u]int64x[2,4]
 *	- vec_floatx[4,8]
 *	- vec_doublex[2,4]

 * And the fast-division types:
 *	- vec_single_div_*
 *	- vec_vector_div_*
*/
#ifdef SSE
class vec_int16x8 {// int16_t[8]
private:
	__m128i vec;
public:
	//Constructors
	vec_int16x8(const int16_t* p, constexpr bool align = false) { load<true>(p,align); }
	vec_int16x8(const __m128i v) { load(v); }
	vec_int16x8(const int16_t i) { load(i); }
	vec_int16x8(const int16_t i1, const int16_t i2, const int16_t i3, const int16_t i4, const int16_t i5, const int16_t i6, const int16_t i7, const int16_t i8) { load(i1,i2,i3,i4,i5,i6,i7,i8); }

	//Misc
	inline __m128i getVec() { return vec; }
	template<bool temporal>
	inline void load(const int16_t* p, constexpr bool align = false) {
		if constexpr (align)
			vec = _mm_load_si128((__m128i*)p);
		else
			vec = _mm_lddqu_si128((__m128i*)p);
	}
	template<> inline void load<false>(const int16_t* p, constexpr bool align = true) {
		static_assert(align,"Non temporal loads must be aligned!");
		vec = _mm_stream_load_si128((__m128i*)p);
	}
	inline void load(const __m128i v) { vec = v; }
	inline void load(const int16_t i) { vec = _mm_set1_epi16(i); }
	inline void load(const int16_t i1, const int16_t i2, const int16_t i3, const int16_t i4, const int16_t i5, const int16_t i6, const int16_t i7, const int16_t i8) { 
		vec = _mm_set_epi16(i1, i2, i3, i4, i5, i6, i7, i8); 
	}
	template<bool aligned, bool temporal> inline void store(const int16_t* p) { static_assert(aligned||temporal,"Non temporal stores must be aligned"); }
	template<> inline void store<true, true>(const int16_t* p) { _mm_store_si128((__m128i*)p,vec); }
	template<> inline void store<false, true>(const int16_t* p) { _mm_storeu_si128((__m128i*)p, vec); }
	template<> inline void store<true, false>(const int16_t* p) { _mm_stream_si128((__m128i*)p, vec); }

	//Arithmetic operators
	inline vec_int16x8 operator+(vec_int16x8 in){
		return (vec_int16x8)_mm_add_epi16(vec, in.getVec());
	}
	inline vec_int16x8 operator-(vec_int16x8 in) {
		return (vec_int16x8)_mm_sub_epi16(vec, in.getVec());
	}
	inline vec_int16x8 operator*(vec_int16x8 in) {
		return (vec_int16x8)_mm_mullo_epi16(vec, in.getVec());
	}
	inline vec_int16x8 operator/(vec_int16x8 in) {//Always correct
		return (vec_int16x8)impl::_mm_div_epi16(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_int16x8 in) {
		vec = _mm_add_epi16(vec, in.getVec());
	}
	inline void operator-=(vec_int16x8 in) {
		vec = _mm_sub_epi16(vec, in.getVec());
	}
	inline void operator*=(vec_int16x8 in) {
		vec = _mm_mullo_epi16(vec, in.getVec());
	}
	inline void operator/=(vec_int16x8 in) {//Always correct
		vec = impl::_mm_div_epi16(vec, in.getVec());
	}
	//Bitwise operator
	inline vec_int16x8 operator&(vec_int16x8 in) {
		return (vec_int16x8)_mm_and_si128(vec, in.getVec());
	}
	inline vec_int16x8 operator|(vec_int16x8 in) {
		return (vec_int16x8)_mm_or_si128(vec, in.getVec());
	}
	inline vec_int16x8 operator^(vec_int16x8 in) {
		return (vec_int16x8)_mm_xor_si128(vec, in.getVec());
	}
	//Bitwise operators, acting on itself
	inline void operator&=(vec_int16x8 in) {
		vec = _mm_and_si128(vec, in.getVec());
	}
	inline void operator|=(vec_int16x8 in) {
		vec = _mm_or_si128(vec, in.getVec());
	}
	inline void operator^=(vec_int16x8 in) {
		vec = _mm_xor_si128(vec, in.getVec());
	}
};
class vec_uint16x8 {// uint16_t[8]
private:
	__m128i vec;
public:
	//Constructors
	vec_int16x8(const uint16_t* p, constexpr bool align = false) { load<true>(p, align); }
	vec_int16x8(const __m128i v) { load(v); }
	vec_int16x8(const uint16_t i) { load(i); }
	vec_int16x8(const uint16_t i1, const uint16_t i2, const uint16_t i3, const uint16_t i4, const uint16_t i5, const uint16_t i6, const uint16_t i7, const uint16_t i8) { load(i1, i2, i3, i4, i5, i6, i7, i8); }

	//Misc
	inline __m128i getVec() { return vec; }
	template<bool temporal>
	inline void load(const uint16_t* p, constexpr bool align = false) {
		if constexpr (align)
			vec = _mm_load_si128((__m128i*)p);
		else
			vec = _mm_lddqu_si128((__m128i*)p);
	}
	template<> inline void load<false>(const uint16_t* p, constexpr bool align = true) {
		static_assert(align, "Non temporal loads must be aligned!");
		vec = _mm_stream_load_si128((__m128i*)p);
	}
	inline void load(const __m128i v) { vec = v; }
	inline void load(const uint16_t i) { vec = _mm_set1_epi16(i); }
	inline void load(const uint16_t i1, const uint16_t i2, const uint16_t i3, const uint16_t i4, const uint16_t i5, const uint16_t i6, const uint16_t i7, const uint16_t i8) {
		vec = _mm_set_epi16(i1, i2, i3, i4, i5, i6, i7, i8);
	}
	template<bool aligned, bool temporal> inline void store(const uint16_t* p) { static_assert(aligned || temporal, "Non temporal stores must be aligned"); }
	template<> inline void store<true, true>(const uint16_t* p) { _mm_store_si128((__m128i*)p, vec); }
	template<> inline void store<false, true>(const uint16_t* p) { _mm_storeu_si128((__m128i*)p, vec); }
	template<> inline void store<true, false>(const uint16_t* p) { _mm_stream_si128((__m128i*)p, vec); }

	//Arithmetic operators
	inline vec_uint16x8 operator+(vec_uint16x8 in) {
		return (vec_uint16x8)_mm_add_epi16(vec, in.getVec());
	}
	inline vec_uint16x8 operator-(vec_uint16x8 in) {
		return (vec_uint16x8)_mm_sub_epi16(vec, in.getVec());
	}
	inline vec_uint16x8 operator*(vec_uint16x8 in) {
		return (vec_int16x8)_mm_mullo_epi16(vec, in.getVec());
	}
	inline vec_uint16x8 operator/(vec_uint16x8 in) {//Always correct
		return (vec_uint16x8)impl::_mm_div_epi16(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_int16x8 in) {
		vec = _mm_add_epu16(vec, in.getVec());
	}
	inline void operator-=(vec_int16x8 in) {
		vec = _mm_sub_epi16(vec, in.getVec());
	}
	inline void operator*=(vec_int16x8 in) {
		vec = _mm_mullo_epi16(vec, in.getVec());
	}
	inline void operator/=(vec_int16x8 in) {//Always correct
		vec = impl::_mm_div_epu16(vec, in.getVec());
	}
	//Bitwise operator
	inline vec_int16x8 operator&(vec_int16x8 in) {
		return (vec_int16x8)_mm_and_si128(vec, in.getVec());
	}
	inline vec_int16x8 operator|(vec_int16x8 in) {
		return (vec_int16x8)_mm_or_si128(vec, in.getVec());
	}
	inline vec_int16x8 operator^(vec_int16x8 in) {
		return (vec_int16x8)_mm_xor_si128(vec, in.getVec());
	}
	//Bitwise operators, acting on itself
	inline void operator&=(vec_int16x8 in) {
		vec = _mm_and_si128(vec, in.getVec());
	}
	inline void operator|=(vec_int16x8 in) {
		vec = _mm_or_si128(vec, in.getVec());
	}
	inline void operator^=(vec_int16x8 in) {
		vec = _mm_xor_si128(vec, in.getVec());
	}
};
class vec_int32x4 {// int32_t[4]
private:
	__m128i vec;
public:
	//Constructors
	vec_int32x4(const int32_t* p, constexpr bool align = false) { load<true>(p,align); }
	vec_int32x4(const __m128i v) { load(v); }
	vec_int32x4(const int32_t i) { load(i); }
	vec_int32x4(const int32_t i1, const int32_t i2, const int32_t i3, const int32_t i4) { load(i1,i2,i3,i4); }

	//Misc
	inline __m128i getVec() { return vec; }
	template<bool temporal>
	inline void load(const int32_t* p, constexpr bool align = false) {
		if constexpr (align)
			vec = _mm_load_si128((__m128i*)p);
		else
			vec = _mm_lddqu_si128((__m128i*)p);
	}
	template<> inline void load<false>(const int32_t* p, constexpr bool align = true) {
		static_assert(align,"Non temporal loads must be aligned!");
		vec = _mm_stream_load_si128((__m128i*)p);
	}
	inline void load(const __m128i v) { vec = v; }
	inline void load(const int32_t i) { vec = _mm_set1_epi32(i); }
	inline void load(const int32_t i1, const int32_t i2, const int32_t i3, const int32_t i4) { 
		vec = _mm_set_epi32(i1, i2, i3, i4); 
	}
	template<bool aligned, bool temporal> inline void store(const int32_t* p) { static_assert(aligned||temporal,"Non temporal stores must be aligned"); }
	template<> inline void store<true, true>(const int32_t* p) { _mm_store_si128((__m128i*)p,vec); }
	template<> inline void store<false, true>(const int32_t* p) { _mm_storeu_si128((__m128i*)p, vec); }
	template<> inline void store<true, false>(const int32_t* p) { _mm_stream_si128((__m128i*)p, vec); }

	//Arithmetic operators
	inline vec_int32x4 operator+(vec_int32x4 in){
		return (vec_int32x4)_mm_add_epi32(vec, in.getVec());
	}
	inline vec_int32x4 operator-(vec_int32x4 in) {
		return (vec_int32x4)_mm_sub_epi32(vec, in.getVec());
	}
	inline vec_int32x4 operator*(vec_int32x4 in) {
		return (vec_int32x4)_mm_mullo_epi32(vec, in.getVec());
	}
	inline vec_int32x4 operator/(vec_int32x4 in) {//Always correct
		return (vec_int32x4)impl::_mm_div_epi32(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_int32x4 in) {
		vec = _mm_add_epi32(vec, in.getVec());
	}
	inline void operator-=(vec_int32x4 in) {
		vec = _mm_sub_epi32(vec, in.getVec());
	}
	inline void operator*=(vec_int32x4 in) {
		vec = _mm_mullo_epi32(vec, in.getVec());
	}
	inline void operator/=(vec_int32x4 in) {//Always correct
		vec = _impl::mm_div_epi32(vec, in.getVec());
	}
	//Bitwise operator
	inline vec_int32x4 operator&(vec_int32x4 in) {
		return (vec_int32x4)_mm_and_si128(vec, in.getVec());
	}
	inline vec_int32x4 operator|(vec_int32x4 in) {
		return (vec_int32x4)_mm_or_si128(vec, in.getVec());
	}
	inline vec_int32x4 operator^(vec_int32x4 in) {
		return (vec_int32x4)_mm_xor_si128(vec, in.getVec());
	}
	//Bitwise operators, acting on itself
	inline void operator&=(vec_int32x4 in) {
		vec = _mm_and_si128(vec, in.getVec());
	}
	inline void operator|=(vec_int32x4 in) {
		vec = _mm_or_si128(vec, in.getVec());
	}
	inline void operator^=(vec_int32x4 in) {
		vec = _mm_xor_si128(vec, in.getVec());
	}
};
#else
#error Your processeor doesn't seem to support SSE instructions. However, they are required in this version of the library for intrinsics
#endif
}
}

//https://stackoverflow.com/questions/42442325/how-to-divide-a-m256i-vector-by-an-integer-variable




/****************************************************
 *                     Makros                       *
 * - _mm_widen_ep[i,u]16 and _mm_compress_ep[i,u]32 * _10, _20, _21 | speed (benchmark together, roundtrip)
 * - _mm_div_ep[i,u]16                              * _div, _rcp    | speed
 * - _mm_idiv_epi32_precise_int                     * _mm_idiv_epi32_avx,_mm_idiv_epi32_split | AVX-support&speed (<PRECISE, __m128i>)
 * - _mm_idiv_epi32_precise_float                   * _mm_idiv_epi32_avx,_mm_idiv_epi32_split | AVX-support&speed (<PRECISE, __m128>)
 ****************************************************/
