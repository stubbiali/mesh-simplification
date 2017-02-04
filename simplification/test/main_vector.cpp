/*!	\file	main_vector.cpp
	\brief	A small executable making some tests on STL vectors. */
	
#include <iostream>
#include <vector>
#include <chrono>
#include <array>

#include "inc.hpp"

int main()
{
	using namespace std;
	using namespace std::chrono;
	using namespace geometry;
	
	//
	// Test speed of scan
	//
	
	/*
	{
		UInt N = 1e6;
		vector<UInt> v(N);
		UInt times = 1000;
		high_resolution_clock::time_point start, stop;
						
		// Standard for loop
		for (UInt i = 0; i < N; i++)
			v[i] = i;
			
		start = high_resolution_clock::now();
		for (UInt n = 0; n < times; n++)
		{
			for (UInt i = 0; i < N; i++)
				v[i]++;
		}
		stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop-start).count();
		cout << "Standard for loop, elapsed time: " << duration << " ms" << endl;
		
		// auto 
		for (UInt i = 0; i < N; i++)
			v[i] = i;
		
		start = high_resolution_clock::now();
		for (UInt n = 0; n < times; n++)
		{
			for (auto & el : v)
				el++;
		}
		stop = high_resolution_clock::now();
		duration = duration_cast<milliseconds>(stop-start).count();
		cout << "auto loop, elapsed time:         " << duration << " ms" << endl;
		
		// Iterator
		for (UInt i = 0; i < N; i++)
			v[i] = i;
		
		start = high_resolution_clock::now();
		for (UInt n = 0; n < times; n++)
		{
			for (auto it = v.begin(); it != v.end(); it++)
				(*it)++;
		}
		stop = high_resolution_clock::now();
		duration = duration_cast<milliseconds>(stop-start).count();
		cout << "Iterators, elapsed time:         " << duration << " ms" << endl;
	}
	*/
    
    //
    // Test array
    //
    
    /*
    {
    	array<double,5> v;
    	v = {1,2,3,4,5};
		for (auto el : v)
			cout << el << endl;
    }
    */
    
    {
    	double a(0.);
    	{
    		double c(5.);
    		a = c;
    	}
    	cout << a << endl;
    }
}
