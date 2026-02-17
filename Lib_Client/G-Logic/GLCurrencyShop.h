#ifndef GLCURRENCYSHOP_H__INCLUDED
#define GLCURRENCYSHOP_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

#include "../../Lib_Engine/G-Logic/GLDefine.h"

struct SCURRENCY_SHOP_ITEM
{
	SNATIVEID	sItemID;

	BYTE		cDAMAGE;
	BYTE		cDEFENSE;
	BYTE		cRESIST_FIRE;
	BYTE		cRESIST_ICE;
	BYTE		cRESIST_ELEC;
	BYTE		cRESIST_POISON;	
	BYTE		cRESIST_SPIRIT;
	BOOL		bGenerateRandomValue;

	LONGLONG	llPriceMoney;
	DWORD		dwPriceGamePoints;
	DWORD		dwPriceWarChips;

	SCURRENCY_SHOP_ITEM()
		: sItemID(false)
		, cDAMAGE(0)
		, cDEFENSE(0)
		, cRESIST_FIRE(0)
		, cRESIST_ICE(0)
		, cRESIST_ELEC(0)
		, cRESIST_POISON(0)
		, cRESIST_SPIRIT(0)
		, bGenerateRandomValue(FALSE)
		, llPriceMoney(0)
		, dwPriceGamePoints(0)
		, dwPriceWarChips(0)
	{
	};

	bool operator < ( const SCURRENCY_SHOP_ITEM& sData )
	{			
		if ( sItemID.wMainID < sData.sItemID.wMainID ) 
			return true;

		else if ( sItemID.wMainID == sData.sItemID.wMainID && 
			sItemID.wSubID < sData.sItemID.wSubID ) 
			return true;

		return  false;
	}
};

typedef std::map<DWORD, SCURRENCY_SHOP_ITEM*>		SCURRENCY_SHOP_ITEM_POINTER_MAP;
typedef SCURRENCY_SHOP_ITEM_POINTER_MAP::iterator	SCURRENCY_SHOP_ITEM_POINTER_MAP_ITER;
typedef std::vector<SCURRENCY_SHOP_ITEM>			SCURRENCY_SHOP_ITEM_VEC;


struct SCURRENCY_SHOP_DATA
{
	SCURRENCY_SHOP_ITEM_POINTER_MAP	m_mapData;
	std::string m_strName;
	std::string m_strTitle;

	bool Load( std::string strFile );
	SCURRENCY_SHOP_ITEM*	FindData( SNATIVEID sItemID );
	void DeleteData( SNATIVEID sItemID );
	void CleanUp();

	SCURRENCY_SHOP_DATA();
	~SCURRENCY_SHOP_DATA();
};

typedef std::map<std::string, SCURRENCY_SHOP_DATA*>		SCURRENCY_SHOP_DATA_POINTER_MAP;
typedef SCURRENCY_SHOP_DATA_POINTER_MAP::iterator		SCURRENCY_SHOP_DATA_POINTER_MAP_ITER;


#endif // GLCURRENCYSHOP_H__INCLUDED
