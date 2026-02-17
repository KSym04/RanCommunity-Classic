#ifndef GLITEM_BUFFS_H_
#define GLITEM_BUFFS_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../Lib_Engine/Common/BaseStream.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/SerialFile.h"
#include "../../Lib_Engine/Common/STRINGUTILS.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"

namespace ITEM
{
	struct SELFBUFFS_ITEM_101
	{
		SNATIVEID	sidSkill;
		WORD		wLevel;

		SELFBUFFS_ITEM_101 ()
		: sidSkill(false)
		, wLevel(1)
		{
		}
	};

	struct SELFBUFFS_ITEM
	{
		SNATIVEID	sidSkill;
		WORD		wLevel;

		SELFBUFFS_ITEM ()
		: sidSkill(false)
		, wLevel(1)
		{
		}
	};

	struct SELFBUFF
	{
		//enum { VERSION = 0x0103, ITEM_SIZE = 10, };
		enum { VERSION = 0x0104, ITEM_SIZE = 10, };
		
		/*dmk14 box selection*/
	//	BOOL		m_bSelect;
	//	int			m_nMaxSelect;

		SELFBUFFS_ITEM	sBUFFS[ITEM_SIZE];
		bool		bData;

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray, int &iCsvCur  );

		SELFBUFF ();
		void CLEAR ();
		bool VALID ();

		bool INSERT ( const SNATIVEID &sidSkill, DWORD wLevel );
		bool DEL ( int nIndex );
	};

};

#endif // GLITEM_BOX_H_