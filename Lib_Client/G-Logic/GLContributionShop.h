/*!
 * \file GLContributionShop.h
 *
 * \author Juver
 * \date 2022/02/24
 *
 * 
 */

#ifndef GLCONTRIBUTIONSHOP_H_INCLUDED__
#define GLCONTRIBUTIONSHOP_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/G-Logic/GLDefine.h"

struct SNPC_CONTRIBUTION_SHOP_TYPE
{
	WORD		wID;
	std::string strName;
	WORD		wItemNum;

	SNPC_CONTRIBUTION_SHOP_TYPE()
		: wID(0)
		, strName("")
		, wItemNum(0)
	{
	};
};

struct SNPC_CONTRIBUTION_SHOP_ITEM
{
	SNATIVEID	sItemID;
	WORD	wType;
	BYTE	cDAMAGE;
	BYTE	cDEFENSE;
	BYTE	cRESIST_FIRE;
	BYTE	cRESIST_ICE;
	BYTE	cRESIST_ELEC;
	BYTE	cRESIST_POISON;	
	BYTE	cRESIST_SPIRIT;
	BOOL	bGenerateRandomValue;

	DWORD	dwContriPrice;
	DWORD	dwGoldPrice;

	SNPC_CONTRIBUTION_SHOP_ITEM()
		: sItemID(false)
		, wType(0)
		, cDAMAGE(0)
		, cDEFENSE(0)
		, cRESIST_FIRE(0)
		, cRESIST_ICE(0)
		, cRESIST_ELEC(0)
		, cRESIST_POISON(0)
		, cRESIST_SPIRIT(0)
		, bGenerateRandomValue(FALSE)
		, dwContriPrice(0)
		, dwGoldPrice(0)
	{
	};

	bool operator < ( const SNPC_CONTRIBUTION_SHOP_ITEM& sData )
	{			
		if ( sItemID.wMainID < sData.sItemID.wMainID ) 
			return true;

		else if ( sItemID.wMainID == sData.sItemID.wMainID && 
			sItemID.wSubID < sData.sItemID.wSubID ) 
			return true;

		return  false;
	}
};

typedef std::map<WORD, SNPC_CONTRIBUTION_SHOP_TYPE*>		SNPC_CONTRIBUTION_SHOP_TYPE_MAP;
typedef SNPC_CONTRIBUTION_SHOP_TYPE_MAP::iterator			SNPC_CONTRIBUTION_SHOP_TYPE_MAP_ITER;
typedef std::vector<SNPC_CONTRIBUTION_SHOP_TYPE>			SNPC_CONTRIBUTION_SHOP_TYPE_VEC;

typedef std::map<DWORD, SNPC_CONTRIBUTION_SHOP_ITEM*>		SNPC_CONTRIBUTION_SHOP_ITEM_MAP;
typedef SNPC_CONTRIBUTION_SHOP_ITEM_MAP::iterator			SNPC_CONTRIBUTION_SHOP_ITEM_MAP_ITER;
typedef std::vector<SNPC_CONTRIBUTION_SHOP_ITEM>			SNPC_CONTRIBUTION_SHOP_ITEM_VEC;


struct SNPC_CONTRIBUTION_SHOP_DATA
{
	std::string m_strName;
	std::string m_strTitle;

	SNPC_CONTRIBUTION_SHOP_TYPE_MAP		m_mapType;
	SNPC_CONTRIBUTION_SHOP_ITEM_MAP		m_mapItem;

	SNPC_CONTRIBUTION_SHOP_DATA();
	~SNPC_CONTRIBUTION_SHOP_DATA();

	bool Load( std::string strFile );
	void CleanUp();

	SNPC_CONTRIBUTION_SHOP_TYPE*	FindType( WORD wType );
	SNPC_CONTRIBUTION_SHOP_ITEM*	FindItem( DWORD dwItemID );
};

typedef std::map<std::string, SNPC_CONTRIBUTION_SHOP_DATA*>		SNPC_CONTRIBUTION_SHOP_DATA_MAP;
typedef SNPC_CONTRIBUTION_SHOP_DATA_MAP::iterator				SNPC_CONTRIBUTION_SHOP_DATA_MAP_ITER;

#endif // GLCONTRIBUTIONSHOP_H_INCLUDED__
