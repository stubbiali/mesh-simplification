/*! \file	utility.hpp
	\brief	A set of utilities. */
	
#ifndef HH_UTILITY_HH
#define HH_UTILITY_HH

#include <string>

#include "inc.hpp"

namespace utility
{
	using namespace std;
	
	/*! Utility to extract the extension of a file.
		\param filename		name of the file
		\return				file extension (if any) */
	string getFileExtension(string filename);
}

#endif
