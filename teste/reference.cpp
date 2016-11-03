/*! \file reference.cpp
	\brief A simple test on references.	*/
	
#include <iostream>

int main() {
	using namespace std;
	
	// Set a, b, c
	double a = 2.3;
	double & b = a;
	double const & c = a;
	
	// Check
	cout << "a is " << a << endl;
	cout << "b is a reference to a; b is " << b << endl;
	cout << "c is a const reference to a; c is " << c << endl << endl;
	
	// Update a
	a += 1.;
	
	// Check
	cout << "a is " << a << endl;
	cout << "b is a reference to a; b is " << b << endl;
	cout << "c is a const reference to a; c is " << c << endl << endl;
	
	// Update b
	b += 2.;
	
	// Check
	cout << "a is " << a << endl;
	cout << "b is a reference to a; b is " << b << endl;
	cout << "c is a const reference to a; c is " << c << endl;
	
	// Something wrong; comment to avoid any abort
	// c += 3.;
}
