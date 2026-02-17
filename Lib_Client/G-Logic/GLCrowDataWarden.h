/*!
 * \file GLCrowDataWarden.h
 *
 * \author Juver
 * \date 2020/11/15
 *
 * 
 */

#ifndef GLCROWDATAWARDEN_H_INCLUDED__
#define GLCROWDATAWARDEN_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/Common/BaseStream.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/SerialFile.h"
#include "../../Lib_Engine/Common/STRINGUTILS.h"


struct SCROWWARDEN
{
	enum 
	{ 
		VERSION = 0x0200, 
		SCROWWARDEN_MAX = 4, 
	};

	//this could be enums
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwGenerateMoney;

	std::string	strGenerateItem[SCROWWARDEN_MAX];
	
	SCROWWARDEN (void)
		: dwData1(0)
		, dwData2(0)
		, dwGenerateMoney(0)
	{
		for( int i=0; i<SCROWWARDEN_MAX; ++i )
		{
			strGenerateItem[i] = "";
		}
	}

	SCROWWARDEN ( const SCROWWARDEN &value )
	{
		operator = ( value );
	}

	SCROWWARDEN& operator = ( const SCROWWARDEN &value )
	{
		dwData1 = value.dwData1;
		dwData2 = value.dwData2;
		dwGenerateMoney = value.dwGenerateMoney;

		for( int i=0; i<SCROWWARDEN_MAX; ++i )
		{
			strGenerateItem[i] = value.strGenerateItem[i];
		}

		return *this;
	}

	bool LOAD_0100 ( basestream &SFile );

	bool LOAD ( basestream &SFile );
	bool SAVE ( CSerialFile &SFile );

	static VOID SaveCsvHead ( std::fstream &SFile );
	VOID SaveCsv ( std::fstream &SFile );
	VOID LoadCsv ( CStringArray &StrArray, int& iCsvCur );
};

#endif // GLCROWDATAWARDEN_H_INCLUDED__
