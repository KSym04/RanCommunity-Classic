#include "stdafx.h"
#include "./GLCurrencyShop.h"

#include "./GLogicData.h"

#include "../../Lib_Engine/Core/NSRPath.h"
#include "../../Lib_Engine/Core/NSRInfo.h"
#include "../../Lib_Engine/Core/NSRLog.h"

#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLOGIC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


SCURRENCY_SHOP_DATA::SCURRENCY_SHOP_DATA()
	: m_strName("")
	, m_strTitle("")
{

}

SCURRENCY_SHOP_DATA::~SCURRENCY_SHOP_DATA()
{
	CleanUp();
}

bool SCURRENCY_SHOP_DATA::Load( std::string strFile )
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
		RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLCURRENCY_SHOP_LOAD ), strPath.c_str() );
		return false;
	}

	CleanUp();

	m_strName = strFile;

	cFILE.getflag( "BASIC", "TITLE", 0, 1, m_strTitle );

	DWORD dwitem = cFILE.GetKeySize( "ITEM_LIST", "ITEM" );
	for ( DWORD i=0; i<dwitem; ++i )
	{
		SCURRENCY_SHOP_ITEM* pShopItem = new SCURRENCY_SHOP_ITEM;

		cFILE.getflag( i, "ITEM_LIST", "ITEM", 0, 13, pShopItem->sItemID.wMainID );
		cFILE.getflag( i, "ITEM_LIST", "ITEM", 1, 13, pShopItem->sItemID.wSubID );

		cFILE.getflag( i, "ITEM_LIST", "ITEM", 2, 13, pShopItem->cDAMAGE );
		cFILE.getflag( i, "ITEM_LIST", "ITEM", 3, 13, pShopItem->cDEFENSE );
		cFILE.getflag( i, "ITEM_LIST", "ITEM", 4, 13, pShopItem->cRESIST_FIRE );
		cFILE.getflag( i, "ITEM_LIST", "ITEM", 5, 13, pShopItem->cRESIST_ICE );
		cFILE.getflag( i, "ITEM_LIST", "ITEM", 6, 13, pShopItem->cRESIST_ELEC );
		cFILE.getflag( i, "ITEM_LIST", "ITEM", 7, 13, pShopItem->cRESIST_POISON );
		cFILE.getflag( i, "ITEM_LIST", "ITEM", 8, 13, pShopItem->cRESIST_SPIRIT );
		cFILE.getflag( i, "ITEM_LIST", "ITEM", 9, 13, pShopItem->bGenerateRandomValue );

		cFILE.getflag( i, "ITEM_LIST", "ITEM", 10, 13, pShopItem->llPriceMoney );
		cFILE.getflag( i, "ITEM_LIST", "ITEM", 11, 13, pShopItem->dwPriceGamePoints );
		cFILE.getflag( i, "ITEM_LIST", "ITEM", 12, 13, pShopItem->dwPriceWarChips );

		SCURRENCY_SHOP_ITEM_POINTER_MAP_ITER it = m_mapData.find( pShopItem->sItemID.dwID );
		if ( it != m_mapData.end() )
		{
			RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLCURRENCY_SHOP_LOAD_EXIST ), pShopItem->sItemID.wMainID, pShopItem->sItemID.wSubID );
			SAFE_DELETE( pShopItem );
			continue;
		}

		m_mapData.insert( std::make_pair( pShopItem->sItemID.dwID, pShopItem ) );
	}

	return true;
}

SCURRENCY_SHOP_ITEM* SCURRENCY_SHOP_DATA::FindData( SNATIVEID sItemID )
{
	if ( sItemID == NATIVEID_NULL() )	return NULL;

	SCURRENCY_SHOP_ITEM_POINTER_MAP_ITER it = m_mapData.find( sItemID.dwID );
	if ( it != m_mapData.end() )	return it->second;

	return NULL;
}

void SCURRENCY_SHOP_DATA::DeleteData( SNATIVEID sItemID )
{
	SCURRENCY_SHOP_ITEM_POINTER_MAP_ITER it = m_mapData.find( sItemID.dwID );
	if ( it == m_mapData.end() )	return;

	delete (*it).second;
	m_mapData.erase ( it );
}

void SCURRENCY_SHOP_DATA::CleanUp()
{
	std::for_each ( m_mapData.begin(), m_mapData.end(), std_afunc::DeleteMapObject() );
	m_mapData.clear();
}

