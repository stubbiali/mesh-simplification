/*! \file	hash.cpp
	\brief	Re-define hash function for user-defined classes. */
	
#include <algorithm>
	
#include "hash.hpp"
#include "boundingBox.hpp"

namespace std
{
	//
	// Specialization for geoElement<Line>
	//
	
	size_t hash<geoElement<Line>>::operator()(const geoElement<Line> & g) const
	{
		// Extract maximum and minimum Id
		auto maxId = max(g[0],g[1]);
		auto minId = min(g[0],g[1]);
		
		// Hash value defined through bit-shift operator
		// Reference: http://cboard.cprogramming.com/c-programming/101067-hash-function-two-numbers.html
		return ((minId << 7) | (maxId << 19));
	}
	
	
	//
	// Specialization for collapseInfo
	//
	
	size_t hash<collapseInfo>::operator()(const collapseInfo & cInfo) const
	{
		// Extract maximum and minimum Id
		auto maxId = max(cInfo.getId1(), cInfo.getId2());
		auto minId = min(cInfo.getId1(), cInfo.getId2());
		
		// Hash value defined through bit-shift operator
		// Reference: http://cboard.cprogramming.com/c-programming/101067-hash-function-two-numbers.html
		return ((minId << 7) | (maxId << 19));
	}
}


