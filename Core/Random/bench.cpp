#include <chrono>
#include <iostream>
#include "fastrand.cpp"

#define N 10000000

//LCG: 23297, 23274, 24545


int main() {
	jlib::random::random_32<jlib::random::LCG2> r(1337);
	volatile int* fill = (int*)malloc(N*sizeof(int));

	auto sta = std::chrono::high_resolution_clock::now();
	for (int i = 0; i != N; i++)
		fill[i] = r.rand();
	auto sto = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(sto - sta).count() << std::endl << std::endl;

	for (int i = 0; i != N; i++)
		std::cout << fill[i];

	return 0;
}