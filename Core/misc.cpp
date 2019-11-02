//This file is for metaprogramming and generating of the other files
//It shouldn't be used in real code
//Quick, dirty and slow

#include <chrono>
#include <string>
#include <iostream>

/* Test, if call really returns expected. If this is not the case, it reports this to std::cout
 */
#define CHECK(call,expected);                                                                                             \
	auto ___out = call;                                                                                                   \
	if(___out != expected){                                                                                               \
		std::cout<<"The check in line "<<__LINE__<<" failed. Expected was "<<expected<<", but we got "<<___out<<std::endl;\
	}


/* Calls the function call n times and measures the time it takes
 * This does not randomize layout or evicts cache
 * 	- call: function to call, can use i to select data, which is declared in it's file
 *	- n: How many times the function should be called
 *	- result: Time for all runs
 *	- T: type of result, e.g. double or std::chrono::nanoseconds
*/
#define BENCH(call,n,T,result);                                      \
	auto ___sta = std::chrono::high_resolution_clock::now();         \
	for (int i = 0; i != n; i++) {                                   \
		call;                                                        \
	}                                                                \
	auto ___sto = std::chrono::high_resolution_clock::now();         \
	result = std::chrono::duration_cast<T>(___sto - ___sta).count();

//Add "#define mak def" to the ofstream ofstr
#define ADD_MAKRO(ofstr, mak, def);\
	ofstr<<"#define "<<mak<<" "<<def<<"\n";

//Add "#inlude "file"" to the ofstream ofstr
#define INC_FILE(ofstr, file);\
	ofstr<<"#include \""<<file<<"\"\n"; 