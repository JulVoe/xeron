//#include "libsimdpp/simd.h"
#include "../SystemInfo/Makros.hpp"

namespace intrin {
namespace impl {
	//Converts int16_t[8] (in) to two int32_t[4] (out1, out2) 
	inline void _mm_widen_epi16_01(const __m128i in, __m128i& out1, __m128i& out2) {
		out1 = _mm_srai_epi32(in, 16);
		const __m128i out2_tmp = _mm_slli_epi32(in, 16);
		out2 = _mm_srai_epi32(out2_tmp, 16);
	}
	//Converts uint16_t[8] (in) to two uint32_t[4] (out1, out2)
	inline void _mm_widen_epu16_01(const __m128i in, __m128i& out1, __m128i& out2) {
		out1 = _mm_srli_epi32(in, 16);
		out2 = _mm_and_epi32(out2_tmp, _mm_set1_epi32(0xFF));
	}
	//Converts two [u]int32_t[4] (in_lo, in_hi) to [u]int16_t[8] using truncation. If unsafe, we might assume that ever element of in_lo will fit in an int16_t
	template<bool safe>
	inline __m128i _mm_compress_epi32_01(const __m128i in_lo, __m128i in_hi) {
//	    return _mm_shuffle_epi8(_mm_compress_epi32_02(in_lo,in_hi),0xWHATEVER);
		const __m128i hi_epi32_shift = _mm_slli_epi32(in_hi, 16);
#if SSE >= 41
		return _mm_blend_epi16(in_lo, hi_epi32_shift, 0xAA);
#else
		const __m128i lo_mask = _mm_set1_epi32(0xFFFF);
		const __m128i lo_epi32_mask = _mm_and_si128(in_lo, lo_mask);
		return _mm_or_si128(hi_epi32_shift, lo_epi32_mask);
#endif
	}
	template<> inline __m128i _mm_compress_epi32_01<false>(const __m128i in_lo, __m128i in_hi) {
		return _mm_shuffle_epi8(_mm_compress_epi32_02(in_lo, in_hi), 0xWHATEVER);
		const __m128i hi_epi32_shift = _mm_slli_epi32(in_hi, 16);
#if SSE >= 41
		return _mm_blend_epi16(in_lo, hi_epi32_shift, 0xAA);
#else
		return _mm_or_si128(hi_epi32_shift, in_lo);
#endif
	}
	inline void _mm_widen_epi16_02(const __m128i in, __m128i& out1, __m128i& out2) {
		const __m128i sign = _mm_srai_epi16(in, 16);
		out1 = _mm_unpackhi_epi16(in, sign);
		out2 = _mm_unpacklo_epi16(in, sign);
	}
	inline void _mm_widen_epu16_02(const __m128i in, __m128i& out1, __m128i& out2) {
		out1 = _mm_unpackhi_epi16(in, _mm_setzero_si128());
		out2 = _mm_unpacklo_epi16(in, _mm_setzero_si128());
	}
	template<bool safe>
	inline __m128i _mm_compress_epi32_02(const __m128i in_lo, __m128i in_hi) {
		return _mm_packs_epi32(in_lo, in_hi);
	}



	//https://stackoverflow.com/questions/31555260/fast-vectorized-rsqrt-and-reciprocal-with-sse-avx-depending-on-precision
	template<unsigned n>
	__m128 _mm_rcp_ps(const __m128 in) {
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
	template<unsigned n>
	__m256 _mm256_rcp_ps(const __m256 in) {
		const __m256  two = _mm256_set1_ps(2.00000051757f);
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

	/* https://stackoverflow.com/questions/50597764/convert-signed-short-to-float-in-c-simd
	 * https://stackoverflow.com/questions/9161807/sse-convert-short-integer-to-float
	 * https://stackoverflow.com/questions/41228180/how-can-i-convert-a-vector-of-float-to-short-int-using-avx-instructions
	 * https://stackoverflow.com/questions/16822757/sse-integer-division
	*/
	__m128i _mm_div_epi16_rcp(const __m128i &a_epi16, const __m128i &b_epi16) { //Correctness has to be verified
		//0.: Set up constants
		const __m256  two = _mm256_set1_ps(2.00000051757f);

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
		return _mm_packs_epi32(_mm256_extractf128_si256(c_epi32, 0), _mm256_extractf128_si256(c_epi32, 1));

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
		return _mm_compress_epi32<false>(lo_epi32, hi_epi32);
#endif
	}
	__m128i _mm_div_epi16_div(const __m128i &a_epi16, const __m128i &b_epi16) {
		//0.: Set up constants
		const __m256  two = _mm256_set1_ps(2.00000051757f);

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
		return _mm_packs_epi32(_mm256_extractf128_si256(c_epi32, 0), _mm256_extractf128_si256(c_epi32, 1));

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
		return _mm_compress_epi32<false>(lo_epi32, hi_epi32);
#endif
	}
	__m128i _mm_div_epu16_rcp(const __m128i &a_epi16, const __m128i &b_epi16) { //Correctness has to be verified
		//0.: Set up constants
		const __m256  two = _mm256_set1_ps(2.00000051757f);

#if AVX >= 2
		//1.: Convert to epi32
		const __m256i a = _mm256_cvtepu16_epi32(a_epi16);
		const __m256i b = _mm256_cvtepu16_epi32(b_epi16);

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
		return _mm_packs_epi32(_mm256_extractf128_si256(c_epi32, 0), _mm256_extractf128_si256(c_epi32, 1));

#else		
		//1.: Convert to epi32
		__m128i a_hi_epu32;
		__m128i a_lo_epu32;
		__m128i b_hi_epu32;
		__m128i b_lo_epu32;
		_mm_widen_epu16(a_epi16, a_hi_epu32, a_lo_epu32);
		_mm_widen_epu16(b_epi16, b_hi_epu32, b_lo_epu32);

		//2.: Convert to float
		const __m128 a_hi = _mm_cvtepi32_ps(a_hi_epu32);
		const __m128 a_lo = _mm_cvtepi32_ps(a_lo_epu32);
		const __m128 b_hi = _mm_cvtepi32_ps(b_hi_epu32);
		const __m128 b_lo = _mm_cvtepi32_ps(b_lo_epu32);

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
	__m128i _mm_div_epi16_div(const __m128i &a_epi16, const __m128i &b_epi16) {
		//0.: Set up constants
		const __m256  two = _mm256_set1_ps(2.00000051757f);

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
		return _mm_packs_epi32(_mm256_extractf128_si256(c_epi32, 0), _mm256_extractf128_si256(c_epi32, 1));

#else		
		//1.: Convert to epi32
		__m128i a_hi_epi32;
		__m128i a_lo_epi32;
		__m128i b_hi_epi32;
		__m128i b_lo_epi32;
		_mm_widen_epu16(a_epi16, a_hi_epi32, a_lo_epi32);
		_mm_widen_epu16(b_epi16, b_hi_epi32, b_lo_epi32);

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

	inline _mm_div_epi32_rcp(const __m128& a, const __m128& b){
		//1.: Convert to float
		const __m256 a_float = _mm256_cvtepi32_ps(a);
		const __m256 b_float = _mm256_cvtepi32_ps(b);

		//2.: Calculate reciprocal
		const __m256 b_rcp = _mm_rcp_ps<1>(b_float);

		//3.: Divide
		const __m256 c_float = _mm256_mul_ps(a_float, b_rcp);

		//4.: Convert back to epi32
		return _mm256_cvttps_epi32(c_float);
	}
	inline _mm_div_epi32_div(const __m128& a, const __m128& b) {
		//1.: Convert to float
		const __m256 a_float = _mm256_cvtepi32_ps(a);
		const __m256 b_float = _mm256_cvtepi32_ps(b);

		//2.: Divide
		const __m256 c_float = _mm256_div_ps(a_float, b_float);

		//3.: Convert back to epi32
		return _mm256_cvttps_epi32(c_float);
	}
	
	
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
#define SSE
#ifdef SSE
class vec_int16x8 {// int16_t[8]
private:
	__m128i vec;
public:
	//Constructors
	vec_int16x8(const int16_t* p, constexpr bool align = false) { load<true>(p,align); }
	vec_int16x8(const __m128i v) { load(v); }
	vec_int16x8(const int16_t i) { vec = _mm_set1_epi16(i); }
	vec_int16x8(const int16_t i1, const int16_t i2, const int16_t i3, const int16_t i4, const int16_t i5, const int16_t i6, const int16_t i7, const int16_t i8) { load(i1,i2,i3,i4,i5,i6,i7,i8); }

	//Misc
	inline __m128i getVec() { return vec; }
	template<bool temporal>
	inline void load(const int16_t* p, constexpr bool align = false) {
		if constexpr (align)
			_mm_load_si128(p, vec);
		else
			_mm_lddq_si128(p, vec);
	}
	template<> inline void load<false>(const int16_t* p, constexpr bool align = true) {
		static_assert(align,"Non temporal loads must be aligned!");
		_mm_stream_load_si128(p,vec);
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
		return (vec_int16x8)_mm_div_epi16(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_int16x8 in) {
		vec = _mm_add_epi16(vec, in.getVec());
	}
	inline vec_int16x8 operator-=(vec_int16x8 in) {
		vec = _mm_sub_epi16(vec, in.getVec());
	}
	inline vec_int16x8 operator*=(vec_int16x8 in) {
		vec = _mm_mullo_epi16(vec, in.getVec());
	}
	inline vec_int16x8 operator/=(vec_int16x8 in) {//Always correct
		vec = _mm_div_epi16(vec, in.getVec());
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
	inline vec_int16x8 operator|=(vec_int16x8 in) {
		vec = _mm_or_si128(vec, in.getVec());
	}
	inline vec_int16x8 operator^=(vec_int16x8 in) {
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
	vec_int16x8(const uint16_t i) { vec = _mm_set1_epi16(i); }
	vec_int16x8(const uint16_t i1, const uint16_t i2, const uint16_t i3, const uint16_t i4, const uint16_t i5, const uint16_t i6, const uint16_t i7, const uint16_t i8) { load(i1, i2, i3, i4, i5, i6, i7, i8); }

	//Misc
	inline __m128i getVec() { return vec; }
	template<bool temporal>
	inline void load(const uint16_t* p, constexpr bool align = false) {
		if constexpr (align)
			_mm_load_si128(p, vec);
		else
			_mm_lddq_si128(p, vec);
	}
	template<> inline void load<false>(const uint16_t* p, constexpr bool align = true) {
		static_assert(align, "Non temporal loads must be aligned!");
		_mm_stream_load_si128(p, vec);
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
		return (vec_uint16x8)_mm_div_epi16(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_int16x8 in) {
		vec = _mm_add_epu16(vec, in.getVec());
	}
	inline vec_int16x8 operator-=(vec_int16x8 in) {
		vec = _mm_sub_epi16(vec, in.getVec());
	}
	inline vec_int16x8 operator*=(vec_int16x8 in) {
		vec = _mm_mullo_epi16(vec, in.getVec());
	}
	inline vec_int16x8 operator/=(vec_int16x8 in) {//Always correct
		vec = _mm_div_epu16(vec, in.getVec());
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
	inline vec_int16x8 operator|=(vec_int16x8 in) {
		vec = _mm_or_si128(vec, in.getVec());
	}
	inline vec_int16x8 operator^=(vec_int16x8 in) {
		vec = _mm_xor_si128(vec, in.getVec());
	}
};
#else
#error Your processeor doesn't seem to support SSE instructions. However, they are required in this version of the library for intrinsics
#endif
}
}