#ifndef GLNPCSHOP_H__INCLUDED
#define GLNPCSHOP_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

#include "../../Lib_Engine/G-Logic/GLDefine.h"

struct SNPC_SHOP_TYPE
{
	WORD		wID;
	std::string strName;
	WORD		wItemNum;

	SNPC_SHOP_TYPE()
		: wID(0)
		, strName("")
		, wItemNum(0)
	{
	};
};

struct SNPC_SHOP_ITEM
{
	SNATIVEID	sItemID;
	WORD		wType;

	BYTE	cDAMAGE;
	BYTE	cDEFENSE;
	BYTE	cRESIST_FIRE;
	BYTE	cRESIST_ICE;
	BYTE	cRESIST_ELEC;
	BYTE	cRESIST_POISON;	
	BYTE	cRESIST_SPIRIT;
	BOOL	bGenerateRandomValue;

	SNPC_SHOP_ITEM()
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
	{
	};

	bool operator < ( const SNPC_SHOP_ITEM& sData )
	{			
		if ( sItemID.wMainID < sData.sItemID.wMainID ) 
			return true;

		else if ( sItemID.wMainID == sData.sItemID.wMainID && 
			sItemID.wSubID < sData.sItemID.wSubID ) 
			return true;

		return  false;
	}
};

typedef std::map<WORD, SNPC_SHOP_TYPE*>			SNPC_SHOP_TYPE_POINTER_MAP;
typedef SNPC_SHOP_TYPE_POINTER_MAP::iterator	SNPC_SHOP_TYPE_POINTER_MAP_ITER;
typedef std::vector<SNPC_SHOP_TYPE>				SNPC_SHOP_TYPE_VEC;

typedef std::map<DWORD, SNPC_SHOP_ITEM*>		SNPC_SHOP_ITEM_POINTER_MAP;
typedef SNPC_SHOP_ITEM_POINTER_MAP::iterator	SNPC_SHOP_ITEM_POINTER_MAP_ITER;
typedef std::vector<SNPC_SHOP_ITEM>				SNPC_SHOP_ITEM_VEC;


struct SNPC_SHOP_DATA
{
	SNPC_SHOP_TYPE_POINTER_MAP		m_mapType;
	SNPC_SHOP_ITEM_POINTER_MAP		m_mapItem;
	std::string m_strName;
	std::string m_strTitle;
	WORD		m_wShopType;	//shop type default 0 for gold

	bool Load( std::string strFile );
	SNPC_SHOP_TYPE*	FindShopType( WORD wType );
	SNPC_SHOP_ITEM*	FindShopItem( DWORD dwItemID );

	void CleanUp();

	SNPC_SHOP_DATA();
	~SNPC_SHOP_DATA();
};

typedef std::map<std::string, SNPC_SHOP_DATA*>		SNPC_SHOP_DATA_POINTER_MAP;
typedef SNPC_SHOP_DATA_POINTER_MAP::iterator		SNPC_SHOP_DATA_POINTER_MAP_ITER;

#endif // GLNPCSHOP_H__INCLUDED



