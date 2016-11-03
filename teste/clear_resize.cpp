/*! file clear_resize.cpp
	\brief A simple test on resizing an STL vector.
	
	We want to see whether clearing an STL vector before resizing it 
	affects the run time. */

#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <ctime>

int main() {
	using namespace std;
	using namespace std::chrono;
	
	// Set the vector to copy
	unsigned int N = 1e6;
	vector<double> va(N);
	for (unsigned int i = 0; i < N; i++)
		va[i] = static_cast<double>(i) / static_cast<double>(N);
		
	// Set the destination vector
	vector<double> vb(1);
		
	// Generate random numbers; these will be the vector size
	unsigned int iter = 1e3;
	vector<unsigned int> s(iter);
	srand(time(0));
	for (unsigned int i = 0; i < iter; i++)
		s[i] = rand() % N + 1;
	
	// (Clear and) resize
	const auto tp1 = high_resolution_clock::now();
	for (unsigned int i = 0; i < iter; i++) {
		//vb.clear();
		vb.resize(s[i]);
		copy(va.begin(), va.begin() + s[i], vb.begin());
	}
	const auto tp2 = high_resolution_clock::now();
	const auto d = duration_cast<milliseconds>(tp2 - tp1);  
    std::cout << d.count() << std::endl;
}
