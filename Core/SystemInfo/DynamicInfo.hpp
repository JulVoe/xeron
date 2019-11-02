#ifndef DYNAMIC_INFO_H
#define DYNAMIC_INFO_H
#include "Makros.hpp"

/* %ecx */
#define bit_SSE3	(1 << 0)
#define bit_PCLMUL	(1 << 1)
#define bit_LZCNT	(1 << 5)
#define bit_SSSE3	(1 << 9)
#define bit_FMA		(1 << 12)
#define bit_CMPXCHG16B	(1 << 13)
#define bit_SSE4_1	(1 << 19)
#define bit_SSE4_2	(1 << 20)
#define bit_MOVBE	(1 << 22)
#define bit_POPCNT	(1 << 23)
#define bit_AES		(1 << 25)
#define bit_XSAVE	(1 << 26)
#define bit_OSXSAVE	(1 << 27)
#define bit_AVX		(1 << 28)
#define bit_F16C	(1 << 29)
#define bit_RDRND	(1 << 30)

/* %edx */
#define bit_CMPXCHG8B	(1 << 8)
#define bit_CMOV	(1 << 15)
#define bit_MMX		(1 << 23)
#define bit_FXSAVE	(1 << 24)
#define bit_SSE		(1 << 25)
#define bit_SSE2	(1 << 26)

/* Extended Features */
/* %ecx */
#define bit_LAHF_LM	(1 << 0)
#define bit_ABM		(1 << 5)
#define bit_SSE4a	(1 << 6)
#define bit_PRFCHW	(1 << 8)
#define bit_XOP         (1 << 11)
#define bit_LWP 	(1 << 15)
#define bit_FMA4        (1 << 16)
#define bit_TBM         (1 << 21)
#define bit_MWAITX      (1 << 29)

/* %edx */
#define bit_MMXEXT	(1 << 22)
#define bit_LM		(1 << 29)
#define bit_3DNOWP	(1 << 30)
#define bit_3DNOW	(1 << 31)

/* Extended Features (%eax == 7) */
/* %ebx */
#define bit_FSGSBASE	(1 << 0)
#define bit_BMI	(1 << 3)
#define bit_HLE	(1 << 4)
#define bit_AVX2	(1 << 5)
#define bit_BMI2	(1 << 8)
#define bit_RTM	(1 << 11)
#define bit_MPX	(1 << 14)
#define bit_AVX512F	(1 << 16)
#define bit_AVX512DQ	(1 << 17)
#define bit_RDSEED	(1 << 18)
#define bit_ADX	(1 << 19)
#define bit_AVX512IFMA	(1 << 21)
#define bit_PCOMMIT	(1 << 22)
#define bit_CLFLUSHOPT	(1 << 23)
#define bit_CLWB	(1 << 24)
#define bit_AVX512PF	(1 << 26)
#define bit_AVX512ER	(1 << 27)
#define bit_AVX512CD	(1 << 28)
#define bit_SHA		(1 << 29)
#define bit_AVX512BW	(1 << 30)
#define bit_AVX512VL	(1 << 31)

/* %ecx */
#define bit_PREFETCHWT1	  (1 << 0)
#define bit_AVX512VBMI	(1 << 1)

/* XFEATURE_ENABLED_MASK register bits (%eax == 13, %ecx == 0) */
#define bit_BNDREGS     (1 << 3)
#define bit_BNDCSR      (1 << 4)

/* Extended State Enumeration Sub-leaf (%eax == 13, %ecx == 1) */
#define bit_XSAVEOPT	(1 << 0)
#define bit_XSAVEC	(1 << 1)
#define bit_XSAVES	(1 << 3)

/* Signatures for different CPU implementations as returned in uses
   of cpuid with level 0.  */
#define signature_AMD_ebx	0x68747541
#define signature_AMD_ecx	0x444d4163
#define signature_AMD_edx	0x69746e65

#define signature_CENTAUR_ebx	0x746e6543
#define signature_CENTAUR_ecx	0x736c7561
#define signature_CENTAUR_edx	0x48727561

#define signature_CYRIX_ebx	0x69727943
#define signature_CYRIX_ecx	0x64616574
#define signature_CYRIX_edx	0x736e4978

#define signature_INTEL_ebx	0x756e6547
#define signature_INTEL_ecx	0x6c65746e
#define signature_INTEL_edx	0x49656e69

#define signature_TM1_ebx	0x6e617254
#define signature_TM1_ecx	0x55504361
#define signature_TM1_edx	0x74656d73

#define signature_TM2_ebx	0x756e6547
#define signature_TM2_ecx	0x3638784d
#define signature_TM2_edx	0x54656e69

#define signature_NSC_ebx	0x646f6547
#define signature_NSC_ecx	0x43534e20
#define signature_NSC_edx	0x79622065

#define signature_NEXGEN_ebx	0x4778654e
#define signature_NEXGEN_ecx	0x6e657669
#define signature_NEXGEN_edx	0x72446e65

#define signature_RISE_ebx	0x65736952
#define signature_RISE_ecx	0x65736952
#define signature_RISE_edx	0x65736952

#define signature_SIS_ebx	0x20536953
#define signature_SIS_ecx	0x20536953
#define signature_SIS_edx	0x20536953

#define signature_UMC_ebx	0x20434d55
#define signature_UMC_ecx	0x20434d55
#define signature_UMC_edx	0x20434d55

#define signature_VIA_ebx	0x20414956
#define signature_VIA_ecx	0x20414956
#define signature_VIA_edx	0x20414956

#define signature_VORTEX_ebx	0x74726f56
#define signature_VORTEX_ecx	0x436f5320
#define signature_VORTEX_edx	0x36387865

#define __cpuid(level, a, b, c, d)			\
  __asm__ ("cpuid\n\t"					\
	   : "=a" (a), "=b" (b), "=c" (c), "=d" (d)	\
	   : "0" (level))

#define __cpuid_count(level, count, a, b, c, d)		\
  __asm__ ("cpuid\n\t"					\
	   : "=a" (a), "=b" (b), "=c" (c), "=d" (d)	\
	   : "0" (level), "2" (count))


   /* Return highest supported input value for cpuid instruction.  ext can
	  be either 0x0 or 0x8000000 to return highest supported value for
	  basic or extended cpuid information.  Function returns 0 if cpuid
	  is not supported or whatever cpuid returns in eax register.  If sig
	  pointer is non-null, then first four bytes of the signature
	  (as found in ebx register) are returned in location pointed by sig.  */

static __inline unsigned int
__get_cpuid_max(unsigned int __ext, unsigned int *__sig)
{
	unsigned int __eax, __ebx, __ecx, __edx;

#ifndef __x86_64__
	/* See if we can use cpuid.  On AMD64 we always can.  */
#if GCC >= 3
	__asm__("pushf{l|d}\n\t"
		"pushf{l|d}\n\t"
		"pop{l}\t%0\n\t"
		"mov{l}\t{%0, %1|%1, %0}\n\t"
		"xor{l}\t{%2, %0|%0, %2}\n\t"
		"push{l}\t%0\n\t"
		"popf{l|d}\n\t"
		"pushf{l|d}\n\t"
		"pop{l}\t%0\n\t"
		"popf{l|d}\n\t"
		: "=&r" (__eax), "=&r" (__ebx)
		: "i" (0x00200000));
#else
/* Host GCCs older than 3.0 weren't supporting Intel asm syntax
   nor alternatives in i386 code.  */
	__asm__("pushfl\n\t"
		"pushfl\n\t"
		"popl\t%0\n\t"
		"movl\t%0, %1\n\t"
		"xorl\t%2, %0\n\t"
		"pushl\t%0\n\t"
		"popfl\n\t"
		"pushfl\n\t"
		"popl\t%0\n\t"
		"popfl\n\t"
		: "=&r" (__eax), "=&r" (__ebx)
		: "i" (0x00200000));
#endif

	if (!((__eax ^ __ebx) & 0x00200000))
		return 0;
#endif

	/* Host supports cpuid.  Return highest supported cpuid input value.  */
	__cpuid(__ext, __eax, __ebx, __ecx, __edx);

	if (__sig)
		*__sig = __ebx;

	return __eax;
}

/* Return cpuid data for requested cpuid level, as found in returned
   eax, ebx, ecx and edx registers.  The function checks if cpuid is
   supported and returns 1 for valid cpuid information or 0 for
   unsupported cpuid level.  All pointers are required to be non-null.  */

static __inline int
__get_cpuid(unsigned int __level,
	unsigned int *__eax, unsigned int *__ebx,
	unsigned int *__ecx, unsigned int *__edx)
{
	unsigned int __ext = __level & 0x80000000;

	if (__get_cpuid_max(__ext, 0) < __level)
		return 0;

	__cpuid(__level, *__eax, *__ebx, *__ecx, *__edx);
	return 1;
}

static __inline int
__get_cpuid_count(unsigned int __level, unsigned int count,
	unsigned int *__eax, unsigned int *__ebx,
	unsigned int *__ecx, unsigned int *__edx)
{
	unsigned int __ext = __level & 0x80000000;

	if (__get_cpuid_max(__ext, 0) < __level)
		return 0;

	__cpuid_count(__level,count, *__eax, *__ebx, *__ecx, *__edx);
	return 1;
}

namespace sysinfo {
	bool supportsSSE1() {
		unsigned int id[4];
		__get_cpuid(1, &id[0], &id[1], &id[2], &id[3]);
		return id[3] & bit_SSE;
	}
	bool supportsSSE2() {
		unsigned int id[4];
		__get_cpuid(1, &id[0], &id[1], &id[2], &id[3]);
		return id[3] & bit_SSE2;
	}
	bool supportsSSE3() {
		unsigned int id[4];
		__get_cpuid(1, &id[0], &id[1], &id[2], &id[3]);
		return id[2] & bit_SSE3;
	}
	bool supportsSSSE3() {
		unsigned int id[4];
		__get_cpuid(1, &id[0], &id[1], &id[2], &id[3]);
		return id[2] & bit_SSSE3;
	}
	bool supportsSSE4_1() {
		unsigned int id[4];
		__get_cpuid(1, &id[0], &id[1], &id[2], &id[3]);
		return id[2] & bit_SSE4_1;
	}
	bool supportsSSE4_2() {
		unsigned int id[4];
		__get_cpuid(1, &id[0], &id[1], &id[2], &id[3]);
		return id[2] & bit_SSE4_2;
	}
	bool supportsAVX() {
		unsigned int id[4];
		__get_cpuid(1, &id[0], &id[1], &id[2], &id[3]);
		return id[2] & bit_AVX;
	}
	bool supportsAVX2() {
		unsigned int id[4];
		__get_cpuid_count(7, 0, &id[0], &id[1], &id[2], &id[3]);
		return id[1] & bit_AVX2;
	}
	bool supportsAVX512() {
		unsigned int id[4];
		__get_cpuid_count(7, 0, &id[0], &id[1], &id[2], &id[3]);
		return id[1] & (bit_AVX512F^bit_AVX512DQ^bit_AVX512IFMA^bit_AVX512PF^bit_AVX512ER^bit_AVX512CD^bit_AVX512BW^bit_AVX512VL);
	}
	bool supportsFMA() {
		unsigned int id[4];
		__get_cpuid(1, &id[0], &id[1], &id[2], &id[3]);
		return id[2] & bit_FMA;
	}
	bool supportsFMA4() {
		unsigned int id[4];
		__get_cpuid(80000001, &id[0], &id[1], &id[2], &id[3]);
		return id[2] & bit_FMA4;
	}
	bool supportsBMI() {
		unsigned int id[4];
		__get_cpuid_count(7, 0, &id[0], &id[1], &id[2], &id[3]);
		return id[1] & bit_BMI;
	}
	bool supportsBMI2() {
		unsigned int id[4];
		__get_cpuid_count(7, 0, &id[0], &id[1], &id[2], &id[3]);
		return id[1] & bit_BMI2;
	}
	int getCpuVendor() {
#define VENDOR_INTEL 0x0
#define VENDOR_AMD 0x1
#define VENDOR_WTF -1
		unsigned int id[4];
		__get_cpuid(0, &id[0], &id[1], &id[2], &id[3]);
		if (id[1] == signature_INTEL_ebx && id[2] == signature_INTEL_ecx && id[3] == signature_INTEL_edx)
			return VENDOR_INTEL;
		else if (id[1] == signature_AMD_ebx && id[2] == signature_AMD_ecx && id[3] == signature_AMD_edx)
			return VENDOR_AMD;
		else
			return VENDOR_WTF;
	}
	int getCpuArchitecture() {
#define ARCT_Atom 0x0
#define ARCT_Nehalem 0x1
#define ARCT_IvyBridge 0x2
#define ARCT_Skylake 0x3
#define ARCT_WTF 0x4
		unsigned int id[4];
		__get_cpuid(0, &id[0], &id[1], &id[2], &id[3]);
		switch (*id) {
		case 0x0A:
			return ARCT_Atom;
			break;
		case 0x0B:
			return ARCT_Nehalem;
			break;
		case 0x0D:
			return ARCT_IvyBridge;
			break;
		case 0x16:
			return ARCT_Skylake;
			break;
		default:
			return ARCT_WTF;
		}
	}
}

// Returns the cache line size (in bytes) of the processor, or 0 on failure

#include <stddef.h>
size_t cache_line_size();

#ifdef(APPLE)
#include <sys/sysctl.h>
size_t cache_line_size() {
	size_t line_size = 0;
	size_t sizeof_line_size = sizeof(line_size);
	sysctlbyname("hw.cachelinesize", &line_size, &sizeof_line_size, 0, 0);
	return line_size;
}

#elif defined(WINDOWS)
#include <stdlib.h>
#include <windows.h>
size_t cache_line_size() {
	size_t line_size = 0;
	DWORD buffer_size = 0;
	DWORD i = 0;
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION * buffer = 0;

	GetLogicalProcessorInformation(0, &buffer_size);
	buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
	GetLogicalProcessorInformation(&buffer[0], &buffer_size);

	for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
		if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) {
			line_size = buffer[i].Cache.LineSize;
			break;
		}
	}

	free(buffer);
	return line_size;
}
#elif defined(LINUX)
#include <stdio.h>
size_t cache_line_size() {
	FILE * p = 0;
	p = fopen("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size", "r");
	unsigned int i = 0;
	if (p) {
		fscanf(p, "%d", &i);
		fclose(p);
	}
	return i;
}
#endif

//Returns the number of cpu cores and threads
unsigned get_number_of_threads() {
	return std::thread::hardware_concurrency();
}

#endif
