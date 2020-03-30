#if 0
//Gets the ymm Register of a xmm-Registers. Does not compile for some reason...
__m256i get_ymm_of_xmm(__m128i in) {
	__m256i out;
	__asm__(
		"vmovaps   %[in], %%xmm0\n\t"
		"vmovdqa64 %%ymm0, %[out]"
		: [out] "=r" (out)
		: [in] "r" (in)
		: "%xmm0", "%ymm0"
	);
	return out;
}
#endif




#if SSE >= 41
//Converts int16_t[8] (in) to two int32_t[4] (hi, lo)
//Partition: in = |hi|hi|hi|hi|lo|lo|lo|lo|
//Note: The shift+unpak could be replaced with unpack+convert as in https://stackoverflow.com/questions/16031149/speedup-a-short-to-float-cast
ALWAYS_INLINE void _mm_cvt_i16x8_2i32x4_10(const __m128i in, __m128i& lo, __m128i& hi) {
	const __m128i sign = _mm_srai_epi16(in, 16);
	lo = _mm_cvtepi16_epi32(in);
	hi = _mm_unpackhi_epi16(in, sign);
}
//Converts uint16_t[8] (in) to two uint32_t[4] (hi, lo)
//Partition: in = |hi|hi|hi|hi|lo|lo|lo|lo|
ALWAYS_INLINE void _mm_cvt_u16x8_2u32x4_10(const __m128i in, __m128i& lo, __m128i& hi) {
	const __m128i zero = _mm_setzero_si128();
	lo = _mm_cvtepu16_epi32(in);
	hi = _mm_unpackhi_epi16(in, zero);
}
#else
//Converts int16_t[8] (in) to two int32_t[4] (hi, lo)
//Partition: in = |hi|hi|hi|hi|lo|lo|lo|lo|
ALWAYS_INLINE void _mm_cvt_i16x8_2i32x4_10(const __m128i in, __m128i& lo, __m128i& hi) {
	const __m128i sign = _mm_srai_epi16(in, 16);
	hi = _mm_unpackhi_epi16(in, sign);
	lo = _mm_unpacklo_epi16(in, sign);
}
//Converts uint16_t[8] (in) to two uint32_t[4] (hi, lo)
//Partition: in = |hi|hi|hi|hi|lo|lo|lo|lo|
ALWAYS_INLINE void _mm_cvt_u16x8_2u32x4_10(const __m128i in, __m128i& lo, __m128i& hi) {
	const __m128i zero = _mm_setzero_si128();
	hi = _mm_unpackhi_epi16(in, zero);
	lo = _mm_unpacklo_epi16(in, zero);
}
#endif
//Converts two int32_t[8] (in_lo, in_hi) to int16_t[8] using saturation
ALWAYS_INLINE __m128i _mm_cvt_2i32x4_i16x8_10(const __m128i in_lo, const __m128i in_hi) {
	return _mm_packs_epi32(in_lo, in_hi);
}
//Converts two int32_t[8] (in_lo, in_hi) to uint16_t[8] using saturation
//Note: also converts 2xuint32_t[4] to uint16_t[8] correct, if highest bit of elements isn't set
//	If that happens, output will be zero and not uint16_t::max()
ALWAYS_INLINE __m128i _mm_cvt_2u32x4_u16x8_10(const __m128i in_lo, const __m128i in_hi) {
	return _mm_packus_epi32(in_lo, in_hi);
}


//Converts int16_t[8] (in) to two int32_t[4] (hi, lo) 
//Partition: in = |hi|lo|hi|lo|hi|lo|hi|lo|
ALWAYS_INLINE void _mm_cvt_i16x8_2i32x4_20(const __m128i in, __m128i& lo, __m128i& hi) {
	hi = _mm_srai_epi32(in, 16);
	const __m128i out2_tmp = _mm_slli_epi32(in, 16);
	lo = _mm_srai_epi32(out2_tmp, 16);
}
//Converts uint16_t[8] (in) to two uint32_t[4] (out1, out2)
//Partition: in = |hi|lo|hi|lo|hi|lo|hi|lo|
ALWAYS_INLINE void _mm_cvt_u16x8_2u32x4_20(const __m128i in, __m128i& lo, __m128i& hi) {
	out1 = _mm_srli_epi32(in, 16);
	out2 = _mm_and_si128(in, _mm_set1_epi32(0xFF));
}
//Converts two [u]int32_t[4] (in_lo, in_hi) to [u]int16_t[8] using truncation.
ALWAYS_INLINE __m128i _mm_cvt_2i32x4_i16x8_20(const __m128i in_lo, const __m128i in_hi) {
	const __m128i hi_epi32_shift = _mm_slli_epi32(in_hi, 16);
#if SSE >= 41
	return _mm_blend_epi16(in_lo, hi_epi32_shift, 0xAA);
#else
	const __m128i lo_mask = _mm_set1_epi32(0xFFFF);
	const __m128i lo_epi32_mask = _mm_and_si128(in_lo, lo_mask);
	return _mm_or_si128(hi_epi32_shift, lo_epi32_mask);
#endif
}
ALWAYS_INLINE __m128i _mm_cvt_2u32x4_u16x8_20(const __m128i in_lo, const __m128i in_hi) {
	return _mm_cvt_2i32x4_i16x8_20(in_lo, in_hi);
}
//Converts two int32_t[4] (in_lo, in_hi) to int16_t[8]
//If SSE4.1 is available, truncation is used, otherwiese saturation.
ALWAYS_INLINE __m128i _mm_cvt_2i32x4_i16x8_21(const __m128i in_lo, const __m128i in_hi) {
#if SSE >= 41
	const __m128i hi_epi32_shift = _mm_slli_epi32(in_hi, 16);
	return _mm_blend_epi16(in_lo, hi_epi32_shift, 0xAA);
#else
	return _mm_shuffle_epi8(_mm_cvt_2i32x4_i16x8_1(in_lo, in_hi), _mm_setr_epi8(0, 1, 8, 9, 2, 3, 10, 11, 4, 5, 12, 13, 6, 7, 14, 15));
#endif
}
//Converts two uint32_t[4] (in_lo, in_hi) to uint16_t[8]
//If SSE4.1 is available, truncation is used, otherwiese saturation.
//Note: If SSE4.1 is not available, no highest bit of the input should be set or the output will be 0 (see _mm_cvt_2u32x4_u16x8_10)
ALWAYS_INLINE __m128i _mm_cvt_2u32x4_u16x8_21(const __m128i in_lo, const __m128i in_hi) {
#if SSE >= 41
	const __m128i hi_epi32_shift = _mm_slli_epi32(in_hi, 16);
	return _mm_blend_epi16(in_lo, hi_epi32_shift, 0xAA);
#else
	return _mm_shuffle_epi8(_mm_cvt_2u32x4_u16x8_1(in_lo, in_hi), _mm_setr_epi8(0, 1, 8, 9, 2, 3, 10, 11, 4, 5, 12, 13, 6, 7, 14, 15));
#endif
}

//Converts int32_t[8] to int16_t[8] using saturation
ALWAYS_INLINE __m128i _mm256_cvt_i32x8_i16x8_10(__m256i in) {
	_mm_cvt_i16x8_2i32x4_1(_mm256_extractf128_si256(in, 0), _mm256_extractf128_si256(in, 1))
}
//Converts uint32_t[8] (in) to uint16_t[8] using saturation
//Note: If the highest bit of on element of in is set, the corresponding returned element will be 0
//	(see _mm_cvt_u16x8_2u32x4_10)
ALWAYS_INLINE __m128i _mm256_cvt_u32x8_u16x8_10(__m256i in) {
	_mm_cvt_u16x8_2u32x4_1(_mm256_extractf128_si256(in, 0), _mm256_extractf128_si256(in, 1))
}
//Converts int16_t[8] to int32_t[8]
ALWAYS_INLINE __m256i _mm256_cvt_i16x8_i32x8_10(__m128i in) {
	return _mm256_cvtepi16_epi32(in);
}
//Converts uint16_t[8] to uint32_t[8]
//Note: A 16-bit shuffle on the ymm registers of "in" would work as well
ALWAYS_INLINE __m256i _mm256_cvt_u16x8_u32x8_10(__m128i in) {
	return _mm256_cvtepu16_epi32(in);
}