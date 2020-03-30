//No hack floats because of sign bit.
//Possibilities for int16_t->float using hack-floats: (all too slow)
//1.: new=And old,0b011...1    | con=sign constant,old                            | unpack like for unsigned
//2.: new=And old,0b011...1    | res=xor old,new         | con=xor res,constant   |             ""
//3.: sgn=_bittestandreset old | sgnsh=shift sgn,31      | con=xor sgnsh,constant |             ""
//Possibilities for float->int16_t using hack-floats: (all too slow)
//1.: use _mm_cvtpsx4_u32x4<SMALL> for 16 bit, transfer sign using _mm_sign_epi32, then use _mm_cvt2i32x4_i16x8.

//Converts int16_t[8](in) to two float[4](lo, hi)
//It first converts to two int32_t[4] and then to float
ALWAYS_INLINE void _mm_cvt_i16x8_2psx4_1(__m128i in, __m128& lo, __m128& hi) {
	//0.: Convert to epi32
	__m128i lo32, hi32;
	_mm_cvti16x8_2i32x4(in, lo32, hi32);

	//1.: Convert to float
	lo = _mm_cvtepi32_ps<SMALL>(lo32); //TODO
	hi = _mm_cvtepi32_ps<SMALL>(hi32); //TODO
}
//Converts int16_t[8](in) to two float[4](lo, hi)
ALWAYS_INLINE void _mm_cvt_i16x8_2psx4_2(__m128i in, __m128& lo, __m128& hi) {
	//0.: Prepare magic constants
	const __m128i magicInt = _mm_sign_epi16(_mm_set1_epi16(0x4B00), in); //Exponent of 23+127
	const __m128  magicFloat = _mm_set1_ps(8388608.0f); //2^23

	//1.: Widen to 32bit
	const __m128i in_pos = _mm_sign_epi16(in,in);
	const __m128i in_lo  = _mm_unpacklo_epi16(in_pos, magicInt);
	const __m128i in_hi  = _mm_unpackhi_epi16(in_pos, magicInt);

	//2.: "Convert" to float
	lo = _mm_sub_ps(_mm_castsi128_ps(in_lo), magicFloat);
	hi = _mm_sub_ps(_mm_castsi128_ps(in_hi), magicFloat);
}
//Converts two float[4](lo, hi) to int16_t[8]
//First converts to two int32_t[4] using the specified rounding mode and then to int16_t
template<int round = PRECISE>
ALWAYS_INLINE __m128i _mm_cvt_2psx4_i16x8_10(__m128 lo, __m128 hi) {
	//0.: Check template parameters
	static_assert(round == FAST || round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_cvt_2psx4_i16x8_10 has to be either FAST(0), TRUNCATE(1) or PRECISE(2)!");

	//1.: Convert to epi32
	__m128i hi_epi32, lo_epi32;
	if constexpr (round == TRUNCATE) {
		lo_epi32 = _mm_cvttps_epi32<SMALL>(lo); //TODO
		hi_epi32 = _mm_cvttps_epi32<SMALL>(hi); //TODO
	}
	else {
		lo_epi32 = _mm_cvtps_epi32<SMALL>(lo); //TODO
		hi_epi32 = _mm_cvtps_epi32<SMALL>(hi); //TODO
	}

	//2.: Convert to epi16
	return _mm_cvt2i32x4_i16x8(lo_epi32, hi_epi32);
}




//-------------------------------------------------------------------------------------------------------------------------------
//Converts uint16_t[8](in) to two float[4](lo, hi)
	//It first converts to two uint32_t[4] and then to float
ALWAYS_INLINE void _mm_cvt_u16x8_2psx4_10(__m128i in, __m128& lo, __m128& hi) {
	//1.: Convert to epu32
	__m128i lo32, hi32;
	_mm_cvtu16x8_2u32x4(in, lo32, hi32);

	//2.: Convert to float
	lo = _mm_cvtepu32_ps<SMALL>(lo32); //TODO
	hi = _mm_cvtepu32_ps<SMALL>(hi32); //TODO
}
//Converts two float[4](lo, hi) to uint16_t[8]
//First converts to two uint32_t[4] using the specified rounding mode and then to uint16_t
template<int round = PRECISE>
ALWAYS_INLINE __m128i _mm_cvt_2psx4_u16x8_10(__m128 lo, __m128 hi) {
	//0.: Check template parameters
	static_assert(round == FAST || round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_cvt_2psx4_u16x8_10 has to be either FAST(0), TRUNCATE(1) or PRECISE(2)!");

	//1.: Convert to epu32
	__m128i hi_epu32, lo_epu32;
	if constexpr (round == TRUNCATE) {
		lo_epu32 = _mm_cvttps_epu32<SMALL>(lo); //TODO
		hi_epu32 = _mm_cvttps_epu32<SMALL>(hi); //TODO
	}
	else {
		lo_epu32 = _mm_cvtps_epu32<SMALL>(lo); //TODO
		hi_epu32 = _mm_cvtps_epu32<SMALL>(hi); //TODO
	}

	//2.: Convert to epu16
	return _mm_cvt2u32x4_u16x8(lo_epu32, hi_epu32);
}

//Note: From now on: Hacking together floats :)

//Converts uint16_t[8](in) to two float[4](lo, hi)
//It does not really use an intermediate uint32_t representation but rather two hacky magic numbers. It uses the pattern of _mm_cvt2u32x4_u16x8_1
//Note: Correctness has to be verified
//Note: From https://stackoverflow.com/questions/9161807/sse-convert-short-integer-to-float
ALWAYS_INLINE void _mm_cvt_u16x8_2psx4_20(__m128i in, __m128& lo, __m128& hi) {
	//0.: Prepare magic constants
	const __m128i magicInt = _mm_set1_epi16(0x4B00); //Exponent of 23+127
	const __m128  magicFloat = _mm_set1_ps(8388608.0f); //2^23

	//1.: Widen to 32bit
	__m128i in_lo = _mm_unpacklo_epi16(in, magicInt);
	__m128i in_hi = _mm_unpackhi_epi16(in, magicInt);

	//2.: "Convert" to float
	lo = _mm_sub_ps(_mm_castsi128_ps(in_lo), magicFloat);
	hi = _mm_sub_ps(_mm_castsi128_ps(in_hi), magicFloat);
}
//Converts two float[4](lo, hi) to uint16_t[8]
//As _mm_cvt_2psx4_u16x8_10 but has to use _mm_cvt2u32x4_u16x8_1, because its widening pattern was used
template<int round = PRECISE>
ALWAYS_INLINE __m128i _mm_cvt_2psx4_u16x8_20(__m128 lo, __m128 hi) {
	//0.: Check template parameters
	static_assert(round == FAST || round == PRECISE || round == TRUNCATE, "The rounding-mode for _mm_cvt_2psx4_u16x8_20 has to be either FAST(0), TRUNCATE(1) or PRECISE(2)!");

	//1.: Convert to epi32
	__m128i hi_epu32, lo_epu32;
	if constexpr (round == TRUNCATE) {
		hi_epi32 = _mm_cvttps_epu32<SMALL>(hi); //TODO
		lo_epi32 = _mm_cvttps_epu32<SMALL>(lo); //TODO
	}
	else {
		hi_epi32 = _mm_cvtps_epu32<SMALL>(hi); //TODO
		lo_epi32 = _mm_cvtps_epu32<SMALL>(lo); //TODO
	}

	//2.: Convert to epu16
	return _mm_cvt2u32x4_u16x8_1(lo, hi);
}