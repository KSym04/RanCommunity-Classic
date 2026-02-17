#include "stdafx.h"
#include "./HLibErrorMsg.h"

#include "./Windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace _HLIB
{
	//code from 
	//https://www.codeproject.com/Tips/479880/GetLastError-as-std-string
	//

	// Create a string with last error message
	std::string GetLastErrorStdStr(DWORD error)
	{
		if (error)
		{
			LPVOID lpMsgBuf;
			DWORD bufLen = FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, NULL );
			if (bufLen)
			{
				LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
				std::string result(lpMsgStr, lpMsgStr+bufLen);

				LocalFree(lpMsgBuf);

				return result;
			}
		}
		return std::string();
	}

	// Create a string with last error message
	std::string GetLastErrorStdStr()
	{
		DWORD error = GetLastError();
		if (error)
		{
			LPVOID lpMsgBuf;
			DWORD bufLen = FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, NULL );
			if (bufLen)
			{
				LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
				std::string result(lpMsgStr, lpMsgStr+bufLen);

				LocalFree(lpMsgBuf);

				return result;
			}
		}
		return std::string();
	}
};
