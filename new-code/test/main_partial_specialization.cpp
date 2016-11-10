#include "partial_specialization.hpp"

int main()
{
	// Base classes
	B1 b1;
	B2 b2;
	
	// Derived classes
	D<B1,Type::BASIC> db;
	D<B1,Type::EXTENDED> de;
	
	// Print
	b1.print();
	b2.print();
	db.print();
	de.print();
}
