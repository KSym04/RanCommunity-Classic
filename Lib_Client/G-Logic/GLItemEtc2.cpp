#include "stdafx.h"
#include "./GLItemEtc2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ITEM
{
	BOOL ETC_2::LOAD ( basestream &SFile )
	{
		DWORD ver, size;
		SFile >> ver;
		SFile >> size;

		if ( ver == VERSION )
		{
			SFile >> unknown_var_1;
			SFile >> unknown_var_2;
			SFile >> unknown_var_3;
			SFile >> unknown_var_4;
		}
		else
		{
			CDebugSet::ErrorVersion( "ITEM::ETCH_2", ver );
			SFile.SetOffSet( SFile.GetfTell() + size );
		}

		return TRUE;
	}

	BOOL ETC_2::SAVE ( CSerialFile &SFile )
	{
		SFile << DWORD(VERSION);
		SFile.BeginBlock();
		{
			SFile << unknown_var_1;
			SFile << unknown_var_2;
			SFile << unknown_var_3;
			SFile << unknown_var_4;
		}
		SFile.EndBlock();

		return TRUE;
	}

	void ETC_2::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "unknown_var_1" << ",";
		SFile << "unknown_var_2" << ",";
		SFile << "unknown_var_3" << ",";
		SFile << "unknown_var_4" << ",";
	}

	void ETC_2::SaveCsv ( std::fstream &SFile )
	{
		SFile << unknown_var_1 << ",";
		SFile << unknown_var_2 << ",";
		SFile << unknown_var_3 << ",";
		SFile << unknown_var_4 << ",";
	}

	void ETC_2::LoadCsv ( CStringArray &StrArray, int &iCsvCur )
	{
		unknown_var_1 = (DWORD)atoi( StrArray[ iCsvCur++ ] );
		unknown_var_2 = (float)atof( StrArray[ iCsvCur++ ] );
		unknown_var_3 = (DWORD)atoi( StrArray[ iCsvCur++ ] );
		unknown_var_4 = (WORD)atoi( StrArray[ iCsvCur++ ] );
	}
};