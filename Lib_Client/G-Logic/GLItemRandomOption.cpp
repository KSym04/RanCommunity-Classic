#include "stdafx.h"
#include "./GLItemRandomOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ITEM
{
	BOOL SRANDOM_OPT::LOAD_OLD ( basestream &SFile )
	{
		SFile.ReadBuffer ( szNAME, NAME_LEN );
		return TRUE;
	}

	BOOL SRANDOM_OPT::LOAD ( basestream &SFile )
	{
		DWORD ver, size;
		SFile >> ver;
		SFile >> size;

		if ( ver == VERSION )
		{
			SFile >> unknown_var_1;
			SFile.ReadBuffer ( szNAME, NAME_LEN );
		}
		else
		{
			CDebugSet::ErrorVersion( "ITEM::SRANDOM_OPT", ver );
			SFile.SetOffSet( SFile.GetfTell() + size );
		}

		return TRUE;
	}

	BOOL SRANDOM_OPT::SAVE ( CSerialFile &SFile )
	{
		SFile << DWORD(VERSION);
		SFile.BeginBlock();
		{
			SFile << unknown_var_1;
			SFile.WriteBuffer ( szNAME, NAME_LEN );
		}
		SFile.EndBlock();

		return TRUE;
	}

	VOID SRANDOM_OPT::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "szNAME" << ",";
		SFile << "unknown_var_1" << ",";
	}

	VOID SRANDOM_OPT::SaveCsv ( std::fstream &SFile )
	{
		if( strlen( szNAME ) < 1 )
			SFile << " " << ",";
		else
			SFile << szNAME << ",";

		SFile << unknown_var_1 << ",";
	}

	VOID SRANDOM_OPT::LoadCsv ( CStringArray &StrArray, int &iCsvCur )
	{
		memset( szNAME, 0, sizeof(char)*NAME_LEN );

		CString random_option = StrArray[ iCsvCur++ ];

		if( strlen( random_option ) > 1 && random_option.GetAt( 0 ) != ' ' )
			StringCchCopy( szNAME, NAME_LEN, random_option );

		unknown_var_1 = (bool)atoi( StrArray[ iCsvCur++ ] );
	}
};