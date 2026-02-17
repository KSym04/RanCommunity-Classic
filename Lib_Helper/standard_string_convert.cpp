#include "stdafx.h"
#include "./standard_string_convert.h"

#include <clocale>
#include <locale>
#include <codecvt>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace  _HLIB
{
	std::wstring s2ws(const std::string& str)
	{
		std::wstring string = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
		return string;
	}

	std::string ws2s(const std::wstring& wstr)
	{
		std::string string = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
		return string;
	}
};