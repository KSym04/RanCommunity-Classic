#include "stdafx.h"
#include "./GLQuestAttendance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLQUEST_ATTENDANCE_OPTION& GLQUEST_ATTENDANCE_OPTION::operator= ( const GLQUEST_ATTENDANCE_OPTION& rvalue )
{
	dummy_var1		= rvalue.dummy_var1;
	dummy_var2		= rvalue.dummy_var2;
	dummy_var3		= rvalue.dummy_var3;
	dummy_var4		= rvalue.dummy_var4;
	dummy_var5		= rvalue.dummy_var5;
	dummy_var6		= rvalue.dummy_var6;
	dummy_var7		= rvalue.dummy_var7;
	dummy_var8		= rvalue.dummy_var8;
	dummy_var9		= rvalue.dummy_var9;
	dummy_var10		= rvalue.dummy_var10;
	dummy_var11		= rvalue.dummy_var11;

	return *this;
}

bool GLQUEST_ATTENDANCE_OPTION::LOAD ( basestream &SFile )
{
	DWORD dwVER, dwSIZE;
	SFile >> dwVER;
	SFile >> dwSIZE;

	if ( dwVER==VERSION )
	{
		SFile >> dummy_var1;
		SFile >> dummy_var2;
		SFile >> dummy_var3;
		SFile >> dummy_var4;
		SFile >> dummy_var5;
		SFile >> dummy_var6;
		SFile >> dummy_var7;
		SFile >> dummy_var8;
		SFile >> dummy_var9;
		SFile >> dummy_var10;
		SFile >> dummy_var11;
	}
	else
	{
		CDebugSet::ErrorVersion( "GLQUEST_ATTENDANCE_OPTION::LOAD", dwVER );
		SFile.SetOffSet ( SFile.GetfTell()+dwSIZE );
	}

	return true;
}

bool GLQUEST_ATTENDANCE_OPTION::SAVE ( CSerialFile &SFile )
{
	SFile << (DWORD) VERSION;
	SFile << (DWORD) sizeof( GLQUEST_ATTENDANCE_OPTION );
	
	SFile << dummy_var1;
	SFile << dummy_var2;
	SFile << dummy_var3;
	SFile << dummy_var4;
	SFile << dummy_var5;
	SFile << dummy_var6;
	SFile << dummy_var7;
	SFile << dummy_var8;
	SFile << dummy_var9;
	SFile << dummy_var10;
	SFile << dummy_var11;

	return true;
}