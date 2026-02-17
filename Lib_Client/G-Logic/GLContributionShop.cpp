#include "stdafx.h"
#include "../../Lib_Engine/Common/StlFunctions.h"
#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/Core/NSRPath.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"

#include "./GLogicData.h"
#include "./GLContributionShop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



SNPC_CONTRIBUTION_SHOP_DATA::SNPC_CONTRIBUTION_SHOP_DATA()
	: m_strName("")
	, m_strTitle("")
{

}

SNPC_CONTRIBUTION_SHOP_DATA::~SNPC_CONTRIBUTION_SHOP_DATA()
{
	CleanUp();
}

void SNPC_CONTRIBUTION_SHOP_DATA::CleanUp()
{
	std::for_each ( m_mapType.begin(), m_mapType.end(), std_afunc::DeleteMapObject() );
	m_mapType.clear();

	std::for_each ( m_mapItem.begin(), m_mapItem.end(), std_afunc::DeleteMapObject() );
	m_mapItem.clear();
}

bool SNPC_CONTRIBUTION_SHOP_DATA::Load( std::string strFile )
{
	if( strFile.empty() )		return false;

	std::string strPath;
	strPath = RPATH::getDataGLogicPath();
	strPath += strFile;

	CIniLoader cFILE;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFILE.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	std::string strSep( ",[]\t" );
	cFILE.reg_sep( strSep );

	if( !cFILE.open( strPath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ErrorFile( "SNPC_CONTRIBUTION_SHOP_DATA::Load", strPath.c_str() );
		return false;
	}

	CleanUp();

	m_strName = strFile;

	cFILE.getflag( "SHOP_BASIC", "SHOP_TITLE", 0, 1, m_strTitle );


	DWORD dwShopType = cFILE.GetKeySize( "SHOP_TYPE_LIST", "SHOP_TYPE" );
	for ( DWORD i=0; i<dwShopType; ++i )
	{
		SNPC_CONTRIBUTION_SHOP_TYPE* pNpcShopType = new SNPC_CONTRIBUTION_SHOP_TYPE;
		cFILE.getflag( i, "SHOP_TYPE_LIST", "SHOP_TYPE", 0, 2, pNpcShopType->wID );
		cFILE.getflag( i, "SHOP_TYPE_LIST", "SHOP_TYPE", 1, 2, pNpcShopType->strName );

		SNPC_CONTRIBUTION_SHOP_TYPE_MAP_ITER it = m_mapType.find( pNpcShopType->wID );
		if ( it != m_mapType.end() )
		{
			SAFE_DELETE( pNpcShopType );
			continue;
		}

		m_mapType.insert( std::make_pair( pNpcShopType->wID, pNpcShopType ) );
	}

	DWORD dwShopItem = cFILE.GetKeySize( "SHOP_ITEM_LIST", "SHOP_ITEM" );
	for ( DWORD i=0; i<dwShopItem; ++i )
	{
		SNPC_CONTRIBUTION_SHOP_ITEM* pNpcShopItem = new SNPC_CONTRIBUTION_SHOP_ITEM;
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 0, 13, pNpcShopItem->wType );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 1, 13, pNpcShopItem->sItemID.wMainID );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 2, 13, pNpcShopItem->sItemID.wSubID );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 3, 13, pNpcShopItem->cDAMAGE );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 4, 13, pNpcShopItem->cDEFENSE );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 5, 13, pNpcShopItem->cRESIST_FIRE );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 6, 13, pNpcShopItem->cRESIST_ICE );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 7, 13, pNpcShopItem->cRESIST_ELEC );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 8, 13, pNpcShopItem->cRESIST_POISON );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 9, 13, pNpcShopItem->cRESIST_SPIRIT );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 10, 13, pNpcShopItem->bGenerateRandomValue );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 11, 13, pNpcShopItem->dwContriPrice );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 12, 13, pNpcShopItem->dwGoldPrice );

		SNPC_CONTRIBUTION_SHOP_ITEM_MAP_ITER it = m_mapItem.find( pNpcShopItem->sItemID.dwID );
		if ( it != m_mapItem.end() )
		{
			SAFE_DELETE( pNpcShopItem );
			continue;
		}

		SNPC_CONTRIBUTION_SHOP_TYPE* pType = FindType( pNpcShopItem->wType );
		if ( !pType )
		{
			SAFE_DELETE( pNpcShopItem );
			continue;
		}

		pType->wItemNum ++;

		m_mapItem.insert( std::make_pair( pNpcShopItem->sItemID.dwID, pNpcShopItem ) );
	}

	return true;
}

SNPC_CONTRIBUTION_SHOP_TYPE* SNPC_CONTRIBUTION_SHOP_DATA::FindType( WORD wType )
{
	SNPC_CONTRIBUTION_SHOP_TYPE_MAP_ITER it = m_mapType.find( wType );
	if( it != m_mapType.end() )	return it->second;

	return NULL;
}

SNPC_CONTRIBUTION_SHOP_ITEM* SNPC_CONTRIBUTION_SHOP_DATA::FindItem( DWORD dwItemID )
{
	SNPC_CONTRIBUTION_SHOP_ITEM_MAP_ITER it = m_mapItem.find( dwItemID );
	if( it != m_mapItem.end() )	return it->second;

	return NULL;
}