/*!
 * \file HLibErrorMsg.h
 *
 * \author Juver
 * \date 2020/06/10
 *
 * 
 */

#ifndef HLIBERRORMSG_H_INCLUDED__
#define HLIBERRORMSG_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace _HLIB
{
	std::string GetLastErrorStdStr(DWORD error);
	std::string GetLastErrorStdStr();
};

#endif // HLIBERRORMSG_H_INCLUDED__
