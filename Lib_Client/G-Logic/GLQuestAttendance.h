/*!
 * \file GLQuestAttendance.h
 *
 * \author Juver
 * \date February 2018
 *
 * 
 */

#ifndef GLQUEST_ATTENDANCE_OPTION_H_
#define GLQUEST_ATTENDANCE_OPTION_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../Lib_Engine/Common/BaseStream.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/SerialFile.h"
#include "../../Lib_Engine/Common/STRINGUTILS.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"

struct GLQUEST_ATTENDANCE_OPTION
{
	enum { VERSION = 0x0001, };

	bool	dummy_var1;
	int		dummy_var2;
	WORD	dummy_var3;
	WORD	dummy_var4;
	DWORD	dummy_var5;
	WORD	dummy_var6;
	WORD	dummy_var7;
	WORD	dummy_var8;
	WORD	dummy_var9;
	WORD	dummy_var10;
	WORD	dummy_var11;

	GLQUEST_ATTENDANCE_OPTION ()
		: dummy_var1(false)
		, dummy_var2(0)
		, dummy_var3(0)
		, dummy_var4(0)
		, dummy_var5(0)
		, dummy_var6(0)
		, dummy_var7(0)
		, dummy_var8(0)
		, dummy_var9(0)
		, dummy_var10(0)
		, dummy_var11(0)
	{
	}

	GLQUEST_ATTENDANCE_OPTION& operator= ( const GLQUEST_ATTENDANCE_OPTION& rvalue );

	bool LOAD ( basestream &SFile );
	bool SAVE ( CSerialFile &SFile );
};


#endif // GLQUEST_ATTENDANCE_OPTION_H_