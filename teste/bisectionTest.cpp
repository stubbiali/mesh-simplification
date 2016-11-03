#include "bisection.hpp"
#include "mybisection.hpp"
#include <iostream>
#include <functional>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace geometry;

class fun{
	public:
		inline Real evaluate_f(Real const & x) const {return x*x*x;};
		inline Real operator()(Real const & x) const {return x*x*x;};
};

int main() {		
	// Test bisection
	bisection<fun> b;
	fun f;
	const auto tp1 = high_resolution_clock::now();
	for (unsigned int i = 0; i < 1e8; i++) {
		auto y = b.find_zero(f, -2., 3.);
	}
	const auto tp2 = high_resolution_clock::now();
	const auto d = duration_cast<milliseconds>(tp2 - tp1);  
    std::cout << d.count() << std::endl;
    
    // Test mybisection
    mybisection mb;
    const auto mtp1 = high_resolution_clock::now();
	for (unsigned int i = 0; i < 1e8; i++) {
		// auto y = mb.run([](Real const & x){return x*x*x;}, -2., 3.);
		auto y = mb.run(fun(), -2., 3.);
	}
	const auto mtp2 = high_resolution_clock::now();
	const auto md = duration_cast<milliseconds>(mtp2 - mtp1);  
    std::cout << md.count() << std::endl;
}
