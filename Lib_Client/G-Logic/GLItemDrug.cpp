#include "stdafx.h"
#include "./GLItemDrug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ITEM
{
	void SDRUG::Assign ( SDRUG_100 &sDRUG )
	{
		bInstance		= sDRUG.bInstance;
		emDrug			= sDRUG.emDrug;
		wPileNum		= sDRUG.wPileNum;
		bRatio			= sDRUG.bRatio;
		wCureVolume		= sDRUG.wCureVolume;
		wArrowNum		= sDRUG.wArrowNum;
		dwCureDISORDER	= sDRUG.dwCureDISORDER;
	}

	void SDRUG::Assign ( SDRUG_101 &sDRUG )
	{
		emDrug			= sDRUG.emDrug;
		bInstance		= sDRUG.bInstance;
		tTIME_LMT		= sDRUG.tTIME_LMT;
		bRatio			= sDRUG.bRatio;
		wPileNum		= sDRUG.wPileNum;
		wCureVolume		= sDRUG.wCureVolume;
		wArrowNum		= sDRUG.wArrowNum;
		dwCureDISORDER	= sDRUG.dwCureDISORDER;
	}

	void SDRUG::Assign ( SDRUG_102 &sDRUG )
	{
		emDrug			= sDRUG.emDrug;
		bInstance		= sDRUG.bInstance;
		tTIME_LMT		= sDRUG.tTIME_LMT;
		bRatio			= sDRUG.bRatio;
		wPileNum		= sDRUG.wPileNum;
		wCureVolume		= sDRUG.wCureVolume;
		wArrowNum		= sDRUG.wArrowNum;
		dwCureDISORDER	= sDRUG.dwCureDISORDER;
	}

	void SDRUG::Assign ( SDRUG_103 &sDRUG )
	{
		emDrug			= sDRUG.emDrug;
		bInstance		= sDRUG.bInstance;
		tTIME_LMT		= sDRUG.tTIME_LMT;
		bRatio			= sDRUG.bRatio;
		wPileNum		= sDRUG.wPileNum;
		wCureVolume		= WORD(sDRUG.dwCureVolume);
		wArrowNum		= WORD(sDRUG.dwCureVolume);
		dwCureDISORDER	= sDRUG.dwCureDISORDER;
	}

	void SDRUG::Assign ( SDRUG_104 &sDRUG )
	{
		emDrug			= sDRUG.emDrug;
		bInstance		= sDRUG.bInstance;
		tTIME_LMT		= sDRUG.tTIME_LMT;
		bRatio			= sDRUG.bRatio;
		wPileNum		= sDRUG.wPileNum;
		wCureVolume		= WORD(sDRUG.dwCureVolume);
		wArrowNum		= WORD(sDRUG.dwCureVolume);
		dwCureDISORDER	= sDRUG.dwCureDISORDER;

		unknown_var_1	= sDRUG.unknown_var_1;
		unknown_var_2	= sDRUG.unknown_var_2;
		//unknown_var_3	= sDRUG.unknown_var_3;
		//unknown_var_4	= sDRUG.unknown_var_4;
	}

	void SDRUG::Assign ( SDRUG_105 &sDRUG )
	{
		emDrug			= sDRUG.emDrug;
		bInstance		= sDRUG.bInstance;
		tTIME_LMT		= sDRUG.tTIME_LMT;
		bRatio			= sDRUG.bRatio;
		wPileNum		= sDRUG.wPileNum;
		wCureVolume		= WORD(sDRUG.dwCureVolume);
		wArrowNum		= WORD(sDRUG.dwCureVolume);
		dwCureDISORDER	= sDRUG.dwCureDISORDER;

		unknown_var_1	= sDRUG.unknown_var_1;
		unknown_var_2	= sDRUG.unknown_var_2;
		unknown_var_3	= sDRUG.unknown_var_3;
		unknown_var_4	= sDRUG.unknown_var_4;
	}

	VOID SDRUG::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "tTIME_LMT" << ",";
		SFile << "bInstance" << ",";
		SFile << "emDrug" << ",";
		SFile << "wPileNum" << ",";

		SFile << "bRatio" << ",";

		SFile << "wCureVolume" << ",";

		SFile << "dwCureDISORDER" << ",";

		SFile << "unknown_var_1" << ",";
		SFile << "unknown_var_2" << ",";
		SFile << "unknown_var_3" << ",";
		SFile << "unknown_var_4" << ",";
	}

	VOID SDRUG::SaveCsv ( std::fstream &SFile )
	{
		SFile << tTIME_LMT << ",";
		SFile << bInstance << ",";
		SFile << emDrug << ",";
		SFile << wPileNum << ",";

		SFile << bRatio << ",";

		SFile << wCureVolume << ",";

		SFile << dwCureDISORDER << ",";

		SFile << unknown_var_1 << ",";
		SFile << unknown_var_2 << ",";
		SFile << unknown_var_3 << ",";
		SFile << unknown_var_4 << ",";
	}

	VOID SDRUG::LoadCsv ( CStringArray &StrArray, int &iCsvCur )
	{
		tTIME_LMT = (__time64_t)_atoi64( StrArray[ iCsvCur++ ] );
		bInstance = (BOOL)atol( StrArray[ iCsvCur++ ] );
		emDrug = (EMITEM_DRUG)atoi( StrArray[ iCsvCur++ ] );
		wPileNum = (WORD)atoi( StrArray[ iCsvCur++ ] );

		bRatio = (BOOL)atol( StrArray[ iCsvCur++ ] );

		wCureVolume = (WORD)atoi( StrArray[ iCsvCur++ ] );

		dwCureDISORDER = (DWORD)atol( StrArray[ iCsvCur++ ] );

		unknown_var_1 = (DWORD)atol( StrArray[ iCsvCur++ ] );
		unknown_var_2 = (DWORD)atol( StrArray[ iCsvCur++ ] );
		unknown_var_3 = (DWORD)atol( StrArray[ iCsvCur++ ] );
		unknown_var_4 = (DWORD)atol( StrArray[ iCsvCur++ ] );
	}
};