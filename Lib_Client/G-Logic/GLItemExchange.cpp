#include "stdafx.h"
#include "./GLItemExchange.h"

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


SNPC_ITEM_EXCHANGE::SNPC_ITEM_EXCHANGE()
	: m_strName("")
	, m_strTitle("")
{

}

SNPC_ITEM_EXCHANGE::~SNPC_ITEM_EXCHANGE()
{
	CleanUp();
}

bool SNPC_ITEM_EXCHANGE::Load( std::string strFile )
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
		RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLITEM_EXCHANGE_LOAD ), strPath.c_str() );
		return false;
	}

	CleanUp();

	m_strName = strFile;


	cFILE.getflag( "NPC_EXCHANGE_BASIC", "NPC_EXCHANGE_TITLE", 0, 1, m_strTitle );

	DWORD dwitem = cFILE.GetKeySize( "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM" );
	for ( DWORD i=0; i<dwitem; ++i )
	{
		SNPC_ITEM_EXCHANGE_DATA* pData = new SNPC_ITEM_EXCHANGE_DATA;
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 0, 27, pData->wID );

		//result item
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 1, 27, pData->sidItemResult.sidItem.wMainID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 2, 27, pData->sidItemResult.sidItem.wSubID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 3, 27, pData->sidItemResult.wItemNum );

		//result item stats
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 4, 27, pData->cDAMAGE );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 5, 27, pData->cDEFENSE );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 6, 27, pData->cRESIST_FIRE );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 7, 27, pData->cRESIST_ICE );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 8, 27, pData->cRESIST_ELEC );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 9, 27, pData->cRESIST_POISON );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 10, 27, pData->cRESIST_SPIRIT );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 11, 27, pData->bGenerateRandomValue );

		//require item
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 12, 27, pData->sidItemRequire[0].sidItem.wMainID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 13, 27, pData->sidItemRequire[0].sidItem.wSubID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 14, 27, pData->sidItemRequire[0].wItemNum );

		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 15, 27, pData->sidItemRequire[1].sidItem.wMainID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 16, 27, pData->sidItemRequire[1].sidItem.wSubID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 17, 27, pData->sidItemRequire[1].wItemNum );

		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 18, 27, pData->sidItemRequire[2].sidItem.wMainID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 19, 27, pData->sidItemRequire[2].sidItem.wSubID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 20, 27, pData->sidItemRequire[2].wItemNum );

		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 21, 27, pData->sidItemRequire[3].sidItem.wMainID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 22, 27, pData->sidItemRequire[3].sidItem.wSubID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 23, 27, pData->sidItemRequire[3].wItemNum );

		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 24, 27, pData->sidItemRequire[4].sidItem.wMainID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 25, 27, pData->sidItemRequire[4].sidItem.wSubID );
		cFILE.getflag( i, "NPC_EXCHANGE_LIST", "NPC_EXCHANGE_ITEM", 26, 27, pData->sidItemRequire[4].wItemNum );


		if ( pData->sidItemResult.wItemNum < 1 )		pData->sidItemResult.wItemNum = 1;
		if ( pData->sidItemRequire[0].wItemNum < 1 )	pData->sidItemRequire[0].wItemNum = 1;
		if ( pData->sidItemRequire[1].wItemNum < 1 )	pData->sidItemRequire[1].wItemNum = 1;
		if ( pData->sidItemRequire[2].wItemNum < 1 )	pData->sidItemRequire[2].wItemNum = 1;
		if ( pData->sidItemRequire[3].wItemNum < 1 )	pData->sidItemRequire[3].wItemNum = 1;
		if ( pData->sidItemRequire[4].wItemNum < 1 )	pData->sidItemRequire[4].wItemNum = 1;

		SNPC_ITEM_EXCHANGE_DATA_POINTER_MAP_ITER it = m_mapData.find( pData->wID );
		if ( it != m_mapData.end() )
		{
			RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLITEM_EXCHANGE_LOAD_EXIST ), pData->wID );
			SAFE_DELETE( pData );
			continue;
		}

		m_mapData.insert( std::make_pair( pData->wID, pData ) );
	}

	return true;
}

SNPC_ITEM_EXCHANGE_DATA* SNPC_ITEM_EXCHANGE::FindData( WORD wExchangeID )
{
	SNPC_ITEM_EXCHANGE_DATA_POINTER_MAP_ITER it = m_mapData.find( wExchangeID );
	if ( it != m_mapData.end() )	return it->second;

	return NULL;
}

void SNPC_ITEM_EXCHANGE::CleanUp()
{
	std::for_each ( m_mapData.begin(), m_mapData.end(), std_afunc::DeleteMapObject() );
	m_mapData.clear();
}





