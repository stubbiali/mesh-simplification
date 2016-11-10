/*! \file	utility.hpp
	\brief	A set of utilities. */
	
#ifndef HH_UTILITY_HH
#define HH_UTILITY_HH

#include <string>

#include "inc.hpp"

namespace geometry
{
	/*! This class provides utilities which are quite general.
		They are all implementated as static members. */
	class utility
	{
		public:
			/*! Utility to extract the extension of a file.
				\param filename		name of the file
				\return				file extension (if any) */
			static string getFileExtension(const string & filename);
	};
}

#endif
