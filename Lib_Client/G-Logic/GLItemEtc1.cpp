#include "stdafx.h"
#include "./GLItemEtc1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ITEM
{
	BOOL ETC_1::LOAD ( basestream &SFile )
	{
		DWORD ver, size;
		SFile >> ver;
		SFile >> size;

		if ( ver == VERSION )
		{
			SFile.ReadBuffer ( Data, DATA_SIZE );
		}
		else
		{
			CDebugSet::ErrorVersion( "ITEM::ETCH_1", ver );
			SFile.SetOffSet( SFile.GetfTell() + size );
		}

		return TRUE;
	}

	BOOL ETC_1::SAVE ( CSerialFile &SFile )
	{
		SFile << DWORD(VERSION);
		SFile.BeginBlock();
		{
			SFile.WriteBuffer ( Data, DATA_SIZE );
		}
		SFile.EndBlock();

		return TRUE;
	}

	void ETC_1::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "Data" << ",";
	}

	void ETC_1::SaveCsv ( std::fstream &SFile )
	{
		if( strlen( Data ) < 1 )
			SFile << " " << ",";
		else
			SFile << Data << ",";
	}

	void ETC_1::LoadCsv ( CStringArray &StrArray, int &iCsvCur )
	{
		CString text = StrArray[ iCsvCur++ ];

		if( strlen( text ) > 1 && text.GetAt( 0 ) != ' ' )
			StringCchCopy( Data, DATA_SIZE, text );
	}
};