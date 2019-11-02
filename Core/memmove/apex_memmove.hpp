#ifndef __APEX_MEMMOVE_H__
#define __APEX_MEMMOVE_H__

#include "../SystemInfo/Makros.hpp"

//	apex_memmove written by Trevor Herselman in 2014

//	FORCE `CDECL` calling convention on 32-bit builds on our function pointers, because we need it to match the original `std::memmove` definition; in-case the user specified a different default function calling convention! (I specified __fastcall as my default calling convention and got errors! So I needed to add this!)
#if BIT==32 && defined(INTEL)
	#if defined(VS)
		#define APEXCALL __cdecl						//	32-bit on Visual Studio
	#else
		#define APEXCALL __attribute__((__cdecl__))		//	32-bit on GCC / LLVM (Clang)
	#endif
#else
	#define APEXCALL									//	64-bit - __fastcall is default on 64-bit!
#endif

#ifdef __cplusplus

#include <cstddef>		//	for `size_t`

namespace apex
{
	extern void *(APEXCALL *memcpy)( void *dst, const void *src, size_t size );
	extern void *(APEXCALL *memmove)( void *dst, const void *src, size_t size );
}

#define apex_memcpy apex::memcpy
#define apex_memmove apex::memmove

#else

#include <stddef.h>		//	ANSI/ISO C	-	for `size_t`

extern void *(APEXCALL *apex_memcpy)( void *dst, const void *src, size_t size );
extern void *(APEXCALL *apex_memmove)( void *dst, const void *src, size_t size );

#endif	// __cplusplus

#endif	// __APEX_MEMMOVE_H__