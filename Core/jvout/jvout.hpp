#pragma once
#include <iostream>        //For cout
#include <unistd.h>        //For size_t
#include <cassert>         //For assert
#include <stdio.h>          //For fwrite, stdout, putchar

#include "../Makros.hpp"   //For STATIC_WARNNING
#include "conversion.hpp"  //For itoa,dtoa,ftoa

#ifdef WINDOWS
#include <Windows.h>       //For WriteConsole
#endif

#define dtoa conversion::dtoa_ryu
#define ftoa conversion::ftoa_ryu

/*
jvout is a class wich handles output to the console
It isn't threadsafe.

It has an internal buffer.
*/
class jvout{
//======================================================================================================
private:
	char *buf_start, *buf_end, *buf_p; //Start and end of buffer as well as the current position
#ifdef WINDOWS
	HANDLE hOut;
	LPDWORD Written;
#define DIRECT_WRITE(p, l); WriteConsole(hOut,p,l,Written,NULL);
#else
#define DIRECT_WRITE(p,l); fwrite_unlocked(p,l,1,stdout); /*Einmal l Bytes*/
#endif

//======================================================================================================
public:
	jvout(size_t size) { // @suppress("Class members should be properly initialized")
		buf_start = (char*)malloc(sizeof(char)*size);
		buf_p = buf_start;
		buf_end = buf_start+size;
#ifdef WINDOWS
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	}

	template<typename Allocator>
	jvout(size_t size, Allocator alloc) { // @suppress("Class members should be properly initialized")
		buf_start = (char*)alloc.malloc(sizeof(char)*size);
		buf_p = buf_start;
		buf_end = buf_start+size;
#ifdef WINDOWS
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	}
//=======================================================================================================

#define WRITE_OVERLOAD(T,C1,C2);\
	inline void write(T obj, bool buffered = true){\
		if constexpr(buffered){\
			C2\
		}\
		else{\
			C1\
		}\
	}

	WRITE_OVERLOAD(std::string,
		DIRECT_WRITE(obj.c_str(),obj.size());,
		1;
	);

	WRITE_OVERLOAD(char,
		putchar(obj);,
		1;
	);

	WRITE_OVERLOAD(uint32_t,
		conversion::u32toa_sse2(obj, buf_p);
		DIRECT_WRITE(buf_start,buf_p-buf_start);
		buf_p = buf_start;,
		1;
	);

	WRITE_OVERLOAD(int32_t,
		conversion::i32toa_sse2(obj, buf_p);
		DIRECT_WRITE(buf_start, buf_p - buf_start);
		buf_p = buf_start; ,
		1;
	);

	WRITE_OVERLOAD(uint64_t,
		conversion::u64toa_sse2(obj, buf_p);
		DIRECT_WRITE(buf_start, buf_p - buf_start);
		buf_p = buf_start; ,
		1;
	);

	WRITE_OVERLOAD(int64_t,
		conversion::i64toa_sse2(obj, buf_p);
		DIRECT_WRITE(buf_start, buf_p - buf_start);
		buf_p = buf_start; ,
		1;
	);

	WRITE_OVERLOAD(float,
		conversion::ftoa(obj, buf_p);
		DIRECT_WRITE(buf_start,buf_p-buf_start);
		buf_p = buf_start;,
		1;
	);

	WRITE_OVERLOAD(double,
		conversion::dtoa(obj, buf_p);
		DIRECT_WRITE(buf_start,buf_p-buf_start);
		buf_p = buf_start;,
		1;
	);
#undef WRITE_OVERLOAD

	template<typename T>//For Arrays
	inline void write(T const*const obj, const size_t N, const bool buffered=true) {
		write('{',buffered);
		for(T* p=obj; p!=obj+N-1; p++){ //No use of AVX!!!
			write(*p,buffered);
			write(',',buffered);
		}
		write(*(obj+N-1),buffered);
		write('}',buffered);
	}

	template<>
	inline void write(char const*const obj, const size_t N, const bool buffered = true){ //Char* sind Ausnahmen, sie lassen sich als Ganzen ausgeben
		if constexpr(buffered){

		}else {
			DIRECT_WRITE(obj,N);
		}
	}

//=======================================================================================================
	inline void update() {//Writes buffer to stdout
		DIRECT_WRITE(buf_start, buf_p-buf_start);
		buf_p = buf_start;
	}
//=======================================================================================================

	template<typename Allocator>
	inline void dealloc(Allocator alloc){
		alloc.free(buf_start);
	}

	inline ~jvout(){
		update();
		free(buf_start);
	}
};
#undef DIRECT_WRITE