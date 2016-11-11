/*! \file	hash.cpp
	\brief	Re-define hash function for user-defined classes. */
	
#include <algorithm>
	
#include "hash.hpp"

namespace std
{
	size_t hash<geoElement<Line>>::operator()(const geoElement<Line> & g) const
	{
		// Extract maximum and minimum Id
		auto maxId = max(g[0],g[1]);
		auto minId = min(g[0],g[1]);
		
		// Hash value defined through bit-shift operator.
		// Reference: http://cboard.cprogramming.com/c-programming/101067-hash-function-two-numbers.html
		return ((minId << 7) | (maxId << 19));
	}
}


