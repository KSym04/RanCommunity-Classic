#include "stdafx.h"
#include "./GLItemBasic.h"
#include "./GLStringTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ITEM
{
	void SITEMBASIC::Assign ( SITEMBASIC_100 &sOld )
	{
		sNativeID		= sOld.sNativeID;

		strName			= sOld.szName;
		emLevel			= sOld.emLevel;

		dwFlags			= sOld.dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		dwBuyPrice		= sOld.dwPrice;
		dwSellPrice		= dwBuyPrice/3;

		emItemType = EXITEM_TO_ITEM(sOld.emItemType);

		//	착용 조건.
		emReqBright		= sOld.emReqBright;
		dwReqCharClass	= sOld.dwReqCharClass;
		wReqLevelDW		= sOld.wReqLevel;
		wReqPA			= sOld.wReqPA;
		wReqSA			= sOld.wReqSA;
		sReqStats		= sOld.sReqStats;

		wInvenSizeX		= sOld.wInvenSizeX;
		wInvenSizeY		= sOld.wInvenSizeY;

		strFieldFile	= sOld.szFieldFile;
		strInventoryFile = sOld.szInventoryFile;

		for ( int i=0; i<GLCI_NUM_4CLASS; i++ )
			strWearingFileRight[i] = sOld.szWearingFIle[i];

		strComment = sOld.szComment;
	}

	void SITEMBASIC::Assign ( SITEMBASIC_101 &sOld )
	{
		sNativeID		= sOld.sNativeID;

		strName			= sOld.szName;
		emLevel			= sOld.emLevel;

		dwFlags			= sOld.dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		dwBuyPrice		= sOld.dwPrice;
		dwSellPrice		= dwBuyPrice/3;

		emItemType = EXITEM_TO_ITEM(sOld.emItemType);

		//	착용 조건.
		emReqBright		= sOld.emReqBright;
		dwReqCharClass	= sOld.dwReqCharClass;
		wReqLevelDW		= sOld.wReqLevel;
		wReqPA			= 0;
		wReqSA			= 0;
		sReqStats		= sOld.sReqStats;

		wInvenSizeX		= sOld.wInvenSizeX;
		wInvenSizeY		= sOld.wInvenSizeY;

		strSelfBodyEffect = sOld.szSelfBodyEffect;
		strTargBodyEffect = sOld.szTargBodyEffect;
		strTargetEffect = sOld.szTargetEffect;

		strFieldFile	= sOld.szFieldFile;
		strInventoryFile = sOld.szInventoryFile;

		for ( int i=0; i<GLCI_NUM_4CLASS; ++i )
			strWearingFileRight[i] = sOld.szWearingFIle[i];

		strComment = sOld.szComment;
	}

	void SITEMBASIC::Assign ( SITEMBASIC_102 &sOld )
	{
		sNativeID = sOld.sNativeID;					//	고유 ID. ( 아이템 설정 최초 생성시에 부여되는 고유 ID )

		strName = sOld.szName;						//	아이템의 이름.	
		emLevel = sOld.emLevel;						//	아이템 가치 등급.

		dwFlags		= sOld.dwFlags;						//	Trade 속성.

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		dwBuyPrice	= sOld.dwPrice;						//	아이템 구입가격.
		dwSellPrice	= dwBuyPrice/3;				//  아이템 판매가격

		emItemType = EXITEM_TO_ITEM(sOld.emItemType);

		//	착용 조건.
		emReqBright = sOld.emReqBright;				//	캐릭 속성. ( 광/암 )
		dwReqCharClass = sOld.dwReqCharClass;		//	착용할 수 있는 직업들. ( EMCHARCLASS Flags )
		wReqLevelDW = sOld.wReqLevel;					//	요구 Level.
		wReqPA = sOld.wReqPA;						//	요구 격투치.
		wReqSA = sOld.wReqSA;						//	요구 사격치.
		sReqStats = sOld.sReqStats;					//	요구 케릭터 Stats 수치.

		wInvenSizeX = sOld.wInvenSizeX;				//	인벤토리 사이즈.
		wInvenSizeY = sOld.wInvenSizeY;				//	인벤토리 사이즈.

		sICONID = sOld.sICONID;						//	아이콘 인덱스.

		strSelfBodyEffect = sOld.szSelfBodyEffect;	//	자기 자신의 몸에 붓는 이팩트.
		strTargBodyEffect = sOld.szTargBodyEffect;	//	목표 유닛의 몸에 붓는 이팩트.
		strTargetEffect = sOld.szTargetEffect;		//	목표 지향 이팩트.

		strFieldFile = sOld.szFieldFile;			//	바닥 형상 파일.
		strInventoryFile = sOld.szInventoryFile;	//	인벤토리 형상 파일.

		for ( int i=0; i<GLCI_NUM_4CLASS; ++i )	
			strWearingFileRight[i] = sOld.szWearingFIle[i];	//	형상 파일.

		strComment = sOld.szComment;			//	아이템에 대한 간단한 설명.
	}

	bool SITEMBASIC::LOAD_103 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		dwSellPrice		= dwBuyPrice/3;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> wReqLevelDW;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_4CLASS; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		return true;
	}

	bool SITEMBASIC::LOAD_104 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		dwSellPrice		= dwBuyPrice/3;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_4CLASS; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		return true;
	}

	bool SITEMBASIC::LOAD_105 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		dwSellPrice		= dwBuyPrice/3;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_4CLASS; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		return true;
	}

	bool SITEMBASIC::LOAD_106 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> fExpMultiple;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		dwSellPrice		= dwBuyPrice/3;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_4CLASS; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		return true;
	}

	bool SITEMBASIC::LOAD_107 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> fExpMultiple;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		dwSellPrice		= dwBuyPrice/3;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_4CLASS; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		// PET
		SFile >> strPetWearingFile;

		return true;
	}

	bool SITEMBASIC::LOAD_108 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> fExpMultiple;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		dwSellPrice		= dwBuyPrice/3;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_5CLASS; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		// PET
		SFile >> strPetWearingFile;

		return true;
	}

	bool SITEMBASIC::LOAD_109 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> fExpMultiple;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		dwSellPrice		= dwBuyPrice/3;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_5CLASS; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		// PET
		SFile >> strPetWearingFile;

		return true;
	}

	bool SITEMBASIC::LOAD_110 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> fExpMultiple;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		dwSellPrice		= dwBuyPrice/3;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;

		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_5CLASS_EX; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		// PET
		SFile >> strPetWearingFile;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_111 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> fExpMultiple;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		dwSellPrice		= dwBuyPrice/3;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_5CLASS_EX; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		// PET
		SFile >> strPetWearingFile;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_112 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> fExpMultiple;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		dwSellPrice		= dwBuyPrice/3;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_5CLASS_EX; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		// PET
		SFile >> strPetWearingFile;

		SFile >> sSubID.dwID;

		SFile >> wPosX;
		SFile >> wPosY;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_113 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> fExpMultiple;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;

		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_5CLASS_EX; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		// PET
		SFile >> strPetWearingFile;

		SFile >> sSubID.dwID;

		SFile >> wPosX;
		SFile >> wPosY;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_114 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;

		SFile >> strName;
		SFile >> dwData;
		emLevel = (EMITEMLEVEL)dwData;

		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> fExpMultiple;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;

		SFile >> dwFlags;

		if ( IsTHROW() )
		{
			dwFlags |= 7;
		}

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;

		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		//	착용 조건.
		SFile >> dwData;
		emReqBright = (EMBRIGHT) dwData;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;

		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );

		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;

		SFile >> sICONID.dwID;

		SFile >> strSelfBodyEffect;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;

		SFile >> strFieldFile;
		SFile >> strInventoryFile;

		for ( int i=0; i<GLCI_NUM_5CLASS_EX; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strComment;

		// PET
		SFile >> strPetWearingFile;

		SFile >> sSubID.dwID;

		SFile >> wPosX;
		SFile >> wPosY;

		SFile >> dwCoolTime;

		SFile >> dwData;
		emCoolType = (EMCOOL_TYPE) dwData;		

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_115 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;
	
		for ( int i=0; i<GLCI_NUM_5CLASS_EX; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_116 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_6CLASS; ++i )
			SFile >> strWearingFileRight[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_117 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_6CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_6CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_118 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);

		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_6CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_6CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_119 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		class_convert();


		return true;
	}

	bool SITEMBASIC::LOAD_121 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		class_convert();

		return true;
	}


	bool SITEMBASIC::LOAD_122 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		//SFile >> wInvenSizeX;
		//SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_123 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> unknown_var_1;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		

		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		class_convert();

		return true;
	}


	bool SITEMBASIC::LOAD_124 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> unknown_var_1;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		

		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;
		SFile >> unknown_var_2;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_125 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> unknown_var_1;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;

		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
	
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;
		SFile >> unknown_var_2;

		std::string wear_right[18];
		std::string wear_left[18];

		for ( int i=0; i<18; ++i )
			SFile >> wear_right[i];

		for ( int i=0; i<18; ++i )
			SFile >> wear_left[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
		{
			strWearingFileRight[i] = wear_right[i];
			strWearingFileLeft[i] = wear_left[i];
		}

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_126 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> unknown_var_1;

		LONGLONG llBuyPrice, llSellPrice;
		SFile.ReadBuffer ( &llBuyPrice, sizeof(llBuyPrice) );
		SFile.ReadBuffer ( &llSellPrice, sizeof(llSellPrice) );
		dwBuyPrice = llBuyPrice;
		dwSellPrice = llSellPrice;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;

		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		

		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;
		SFile >> unknown_var_2;

		std::string wear_right[18];
		std::string wear_left[18];

		for ( int i=0; i<18; ++i )
			SFile >> wear_right[i];

		for ( int i=0; i<18; ++i )
			SFile >> wear_left[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
		{
			strWearingFileRight[i] = wear_right[i];
			strWearingFileLeft[i] = wear_left[i];
		}

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_127 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;

		SFile >> dwFlags;
		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> unknown_var_1;
		
		LONGLONG llBuyPrice, llSellPrice;
		SFile.ReadBuffer ( &llBuyPrice, sizeof(llBuyPrice) );
		SFile.ReadBuffer ( &llSellPrice, sizeof(llSellPrice) );
		dwBuyPrice = llBuyPrice;
		dwSellPrice = llSellPrice;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;

		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		

		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;
		SFile >> unknown_var_2;

		std::string wear_right[20];
		std::string wear_left[20];

		for ( int i=0; i<20; ++i )
			SFile >> wear_right[i];

		for ( int i=0; i<20; ++i )
			SFile >> wear_left[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
		{
			strWearingFileRight[i] = wear_right[i];
			strWearingFileLeft[i] = wear_left[i];
		}

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_128 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> unknown_var_1;
		
		LONGLONG llBuyPrice, llSellPrice;
		SFile.ReadBuffer ( &llBuyPrice, sizeof(llBuyPrice) );
		SFile.ReadBuffer ( &llSellPrice, sizeof(llSellPrice) );
		dwBuyPrice = llBuyPrice;
		dwSellPrice = llSellPrice;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;

		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		

		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;
		SFile >> unknown_var_2;

		std::string wear_right[20];
		std::string wear_left[20];

		for ( int i=0; i<20; ++i )
			SFile >> wear_right[i];

		for ( int i=0; i<20; ++i )
			SFile >> wear_left[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
		{
			strWearingFileRight[i] = wear_right[i];
			strWearingFileLeft[i] = wear_left[i];
		}

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		SFile >> unknown_var_3;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_129 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	
		EMEXITEM_TYPE exemItemType = (EMEXITEM_TYPE) dwData;
		emItemType = EXITEM_TO_ITEM(exemItemType);
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> unknown_var_1;
		
		LONGLONG llBuyPrice, llSellPrice;
		SFile.ReadBuffer ( &llBuyPrice, sizeof(llBuyPrice) );
		SFile.ReadBuffer ( &llSellPrice, sizeof(llSellPrice) );
		dwBuyPrice = llBuyPrice;
		dwSellPrice = llSellPrice;

		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;

		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		

		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;
		SFile >> unknown_var_2;

		std::string wear_right[20];
		std::string wear_left[20];

		for ( int i=0; i<20; ++i )
			SFile >> wear_right[i];

		for ( int i=0; i<20; ++i )
			SFile >> wear_left[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
		{
			strWearingFileRight[i] = wear_right[i];
			strWearingFileLeft[i] = wear_left[i];
		}

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		SFile >> unknown_var_3;

		class_convert();

		return true;
	}

	bool SITEMBASIC::LOAD_200 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		return true;
	}

	bool SITEMBASIC::LOAD_201 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		DWORD dwActivityReqType;
		SFile >> dwActivityReqType;
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*activity point, Juver, 2017/08/23 */
		if ( dwActivityReqType == 1 )
			dwReqActivityPoint = dwReqActivityPoint * 2;
		else if ( dwActivityReqType == 2 )
			dwReqActivityPoint = dwReqActivityPoint * 3;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		return true;
	}

	bool SITEMBASIC::LOAD_202 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		return true;
	}

	bool SITEMBASIC::LOAD_203 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		return true;
	}


	bool SITEMBASIC::LOAD_204 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		return true;
	}

	bool SITEMBASIC::LOAD_205 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		return true;
	}

	bool SITEMBASIC::LOAD_206 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;


		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		return true;
	}

	bool SITEMBASIC::LOAD_207 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		return true;
	}

	bool SITEMBASIC::LOAD_208 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsEXCHANGE() )
		{
			dwFlags |= ITEM_LOCKER; 
		}

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		return true;
	}

	bool SITEMBASIC::LOAD_209 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;

		if ( IsLocker() )
		{
			dwFlags |= ITEM_CLUB_LOCKER; 
		}

		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		return true;
	}

	bool SITEMBASIC::LOAD_210 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		return true;
	}

	bool SITEMBASIC::LOAD_211 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		return true;
	}

	bool SITEMBASIC::LOAD_212 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;	
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );		/* play time system, Juver, 2021/01/27 */
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		return true;
	}

	bool SITEMBASIC::LOAD_213 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;	
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );		/* play time system, Juver, 2021/01/27 */
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		SFile >> bBlockWrapper;

		return true;
	}

	bool SITEMBASIC::LOAD_214 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;	
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );		/* play time system, Juver, 2021/01/27 */
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		SFile >> bBlockWrapper;

		/* chaos machine, Juver, 2021/07/07 */
		SFile >> dwChaosMachineID;

		return true;
	}

	bool SITEMBASIC::LOAD_215 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;	
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );		/* play time system, Juver, 2021/01/27 */
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		SFile >> bBlockWrapper;

		/* chaos machine, Juver, 2021/07/07 */
		SFile >> dwChaosMachineID;

		/* Item Reform use limit, Juver, 2021/07/23 */
		SFile >> wMaxItemReform;

		return true;
	}

	bool SITEMBASIC::LOAD_216 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;	
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );		/* play time system, Juver, 2021/01/27 */
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		SFile >> bBlockWrapper;

		/* chaos machine, Juver, 2021/07/07 */
		SFile >> dwChaosMachineID;

		/* Item Reform use limit, Juver, 2021/07/23 */
		SFile >> wMaxItemReform;

		/* item refines ID, Juver, 2021/07/30 */
		SFile >> wItemRefinesID;

		return true;
	}

	bool SITEMBASIC::LOAD_217 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;	
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );		/* play time system, Juver, 2021/01/27 */
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		SFile >> bBlockWrapper;

		/* chaos machine, Juver, 2021/07/07 */
		SFile >> dwChaosMachineID;

		/* Item Reform use limit, Juver, 2021/07/23 */
		SFile >> wMaxItemReform;

		/* item refines ID, Juver, 2021/07/30 */
		SFile >> wItemRefinesID;

		/* set item option, Juver, 2021/07/30 */
		SFile >> strGenSetOption;

		return true;
	}

	bool SITEMBASIC::LOAD_218 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;	
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );		/* play time system, Juver, 2021/01/27 */
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		SFile >> bBlockWrapper;

		/* chaos machine, Juver, 2021/07/07 */
		SFile >> dwChaosMachineID;

		/* Item Reform use limit, Juver, 2021/07/23 */
		SFile >> wMaxItemReform;

		/* item refines ID, Juver, 2021/07/30 */
		SFile >> wItemRefinesID;

		/* set item option, Juver, 2021/07/30 */
		SFile >> strGenSetOption;

		SFile >> strExchangeItem;

		return true;
	}

	bool SITEMBASIC::LOAD_219 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;	
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );		/* play time system, Juver, 2021/01/27 */
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		SFile >> bBlockWrapper;

		/* chaos machine, Juver, 2021/07/07 */
		SFile >> dwChaosMachineID;

		/* Item Reform use limit, Juver, 2021/07/23 */
		SFile >> wMaxItemReform;

		/* item refines ID, Juver, 2021/07/30 */
		SFile >> wItemRefinesID;

		/* set item option, Juver, 2021/07/30 */
		SFile >> strGenSetOption;

		SFile >> strExchangeItem;

		SFile >> dwData;	
		emVIPLevel = static_cast<EMVIP_LEVEL>(dwData);

		return true;
	}

	bool SITEMBASIC::LOAD_220 ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;	
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );		/* play time system, Juver, 2021/01/27 */
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		SFile >> bBlockWrapper;

		/* chaos machine, Juver, 2021/07/07 */
		SFile >> dwChaosMachineID;

		/* Item Reform use limit, Juver, 2021/07/23 */
		SFile >> wMaxItemReform;

		/* item refines ID, Juver, 2021/07/30 */
		SFile >> wItemRefinesID;

		/* set item option, Juver, 2021/07/30 */
		SFile >> strGenSetOption;

		SFile >> strExchangeItem;

		SFile >> dwData;	
		emVIPLevel = static_cast<EMVIP_LEVEL>(dwData);

		SFile >> dwRebornReq;	

		return true;
	}

	bool SITEMBASIC::LOAD ( basestream &SFile )
	{
		DWORD dwData(0);

		SFile >> sNativeID.dwID;
		SFile >> sGroupID.dwID;
		SFile >> dwData;	emLevel = (EMITEMLEVEL)dwData;
		SFile >> dwData;	emItemType = (EMITEM_TYPE) dwData;
		SFile >> strName;
		SFile >> fExpMultiple;
		SFile >> wGradeAttack;
		SFile >> wGradeDefense;
		SFile >> dwFlags;
		SFile >> dwBuyPrice;
		SFile >> dwSellPrice;	
		SFile.ReadBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );		/* play time system, Juver, 2021/01/27 */
		SFile >> wReserved1;
		SFile >> wReserved2;
		SFile >> wReserved3;
		SFile >> wReserved4;
		SFile >> wReserved5;
		SFile >> dwReqCharClass;
		SFile >> dwReqSchool;
		SFile.ReadBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile >> wReqLevelDW;
		SFile >> wReqLevelUP;
		SFile >> wReqPA;
		SFile >> wReqSA;
		SFile >> dwData;	emReqBright = (EMBRIGHT) dwData;
		SFile >> dwCoolTime;
		SFile >> dwData;	emCoolType = (EMCOOL_TYPE) dwData;		
		SFile >> wInvenSizeX;
		SFile >> wInvenSizeY;
		SFile >> sICONID.dwID;
		SFile >> strFieldFile;
		SFile >> strInventoryFile;
		SFile >> strTargBodyEffect;
		SFile >> strTargetEffect;
		SFile >> strSelfBodyEffect;

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile >> strWearingFileLeft[i];

		SFile >> strPetWearingFile;
		SFile >> strComment;
		SFile >> sSubID.dwID;
		SFile >> wPosX;
		SFile >> wPosY;
		SFile >> bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile >> dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile >> dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile >> dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile >> bItemColor;
		SFile >> wItemColor1;
		SFile >> wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile >> sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile >> bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile >> bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile >> bWrapperBypass;

		SFile >> unknown_var_1;
		SFile >> unknown_var_2;
		SFile >> unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile >> bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		SFile >> bBlockWrapper;

		/* chaos machine, Juver, 2021/07/07 */
		SFile >> dwChaosMachineID;

		/* Item Reform use limit, Juver, 2021/07/23 */
		SFile >> wMaxItemReform;

		/* item refines ID, Juver, 2021/07/30 */
		SFile >> wItemRefinesID;

		/* set item option, Juver, 2021/07/30 */
		SFile >> strGenSetOption;

		SFile >> strExchangeItem;

		SFile >> dwData;	
		emVIPLevel = static_cast<EMVIP_LEVEL>(dwData);

		SFile >> dwRebornReq;	

		SFile >> bUseRVCard;

		return true;
	}

	bool SITEMBASIC::SAVE ( CSerialFile &SFile )
	{
		CString cstrName;
		cstrName.Format( _T("IN_%03d_%03d"), sNativeID.wMainID , sNativeID.wSubID ); // by 경대
		strName = cstrName.GetString();

		cstrName.Format( _T("ID_%03d_%03d"), sNativeID.wMainID , sNativeID.wSubID ); // by 경대
		strComment = cstrName.GetString();

		SFile << sNativeID.dwID;
		SFile << sGroupID.dwID;
		SFile << (DWORD)emLevel;
		SFile << (DWORD)emItemType;
		SFile << strName;
		SFile << fExpMultiple;
		SFile << wGradeAttack;
		SFile << wGradeDefense;
		SFile << dwFlags;
		SFile << dwBuyPrice;
		SFile << dwSellPrice;	
		SFile.WriteBuffer ( &llPlayTimeReq, sizeof(llPlayTimeReq) );	/* play time system, Juver, 2021/01/27 */
		SFile << wReserved1;
		SFile << wReserved2;
		SFile << wReserved3;
		SFile << wReserved4;
		SFile << wReserved5;
		SFile << dwReqCharClass;
		SFile << dwReqSchool;
		SFile.WriteBuffer ( &sReqStats, sizeof(sReqStats) );
		SFile << wReqLevelDW;
		SFile << wReqLevelUP;
		SFile << wReqPA;
		SFile << wReqSA;
		SFile << (DWORD)emReqBright;
		SFile << dwCoolTime;
		SFile << (DWORD)emCoolType;
		SFile << wInvenSizeX;
		SFile << wInvenSizeY;
		SFile << sICONID.dwID;
		SFile << strFieldFile;
		SFile << strInventoryFile;
		SFile << strTargBodyEffect;
		SFile << strTargetEffect;
		SFile << strSelfBodyEffect;
		
		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile << strWearingFileRight[i];

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile << strWearingFileLeft[i];

		SFile << strPetWearingFile;
		SFile << strComment;
		SFile << sSubID.dwID;
		SFile << wPosX;
		SFile << wPosY;
		SFile << bEnable;

		/*activity point, Juver, 2017/08/23 */
		SFile << dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		SFile << dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile << dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		SFile << bItemColor;
		SFile << wItemColor1;
		SFile << wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		SFile << sidWrapperBox.dwID;

		/*item transfer card, Juver, 2018/01/18 */
		SFile << bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile << bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile << bWrapperBypass;

		SFile << unknown_var_1;
		SFile << unknown_var_2;
		SFile << unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		SFile << bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		SFile << bBlockWrapper;

		/* chaos machine, Juver, 2021/07/07 */
		SFile << dwChaosMachineID;

		/* Item Reform use limit, Juver, 2021/07/23 */
		SFile << wMaxItemReform;

		/* item refines ID, Juver, 2021/07/30 */
		SFile << wItemRefinesID;

		/* set item option, Juver, 2021/07/30 */
		SFile << strGenSetOption;

		SFile << strExchangeItem;

		SFile << static_cast<int>(emVIPLevel);

		SFile << dwRebornReq;	

		SFile << bUseRVCard;

		return true;
	}

	VOID SITEMBASIC::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "sNativeID wMainID" << ",";
		SFile << "sNativeID wSubID" << ",";
		SFile << "sGroupID wMainID" << ",";
		SFile << "sGroupID wSubID" << ",";

		SFile << "strName" << ",";

	//		기획팀 요청으로 제거함
		SFile << "strName_FULL" << ",";

		SFile << "emLevel" << ",";

		SFile << "wGradeAttack" << ",";
		SFile << "wGradeDefense" << ",";

		SFile << "fExpMultiple" << ",";

		SFile << "wReserved1" << ",";
		SFile << "wReserved2" << ",";
		SFile << "wReserved3" << ",";
		SFile << "wReserved4" << ",";
		SFile << "wReserved5" << ",";

		SFile << "dwFlags" << ",";
		SFile << "dwBuyPrice" << ",";
		SFile << "dwSellPrices" << ",";

		SFile << "llPlayTimeReq" << ",";		/* play time system, Juver, 2021/01/27 */

		SFile << "emItemType" << ",";

		SFile << "emReqBright" << ",";
		SFile << "dwReqCharClass" << ",";
		SFile << "dwReqSchool" << ",";
		SFile << "wReqLevelDW" << ",";
		SFile << "wReqLevelUP" << ",";
		SFile << "wReqPA" << ",";
		SFile << "wReqSA" << ",";
		SFile << "sReqStats wPow" << ",";
		SFile << "sReqStats wStr" << ",";
		SFile << "sReqStats wSpi" << ",";
		SFile << "sReqStats wDex" << ",";
		SFile << "sReqStats wInt" << ",";
		SFile << "sReqStats wSta" << ",";

		SFile << "wInvenSizeX" << ",";
		SFile << "wInvenSizeY" << ",";

		SFile << "sICONID wMainID" << ",";
		SFile << "sICONID wSubID" << ",";

		SFile << "strSelfBodyEffect" << ",";
		SFile << "strTargBodyEffect" << ",";
		SFile << "strTargetEffect" << ",";

		SFile << "strFieldFile" << ",";
		SFile << "strInventoryFile" << ",";

		for( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile << "strWearingFileRight " << i << ",";

		for( int i=0; i<GLCI_NUM_8CLASS; ++i )
			SFile << "strWearingFileLeft " << i << ",";

		SFile << "strComment" << ",";

		// PET
		SFile << "strPetWearingFile" << ",";

		SFile << "sMapID wMainID" << ",";
		SFile << "sMapID wSubID" << ",";

		SFile << "wPosX" << ",";
		SFile << "wPosY" << ",";

		SFile << "dwCoolTime" << ",";
		SFile << "emCoolType" << ",";
		SFile << "bEnable" << ",";

		/*activity point, Juver, 2017/08/23 */
		SFile << "dwReqActivityPoint" << ",";

		/*contribution point, Juver, 2017/08/23 */
		SFile << "dwReqContributionPoint" << ",";

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile << "dwReqUserNum" << ",";

		/*item color, Juver, 2018/01/05 */
		SFile << "bItemColor" << ",";
		SFile << "wItemColor1" << ",";
		SFile << "wItemColor2" << ",";

		/*item wrapper, Juver, 2018/01/11 */
		SFile << "sidWrapperBox wMainID" << ",";
		SFile << "sidWrapperBox wSubID" << ",";

		/*item transfer card, Juver, 2018/01/18 */
		SFile << "bItemTransfer" << ",";

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile << "bUseRebuild" << ",";

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile << "bWrapperBypass" << ",";

		SFile << "unknown_var_1" << ",";
		SFile << "unknown_var_2" << ",";
		SFile << "unknown_var_3" << ",";

		/* item always log, Juver, 2020/10/23 */
		SFile << "bAlwaysLog" << ",";

		/* block wrapper, Juver, 2021/06/08 */
		SFile << "bBlockWrapper" << ",";

		/* chaos machine, Juver, 2021/07/07 */
		SFile << "dwChaosMachineID" << ",";

		/* Item Reform use limit, Juver, 2021/07/23 */
		SFile << "wMaxItemReform" << ",";

		/* item refines ID, Juver, 2021/07/30 */
		SFile << "wItemRefinesID" << ",";

		/* set item option, Juver, 2021/07/30 */
		SFile << "strGenSetOption" << ",";

		SFile << "strExchangeItem" << ",";

		SFile << "emVIPLevel" << ",";

		SFile << "dwRebornReq" << ",";

		SFile << "bUseRVCard" << ",";
	}

	VOID SITEMBASIC::SaveCsv ( std::fstream &SFile )
	{
		SFile << sNativeID.wMainID << ",";
		SFile << sNativeID.wSubID << ",";
		SFile << sGroupID.wMainID << ",";
		SFile << sGroupID.wSubID << ",";

		STRUTIL::OutputStrCsv( SFile, strName );

	//		기획팀 요청으로 제거함

		std::string str = strName;
		const char* szpName = str.c_str();
		if( szpName )
		{
			const char* szpLongName = GLStringTable::GetInstance().GetString( szpName, GLStringTable::ITEM );
			if( szpLongName )
				str = szpLongName;
		}
		STRUTIL::OutputStrCsv( SFile, str );

		SFile << emLevel << ",";

		SFile << wGradeAttack << ",";
		SFile << wGradeDefense << ",";

		SFile << fExpMultiple << ",";

		SFile << wReserved1 << ",";
		SFile << wReserved2 << ",";
		SFile << wReserved3 << ",";
		SFile << wReserved4 << ",";
		SFile << wReserved5 << ",";

		SFile << dwFlags << ",";
		SFile << dwBuyPrice << ",";
		SFile << dwSellPrice << ",";

		SFile << llPlayTimeReq << ",";	/* play time system, Juver, 2021/01/27 */

		SFile << emItemType << ",";

		SFile << emReqBright << ",";
		SFile << dwReqCharClass << ",";
		SFile << dwReqSchool << ",";
		SFile << wReqLevelDW << ",";
		SFile << wReqLevelUP << ",";
		SFile << wReqPA << ",";
		SFile << wReqSA << ",";
		SFile << sReqStats.wPow << ",";
		SFile << sReqStats.wStr << ",";
		SFile << sReqStats.wSpi << ",";
		SFile << sReqStats.wDex << ",";
		SFile << sReqStats.wInt << ",";
		SFile << sReqStats.wSta << ",";

		SFile << wInvenSizeX << ",";
		SFile << wInvenSizeY << ",";

		SFile << sICONID.wMainID << ",";
		SFile << sICONID.wSubID << ",";

		STRUTIL::OutputStrCsv( SFile, strSelfBodyEffect );
		STRUTIL::OutputStrCsv( SFile, strTargBodyEffect );
		STRUTIL::OutputStrCsv( SFile, strTargetEffect );

		STRUTIL::OutputStrCsv( SFile, strFieldFile );
		STRUTIL::OutputStrCsv( SFile, strInventoryFile );

		for( int i=0; i<GLCI_NUM_8CLASS; ++i )
			STRUTIL::OutputStrCsv( SFile, strWearingFileRight[i] );

		for( int i=0; i<GLCI_NUM_8CLASS; ++i )
			STRUTIL::OutputStrCsv( SFile, strWearingFileLeft[i] );

		STRUTIL::OutputStrCsv( SFile, strComment );

		// PET
		STRUTIL::OutputStrCsv ( SFile, strPetWearingFile );

		SFile << sSubID.wMainID << ",";
		SFile << sSubID.wSubID << ",";
		
		SFile << wPosX << ",";
		SFile << wPosY << ",";

		SFile << dwCoolTime << ",";
		SFile << emCoolType << ",";

		SFile << bEnable << ",";

		/*activity point, Juver, 2017/08/23 */
		SFile << dwReqActivityPoint << ",";

		/*contribution point, Juver, 2017/08/23 */
		SFile << dwReqContributionPoint << ",";

		/*UserNum ItemReq, Juver, 2017/06/27 */
		SFile << dwReqUserNum << ",";

		/*item color, Juver, 2018/01/05 */
		SFile << bItemColor << ",";
		SFile << wItemColor1 << ",";
		SFile << wItemColor2 << ",";

		/*item wrapper, Juver, 2018/01/11 */
		SFile << sidWrapperBox.wMainID << ",";
		SFile << sidWrapperBox.wSubID << ",";

		/*item transfer card, Juver, 2018/01/18 */
		SFile << bItemTransfer << ",";

		/*use rebuild flag, Juver, 2019/02/18 */
		SFile << bUseRebuild << ",";

		/*wrapper bypass, Juver, 2019/02/19 */
		SFile << bWrapperBypass << ",";

		SFile << unknown_var_1 << ",";
		STRUTIL::OutputStrCsv ( SFile, unknown_var_2 );
		STRUTIL::OutputStrCsv ( SFile, unknown_var_3 );

		/* item always log, Juver, 2020/10/23 */
		SFile << bAlwaysLog << ",";

		/* block wrapper, Juver, 2021/06/08 */
		SFile << bBlockWrapper << ",";

		/* chaos machine, Juver, 2021/07/07 */
		SFile << dwChaosMachineID << ",";

		/* Item Reform use limit, Juver, 2021/07/23 */
		SFile << wMaxItemReform << ",";

		/* item refines ID, Juver, 2021/07/30 */
		SFile << wItemRefinesID << ",";

		/* set item option, Juver, 2021/07/30 */
		STRUTIL::OutputStrCsv ( SFile, strGenSetOption );

		STRUTIL::OutputStrCsv ( SFile, strExchangeItem );

		SFile << emVIPLevel << ",";

		SFile << dwRebornReq << ",";

		SFile << bUseRVCard << ",";
	}

	VOID SITEMBASIC::LoadCsv ( CStringArray &StrArray, int &iCsvCur  )
	{
		sNativeID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sNativeID.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sGroupID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sGroupID.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );

		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strName );

	//		기획팀 요청으로 제거함
		++iCsvCur;	// Item Name

		emLevel = (EMITEMLEVEL)atoi( StrArray[ iCsvCur++ ] );

		wGradeAttack = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wGradeDefense = (WORD)atoi( StrArray[ iCsvCur++ ] );

		fExpMultiple = (float)atof( StrArray[ iCsvCur++ ] );

		wReserved1 = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wReserved2 = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wReserved3 = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wReserved4 = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wReserved5 = (WORD)atoi( StrArray[ iCsvCur++ ] );

		dwFlags = (DWORD)atol( StrArray[ iCsvCur++ ] );
		dwBuyPrice  = (DWORD)atol( StrArray[ iCsvCur++ ] );
		dwSellPrice = (DWORD)atol( StrArray[ iCsvCur++ ] );

		llPlayTimeReq = _atoi64( StrArray[ iCsvCur++ ] );	/* play time system, Juver, 2021/01/27 */

		emItemType = (EMITEM_TYPE)atoi( StrArray[ iCsvCur++ ] );

		emReqBright = (EMBRIGHT)atoi( StrArray[ iCsvCur++ ] );
		dwReqCharClass = (DWORD)atol( StrArray[ iCsvCur++ ] );
		dwReqSchool = (DWORD)atol( StrArray[ iCsvCur++ ] );
		wReqLevelDW = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wReqLevelUP = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wReqPA = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wReqSA = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sReqStats.wPow = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sReqStats.wStr = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sReqStats.wSpi = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sReqStats.wDex = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sReqStats.wInt = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sReqStats.wSta = (WORD)atoi( StrArray[ iCsvCur++ ] );

		wInvenSizeX = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wInvenSizeY = (WORD)atoi( StrArray[ iCsvCur++ ] );

		sICONID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sICONID.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );

		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strSelfBodyEffect );
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTargBodyEffect );
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTargetEffect );

		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strFieldFile );
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strInventoryFile );

		for( int i=0; i<GLCI_NUM_8CLASS; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strWearingFileRight[i] );

		for( int i=0; i<GLCI_NUM_8CLASS; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strWearingFileLeft[i] );

		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strComment );

		// PET
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strPetWearingFile );

		sSubID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sSubID.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		
		wPosX = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wPosY = (WORD)atoi( StrArray[ iCsvCur++ ] );

		dwCoolTime = (DWORD)atoi( StrArray[ iCsvCur++ ] );
		emCoolType = (EMCOOL_TYPE)atoi( StrArray[ iCsvCur++ ] );

		bEnable = (bool)atoi( StrArray[ iCsvCur++ ] );

		/*activity point, Juver, 2017/08/23 */
		dwReqActivityPoint = (DWORD)atoi( StrArray[ iCsvCur++ ] );

		/*contribution point, Juver, 2017/08/23 */
		dwReqContributionPoint = (DWORD)atoi( StrArray[ iCsvCur++ ] );

		/*UserNum ItemReq, Juver, 2017/06/27 */
		dwReqUserNum = (DWORD)atoi( StrArray[ iCsvCur++ ] );

		/*item color, Juver, 2018/01/05 */
		bItemColor = (bool)atoi( StrArray[ iCsvCur++ ] );
		wItemColor1 = (WORD)atoi( StrArray[ iCsvCur++ ] );
		wItemColor2 = (WORD)atoi( StrArray[ iCsvCur++ ] );

		/*item wrapper, Juver, 2018/01/11 */
		sidWrapperBox.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sidWrapperBox.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );

		/*item transfer card, Juver, 2018/01/18 */
		bItemTransfer = (bool)atoi( StrArray[ iCsvCur++ ] );

		/*use rebuild flag, Juver, 2019/02/18 */
		bUseRebuild = (bool)atoi( StrArray[ iCsvCur++ ] );

		/*wrapper bypass, Juver, 2019/02/19 */
		bWrapperBypass = (bool)atoi( StrArray[ iCsvCur++ ] );

		unknown_var_1 = (int)atoi( StrArray[ iCsvCur++ ] );
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], unknown_var_2 );
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], unknown_var_3 );

		/* item always log, Juver, 2020/10/23 */
		bAlwaysLog = (bool)atoi( StrArray[ iCsvCur++ ] );

		/* block wrapper, Juver, 2021/06/08 */
		bBlockWrapper = (bool)atoi( StrArray[ iCsvCur++ ] );

		/* chaos machine, Juver, 2021/07/07 */
		dwChaosMachineID = (DWORD)atoi( StrArray[ iCsvCur++ ] );

		/* Item Reform use limit, Juver, 2021/07/23 */
		wMaxItemReform = (WORD)atoi( StrArray[ iCsvCur++ ] );

		/* item refines ID, Juver, 2021/07/30 */
		wItemRefinesID = (WORD)atoi( StrArray[ iCsvCur++ ] );

		/* set item option, Juver, 2021/07/30 */
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strGenSetOption );

		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strExchangeItem );

		emVIPLevel = static_cast<EMVIP_LEVEL>(atoi( StrArray[ iCsvCur++ ] ));

		dwRebornReq = (DWORD)atoi( StrArray[ iCsvCur++ ] );

		bUseRVCard = atoi( StrArray[ iCsvCur++ ] ) ? TRUE : FALSE;
	}

	SITEMBASIC& SITEMBASIC::operator = ( const SITEMBASIC& rvalue )
	{
		sNativeID = rvalue.sNativeID;					//	고유 ID. ( 아이템 설정 최초 생성시에 부여되는 고유 ID )
		sGroupID = rvalue.sGroupID;
		emLevel = rvalue.emLevel;						//	아이템 가치 등급.
		emItemType = rvalue.emItemType;					//	아이템 종류.
		strName = rvalue.strName;						//	아이템의 이름.	
		fExpMultiple = rvalue.fExpMultiple;				// 경험치 개조 등급
		wGradeAttack = rvalue.wGradeAttack;				// 공격 개조 등급
		wGradeDefense = rvalue.wGradeDefense;			// 방어 개조 등급
		dwFlags = rvalue.dwFlags;						//	Trade 속성.
		dwBuyPrice	= rvalue.dwBuyPrice;				//	아이템 구입 가격.
		dwSellPrice = rvalue.dwSellPrice;				//	아이템 판매 가격.
		llPlayTimeReq = rvalue.llPlayTimeReq;			/* play time system, Juver, 2021/01/27 */
		wReserved1 = rvalue.wReserved1;					// 사용 안함
		wReserved2 = rvalue.wReserved2;					// 사용 안함
		wReserved3 = rvalue.wReserved3;					// 사용 안함
		wReserved4 = rvalue.wReserved4;					// 사용 안함
		wReserved5 = rvalue.wReserved4;					// 사용 안함
		dwReqCharClass = rvalue.dwReqCharClass;			//	착용할 수 있는 직업들. ( EMCHARCLASS Flags )
		dwReqSchool = rvalue.dwReqSchool;				//	착용할 수 있는 학원.
		sReqStats = rvalue.sReqStats;					//	요구 케릭터 Stats 수치.
		wReqLevelDW = rvalue.wReqLevelDW;				//	요구 Level.
		wReqLevelUP = rvalue.wReqLevelUP;				//	요구 Level.
		wReqPA = rvalue.wReqPA;							//	요구 격투치.
		wReqSA = rvalue.wReqSA;							//	요구 사격치.
		emReqBright = rvalue.emReqBright;				//	캐릭 속성. ( 광/암 )
		dwCoolTime = rvalue.dwCoolTime;
		emCoolType = rvalue.emCoolType;
		wInvenSizeX = rvalue.wInvenSizeX;				//	인벤토리 사이즈. 
		wInvenSizeY = rvalue.wInvenSizeY;				//	인벤토리 사이즈.
		sICONID = rvalue.sICONID;						//	아이콘 인덱스.
		strFieldFile = rvalue.strFieldFile;				//	바닥 형상 파일.
		strInventoryFile = rvalue.strInventoryFile;		//	인벤토리 형상 파일.
		strTargBodyEffect = rvalue.strTargBodyEffect;	//	목표 유닛의 몸에 붓는 이팩트.
		strTargetEffect = rvalue.strTargetEffect;		//	목표 지향 이팩트.
		strSelfBodyEffect = rvalue.strSelfBodyEffect;	//	자기 자신의 몸에 붓는 이팩트.
	
		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )	
			strWearingFileRight[i] = rvalue.strWearingFileRight[i];	//	형상 파일.

		for ( int i=0; i<GLCI_NUM_8CLASS; ++i )	
			strWearingFileLeft[i] = rvalue.strWearingFileLeft[i];	//	형상 파일.

		strPetWearingFile = rvalue.strPetWearingFile;
		strComment = rvalue.strComment;
		sSubID = rvalue.sSubID;
		wPosX = rvalue.wPosX;
		wPosY = rvalue.wPosY;

		bEnable = rvalue.bEnable;

		/*activity point, Juver, 2017/08/23 */
		dwReqActivityPoint = rvalue.dwReqActivityPoint;

		/*contribution point, Juver, 2017/08/23 */
		dwReqContributionPoint = rvalue.dwReqContributionPoint;

		/*UserNum ItemReq, Juver, 2017/06/27 */
		dwReqUserNum = rvalue.dwReqUserNum;

		/*item color, Juver, 2018/01/05 */
		bItemColor	= rvalue.bItemColor;
		wItemColor1 = rvalue.wItemColor1;
		wItemColor2 = rvalue.wItemColor2;

		/*item wrapper, Juver, 2018/01/11 */
		sidWrapperBox = rvalue.sidWrapperBox;

		/*item transfer card, Juver, 2018/01/18 */
		bItemTransfer = rvalue.bItemTransfer;

		/*use rebuild flag, Juver, 2019/02/18 */
		bUseRebuild		= rvalue.bUseRebuild;

		/*wrapper bypass, Juver, 2019/02/19 */
		bWrapperBypass	= rvalue.bWrapperBypass;

		unknown_var_1 = rvalue.unknown_var_1;
		unknown_var_2 = rvalue.unknown_var_2;
		unknown_var_3 = rvalue.unknown_var_3;

		/* item always log, Juver, 2020/10/23 */
		bAlwaysLog = rvalue.bAlwaysLog;

		/* block wrapper, Juver, 2021/06/08 */
		bBlockWrapper = rvalue.bBlockWrapper;

		/* chaos machine, Juver, 2021/07/07 */
		dwChaosMachineID = rvalue.dwChaosMachineID;

		/* Item Reform use limit, Juver, 2021/07/23 */
		wMaxItemReform = rvalue.wMaxItemReform;

		/* item refines ID, Juver, 2021/07/30 */
		wItemRefinesID = rvalue.wItemRefinesID;

		/* set item option, Juver, 2021/07/30 */
		strGenSetOption	= rvalue.strGenSetOption;

		strExchangeItem	= rvalue.strExchangeItem;

		emVIPLevel = rvalue.emVIPLevel;

		dwRebornReq = rvalue.dwRebornReq;

		bUseRVCard	= rvalue.bUseRVCard;

		return *this;
	}


	void SITEMBASIC::class_convert()
	{
		DWORD old_class = dwReqCharClass;

		if ( dwReqCharClass == GLCC_ALL_5CLASS_EX )
			dwReqCharClass = GLCC_ALL_8CLASS;
		else if ( dwReqCharClass == GLCC_MAN_5CLASS_EX )
			dwReqCharClass = GLCC_MAN_8CLASS;
		else if ( dwReqCharClass == GLCC_WOMAN_5CLASS_EX )
			dwReqCharClass = GLCC_WOMAN_8CLASS;
		else if ( dwReqCharClass == GLCC_ALL_6CLASS )
			dwReqCharClass = GLCC_ALL_8CLASS;
		else if ( dwReqCharClass == GLCC_MAN_6CLASS )
			dwReqCharClass = GLCC_MAN_8CLASS;
		else if ( dwReqCharClass == GLCC_WOMAN_6CLASS )
			dwReqCharClass = GLCC_WOMAN_8CLASS;
		else if ( dwReqCharClass == GLCC_ALL_7CLASS )
			dwReqCharClass = GLCC_ALL_8CLASS;
		else if ( dwReqCharClass == GLCC_MAN_7CLASS )
			dwReqCharClass = GLCC_MAN_8CLASS;
		else if ( dwReqCharClass == GLCC_WOMAN_7CLASS )
			dwReqCharClass = GLCC_WOMAN_8CLASS;

		if ( old_class != dwReqCharClass )
		{
			std::string piece_l_m;
			std::string piece_l_w;
			std::string piece_r_m;
			std::string piece_r_w;

			for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			{
				if ( CharIndexToGender((EMCHARINDEX)i ) == GLGENDER_M && !piece_l_m.size() ) 
				{
					piece_l_m = strWearingFileLeft[i];
				}

				if ( CharIndexToGender((EMCHARINDEX)i ) == GLGENDER_W && !piece_l_w.size() ) 
				{
					piece_l_w = strWearingFileLeft[i];
				}

				if ( CharIndexToGender((EMCHARINDEX)i ) == GLGENDER_M && !piece_r_m.size() ) 
				{
					piece_r_m = strWearingFileRight[i];
				}

				if ( CharIndexToGender((EMCHARINDEX)i ) == GLGENDER_W && !piece_r_w.size() ) 
				{
					piece_r_w = strWearingFileRight[i];
				}
			}

			for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
			{
				if ( CharIndexToGender((EMCHARINDEX)i ) == GLGENDER_M && piece_l_m.size() ) 
				{
					strWearingFileLeft[i] = piece_l_m;
				}

				if ( CharIndexToGender((EMCHARINDEX)i ) == GLGENDER_W && piece_l_w.size() ) 
				{
					strWearingFileLeft[i] = piece_l_w;
				}

				if ( CharIndexToGender((EMCHARINDEX)i ) == GLGENDER_M && piece_r_m.size() ) 
				{
					strWearingFileRight[i] = piece_r_m;
				}

				if ( CharIndexToGender((EMCHARINDEX)i ) == GLGENDER_W && piece_r_w.size() ) 
				{
					strWearingFileRight[i] = piece_r_w;
				}
			}
		}
	}
};