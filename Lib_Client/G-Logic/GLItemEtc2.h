#ifndef GLITEM_ETCH_2_H_
#define GLITEM_ETCH_2_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../Lib_Engine/Common/BaseStream.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/SerialFile.h"
#include "../../Lib_Engine/Common/STRINGUTILS.h"

namespace ITEM
{
	struct ETC_2
	{
		enum { VERSION = 0x0100 };

		DWORD	unknown_var_1;
		float	unknown_var_2;
		DWORD	unknown_var_3;
		WORD	unknown_var_4;

		ETC_2 ()
			: unknown_var_1(0)
			, unknown_var_2(0.0f)
			, unknown_var_3(0)
			, unknown_var_4(0)
		{
		}

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static void SaveCsvHead ( std::fstream &SFile );
		void SaveCsv ( std::fstream &SFile );
		void LoadCsv ( CStringArray &StrArray, int &iCsvCur );
	};
};

#endif // GLITEM_ETCH_2_H_