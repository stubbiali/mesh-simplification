/*!	\file	utility.cpp
	\brief	Implementations of static members of class utility. */
	
#include <istream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "utility.hpp"

namespace geometry
{
	string utility::getFileExtension(string filename)
	{
		// Reverse name of the file
		reverse(filename.begin(), filename.end());
				
		// Extract the format
		string format;
		istringstream iss;
		iss.str(filename);
		if (getline(iss, format, '.'))
		{
			reverse(format.begin(), format.end());
			return format;
		}
		else
		{
			reverse(filename.begin(), filename.end());
			throw runtime_error("The file " + filename + " does not have any extension.");
		}
	}
}
