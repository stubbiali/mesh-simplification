#include <istream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "utility.hpp"

namespace geometry
{
	string utility::getFileExtension(const string & filename)
	{
		// Reverse name of the file
		string tmp(filename);
		reverse(tmp.begin(), tmp.end());
				
		// Extract the format
		string format;
		istringstream iss;
		iss.str(tmp);
		if (getline(iss, format, '.'))
		{
			reverse(format.begin(), format.end());
			return format;
		}
		else
			throw runtime_error("The file " + filename + " does not have any extension.");
	}
}
