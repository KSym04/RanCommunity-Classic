#include "stdafx.h"
#include "./GLNPCShop.h"

#include "./GLogicData.h"

#include "../../Lib_Engine/Core/NSRPath.h"
#include "../../Lib_Engine/Core/NSRInfo.h"
#include "../../Lib_Engine/Core/NSRLog.h"

#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLOGIC.h"
#include "../../Lib_Engine/Common/StlFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SNPC_SHOP_DATA::SNPC_SHOP_DATA()
	: m_strName("")
	, m_strTitle("")
	, m_wShopType(0)
{

}

SNPC_SHOP_DATA::~SNPC_SHOP_DATA()
{
	CleanUp();
}

bool SNPC_SHOP_DATA::Load( std::string strFile )
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
		RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLNPC_SHOP_LOAD ), strPath.c_str() );
		return false;
	}

	CleanUp();

	m_strName = strFile;

	cFILE.getflag( "SHOP_BASIC", "SHOP_TITLE", 0, 1, m_strTitle );
	cFILE.getflag( "SHOP_BASIC", "SHOP_OPTION", 0, 1, m_wShopType );

	DWORD dwShopType = cFILE.GetKeySize( "SHOP_TYPE_LIST", "SHOP_TYPE" );
	for ( DWORD i=0; i<dwShopType; ++i )
	{
		SNPC_SHOP_TYPE* pNpcShopType = new SNPC_SHOP_TYPE;
		cFILE.getflag( i, "SHOP_TYPE_LIST", "SHOP_TYPE", 0, 2, pNpcShopType->wID );
		cFILE.getflag( i, "SHOP_TYPE_LIST", "SHOP_TYPE", 1, 2, pNpcShopType->strName );

		SNPC_SHOP_TYPE_POINTER_MAP_ITER it = m_mapType.find( pNpcShopType->wID );
		if ( it != m_mapType.end() )
		{
			RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLNPC_SHOP_LOAD_EXIST_TYPE ), pNpcShopType->wID );
			SAFE_DELETE( pNpcShopType );
			continue;
		}

		m_mapType.insert( std::make_pair( pNpcShopType->wID, pNpcShopType ) );
	}

	DWORD dwShopItem = cFILE.GetKeySize( "SHOP_ITEM_LIST", "SHOP_ITEM" );
	for ( DWORD i=0; i<dwShopItem; ++i )
	{
		SNPC_SHOP_ITEM* pNpcShopItem = new SNPC_SHOP_ITEM;
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 0, 11, pNpcShopItem->wType );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 1, 11, pNpcShopItem->sItemID.wMainID );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 2, 11, pNpcShopItem->sItemID.wSubID );

		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 3, 11, pNpcShopItem->cDAMAGE );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 4, 11, pNpcShopItem->cDEFENSE );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 5, 11, pNpcShopItem->cRESIST_FIRE );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 6, 11, pNpcShopItem->cRESIST_ICE );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 7, 11, pNpcShopItem->cRESIST_ELEC );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 8, 11, pNpcShopItem->cRESIST_POISON );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 9, 11, pNpcShopItem->cRESIST_SPIRIT );
		cFILE.getflag( i, "SHOP_ITEM_LIST", "SHOP_ITEM", 10, 11, pNpcShopItem->bGenerateRandomValue );

		SNPC_SHOP_ITEM_POINTER_MAP_ITER it = m_mapItem.find( pNpcShopItem->sItemID.dwID );
		if ( it != m_mapItem.end() )
		{
			RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLNPC_SHOP_LOAD_EXIST_ITEM ), pNpcShopItem->sItemID.wMainID, pNpcShopItem->sItemID.wSubID );
			SAFE_DELETE( pNpcShopItem );
			continue;
		}

		SNPC_SHOP_TYPE* pType = FindShopType( pNpcShopItem->wType );
		if ( !pType )
		{
			RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLNPC_SHOP_LOAD_ITEM_NO_TYPE ), pNpcShopItem->sItemID.wMainID, pNpcShopItem->sItemID.wSubID );
			SAFE_DELETE( pNpcShopItem );
			continue;
		}

		pType->wItemNum ++;

		m_mapItem.insert( std::make_pair( pNpcShopItem->sItemID.dwID, pNpcShopItem ) );
	}

	return true;
}

SNPC_SHOP_TYPE* SNPC_SHOP_DATA::FindShopType( WORD wType )
{
	SNPC_SHOP_TYPE_POINTER_MAP_ITER it = m_mapType.find( wType );
	if( it != m_mapType.end() )	return it->second;

	return NULL;
}

SNPC_SHOP_ITEM* SNPC_SHOP_DATA::FindShopItem( DWORD dwItemID )
{
	SNPC_SHOP_ITEM_POINTER_MAP_ITER it = m_mapItem.find( dwItemID );
	if( it != m_mapItem.end() )	return it->second;

	return NULL;
}

void SNPC_SHOP_DATA::CleanUp()
{
	std::for_each ( m_mapType.begin(), m_mapType.end(), std_afunc::DeleteMapObject() );
	m_mapType.clear();

	std::for_each ( m_mapItem.begin(), m_mapItem.end(), std_afunc::DeleteMapObject() );
	m_mapItem.clear();
}
