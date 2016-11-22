/*!	\file	test.cpp
	\brief	Various tests. */

#include <iostream>
#include <array>
#include <numeric>

int main()
{
	using namespace std;
	
	//
	// Test shortcut for if statement with multiline commands
	//
	
	{
		int a(0), b(1);
		int c, d(2);
		
		a < b ? c = a, d = b : c = b, d = a;
		
		cout << "c = " << c << ", d = " << d << endl;
	}
	
	//
	// Test STL array constructor
	//
	
	{
		array<int,4> v;
		v.fill(1);
	}
	
	//
	// Test accumulate algorithm
	//
	
	{
		array<int,4> v{2,3,4,5};
		for (int i = 0; i < 4; i++)
			cout << accumulate(v.begin(), v.begin()+i, 1, multiplies<int>()) << endl;
	}
}
