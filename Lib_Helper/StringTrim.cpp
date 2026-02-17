#include "stdafx.h"
#include "./StringTrim.h"

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace _HLIB
{
	//code https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
	//answer from Evan Teran

	// trim from start (in place)
	void ltrim(std::string &s) 
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	}

	// trim from end (in place)
	void rtrim(std::string &s) 
	{
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	}

	// trim from both ends (in place)
	void trim(std::string &s)
	{
		ltrim(s);
		rtrim(s);
	}

	// trim from start (copying)
	std::string ltrim_copy(std::string s) 
	{
		ltrim(s);
		return s;
	}

	// trim from end (copying)
	std::string rtrim_copy(std::string s) 
	{
		rtrim(s);
		return s;
	}
};
