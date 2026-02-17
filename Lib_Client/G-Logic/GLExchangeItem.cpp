#include "stdafx.h"

#include "../../Lib_Engine/G-Logic/GLogic.h"
#include "../../Lib_Engine/Common/gltexfile.h"

#include "./GLogicData.h"
#include "./GLExchangeItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace nsexchange_item
{
	Require* Data::findRequire(DWORD dwID)
	{
		MAP_REQUIRE_ITER it = mapRequire.find(dwID);
		if ( it != mapRequire.end() )
			return &(*it).second;
		else
			return NULL;
	}

	Result* Data::findResult(DWORD dwID)
	{
		MAP_RESULT_ITER it = mapResult.find(dwID);
		if ( it != mapResult.end() )
			return &(*it).second;
		else
			return NULL;
	}


	DataManager& DataManager::GetInstance ()
	{
		static DataManager Instance;
		return Instance;
	}

	DataManager::DataManager()
	{

	}

	DataManager::~DataManager()
	{

	}

	void DataManager::LoadData( std::string strName )
	{
		if( strName.empty() )
			return;

		std::string strPath;
		strPath = GLOGIC::GetPath();
		strPath += strName;

		gltexfile cFILE;
		cFILE.reg_sep( '\t' );
		cFILE.reg_sep( ' ' );
		cFILE.reg_sep( ',' );

		if( GLOGIC::bGLOGIC_ZIPFILE )
			cFILE.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE);

		if( !cFILE.open( strPath, true ) )
		{
			CDebugSet::ToLogFile( "ERROR : GLItemMan::LoadItemList(), %s", strName.c_str() );
			return;
		}

		Data sData;
		sData.strName = strName;

		//key retained from old ini

		DWORD dwRequireNum = cFILE.getflagnum ( "ALLOWED" );
		for ( DWORD i=0; i<dwRequireNum; ++i )
		{
			Require sRequire;
			cFILE.getflag ( i, "ALLOWED", 1, 2, sRequire.sItemID.wMainID );
			cFILE.getflag ( i, "ALLOWED", 2, 2, sRequire.sItemID.wSubID );

			sData.mapRequire.insert( std::make_pair( sRequire.sItemID.dwID, sRequire ) );
		}

		DWORD dwResultNum = cFILE.getflagnum ( "ITEM" );
		for ( DWORD i=0; i<dwResultNum; ++i )
		{
			Result sResult;
			cFILE.getflag ( i, "ITEM", 1, 3, sResult.sItemID.wMainID );
			cFILE.getflag ( i, "ITEM", 2, 3, sResult.sItemID.wSubID );
			cFILE.getflag ( i, "ITEM", 3, 3, sResult.wPointCost );

			sData.mapResult.insert( std::make_pair( sResult.sItemID.dwID, sResult ) );
		}

		m_mapData.insert( std::make_pair( sData.strName, sData ) );
	}


	Data* DataManager::findData( std::string strName )
	{
		if ( strName.empty() )
			return NULL;

		MAP_DATA_ITER it = m_mapData.find(strName);
		if ( it != m_mapData.end() )
			return &(*it).second;
		else
			return NULL;
	}
};