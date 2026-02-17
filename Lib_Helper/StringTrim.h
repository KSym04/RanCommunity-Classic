/*!
 * \file StringTrim.h
 *
 * \author Juver
 * \date 2020/12/04
 *
 * 
 */

#ifndef STRINGTRIM_H_INCLUDED__
#define STRINGTRIM_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace _HLIB
{
	void ltrim(std::string &s);
	void rtrim(std::string &s);
	void trim(std::string &s);
	std::string ltrim_copy(std::string s);
	std::string rtrim_copy(std::string s);
};


#endif // STRINGTRIM_H_INCLUDED__
