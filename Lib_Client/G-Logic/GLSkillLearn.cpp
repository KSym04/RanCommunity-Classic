#include "stdafx.h"
#include "./GLSkillLearn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace SKILL
{
	VOID SLEARN::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "emBRIGHT" << ",";
		SFile << "dwCLASS" << ",";
		SFile << "sSKILL wMainID" << ",";
		SFile << "sSKILL wSubID" << ",";

		for( int i=1; i<SKILL::MAX_SKILL_LEVEL+1; ++i )
		{
			SFile << "sLVL_STEP " << i << " dwSKP" << ",";
			SFile << "sLVL_STEP " << i << " dwLEVEL" << ",";

			SFile << "sLVL_STEP " << i << " sSTATS wPow" << ",";
			SFile << "sLVL_STEP " << i << " sSTATS wStr" << ",";
			SFile << "sLVL_STEP " << i << " sSTATS wSpi" << ",";
			SFile << "sLVL_STEP " << i << " sSTATS wDex" << ",";
			SFile << "sLVL_STEP " << i << " sSTATS wInt" << ",";
			SFile << "sLVL_STEP " << i << " sSTATS wSta" << ",";

			SFile << "sLVL_STEP " << i << " dwSKILL_LVL" << ",";
		}

		SFile << "unknown_var_0" << ",";
		SFile << "unknown_var_1 wMainID" << ",";
		SFile << "unknown_var_1 wSubID" << ",";
		SFile << "unknown_var_2" << ",";

		SFile << "emVIPLevel" << ",";

		SFile << "dwRebornReq" << ",";

		SFile << "sHiddenWeapon wMainID" << ",";
		SFile << "sHiddenWeapon wSubID" << ",";

		SFile << "bHiddenWeapon" << ",";
	}

	VOID SLEARN::SaveCsv ( std::fstream &SFile )
	{
		SFile << emBRIGHT << ",";
		SFile << dwCLASS << ",";
		SFile << sSKILL.wMainID << ",";
		SFile << sSKILL.wSubID << ",";

		for( int i=0; i<SKILL::MAX_SKILL_LEVEL; ++i )
		{
			SFile << sLVL_STEP[i].dwSKP << ",";
			SFile << sLVL_STEP[i].dwLEVEL << ",";

			SFile << sLVL_STEP[i].sSTATS.wPow << ",";
			SFile << sLVL_STEP[i].sSTATS.wStr << ",";
			SFile << sLVL_STEP[i].sSTATS.wSpi << ",";
			SFile << sLVL_STEP[i].sSTATS.wDex << ",";
			SFile << sLVL_STEP[i].sSTATS.wInt << ",";
			SFile << sLVL_STEP[i].sSTATS.wSta << ",";

			SFile << sLVL_STEP[i].dwSKILL_LVL << ",";
		}

		SFile << unknown_var_0 << ",";
		SFile << unknown_var_1.wMainID << ",";
		SFile << unknown_var_1.wSubID << ",";
		SFile << unknown_var_2 << ",";

		SFile << emVIPLevel << ",";

		SFile << dwRebornReq << ",";

		SFile << sHiddenWeapon.wMainID << ",";
		SFile << sHiddenWeapon.wSubID << ",";

		SFile << bHiddenWeapon << ",";
	}

	VOID SLEARN::LoadCsv ( CStringArray &StrArray, int &iCsvCur )
	{
		emBRIGHT = (EMBRIGHT)atoi( StrArray[ iCsvCur++ ] );
		dwCLASS = (DWORD)atol( StrArray[ iCsvCur++ ] );
		sSKILL.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sSKILL.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );

		for( int i=0; i<SKILL::MAX_SKILL_LEVEL; ++i )
		{
			sLVL_STEP[i].dwSKP = (DWORD)atol( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].dwLEVEL = (DWORD)atol( StrArray[ iCsvCur++ ] );

			sLVL_STEP[i].sSTATS.wPow = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].sSTATS.wStr = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].sSTATS.wSpi = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].sSTATS.wDex = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].sSTATS.wInt = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].sSTATS.wSta = (WORD)atoi( StrArray[ iCsvCur++ ] );

			sLVL_STEP[i].dwSKILL_LVL = (DWORD)atol( StrArray[ iCsvCur++ ] );
		}

		unknown_var_0 = (DWORD)atol( StrArray[ iCsvCur++ ] );
		unknown_var_1.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		unknown_var_1.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		unknown_var_2 = (DWORD)atol( StrArray[ iCsvCur++ ] );

		emVIPLevel =  static_cast<EMVIP_LEVEL> (atoi( StrArray[ iCsvCur++ ] ));

		dwRebornReq = (DWORD)atol( StrArray[ iCsvCur++ ] );

		sHiddenWeapon.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sHiddenWeapon.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );

		bHiddenWeapon = (BOOL)atoi( StrArray[ iCsvCur++ ] );
	}

	void SLEARN::ReadStringStream ( std::stringstream &sStream )
	{
		sStream << emBRIGHT << ",";
		sStream << dwCLASS << ",";
		sStream << sSKILL.wMainID << ",";
		sStream << sSKILL.wSubID << ",";

		for( int i=0; i<SKILL::MAX_SKILL_LEVEL; ++i )
		{
			sStream << sLVL_STEP[i].dwSKP << ",";
			sStream << sLVL_STEP[i].dwLEVEL << ",";

			sStream << sLVL_STEP[i].sSTATS.wPow << ",";
			sStream << sLVL_STEP[i].sSTATS.wStr << ",";
			sStream << sLVL_STEP[i].sSTATS.wSpi << ",";
			sStream << sLVL_STEP[i].sSTATS.wDex << ",";
			sStream << sLVL_STEP[i].sSTATS.wInt << ",";
			sStream << sLVL_STEP[i].sSTATS.wSta << ",";

			sStream << sLVL_STEP[i].dwSKILL_LVL << ",";
		}

		sStream << unknown_var_0 << ",";
		sStream << unknown_var_1.wMainID << ",";
		sStream << unknown_var_1.wSubID << ",";
		sStream << unknown_var_2 << ",";

		sStream << emVIPLevel << ",";

		sStream << dwRebornReq << ",";
	}
};