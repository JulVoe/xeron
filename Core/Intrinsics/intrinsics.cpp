#include "../SystemInfo/Makros.hpp"

namespace intrin {

/*******************************
 *            Enums            *
 *******************************/
	
//Integer division. Don't mess with the values or error messages will be wrong
enum ROUNDING_MODE { FAST = 0, TRUNCATE = 1, PRECISE = 2 }; //PRECISE=highest precision possible, FAST=allows functions to sacrifice some precision for speed, TRUNCATE=same behaviour as sisd integer division.
typedef BITNUM short; //Number of set bits in a number. When a type is signed, only n-1 bits are data-bits.
enum { SMALL = 23, MEDIUM = 31, BIG = 32-epsilon, HUGE = 32 }; //HUGE=as big as possible, BIG:32bit, aber nicht UINT_MAX, Medium:31bit, SMALL:23bit
	
namespace impl {
//--------------------------------------Reciprocals with newton-iterations--------------------------------------//
	//https://stackoverflow.com/questions/31555260/fast-vectorized-rsqrt-and-reciprocal-with-sse-avx-depending-on-precision
	//Approximates the inverse of float[4] (in) using intrinsic and n passes of newton iterations
	template<unsigned n>
	ALWAYS_INLINE __m128 _mm_rcp_ps(const __m128 in) {
		const __m128  two = _mm_set1_ps(2.0f);//2.00000051757f
		__m128 ret = _mm_rcp_ps(in);

		for (unsigned i = 0; i < n; i++) {
#if FMA
			const __m128 ret_mult = _mm_fnmadd_ps(in, ret, two);
#else
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
//--------------------------------------_mm_div_ep[i,u]16--------------------------------------//
	/* https://stackoverflow.com/questions/50597764/convert-signed-short-to-float-in-c-simd
	 * https://stackoverflow.com/questions/9161807/sse-convert-short-integer-to-float
	 * https://stackoverflow.com/questions/41228180/how-can-i-convert-a-vector-of-float-to-short-int-using-avx-instructions
	 * https://stackoverflow.com/questions/16822757/sse-integer-division
	*/
	//Divides a_epi16 by b_epi16 elementwise. If AVX2 is available, it can also return the result as floats.
	//Note: Correctness has to be verified when use_rcp is true
	//Note: FAST only differs from the rest, if use_rcp is true
	template<ROUNDING_MODE round, bool use_rcp, typename T = __m128i>
	ALWAYS_INLINE T _mm_div_epi16_impl(const __m128i &a_epi16, const __m128i &b_epi16) {
		//0.: Are the template parameters valid?
		static_assert(std::is_same<T,__m128i>{} || std::is_same<T,__m256>{}, "_mm_div_epi16_impl can only return either __m128i or __m256!");
		static_assert(!(use_rcp && round == PRECISE), "If you want a precise division, don't use reciprocals!");
#if AVX >= 2
		//1.: Convert to epi32
		const __m256i a = _mm256_cvtepi16_epi32(a_epi16);
		const __m256i b = _mm256_cvtepi16_epi32(b_epi16);

		//2.: Convert to float
		const __m256 a_float = _mm256_cvtepi32_ps(a);
		const __m256 b_float = _mm256_cvtepi32_ps(b);

		//3.: Perform the actual computation
		__m256 c_float;
		if constexpr(use_rcp){
			const __m256 b_rcp = _mm256_rcp_ps<1>(b_float); //Calculate reciprocal (b_rcp = 1/b_float)
			c_float = _mm256_mul_ps(a_float, b_rcp);
		} else {
			c_float = _mm256_div_ps(a_float, b_float);
		}
		
		if constexpr(std::is_same<T,__m256>{})
			return c_float;

		//4.: Convert back to epi32
		__m256i c_epi32;
		if constexpr(round==TRUNCATE)
			c_epi32 = _mm256_cvttps_epi32(c_float);
		else
			c_epi32 = _mm256_cvtps_epi32(c_float);

		//5.: Convert back to epi16
		return _mm256_cvt_i32x8_i16x8(c_epi32);

#else		
		//1.: Convert to float
		__m128 a_lo;
		__m128 a_hi;
		__m128 b_lo;
		__m128 b_hi;
		_mm_cvt_i16x8_2psx4(a_epi16, a_lo, a_hi);
		_mm_cvt_i16x8_2psx4(b_epi16, b_lo, b_hi);

		//2.: Perform the actual computation
		__m128 hi, lo;
		if constexpr(use_rcp){
			const __m128 b_hi_rcp = _mm_rcp_ps<round!=FAST>(b_hi); //Calculate reciprocal (b_hi_rcp = 1/b_hi)
			const __m128 b_lo_rcp = _mm_rcp_ps<round!=FAST>(b_lo); //Calculate reciprocal (b_lo_rcp = 1/b_lo)

			hi = _mm_mul_ps(a_hi, b_hi_rcp);
			lo = _mm_mul_ps(a_lo, b_lo_rcp);
		} else {
			hi = _mm_div_ps(a_hi, b_hi);
			lo = _mm_div_ps(a_lo, b_lo);
		}

		//3.: Convert back to epi16
		return _mm_cvt_2psx4_i16x8(lo, hi);
#endif
	}
	//Divides a_epu16 by b_epu16 elementwise. If AVX2 is available, it can also return the result as floats.
	//Note: Correctness has to be verified, if use_rcp is true
	//Note: FAST only differs from the rest, if use_rcp is true
	template<ROUNDING_MODE round, bool use_rcp, typename T = __m128i>
	ALWAYS_INLINE T _mm_div_epu16_impl(const __m128i &a_epu16, const __m128i &b_epu16) { 
		//0.: Are the template parameters valid?
		static_assert(std::is_same<T,__m128i>{} || std::is_same<T,__m256>{}, "_mm_div_epu16_impl can only return either __m128i or __m256!");
		static_assert(!(use_rcp && round == PRECISE), "If you want a precise division, don't use reciprocals!");
#if AVX >= 2
		//1.: Convert to epi32
		const __m256i a = _mm256_cvtepu16_epi32(a_epu16);
		const __m256i b = _mm256_cvtepu16_epi32(b_epu16);

		//2.: Convert to float
		const __m256 a_float = _mm256_cvtepi32_ps(a);
		const __m256 b_float = _mm256_cvtepi32_ps(b);

		//3.: Perform the actual computation
		__m256 c_float;
		if constexpr(use_rcp){
			const __m256 b_rcp = _mm256_rcp_ps<round!=FAST>(b_float); //Calculate reciprocal(b_rcp = 1/b_float)
			c_float = _mm256_mul_ps(a_float, b_rcp);
		} else {
			c_float = _mm256_div_ps(a_float, b_float);
		}
		
		if constexpr(std::is_same<T,__m256>{})
			return c_float;

		//5.: Convert back to epi32
		__m256i c_epi32;
		if constexpr(round==TRUNCATE)
			c_epi32 = _mm256_cvttps_epi32(c_float);
		else 
			c_epi32 = _mm256_cvtps_epi32(c_float);

		//6.: Convert back to epu16
		return _mm256_cvt_u32x8_u16x8(c_epi32);
#else		
		//1.: Convert to float
		__m128 a_lo;
		__m128 a_hi;
		__m128 b_lo;
		__m128 b_hi;
		_mm_cvt_u16x8_2psx4(a_epu16, a_lo, a_hi);
		_mm_cvt_u16x8_2psx4(b_epu16, b_lo, b_hi);

		//2.: Perform the actual computation
		__m128 hi, lo;
		if constexpr(use_rcp){
			const __m128 b_hi_rcp = _mm_rcp_ps<round!=FAST>(b_hi); //Calculate reciprocal (b_hi_rcp = 1/b_hi)
			const __m128 b_lo_rcp = _mm_rcp_ps<round!=FAST>(b_lo); //Calculate reciprocal (b_lo_rcp = 1/b_lo)

			const __m128 hi = _mm_mul_ps(a_hi, b_hi_rcp);
			const __m128 lo = _mm_mul_ps(a_lo, b_lo_rcp);
		} else {
			const __m128 hi = _mm_div_ps(a_hi, b_hi);
			const __m128 lo = _mm_div_ps(a_lo, b_lo);
		}

		//3.: Convert back to epu16
		return _mm_cvt_2psx4_u16x8(lo, hi);
#endif
	}
//--------------------------------------_mm_div_epi32--------------------------------------//
	//Divides [u]int32_t[4](a) by [u]int32_t[4](b) elementwise
	//_mm_idiv_epi32_split with the extra assumption that a<=2^24. If use_rcp is true, it uses _mm_rcp_ps with multiplication instead of division
	//Note: Very fast. Precise for a<=2^24, if !(round==FAST&&use_rcp). From there on inprecise for small b.
	//Note: FAST only differs from the other modes, if use_rcp is true.
	//Note: If use_rcp is true, the correctness of the result has to be verified.
	template<ROUNDING_MODE round, bool use_rcp, typename T = __m128i>
	ALWAYS_INLINE T _mm_idiv_epi32_small(__m128i a, __m128i b) {
		//0.: Are the template parameters valid?
		static_assert(!(use_rcp && round == PRECISE), "If you want a precise division, don't use reciprocals!");
		static_assert(std::is_same<T, __m128i>{} || std::is_same<T, __m128>{}, "_mm_idiv_epi32_small can only return either __m128i or __m128!");

		//1.: Computation
		const __m128 fa = _mm_cvtepi32_ps(a);
		const __m128 fb = _mm_cvtepi32_ps(b);
		__m128 fr;
		if constexpr(use_rcp){
			const __m128 rcp = _mm_rcp_ps<round!=FAST>(fb);
			const __m128 fr = _mm_mul_ps(fa, rcp);
		} else {
			fr = _mm_div_ps(fa, fb);
		}

		//2.: Return in the right form
		if constexpr (std::is_same<T,__m128i>{}) {
			if constexpr (round == TRUNCATE)
				return _mm_cvttps_epi32(fr);
			else
				return _mm_cvtps_epi32(fr);
		}
		else if constexpr (std::is_same<T,__m128>{}) {
			return fr;
		}
		else {
			UNREACHABLE();
		}
	}
	//Divides a by b elementwise.
	//Converts a and b to 256-double vectors and divides them. Very precise.
	template<ROUNDING_MODE round, typename T = __m128i>
	ALWAYS_INLINE T _mm_idiv_epi32_avx(__m128i a, __m128i b) {
		//0.: Are the template parameters valid?
		static_assert(std::is_same<T, __m128i>{} || std::is_same<T, __m128>{} || std::is_same<T, __m256d>{}, "_mm_idiv_epi32_avx can only return either __m128i, __m128 or __m256d!");

		//1.: Computation
		const __m256d da = _mm256_cvtepi32_pd(a);
		const __m256d db = _mm256_cvtepi32_pd(b);
		const __m256d dr = _mm256_div_pd(da, db);

		//2.: Return in the right form
		if constexpr (std::is_same<T,__m128i>{}) {
			if constexpr (round == TRUNCATE)
				return _mm256_cvttpd_epi32(dr);
			else
				return _mm256_cvtpd_epi32(dr);
		}
		else if constexpr (std::is_same<T,__m128>{}) {
			return _mm256_cvtpd_ps(dr);
		}
		else if constexpr (std::is_same<T,__m256d>{}){
			return dr;
		}
		else {
			UNREACHABLE();
		}
	}
	//Divides a by b elementwise.
	//Splits a up into two parts which will fit into a float without loss of accuracy. Divides them and add the result
	//Note: rcp: 0=div-div, 1=div-rcp, 2=rcp-div, 3=rcp-rcp. 
	//Note: Fast has 0 Newton-iterations, otherwise 1. (0f course only makes a difference, if rcp!=0)
	template<ROUNDING_MODE round, int rcp, typename T = __m128i>
	ALWAYS_INLINE T _mm_idiv_epi32_split(__m128i a, __m128i b) {
		//0.: Are the template parameters valid?
		static_assert(rcp==0 || rcp==1 || rcp==2 || rcp==3, "_mm_idiv_epi32_split only accepts rcp 0 to 3");
		static_assert(!(rcp != 0 && round == PRECISE), "If you want a precise division, don't use reciprocals!");
		static_assert(std::is_same<T, __m128i>{} || std::is_same<T, __m128>{}, "_mm_idiv_epi32_split can only return either __m128i or __m128!");
		
		//1.: Computation
		const __m128 ha = _mm_cvtepi32_ps(_mm_srai_epi32(a, 24)); //High 8 bits of a
		const __m128 la = _mm_cvtepi32_ps(_mm_and_si128(a, _mm_set1_epi32((1 << 24) - 1)|(1<<31))); //Lower 24 bits of a, will fit in mantissa of float
		const __m128 fb = _mm_cvtepi32_ps(b); //If b has more than 24 bits, some error will occur(at most 2^-23+2^-24). This is ok, because if b is that huge, the answer is at most 2^8 and the error won't show

		__m128 fr;
		if constexpr(rcp==0){
			const __m128 hr = _mm_mul_ps(_mm_div_ps(ha, fb), _mm_set1_ps((float)(1 << 24))); //Divide and shift back up(upper 8 bits, shift left by 24)
			fr = _mm_add_ps(_mm_div_ps(la, fb), hr); //fr=a/b=(2^24*ha+la)/b=2^24*(ha/b)+la/b
		} else if constexpr(rcp==1){
			const __m128 rcp = _mm_rcp_ps<round!=FAST>(fb);
			const __m128 hr = _mm_mul_ps(_mm_div_ps(ha, fb), _mm_set1_ps((float)(1 << 24))); //Divide and shift back up(upper 8 bits, shift left by 24)
			fr = _mm_add_ps(_mm_mul_ps(la, rcp), hr); //fr=a/b=(2^24*ha+la)/b=2^24*(ha/b)+la/b
		} else if constexpr(rcp==2){
			const __m128 rcp = _mm_rcp_ps<round!=FAST>(fb);
			const __m128 hr = _mm_mul_ps(_mm_mul_ps(ha, rcp), _mm_set1_ps((float)(1 << 24))); //Divide and shift back up(upper 8 bits, shift left by 24)
			fr = _mm_add_ps(_mm_div_ps(la, fb), hr); //fr=a/b=(2^24*ha+la)/b=2^24*(ha/b)+la/b
		} else if constexpr(rcp==3){
			const __m128 rcp = _mm_rcp_ps<round!=FAST>(fb);
			const __m128 hr = _mm_mul_ps(_mm_mul_ps(ha, rcp), _mm_set1_ps((float)(1 << 24))); //Divide and shift back up(upper 8 bits, shift left by 24)
			fr = _mm_add_ps(_mm_mul_ps(la, rcp), hr); //fr=a/b=(2^24*ha+la)/b=2^24*(ha/b)+la/b
		} else {
			UNREACHABLE();
		}
		

		//2.: Return in the right form
		if constexpr (std::is_same<T,__m128i>{}) {
			if constexpr (round == TRUNCATE)
				return _mm_cvttps_epi32(fr);
			else
				return _mm_cvtps_epi32(fr);
		}
		else if constexpr (std::is_same<T,__m128>{}) {
			return fr;
		}
		else {
			UNREACHABLE();
		}
	}
	//Divides a by b elementwise.
	//Splits a up into two parts which will fit into a float without loss of accuracy. Divides them and add the result
	//rcp: 0=div-div, 1=div-rcp, 2=rcp-div, 3=rcp-rcp. Fast has 0 Newton-iterations, otherwise 1.
	template<ROUNDING_MODE round, int rcp, typename T = __m128i>
	ALWAYS_INLINE T _mm_idiv_epu32_split(__m128i a, __m128i b) {
		//0.: Are the template parameters valid?
		static_assert(rcp==0 || rcp==1 || rcp==2 || rcp==3, "_mm_idiv_epu32_split only accepts rcp 0 to 3");
		static_assert(!(rcp != 0 && round == PRECISE), "If you want a precise division, don't use reciprocals!");
		static_assert(std::is_same<T, __m128i>{} || std::is_same<T, __m128>{}, "_mm_idiv_epu32_split can only return either __m128i or __m128!");
		
		//1.: Cast to float
		const __m128 ha = _mm_cvtepi32_ps(_mm_srli_epi32(a, 24)); //High 8 bits of a
		const __m128 la = _mm_cvtepi32_ps(_mm_and_si128(a, _mm_set1_epi32((1 << 24) - 1))); //Lower 24 bits of a, will fit in mantissa of float
		const __m128 fb = _mm_cvtepi32_ps(b); //If b has more than 24 bits, some error will occur(at most 2^-23+2^-24). This is ok, because if b is that huge, the answer is at most 2^8 and the error won't show

		//2.: Computation
		__m128 fr;
		if constexpr(rcp==0){
			const __m128 hr = _mm_mul_ps(_mm_div_ps(ha, fb), _mm_set1_ps((float)(1 << 24))); //Divide and shift back up(upper 8 bits, shift left by 24)
			fr = _mm_add_ps(_mm_div_ps(la, fb), hr); //fr=a/b=(2^24*ha+la)/b=2^24*(ha/b)+la/b
		} else if constexpr(rcp==1){
			const __m128 rcp = _mm_rcp_ps<round!=FAST>(fb);
			const __m128 hr = _mm_mul_ps(_mm_div_ps(ha, fb), _mm_set1_ps((float)(1 << 24))); //Divide and shift back up(upper 8 bits, shift left by 24)
			fr = _mm_add_ps(_mm_mul_ps(la, rcp), hr); //fr=a/b=(2^24*ha+la)/b=2^24*(ha/b)+la/b
		} else if constexpr(rcp==2){
			const __m128 rcp = _mm_rcp_ps<round!=FAST>(fb);
			const __m128 hr = _mm_mul_ps(_mm_mul_ps(ha, rcp), _mm_set1_ps((float)(1 << 24))); //Divide and shift back up(upper 8 bits, shift left by 24)
			fr = _mm_add_ps(_mm_div_ps(la, fb), hr); //fr=a/b=(2^24*ha+la)/b=2^24*(ha/b)+la/b
		} else if constexpr(rcp==3){
			const __m128 rcp = _mm_rcp_ps<round!=FAST>(fb);
			const __m128 hr = _mm_mul_ps(_mm_mul_ps(ha, rcp), _mm_set1_ps((float)(1 << 24))); //Divide and shift back up(upper 8 bits, shift left by 24)
			fr = _mm_add_ps(_mm_mul_ps(la, rcp), hr); //fr=a/b=(2^24*ha+la)/b=2^24*(ha/b)+la/b
		} else {
			UNREACHABLE();
		}

		//4.: Return in the right form
		if constexpr (std::is_same<T,__m128i>{}) {
			if constexpr (round == TRUNCATE){
#ifdef AVX512
				return _mm_cvttps_epu32(fr);
#else
				return _mm_cvttps_epi32(fr);
#endif
			} else {
#ifdef AVX512
				return _mm_cvtps_epu32(fr);
#else
				return _mm_cvtps_epi32(fr);
#endif
			}

		}
		else if constexpr (std::is_same<T,__m128>{}) {
			return fr;
		}
		else {
			UNREACHABLE();
		}
	}
	
//--------------------------------------Interface for integer division--------------------------------------//	
	//Vector-equivalent of "return (T)a/(T)b;"
	template<ROUNDING_MODE precision = PRECISE, typename T = __m128i>
	ALWAYS_INLINE T _mm_idiv_epi16(__m128i a, __m128i b) {
#if AVX>=2 //Otherwise, two __m128 have to be returned wich would not work with this signature		
		static_assert(std::is_same<T, __m128i>{} || std::is_same<T, __m256>{}, "_mm_idiv_epi16 can only return either __m128i or __m256!");
#else
		static_assert(std::is_same<T, __m128i>{}, "_mm_idiv_epi16 can only return __m128i because AVX2 is not available.");
#endif
		UNREACHABLE();
	}
	ALWAYS_INLINE template<> __m128i _mm_idiv_epi16<PRECISE , __m128i>(__m128i a, __m128i b) { return _mm_div_epi16_impl<PRECISE , USE_RCP, __m128i>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epi16<FAST    , __m128i>(__m128i a, __m128i b) { return _mm_div_epi16_impl<FAST    , true   , __m128i>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epi16<TRUNCATE, __m128i>(__m128i a, __m128i b) { return _mm_div_epi16_impl<TRUNCATE, USE_RCP, __m128i>(a, b); }
	ALWAYS_INLINE template<> __m256  _mm_idiv_epi16<PRECISE , __m256 >(__m128i a, __m128i b) { return _mm_div_epi16_impl<PRECISE , false  , __m256 >(a, b); }
	ALWAYS_INLINE template<> __m256  _mm_idiv_epi16<FAST    , __m256 >(__m128i a, __m128i b) { return _mm_div_epi16_impl<FAST    , true   , __m256 >(a, b); }
	ALWAYS_INLINE template<> __m256  _mm_idiv_epi16<TRUNCATE, __m256 >(__m128i a, __m128i b) { return _mm_div_epi16_impl<TRUNCATE, USE_RCP, __m256 >(a, b); }
	
	//Vector-equivalent of "return (T)a/(T)b;"
	template<ROUNDING_MODE precision = PRECISE, typename T = __m128i>
	ALWAYS_INLINE T _mm_idiv_epu16(__m128i a, __m128i b) {
#if AVX>=2 //Otherwise, two __m128 have to be returned wich would not work with this signature		
		static_assert(std::is_same<T, __m128i>{} || std::is_same<T, __m256>{}, "_mm_idiv_epu16 can only return either __m128i or __m256!");
#else
		static_assert(std::is_same<T, __m128i>{}, "_mm_idiv_epu16 can only return __m128i because AVX2 is not available.");
#endif
		UNREACHABLE();
	}
	ALWAYS_INLINE template<> __m128i _mm_idiv_epu16<PRECISE , __m128i>(__m128i a, __m128i b) { return _mm_div_epu16_impl<PRECISE , USE_RCP, __m128i>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epu16<FAST    , __m128i>(__m128i a, __m128i b) { return _mm_div_epu16_impl<FAST    , true   , __m128i>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epu16<TRUNCATE, __m128i>(__m128i a, __m128i b) { return _mm_div_epu16_impl<TRUNCATE, USE_RCP, __m128i>(a, b); }
	ALWAYS_INLINE template<> __m256  _mm_idiv_epu16<PRECISE , __m256 >(__m128i a, __m128i b) { return _mm_div_epu16_impl<PRECISE , false  , __m256 >(a, b); }
	ALWAYS_INLINE template<> __m256  _mm_idiv_epu16<FAST    , __m256 >(__m128i a, __m128i b) { return _mm_div_epu16_impl<FAST    , true   , __m256 >(a, b); }
	ALWAYS_INLINE template<> __m256  _mm_idiv_epu16<TRUNCATE, __m256 >(__m128i a, __m128i b) { return _mm_div_epu16_impl<TRUNCATE, USE_RCP, __m256 >(a, b); }

	//Vector-equivalent of "return (T)a/(T)b;"
	template<ROUNDING_MODE round = PRECISE, BITNUM size = 32, typename T = __m128i>
	ALWAYS_INLINE T _mm_idiv_epi32(__m128i a, __m128i b) {
		static_assert(std::is_same<T, __m128i>{} || std::is_same<T, __m128>{} || std::is_same<T, __m256d>{}, "_mm_idiv_epi32 can only return either __m128i, __m128 or __m256d!");
		if constexpr (std::is_same<T, __m256d>{}) {
			return _mm_idiv_epi32_avx<precision, __m256d>(a, b);
		}
		else if constexpr (std::is_same<T, __m128i>{}) {
				          if (size <= 23 && round == PRECISE ) { return return _mm_idiv_epi32_small      <PRECISE , 0     , __m128i>(a, b); }
					 else if (size <= 23 && round == FAST    ) { return return _mm_idiv_epi32_small      <FAST    , __RCPf, __m128i>(a, b); }
					 else if (size <= 23 && round == TRUNCATE) { return return _mm_idiv_epi32_small      <TRUNCATE, __RCPt, __m128i>(a, b); }
					 else if (              round == PRECISE ) { return return _mm_idiv_epi32_precise_int<PRECISE ,         __m128i>(a, b); }
					 else if (              round == FAST    ) { return return _mm_idiv_epi32_small      <FAST    , __RCPf, __m128i>(a, b); }
					 else if (              round == TRUNCATE) { return return _mm_idiv_epi32_precise_int<TRUNCATE,         __m128i>(a, b); }
		}
		else if constexpr (std::is_same<T, __m128>{}) {
			     if (size <= 23 && round == PRECISE ) { return return _mm_idiv_epi32_small        <PRECISE , 0     , __m128i>(a, b); }
			else if (size <= 23 && round == FAST    ) { return return _mm_idiv_epi32_small        <FAST    , __RCPf, __m128i>(a, b); }
			else if (size <= 23 && round == TRUNCATE) { return return _mm_idiv_epi32_small        <TRUNCATE, __RCPt, __m128i>(a, b); }
			else if (              round == PRECISE ) { return return _mm_idiv_epi32_precise_float<PRECISE ,         __m128i>(a, b); }
			else if (              round == FAST    ) { return return _mm_idiv_epi32_small        <FAST    , __RCPf, __m128i>(a, b); }
			else if (              round == TRUNCATE) { return return _mm_idiv_epi32_precise_float<TRUNCATE,         __m128i>(a, b); }
		}
		else if constexpr (std::is_same<T, __m128>{}) {
			return _mm_idiv_epi32_avx<round, __m256d>(a, b);
		}
		else {
			UNREACHABLE;
		}
	}
	
	//Vector-equivalent of "return (T)a/(T)b;"
	template<ROUNDING_MODE precision = PRECISE, BITNUM size = BIG, typename T = __m128i>
	ALWAYS_INLINE T _mm_idiv_epu32(__m128i a, __m128i b) {
		static_assert(size == SMALL || size == MEDIUM || size == BIG , "_mm_idiv_epu32 only supports size SMALL(0), MEDIUM(1) or BIG(2)!");
		static_assert(std::is_same<T, __m128i>{} || std::is_same<T, __m128>{} || std::is_same<T, __m256d>{}, "_mm_idiv_epu32 can only return either __m128i, __m128 or __m256d!");
		UNREACHABLE();
	}
	//TODO: use _mm_idiv_epi32_small for SMALL
	//TODO: Update to syntax above
	ALWAYS_INLINE template<> __m128i _mm_idiv_epu32<PRECISE,  BIG  , __m128i>(__m128i a, __m128i b) { return _mm_idiv_epu32_split<PRECISE,  __m128i, __RCP, false>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epu32<PRECISE,  SMALL, __m128i>(__m128i a, __m128i b) { return _mm_idiv_epu32_split<PRECISE,  __m128i, __RCP, false>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epu32<FAST,     BIG  , __m128i>(__m128i a, __m128i b) { return _mm_idiv_epu32_split<PRECISE,  __m128i, 3    , true >(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epu32<FAST,     SMALL, __m128i>(__m128i a, __m128i b) { return _mm_idiv_epu32_split<PRECISE,  __m128i, 3    , true >(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epu32<TRUNCATE, BIG  , __m128i>(__m128i a, __m128i b) { return _mm_idiv_epu32_split<TRUNCATE, __m128i, __RCP, false>(a, b); }
	ALWAYS_INLINE template<> __m128i _mm_idiv_epu32<TRUNCATE, SMALL, __m128i>(__m128i a, __m128i b) { return _mm_idiv_epu32_split<TRUNCATE, __m128i, __RCP, false>(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epu32<PRECISE,  BIG,   __m128 >(__m128i a, __m128i b) { return _mm_idiv_epu32_split<PRECISE,  __m128i, __RCP, false>(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epu32<PRECISE,  SMALL, __m128 >(__m128i a, __m128i b) { return _mm_idiv_epu32_split<PRECISE,  __m128i, __RCP, false>(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epu32<FAST,     BIG,   __m128 >(__m128i a, __m128i b) { return _mm_idiv_epu32_split<PRECISE,  __m128i, 3    , true >(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epu32<FAST,     SMALL, __m128 >(__m128i a, __m128i b) { return _mm_idiv_epu32_split<PRECISE,  __m128i, 3    , true >(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epu32<TRUNCATE, BIG,   __m128 >(__m128i a, __m128i b) { return _mm_idiv_epu32_split<TRUNCATE, __m128i, __RCP, false>(a, b); }
	ALWAYS_INLINE template<> __m128  _mm_idiv_epu32<TRUNCATE, SMALL, __m128 >(__m128i a, __m128i b) { return _mm_idiv_epu32_split<TRUNCATE, __m128i, __RCP, false>(a, b); }
	ALWAYS_INLINE template<> __m256d _mm_idiv_epu32<PRECISE,  BIG,   __m256d>(__m128i a, __m128i b) { return _mm_idiv_epu32_avx  <PRECISE , __m256d              >(a, b); }
	ALWAYS_INLINE template<> __m256d _mm_idiv_epu32<PRECISE,  SMALL, __m256d>(__m128i a, __m128i b) { return _mm_idiv_epu32_avx  <PRECISE , __m256d              >(a, b); }
	ALWAYS_INLINE template<> __m256d _mm_idiv_epu32<FAST,     BIG,   __m256d>(__m128i a, __m128i b) { return _mm_idiv_epu32_avx  <PRECISE , __m256d              >(a, b); }
	ALWAYS_INLINE template<> __m256d _mm_idiv_epu32<FAST,     SMALL, __m256d>(__m128i a, __m128i b) { return _mm_idiv_epu32_avx  <PRECISE , __m256d              >(a, b); }
	ALWAYS_INLINE template<> __m256d _mm_idiv_epu32<TRUNCATE, BIG,   __m256d>(__m128i a, __m128i b) { return _mm_idiv_epu32_avx  <TRUNCATE, __m256d              >(a, b); }
	ALWAYS_INLINE template<> __m256d _mm_idiv_epu32<TRUNCATE, SMALL, __m256d>(__m128i a, __m128i b) { return _mm_idiv_epu32_avx  <TRUNCATE, __m256d              >(a, b); }
	
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
	using intrin_type = __m128i;
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
		vec = _mm_setr_epi16(i1, i2, i3, i4, i5, i6, i7, i8); 
	}
	template<bool aligned, bool temporal> inline void store(const int16_t* p) { static_assert(aligned||temporal,"Non temporal stores must be aligned"); UNREACHABLE; }
	template<> inline void store<true, true> (const int16_t* p) { _mm_store_si128 ((__m128i*)p, vec); }
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
		return (vec_int16x8)impl::_mm_idiv_epi16<TRUNCATE, __m128i>(vec, in.getVec());
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
		vec = impl::_mm_idiv_epi16<TRUNCATE, __m128i>(vec, in.getVec());
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
	using intrin_type = __m128i;
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
	inline void load(const __m128i  v) { vec = v; }
	inline void load(const uint16_t i) { vec = _mm_set1_epi16(i); }
	inline void load(const uint16_t i1, const uint16_t i2, const uint16_t i3, const uint16_t i4, const uint16_t i5, const uint16_t i6, const uint16_t i7, const uint16_t i8) {
		vec = _mm_setr_epi16(i1, i2, i3, i4, i5, i6, i7, i8);
	}
	template<bool aligned, bool temporal> inline void store(const uint16_t* p) { static_assert(aligned || temporal, "Non temporal stores must be aligned"); UNREACHABLE; }
	template<> inline void store<true, true> (const uint16_t* p) { _mm_store_si128 ((__m128i*)p, vec); }
	template<> inline void store<false, true>(const uint16_t* p) { _mm_storeu_si128((__m128i*)p, vec); }
	template<> inline void store<true, false>(const uint16_t* p) { _mm_stream_si128((__m128i*)p, vec); }

	//Arithmetic operators
	inline vec_uint16x8 operator+(vec_uint16x8 in) {
		return (vec_uint16x8)_mm_adds_epu16(vec, in.getVec());
	}
	inline vec_uint16x8 operator-(vec_uint16x8 in) {
		return (vec_uint16x8)_mm_subs_epu16(vec, in.getVec());
	}
	inline vec_uint16x8 operator*(vec_uint16x8 in) {
		return (vec_int16x8)_mm_mullo_epi16(vec, in.getVec()); //Works for unsigned too
	}
	inline vec_uint16x8 operator/(vec_uint16x8 in) {//Always correct
		return (vec_uint16x8)impl::_mm_idiv_epu16<TRUNCATE, __m128i>(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_int16x8 in) {
		vec = _mm_adds_epu16(vec, in.getVec());
	}
	inline void operator-=(vec_int16x8 in) {
		vec = _mm_subs_epu16(vec, in.getVec());
	}
	inline void operator*=(vec_int16x8 in) {
		vec = _mm_mullo_epi16(vec, in.getVec()); //Works for unsigned too
	}
	inline void operator/=(vec_int16x8 in) {//Always correct
		vec = impl::_mm_idiv_epu16<TRUNCATE, __m128i>(vec, in.getVec());
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
	using intrin_type = __m128i;
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
		vec = _mm_setr_epi32(i1, i2, i3, i4); 
	}
	template<bool aligned, bool temporal> inline void store(const int32_t* p) { static_assert(aligned||temporal,"Non temporal stores must be aligned"); UNREACHABLE; }
	template<> inline void store<true, true> (const int32_t* p) { _mm_store_si128 ((__m128i*)p, vec); }
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
#ifdef SSE41
		return (vec_int32x4)_mm_mullo_epi32(vec, in.getVec());
#else
    		__m128i a13 = _mm_shuffle_epi32(vec, 0xF5);            // (-,a3,-,a1)
    		__m128i b13 = _mm_shuffle_epi32(in.getVec(), 0xF5);    // (-,b3,-,b1)
   		__m128i prod02 = _mm_mul_epi32(vec, in.getVec());      // (-,a2*b2,-,a0*b0)
    		__m128i prod13 = _mm_mul_epi32(a13, b13);              // (-,a3*b3,-,a1*b1)
   		__m128i prod01 = _mm_unpacklo_epi32(prod02, prod13);   // (-,-,a1*b1,a0*b0) 
    		__m128i prod23 = _mm_unpackhi_epi32(prod02, prod13);   // (-,-,a3*b3,a2*b2) 
    		return           _mm_unpacklo_epi64(prod01, prod23);   // (ab3,ab2,ab1,ab0)
#endif
	}
	inline vec_int32x4 operator/(vec_int32x4 in) {//Always correct
		return (vec_int32x4)impl::_mm_idiv_epi32<TRUNCATE, BIG, __m128i>(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_int32x4 in) {
		vec = _mm_add_epi32(vec, in.getVec());
	}
	inline void operator-=(vec_int32x4 in) {
		vec = _mm_sub_epi32(vec, in.getVec());
	}
	inline void operator*=(vec_int32x4 in) {
#ifdef SSE41
		vec = _mm_mullo_epi32(vec, in.getVec());
#else
    		__m128i a13 = _mm_shuffle_epi32(vec, 0xF5);            // (-,a3,-,a1)
    		__m128i b13 = _mm_shuffle_epi32(in.getVec(), 0xF5);    // (-,b3,-,b1)
   		__m128i prod02 = _mm_mul_epi32(vec, in.getVec());      // (-,a2*b2,-,a0*b0)
    		__m128i prod13 = _mm_mul_epi32(a13, b13);              // (-,a3*b3,-,a1*b1)
   		__m128i prod01 = _mm_unpacklo_epi32(prod02, prod13);   // (-,-,a1*b1,a0*b0) 
    		__m128i prod23 = _mm_unpackhi_epi32(prod02, prod13);   // (-,-,a3*b3,a2*b2) 
    		vec = _mm_unpacklo_epi64(prod01, prod23);              // (ab3,ab2,ab1,ab0)
#endif
	}
	inline void operator/=(vec_int32x4 in) {//Always correct
		vec = _impl::_mm_idiv_epi32<TRUNCATE, BIG, __m128i>(vec, in.getVec());
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
class vec_uint32x4 {// uint32_t[4]
private:
	using intrin_type = __m128i;
	__m128i vec;
public:
	//Constructors
	vec_uint32x4(const uint32_t* p, constexpr bool align = false) { load<true>(p,align); }
	vec_uint32x4(const __m128i v) { load(v); }
	vec_uint32x4(const uint32_t i) { load(i); }
	vec_uint32x4(const uint32_t i1, const uint32_t i2, const uint32_t i3, const uint32_t i4) { load(i1,i2,i3,i4); }

	//Misc
	inline __m128i getVec() { return vec; }
	template<bool temporal>
	inline void load(const uint32_t* p, constexpr bool align = false) {
		if constexpr (align)
			vec = _mm_load_si128((__m128i*)p);
		else
			vec = _mm_lddqu_si128((__m128i*)p);
	}
	template<> inline void load<false>(const uint32_t* p, constexpr bool align = true) {
		static_assert(align,"Non temporal loads must be aligned!");
		vec = _mm_stream_load_si128((__m128i*)p);
	}
	inline void load(const __m128i  v) { vec = v; }
	inline void load(const uint32_t i) { vec = _mm_set1_epi32(i); }
	inline void load(const uint32_t i1, const uint32_t i2, const uint32_t i3, const uint32_t i4) { 
		vec = _mm_setr_epi32(i1, i2, i3, i4); 
	}
	template<bool aligned, bool temporal> inline void store(const uint32_t* p) { static_assert(aligned||temporal,"Non temporal stores must be aligned"); UNREACHABLE; }
	template<> inline void store<true, true> (const uint32_t* p) { _mm_store_si128 ((__m128i*)p, vec); }
	template<> inline void store<false, true>(const uint32_t* p) { _mm_storeu_si128((__m128i*)p, vec); }
	template<> inline void store<true, false>(const uint32_t* p) { _mm_stream_si128((__m128i*)p, vec); }

	//Arithmetic operators
	inline vec_uint32x4 operator+(vec_uint32x4 in){
		return (vec_uint32x4)_mm_add_epi32(vec, in.getVec());
	}
	inline vec_uint32x4 operator-(vec_uint32x4 in) {
		return (vec_uint32x4)_mm_sub_epi32(vec, in.getVec());
	}
	inline vec_uint32x4 operator*(vec_uint32x4 in) {
#ifdef SSE41
		return (vec_uint32x4)_mm_mullo_epi32(vec, in.getVec());
#else
    		__m128i a13 = _mm_shuffle_epi32(vec, 0xF5);            // (-,a3,-,a1)
    		__m128i b13 = _mm_shuffle_epi32(in.getVec(), 0xF5);    // (-,b3,-,b1)
   		__m128i prod02 = _mm_mul_epu32(vec, in.getVec());      // (-,a2*b2,-,a0*b0)
    		__m128i prod13 = _mm_mul_epu32(a13, b13);              // (-,a3*b3,-,a1*b1)
   		__m128i prod01 = _mm_unpacklo_epi32(prod02, prod13);   // (-,-,a1*b1,a0*b0) 
    		__m128i prod23 = _mm_unpackhi_epi32(prod02, prod13);   // (-,-,a3*b3,a2*b2) 
    		return           _mm_unpacklo_epi64(prod01, prod23);   // (ab3,ab2,ab1,ab0)
#endif
	}
	inline vec_uint32x4 operator/(vec_uint32x4 in) {//Always correct
		return (vec_uint32x4)impl::_mm_idiv_epu32<TRUNCATE, BIG, __m128i>(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_uint32x4 in) {
		vec = _mm_add_epi32(vec, in.getVec());
	}
	inline void operator-=(vec_uint32x4 in) {
		vec = _mm_sub_epi32(vec, in.getVec());
	}
	inline void operator*=(vec_uint32x4 in) {
#ifdef SSE41
		vec = _mm_mullo_epi32(vec, in.getVec());
#else
    		__m128i a13 = _mm_shuffle_epi32(vec, 0xF5);            // (-,a3,-,a1)
    		__m128i b13 = _mm_shuffle_epi32(in.getVec(), 0xF5);    // (-,b3,-,b1)
   		__m128i prod02 = _mm_mul_epu32(vec, in.getVec());      // (-,a2*b2,-,a0*b0)
    		__m128i prod13 = _mm_mul_epu32(a13, b13);              // (-,a3*b3,-,a1*b1)
   		__m128i prod01 = _mm_unpacklo_epi32(prod02, prod13);   // (-,-,a1*b1,a0*b0) 
    		__m128i prod23 = _mm_unpackhi_epi32(prod02, prod13);   // (-,-,a3*b3,a2*b2) 
    		vec = _mm_unpacklo_epi64(prod01, prod23);              // (ab3,ab2,ab1,ab0)
#endif
	}
	inline void operator/=(vec_uint32x4 in) {//Always correct
		vec = _impl::_mm_idiv_epu32<TRUNCATE, BIG, __m128i>(vec, in.getVec());
	}
	//Bitwise operator
	inline vec_uint32x4 operator&(vec_uint32x4 in) {
		return (vec_uint32x4)_mm_and_si128(vec, in.getVec());
	}
	inline vec_uint32x4 operator|(vec_uint32x4 in) {
		return (vec_uint32x4)_mm_or_si128(vec, in.getVec());
	}
	inline vec_uint32x4 operator^(vec_uint32x4 in) {
		return (vec_uint32x4)_mm_xor_si128(vec, in.getVec());
	}
	//Bitwise operators, acting on itself
	inline void operator&=(vec_uint32x4 in) {
		vec = _mm_and_si128(vec, in.getVec());
	}
	inline void operator|=(vec_uint32x4 in) {
		vec = _mm_or_si128(vec, in.getVec());
	}
	inline void operator^=(vec_uint32x4 in) {
		vec = _mm_xor_si128(vec, in.getVec());
	}
};
class vec_floatx4 {// float[4]
private:
	using intrin_type = __m128;
	__m128 vec;
public:
	//Constructors
	vec_floatx4(const float* p, constexpr bool align = false) { load<true>(p,align); }
	vec_floatx4(const __m128 v) { load(v); }
	vec_floatx4(const float i) { load(i); }
	vec_floatx4(const float i1, const float i2, const float i3, const float i4) { load(i1,i2,i3,i4); }

	//Misc
	inline __m128 getVec() { return vec; }
	template<bool temporal>
	inline void load(const float* p, constexpr bool align = false) {
		if constexpr (align)
			vec = _mm_load_ps((__m128*)p);
		else
			vec = _mm_lddqu_si128((__m128i*)p);
	}
	template<> inline void load<false>(const float* p, constexpr bool align = true) {
		static_assert(align,"Non temporal loads must be aligned!");
		vec = _mm_stream_load_si128((__m128i*)p);
	}
	inline void load(const __m128 v) { vec = v; }
	inline void load(const float  i) { vec = _mm_set1_ps(i); }
	inline void load(const float i1, const float i2, const float i3, const float i4) { 
		vec = _mm_setr_ps(i1, i2, i3, i4); 
	}
	template<bool aligned, bool temporal> inline void store(const float* p) { static_assert(aligned||temporal,"Non temporal stores must be aligned"); UNREACHABLE; }
	template<> inline void store<true, true> (const float* p) { _mm_store_ps ((__m128*)p, vec); }
	template<> inline void store<false, true>(const float* p) { _mm_storeu_ps((__m128*)p, vec); }
	template<> inline void store<true, false>(const float* p) { _mm_stream_ps((__m128*)p, vec); }

	//Arithmetic operators
	inline vec_floatx4 operator+(vec_floatx4 in){
		return (vec_floatx4)_mm_add_ps(vec, in.getVec());
	}
	inline vec_floatx4 operator-(vec_floatx4 in) {
		return (vec_floatx4)_mm_sub_ps(vec, in.getVec());
	}
	inline vec_floatx4 operator*(vec_floatx4 in) {
		return (vec_floatx4)_mm_mul_ps(vec, in.getVec());
	}
	inline vec_floatx4 operator/(vec_floatx4 in) {//Always correct
		return (vec_floatx4)_mm_div_ps(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_floatx4 in) {
		vec = _mm_add_ps(vec, in.getVec());
	}
	inline void operator-=(vec_floatx4 in) {
		vec = _mm_sub_ps(vec, in.getVec());
	}
	inline void operator*=(vec_floatx4 in) {
		vec = _mm_mul_ps(vec, in.getVec());
	}
	inline void operator/=(vec_floatx4 in) {//Always correct
		vec = _mm_div_ps(vec, in.getVec());
	}
	//Bitwise operator
	inline vec_floatx4 operator&(vec_floatx4 in) {
		return (vec_floatx4)_mm_and_ps(vec, in.getVec());
	}
	inline vec_floatx4 operator|(vec_floatx4 in) {
		return (vec_floatx4)_mm_or_ps(vec, in.getVec());
	}
	inline vec_floatx4 operator^(vec_floatx4 in) {
		return (vec_floatx4)_mm_xor_ps(vec, in.getVec());
	}
	//Bitwise operators, acting on itself
	inline void operator&=(vec_floatx4 in) {
		vec = _mm_and_ps(vec, in.getVec());
	}
	inline void operator|=(vec_floatx4 in) {
		vec = _mm_or_ps(vec, in.getVec());
	}
	inline void operator^=(vec_floatx4 in) {
		vec = _mm_xor_ps(vec, in.getVec());
	}
};
class vec_doublex2 {// double[2]
private:
	using intrin_type = __m128d;
	__m128d vec;
public:
	//Constructors
	vec_doublex2(const double* p, constexpr bool align = false) { load<true>(p,align); }
	vec_doublex2(const __m128d v) { load(v); }
	vec_doublex2(const double i) { load(i); }
	vec_doublex2(const double i1, const double i2) { load(i1,i2); }

	//Misc
	inline __m128d getVec() { return vec; }
	template<bool temporal>
	inline void load(const double* p, constexpr bool align = false) {
		if constexpr (align)
			vec = _mm_load_pd((__m128d*)p);
		else
			vec = _mm_lddqu_si128((__m128i*)p);
	}
	template<> inline void load<false>(const double* p, constexpr bool align = true) {
		static_assert(align,"Non temporal loads must be aligned!");
		vec = _mm_stream_load_si128((__m128d*)p);
	}
	inline void load(const __m128d v) { vec = v; }
	inline void load(const double  i) { vec = _mm_set1_pd(i); }
	inline void load(const double i1, const double i2) { 
		vec = _mm_setr_pd(i1, i2); 
	}
	template<bool aligned, bool temporal> inline void store(const double* p) { static_assert(aligned||temporal,"Non temporal stores must be aligned"); UNREACHABLE; }
	template<> inline void store<true, true> (const double* p) { _mm_store_pd ((__m128d*)p, vec); }
	template<> inline void store<false, true>(const double* p) { _mm_storeu_pd((__m128d*)p, vec); }
	template<> inline void store<true, false>(const double* p) { _mm_stream_pd((__m128d*)p, vec); }

	//Arithmetic operators
	inline vec_doublex2 operator+(vec_doublex2 in){
		return (vec_doublex2)_mm_add_pd(vec, in.getVec());
	}
	inline vec_doublex2 operator-(vec_doublex2 in) {
		return (vec_doublex2)_mm_sub_pd(vec, in.getVec());
	}
	inline vec_doublex2 operator*(vec_doublex2 in) {
		return (vec_doublex2)_mm_mul_pd(vec, in.getVec());
	}
	inline vec_doublex2 operator/(vec_doublex2 in) {//Always correct
		return (vec_doublex2)_mm_div_pd(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_doublex2 in) {
		vec = _mm_add_pd(vec, in.getVec());
	}
	inline void operator-=(vec_doublex2 in) {
		vec = _mm_sub_pd(vec, in.getVec());
	}
	inline void operator*=(vec_doublex2 in) {
		vec = _mm_mul_pd(vec, in.getVec());
	}
	inline void operator/=(vec_doublex2 in) {//Always correct
		vec = _mm_div_pd(vec, in.getVec());
	}
	//Bitwise operator
	inline vec_doublex2 operator&(vec_doublex2 in) {
		return (vec_doublex2)_mm_and_pd(vec, in.getVec());
	}
	inline vec_doublex2 operator|(vec_doublex2 in) {
		return (vec_doublex2)_mm_or_pd(vec, in.getVec());
	}
	inline vec_doublex2 operator^(vec_doublex2 in) {
		return (vec_doublex2)_mm_xor_pd(vec, in.getVec());
	}
	//Bitwise operators, acting on itself
	inline void operator&=(vec_doublex2 in) {
		vec = _mm_and_pd(vec, in.getVec());
	}
	inline void operator|=(vec_doublex2 in) {
		vec = _mm_or_pd(vec, in.getVec());
	}
	inline void operator^=(vec_doublex2 in) {
		vec = _mm_xor_pd(vec, in.getVec());
	}
};
class vec_floatx8 {// float[8]
private:
	using intrin_type = __m256;
	__m256 vec;
public:
	//Constructors
	vec_floatx8(const float* p, constexpr bool align = false) { load<true>(p,align); }
	vec_floatx8(const __m256 v) { load(v); }
	vec_floatx8(const float i) { load(i); }
	vec_floatx8(const float i1, const float i2, const float i3, const float i4, const float i5, const float i6, const float i7, const float i8) { load(i1,i2,i3,i4,i5,i6,i7,i8); }

	//Misc
	inline __m256 getVec() { return vec; }
	template<bool temporal>
	inline void load(const float* p, constexpr bool align = false) {
		if constexpr (align)
			vec = _mm256_load_ps((__m256*)p);
		else
			vec = _mm256_lddqu_si128((__m256*)p);
	}
	template<> inline void load<false>(const float* p, constexpr bool align = true) {
		static_assert(align,"Non temporal loads must be aligned!");
		vec = _mm256_stream_load_si128((__m256*)p);
	}
	inline void load(const __m256 v) { vec = v; }
	inline void load(const float  i) { vec = _mm256_set1_ps(i); }
	inline void load(const float i1, const float i2, const float i3, const float i4, const float i5, const float i6, const float i7, const float i8) { 
		vec = _mm256_setr_ps(i1, i2, i3, i4, i5, i6, i7, i8); 
	}
	template<bool aligned, bool temporal> inline void store(const float* p) { static_assert(aligned||temporal,"Non temporal stores must be aligned"); UNREACHABLE; }
	template<> inline void store<true, true>(const float* p)  { _mm256_store_ps ((__m256*)p, vec); }
	template<> inline void store<false, true>(const float* p) { _mm256_storeu_ps((__m256*)p, vec); }
	template<> inline void store<true, false>(const float* p) { _mm256_stream_ps((__m256*)p, vec); }

	//Arithmetic operators
	inline vec_floatx8 operator+(vec_floatx8 in){
		return (vec_floatx8)_mm256_add_ps(vec, in.getVec());
	}
	inline vec_floatx8 operator-(vec_floatx8 in) {
		return (vec_floatx8)_mm256_sub_ps(vec, in.getVec());
	}
	inline vec_floatx8 operator*(vec_floatx8 in) {
		return (vec_floatx8)_mm256_mul_ps(vec, in.getVec());
	}
	inline vec_floatx8 operator/(vec_floatx8 in) {//Always correct
		return (vec_floatx8)_mm256_div_ps(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_floatx8 in) {
		vec = _mm256_add_ps(vec, in.getVec());
	}
	inline void operator-=(vec_floatx8 in) {
		vec = _mm256_sub_ps(vec, in.getVec());
	}
	inline void operator*=(vec_floatx8 in) {
		vec = _mm256_mul_ps(vec, in.getVec());
	}
	inline void operator/=(vec_floatx8 in) {//Always correct
		vec = _mm256_div_ps(vec, in.getVec());
	}
	//Bitwise operator
	inline vec_floatx8 operator&(vec_floatx8 in) {
		return (vec_floatx8)_mm256_and_ps(vec, in.getVec());
	}
	inline vec_floatx8 operator|(vec_floatx8 in) {
		return (vec_floatx8)_mm256_or_ps(vec, in.getVec());
	}
	inline vec_floatx8 operator^(vec_floatx8 in) {
		return (vec_floatx8)_mm256_xor_ps(vec, in.getVec());
	}
	//Bitwise operators, acting on itself
	inline void operator&=(vec_floatx8 in) {
		vec = _mm256_and_ps(vec, in.getVec());
	}
	inline void operator|=(vec_floatx8 in) {
		vec = _mm256_or_ps(vec, in.getVec());
	}
	inline void operator^=(vec_floatx8 in) {
		vec = _mm256_xor_ps(vec, in.getVec());
	}
};
class vec_doublex4 {// double[4]
private:
	using intrin_type = __m256d;
	__m256d vec;
public:
	//Constructors
	vec_doublex4(const double* p, constexpr bool align = false) { load<true>(p,align); }
	vec_doublex4(const __m256d v) { load(v); }
	vec_doublex4(const double i) { load(i); }
	vec_doublex4(const double i1, const double i2, const double i3, const double i4) { load(i1,i2,i3,i4); }

	//Misc
	inline __m256d getVec() { return vec; }
	template<bool temporal>
	inline void load(const double* p, constexpr bool align = false) {
		if constexpr (align)
			vec = _mm256_load_pd((__m256d*)p);
		else
			vec = _mm256_lddqu_si128((__m256d*)p);
	}
	template<> inline void load<false>(const double* p, constexpr bool align = true) {
		static_assert(align,"Non temporal loads must be aligned!");
		vec = _mm256_stream_load_si128((__m256d*)p);
	}
	inline void load(const __m256d v) { vec = v; }
	inline void load(const double i) { vec = _mm256_set1_pd(i); }
	inline void load(const double i1, const double i2) { 
		vec = _mm256_set_pd(i1, i2); 
	}
	template<bool aligned, bool temporal> inline void store(const double* p) { static_assert(aligned||temporal,"Non temporal stores must be aligned"); UNREACHABLE; }
	template<> inline void store<true, true>(const double* p) { _mm256_store_pd((__m256d*)p,vec); }
	template<> inline void store<false, true>(const double* p) { _mm256_storeu_pd((__m256d*)p, vec); }
	template<> inline void store<true, false>(const double* p) { _mm256_stream_pd((__m256d*)p, vec); }

	//Arithmetic operators
	inline vec_doublex4 operator+(vec_doublex4 in){
		return (vec_doublex4)_mm256_add_pd(vec, in.getVec());
	}
	inline vec_doublex4 operator-(vec_doublex4 in) {
		return (vec_doublex4)_mm256_sub_pd(vec, in.getVec());
	}
	inline vec_doublex4 operator*(vec_doublex4 in) {
		return (vec_doublex4)_mm256_mul_pd(vec, in.getVec());
	}
	inline vec_doublex4 operator/(vec_doublex4 in) {//Always correct
		return (vec_doublex4)_mm256_div_pd(vec, in.getVec());
	}
	//Arithmetic operators, acting on itself
	inline void operator+=(vec_doublex4 in) {
		vec = _mm256_add_pd(vec, in.getVec());
	}
	inline void operator-=(vec_doublex4 in) {
		vec = _mm256_sub_pd(vec, in.getVec());
	}
	inline void operator*=(vec_doublex4 in) {
		vec = _mm256_mul_pd(vec, in.getVec());
	}
	inline void operator/=(vec_doublex4 in) {//Always correct
		vec = _mm256_div_pd(vec, in.getVec());
	}
	//Bitwise operator
	inline vec_doublex4 operator&(vec_doublex4 in) {
		return (vec_doublex4)_mm256_and_pd(vec, in.getVec());
	}
	inline vec_doublex4 operator|(vec_doublex4 in) {
		return (vec_doublex4)_mm256_or_pd(vec, in.getVec());
	}
	inline vec_doublex4 operator^(vec_doublex4 in) {
		return (vec_doublex4)_mm256_xor_pd(vec, in.getVec());
	}
	//Bitwise operators, acting on itself
	inline void operator&=(vec_doublex4 in) {
		vec = _mm256_and_pd(vec, in.getVec());
	}
	inline void operator|=(vec_doublex4 in) {
		vec = _mm256_or_pd(vec, in.getVec());
	}
	inline void operator^=(vec_doublex4 in) {
		vec = _mm256_xor_pd(vec, in.getVec());
	}
};	
#else
#error Your processeor doesn't seem to support SSE instructions. However, they are required in this version of the library for intrinsics!
#endif
}
}
#undef USE_RCP
#undef __RCPf
#undef __RCPt

//https://stackoverflow.com/questions/42442325/how-to-divide-a-m256i-vector-by-an-integer-variable
//TODO: https://stackoverflow.com/questions/12118910/converting-float-vector-to-16-bit-int-without-saturating
//TODO: https://stackoverflow.com/questions/16585639/sse2-instruction-to-typecast-an-integer-register-to-short-register-and-vice-vers
//TODO: 32bit-64bit conversion
//TODO: double-32bit (also __m256)
//TODO: Split files
//TODO: Integer division, 4 new classes
//TODO: Fix comments and makros
//TODO: Add _11 conversion on the top instead of #if
//TODO: https://www.flipcode.com/archives/floattoint.txt

/*************************************************************************
 *                     Makros                                            *
 * - _mm_cvt_[i,u]16x8_2[i,u]32x4_1                                      * _10 | Speed
 * - _mm_cvt_[i,u]16x8_2[i,u]32x4_2                                      * _20,_21 | Speed
 * - _mm_cvt_[i,u]16x8_2[i,u]32x4 and _mm_cvt_[i,u]32x4_2[i,u]16x8       * _1, _2 | SSE4.1-support & speed (benchmark together, roundtrip)
 * - _mm256_cvt_[i,u]32x8_[i,u]16x8 and _mm256_cvt_[i,u]16x8_[i,u]32x8   * _10 | Roundtrip Speed
 * - _mm_cvt_i16x8_2psx4 and _mm_cvt_2psx4_i16x8                         * _10 | Correctness & Roundtrip speed
 * - _mm_cvt_u16x8_2psx4 and _mm_cvt_2psx4_u16x8                         * _10, _20 | Correctness & Roundtrip speed
 * - _mm_cvt_i32x4_psx4                                                  * _1 | Speed
 * - _mm_cvt_u32x4_psx4_fast_huge                                        * _1 | Speed
 * - _mm_cvt_u32x4_psx4_truncate_big                                     * _1 | Speed 
 * - _mm_cvt_u32x4_psx4_truncate_huge                                    * _1,_2 |Speed
 * - _mm_cvt_u32x4_psx4_precise_huge                                     * _1 | Speed
 * - USE_RCP                                                             * true, false for _mm_div_epi16_impl | Has to be correct & speed
 * - _mm_idiv_epi32_precise_int                                          * _mm_idiv_epi32_avx,_mm_idiv_epi32_split | AVX-support & correct & speed (<PRECISE, __m128i>)
 * - _mm_idiv_epi32_precise_float                                        * _mm_idiv_epi32_avx,_mm_idiv_epi32_split | AVX-support & correct & speed (<PRECISE, __m128>)
 * - __RCPf, __RCPt                                                      * 0 to 3, for _mm_idiv_ep[i,u]32_split | speed. Correctness & speed
 *************************************************************************/
