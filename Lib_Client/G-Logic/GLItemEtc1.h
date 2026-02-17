#ifndef GLITEM_ETCH_1_H_
#define GLITEM_ETCH_1_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../Lib_Engine/Common/BaseStream.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/SerialFile.h"
#include "../../Lib_Engine/Common/STRINGUTILS.h"

namespace ITEM
{
	struct ETC_1
	{
		enum { VERSION = 0x0100, DATA_SIZE = 65 };

		char	Data[DATA_SIZE];

		ETC_1 ()
		{
			memset(Data, 0, sizeof(char) * DATA_SIZE);
		}

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static void SaveCsvHead ( std::fstream &SFile );
		void SaveCsv ( std::fstream &SFile );
		void LoadCsv ( CStringArray &StrArray, int &iCsvCur );
	};
};

#endif // GLITEM_ETCH_1_H_