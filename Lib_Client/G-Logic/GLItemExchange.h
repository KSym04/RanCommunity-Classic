#ifndef GLITEMEXCHANGE_H__INCLUDED
#define GLITEMEXCHANGE_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "./GLCharDefine.h"

#define NPC_ITEM_EXCHANGE_MAX_REQ	5
#define NPC_ITEM_EXCHANGE_NULL		512

struct SNPC_ITEM_EXCHANGE_ITEM
{
	SNATIVEID	sidItem;
	WORD		wItemNum;

	SNPC_ITEM_EXCHANGE_ITEM()
		: sidItem(false)
		, wItemNum(1)
	{
	};
};

struct SNPC_ITEM_EXCHANGE_DATA
{
	WORD	wID;
	SNPC_ITEM_EXCHANGE_ITEM	sidItemResult;
	SNPC_ITEM_EXCHANGE_ITEM	sidItemRequire[NPC_ITEM_EXCHANGE_MAX_REQ];

	BYTE	cDAMAGE;
	BYTE	cDEFENSE;
	BYTE	cRESIST_FIRE;
	BYTE	cRESIST_ICE;
	BYTE	cRESIST_ELEC;
	BYTE	cRESIST_POISON;	
	BYTE	cRESIST_SPIRIT;
	BOOL	bGenerateRandomValue;

	SNPC_ITEM_EXCHANGE_DATA()
		: wID( NPC_ITEM_EXCHANGE_NULL )
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

	bool operator < ( const SNPC_ITEM_EXCHANGE_DATA& sData )
	{			
		if ( wID < sData.wID ) 
			return true;

		return  false;
	}
};

typedef std::map<DWORD, SNPC_ITEM_EXCHANGE_DATA*>		SNPC_ITEM_EXCHANGE_DATA_POINTER_MAP;
typedef SNPC_ITEM_EXCHANGE_DATA_POINTER_MAP::iterator	SNPC_ITEM_EXCHANGE_DATA_POINTER_MAP_ITER;
typedef std::vector<SNPC_ITEM_EXCHANGE_DATA>			SNPC_ITEM_EXCHANGE_DATA_VEC;


struct SNPC_ITEM_EXCHANGE
{
	SNPC_ITEM_EXCHANGE_DATA_POINTER_MAP	m_mapData;
	std::string m_strName;
	std::string m_strTitle;


	bool Load( std::string strFile );
	SNPC_ITEM_EXCHANGE_DATA*	FindData( WORD wExchangeID );

	void CleanUp();

	SNPC_ITEM_EXCHANGE();
	~SNPC_ITEM_EXCHANGE();
};

typedef std::map<std::string, SNPC_ITEM_EXCHANGE*>		SNPC_ITEM_EXCHANGE_POINTER_MAP;
typedef SNPC_ITEM_EXCHANGE_POINTER_MAP::iterator		SNPC_ITEM_EXCHANGE_POINTER_MAP_ITER;


#endif // GLITEMEXCHANGE_H__INCLUDED

