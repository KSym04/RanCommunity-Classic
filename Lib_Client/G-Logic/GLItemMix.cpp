#include "stdafx.h"
#include "./GLItemMix.h"
#include "../../Lib_Engine/Common/StringUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL ITEM_MIX::LOAD ( basestream &SFile )
{
	DWORD dwVer(0);

	SFile >> dwVer;

	if ( dwVer == VERSION )
	{
		SFile >> dwKey;

		for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
		{
			SFile >> sMeterialItem[i].sNID.wMainID;
			SFile >> sMeterialItem[i].sNID.wSubID;		
			SFile >> sMeterialItem[i].nNum;
		}

		SFile >> sResultItem.sNID.wMainID;
		SFile >> sResultItem.sNID.wSubID;		
		SFile >> sResultItem.nNum;

		SFile >> dwRate;
		SFile >> dwPrice;

		/*product item, Juver, 2017/10/15 */
		SFile >> fTime;
		SFile >> dwData1;
		SFile >> dwData2;
		SFile >> nData1;
		SFile >> nData2;
		SFile >> wLevelReq;
		SFile >> wCategory;
		SFile >> wData1;
		SFile >> wData2;
		SFile >> wData3;

		SFile >> bUseStat;
		SFile >> cDAMAGE;
		SFile >> cDEFENSE;
		SFile >> cRESIST_FIRE;
		SFile >> cRESIST_ICE;
		SFile >> cRESIST_ELEC;
		SFile >> cRESIST_POISON;
		SFile >> cRESIST_SPIRIT;
		SFile >> bGenerateRandomValue;

		/* play time system, Juver, 2021/01/26 */
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );
		SFile.ReadBuffer ( &llPlayPointUse, sizeof(llPlayPointUse) );

		SFile >> dwContriUse;
	}
	else if ( dwVer == 0x0202 )
	{
		SFile >> dwKey;

		for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
		{
			SFile >> sMeterialItem[i].sNID.wMainID;
			SFile >> sMeterialItem[i].sNID.wSubID;		
			SFile >> sMeterialItem[i].nNum;
		}

		SFile >> sResultItem.sNID.wMainID;
		SFile >> sResultItem.sNID.wSubID;		
		SFile >> sResultItem.nNum;

		SFile >> dwRate;
		SFile >> dwPrice;

		/*product item, Juver, 2017/10/15 */
		SFile >> fTime;
		SFile >> dwData1;
		SFile >> dwData2;
		SFile >> nData1;
		SFile >> nData2;
		SFile >> wLevelReq;
		SFile >> wCategory;
		SFile >> wData1;
		SFile >> wData2;
		SFile >> wData3;

		SFile >> bUseStat;
		SFile >> cDAMAGE;
		SFile >> cDEFENSE;
		SFile >> cRESIST_FIRE;
		SFile >> cRESIST_ICE;
		SFile >> cRESIST_ELEC;
		SFile >> cRESIST_POISON;
		SFile >> cRESIST_SPIRIT;
		SFile >> bGenerateRandomValue;

		/* play time system, Juver, 2021/01/26 */
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );
		SFile.ReadBuffer ( &llPlayPointUse, sizeof(llPlayPointUse) );
	}
	else if ( dwVer == 0x0201 )
	{
		SFile >> dwKey;

		for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
		{
			SFile >> sMeterialItem[i].sNID.wMainID;
			SFile >> sMeterialItem[i].sNID.wSubID;		
			SFile >> sMeterialItem[i].nNum;
		}

		SFile >> sResultItem.sNID.wMainID;
		SFile >> sResultItem.sNID.wSubID;		
		SFile >> sResultItem.nNum;

		SFile >> dwRate;
		SFile >> dwPrice;

		/*product item, Juver, 2017/10/15 */
		SFile >> fTime;
		SFile >> dwData1;
		SFile >> dwData2;
		SFile >> nData1;
		SFile >> nData2;
		SFile >> wLevelReq;
		SFile >> wCategory;
		SFile >> wData1;
		SFile >> wData2;
		SFile >> wData3;

		SFile >> bUseStat;
		SFile >> cDAMAGE;
		SFile >> cDEFENSE;
		SFile >> cRESIST_FIRE;
		SFile >> cRESIST_ICE;
		SFile >> cRESIST_ELEC;
		SFile >> cRESIST_POISON;
		SFile >> cRESIST_SPIRIT;
		SFile >> bGenerateRandomValue;
	}
	else if ( dwVer == 0x0200 )
	{
		SFile >> dwKey;

		for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
		{
			SFile >> sMeterialItem[i].sNID.wMainID;
			SFile >> sMeterialItem[i].sNID.wSubID;		
			SFile >> sMeterialItem[i].nNum;
		}

		SFile >> sResultItem.sNID.wMainID;
		SFile >> sResultItem.sNID.wSubID;		
		SFile >> sResultItem.nNum;

		SFile >> dwRate;
		SFile >> dwPrice;

		/*product item, Juver, 2017/10/15 */
		SFile >> fTime;
		SFile >> dwData1;
		SFile >> dwData2;
		SFile >> nData1;
		SFile >> nData2;
		SFile >> wLevelReq;
		SFile >> wCategory;
		SFile >> wData1;
		SFile >> wData2;
		SFile >> wData3;

		SFile >> cDAMAGE;
		SFile >> cDEFENSE;
		SFile >> cRESIST_FIRE;
		SFile >> cRESIST_ICE;
		SFile >> cRESIST_ELEC;
		SFile >> cRESIST_POISON;
		SFile >> cRESIST_SPIRIT;
		SFile >> bGenerateRandomValue;
	}
	else if ( dwVer == 0x0102 )
	{
		SFile >> dwKey;

		for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
		{
			SFile >> sMeterialItem[i].sNID.wMainID;
			SFile >> sMeterialItem[i].sNID.wSubID;		
			SFile >> sMeterialItem[i].nNum;
		}

		SFile >> sResultItem.sNID.wMainID;
		SFile >> sResultItem.sNID.wSubID;		
		SFile >> sResultItem.nNum;

		SFile >> dwRate;
		SFile >> dwPrice;

		/*product item, Juver, 2017/10/15 */
		SFile >> fTime;
		SFile >> dwData1;
		SFile >> dwData2;
		SFile >> nData1;
		SFile >> nData2;
		SFile >> wLevelReq;
		SFile >> wCategory;
		SFile >> wData1;
		SFile >> wData2;
		SFile >> wData3;
	}
	else if ( dwVer == 0x0101 )
	{
		SFile >> dwKey;

		for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
		{
			SFile >> sMeterialItem[i].sNID.wMainID;
			SFile >> sMeterialItem[i].sNID.wSubID;		
			SFile >> sMeterialItem[i].nNum;
		}

		SFile >> sResultItem.sNID.wMainID;
		SFile >> sResultItem.sNID.wSubID;		
		SFile >> sResultItem.nNum;

		SFile >> dwRate;
		SFile >> dwPrice;

		/*product item, Juver, 2017/10/15 */
		SFile >> fTime;
		SFile >> dwData1;
		SFile >> dwData2;
		SFile >> nData1;
		SFile >> nData2;
	}
	else if ( dwVer == 0x0100 )
	{
		SFile >> dwKey;

		for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
		{
			SFile >> sMeterialItem[i].sNID.wMainID;
			SFile >> sMeterialItem[i].sNID.wSubID;		
			SFile >> sMeterialItem[i].nNum;
		}

		SFile >> sResultItem.sNID.wMainID;
		SFile >> sResultItem.sNID.wSubID;		
		SFile >> sResultItem.nNum;

		SFile >> dwRate;
		SFile >> dwPrice;
	}
	else
	{
		CDebugSet::ErrorVersion( "ITEM_MIX::LOAD", dwVer );
	}

	return TRUE;
}

BOOL ITEM_MIX::SAVE ( CSerialFile &SFile )
{
	SFile << (DWORD)VERSION;
	
	SFile << dwKey;

	for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{
		SFile << sMeterialItem[i].sNID.wMainID;
		SFile << sMeterialItem[i].sNID.wSubID;		
		SFile << sMeterialItem[i].nNum;
	}

	SFile << sResultItem.sNID.wMainID;                    
	SFile << sResultItem.sNID.wSubID;		
	SFile << sResultItem.nNum;

	SFile << dwRate;
	SFile << dwPrice;

	/*product item, Juver, 2017/10/15 */
	SFile << fTime;
	SFile << dwData1;
	SFile << dwData2;
	SFile << nData1;
	SFile << nData2;
	SFile << wLevelReq;
	SFile << wCategory;
	SFile << wData1;
	SFile << wData2;
	SFile << wData3;

	SFile << bUseStat;
	SFile << cDAMAGE;
	SFile << cDEFENSE;
	SFile << cRESIST_FIRE;
	SFile << cRESIST_ICE;
	SFile << cRESIST_ELEC;
	SFile << cRESIST_POISON;
	SFile << cRESIST_SPIRIT;
	SFile << bGenerateRandomValue;

	/* play time system, Juver, 2021/01/26 */
	SFile.WriteBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );
	SFile.WriteBuffer ( &llPlayPointUse, sizeof(llPlayPointUse) );

	SFile << dwContriUse;

	return TRUE;
}

VOID ITEM_MIX::SaveCsvHead ( std::fstream &SFile )
{
	SFile << "dwKey" << ",";

	for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{
		SFile << "sMeterialItem " << i << " sNID wMainID" << ",";
		SFile << "sMeterialItem " << i << " sNID wSubID" << ",";
		SFile << "sMeterialItem " << i << " nNum" << ",";
	}

	SFile << "sResultItem.sNID.wMainID" << ",";                    
	SFile << "sResultItem.sNID.wSubID" << ",";	
	SFile << "sResultItem.nNum" << ",";

	SFile << "dwRate" << ",";
	SFile << "dwPrice" << ",";

	/*product item, Juver, 2017/10/15 */
	SFile << "fTime" << ",";
	SFile << "dwData1" << ",";
	SFile << "dwData2" << ",";
	SFile << "nData1" << ",";
	SFile << "nData2" << ",";
	SFile << "wLevelReq" << ",";
	SFile << "wCategory" << ",";
	SFile << "wData1" << ",";
	SFile << "wData2" << ",";
	SFile << "wData3" << ",";

	SFile << "bUseStat" << ",";
	SFile << "cDAMAGE" << ",";
	SFile << "cDEFENSE" << ",";
	SFile << "cRESIST_FIRE" << ",";
	SFile << "cRESIST_ICE" << ",";
	SFile << "cRESIST_ELEC" << ",";
	SFile << "cRESIST_POISON" << ",";
	SFile << "cRESIST_SPIRIT" << ",";
	SFile << "bGenerateRandomValue" << ",";

	/* play time system, Juver, 2021/01/26 */
	SFile << "llPlayTimeReq" << ",";
	SFile << "llPlayPointUse" << ",";

	SFile << "dwContriUse" << ",";

	SFile << std::endl;
}

VOID ITEM_MIX::SaveCsv ( std::fstream &SFile )
{
	SFile << dwKey << ",";

	for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{
		SFile << sMeterialItem[i].sNID.wMainID << ",";
		SFile << sMeterialItem[i].sNID.wSubID << ",";
		SFile << (WORD)sMeterialItem[i].nNum << ",";
	}

	SFile << sResultItem.sNID.wMainID << ",";              
	SFile << sResultItem.sNID.wSubID << ",";
	SFile << (WORD)sResultItem.nNum << ",";

	SFile << dwRate << ",";
	SFile << dwPrice << ",";

	/*product item, Juver, 2017/10/15 */
	SFile << fTime << ",";
	SFile << dwData1 << ",";
	SFile << dwData2 << ",";
	SFile << (WORD)nData1 << ",";
	SFile << (WORD)nData2 << ",";
	SFile << wLevelReq << ",";
	SFile << wCategory << ",";
	SFile << wData1 << ",";
	SFile << wData2 << ",";
	SFile << wData3 << ",";

	SFile << bUseStat << ",";
	SFile << (WORD)cDAMAGE << ",";
	SFile << (WORD)cDEFENSE << ",";
	SFile << (WORD)cRESIST_FIRE << ",";
	SFile << (WORD)cRESIST_ICE << ",";
	SFile << (WORD)cRESIST_ELEC << ",";
	SFile << (WORD)cRESIST_POISON << ",";
	SFile << (WORD)cRESIST_SPIRIT << ",";
	SFile << bGenerateRandomValue << ",";

	/* play time system, Juver, 2021/01/26 */
	SFile << llPlayTimeReq << ",";
	SFile << llPlayPointUse << ",";

	SFile << dwContriUse << ",";

	SFile << std::endl;
}

VOID ITEM_MIX::LoadCsv ( CStringArray &StrArray )
{
	int iCsvCur = 0;

	dwKey = (DWORD)atoi( StrArray[ iCsvCur++ ] );

	for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{
		sMeterialItem[i].sNID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sMeterialItem[i].sNID.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sMeterialItem[i].nNum = (BYTE)atoi( StrArray[ iCsvCur++ ] );
	}

	sResultItem.sNID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );     
	sResultItem.sNID.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
	sResultItem.nNum = (BYTE)atoi( StrArray[ iCsvCur++ ] );

	dwRate = (DWORD)atoi( StrArray[ iCsvCur++ ] );
	dwPrice = (DWORD)atoi( StrArray[ iCsvCur++ ] );

	/*product item, Juver, 2017/10/15 */
	fTime = (float)atof( StrArray[ iCsvCur++ ] );
	dwData1 = (DWORD)atoi( StrArray[ iCsvCur++ ] );
	dwData2 = (DWORD)atoi( StrArray[ iCsvCur++ ] );
	nData1 = (BYTE)atoi( StrArray[ iCsvCur++ ] );
	nData2 = (BYTE)atoi( StrArray[ iCsvCur++ ] );
	wLevelReq = (WORD)atoi( StrArray[ iCsvCur++ ] );
	wCategory = (WORD)atoi( StrArray[ iCsvCur++ ] );
	wData1 = (WORD)atoi( StrArray[ iCsvCur++ ] );
	wData2 = (WORD)atoi( StrArray[ iCsvCur++ ] );
	wData3 = (WORD)atoi( StrArray[ iCsvCur++ ] );

	bUseStat = (BOOL)atoi( StrArray[ iCsvCur++ ] );
	cDAMAGE = (BYTE)atoi( StrArray[ iCsvCur++ ] );
	cDEFENSE = (BYTE)atoi( StrArray[ iCsvCur++ ] );
	cRESIST_FIRE = (BYTE)atoi( StrArray[ iCsvCur++ ] );
	cRESIST_ICE = (BYTE)atoi( StrArray[ iCsvCur++ ] );
	cRESIST_ELEC = (BYTE)atoi( StrArray[ iCsvCur++ ] );
	cRESIST_POISON = (BYTE)atoi( StrArray[ iCsvCur++ ] );
	cRESIST_SPIRIT = (BYTE)atoi( StrArray[ iCsvCur++ ] );
	bGenerateRandomValue = (BOOL)atoi( StrArray[ iCsvCur++ ] );

	/* play time system, Juver, 2021/01/26 */
	llPlayTimeReq = _atoi64 ( StrArray[ iCsvCur++ ] );
	llPlayPointUse = _atoi64 ( StrArray[ iCsvCur++ ] );

	dwContriUse = static_cast<DWORD>(atoi( StrArray[ iCsvCur++ ] ));
}