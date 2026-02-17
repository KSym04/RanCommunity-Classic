/*!
 * \file standard_string_convert.h
 *
 * \author Juver
 * \date 2020/04/14
 *
 * 
 */

#ifndef STANDARD_STRING_CONVERT_H_INCLUDED__
#define STANDARD_STRING_CONVERT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace _HLIB
{
	std::wstring s2ws(const std::string& str);
	std::string ws2s(const std::wstring& wstr);
};

#endif // STANDARD_STRING_CONVERT_H_INCLUDED__
