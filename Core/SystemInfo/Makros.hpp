#ifndef MAKROS_H
#define MAKROS_H

//First, get more handy Compiler-Macros
#if defined(__GNUC__)
#define GCC __GNUC__
#define COMP_VER_MINOR __GNUC_MINOR__
#elif defined(__clang__)
#define CLANG __clang__
#define COMP_VER_MINOR __clang_minor__
#elif defined(_MSC_VER)
#define VS _MSC_VER
#define COMP_VER_MINOR _MSC_FULL_VER /*There doesn't exist a Makro that just hold the minor version*/
#endif
#if (not defined(GCC)) && (not defined(CLANG)) && (not defined(VS))
#error You are using an unsupported compiler; neither GCC, nor CLANG or VS was detected!
#endif

//Next, detect the system
#if defined(_WIN32)
#define WINDOWS _WIN32
#elif defined(__linux__) || defined(__CYGWIN__)
#define LINUX __linux__
#elif defined(__APPLE__)
#define APPLE __APPLE__
#error Apple sucks! I refuse to make my library work with an Apple product!
#endif
#if (not defined(WINDOWS)) && (not defined(LINUX)) && (not defined(APPLE))
#error You are using an unsupported platform; neither Windows, nor Linux or Apple was detected!
#endif

//Next, detect the CPU-Vendor
#if defined(GCC) || defined(CLANG)
#if defined(__amd64__) || defined(__amd64)
#define AMD
#elif defined(__arm__) || defined (__aarch64__)
#define ARM
#elif defined(i386)||defined(__i386)||defined(__i386)
#define INTEL
#endif
#else //VS
#if defined(_M_AMD64)
#define AMD
#elif defined(_M_ARM)
#define ARM
#elif defined(_M_IX86)
#define INTEL
#endif
#endif

//Detect 32-/64-Bit
#include <stdint.h>
#if UINTPTR_MAX == 0xffffffff
#define BIT 32
#elif UINTPTR_MAX == 0xffffffffffffffff
#define BIT 64
#else
#error Wtf, this is neither 32-Bit nor 64-Bit
#endif

//Which SIMD extensions are supported?
#ifdef __SSE__ //SSE
#define SSE 1
#endif
#ifdef __SSE2__
#undef SSE
#define SSE 2
#endif
#ifdef __SSE3__
#undef SSE
#define SSE 3
#endif
#ifdef __SSSE3__
#undef SSE
#define SSE 35
#endif
#ifdef __SSE4_1__
#undef SSE
#define SSE 41
#endif
#ifdef __SSE4_2__
#undef SSE
#define SSE 42
#endif

#ifdef __AVX__  //AVX
#define AVX 1
#endif
#ifdef __AVX2__
#define AVX 2
#endif
#ifdef __AVX512F__
#define AVX512
#endif

#ifdef __FMA__ //FMA
#define FMA 1
#endif
#ifdef __FMA4__
#undef FMA
#define FMA 4
#endif

#ifdef __BMI__ //BMI
#define BMI 1
#endif
#ifdef __BMI2__
#undef BMI
#define BMI 2
#endif

#ifdef __TBM__ //TBM (worthless, just on AMD)
#define TBM
#endif

#ifdef __ARM_NEON //Arm Neon
#define NEON
#endif

#endif
