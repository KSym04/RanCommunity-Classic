#include "stdafx.h"
#include "./GLPartyFinder.h"


#include "./GLogicData.h"

#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"

#include "../../Lib_Engine/Core/NSRLog.h"
#include "../../Lib_Engine/Core/NSRPath.h"
#include "../../Lib_Engine/Core/NSRInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLPartyFinder& GLPartyFinder::GetInstance ()
{
	static GLPartyFinder Instance;
	return Instance;
}


GLPartyFinder::GLPartyFinder()
{
	CleanData();
}

GLPartyFinder::~GLPartyFinder()
{

}

void GLPartyFinder::CleanData()
{
	m_mapPartyFinderMap.clear();
}

void GLPartyFinder::LoadData()
{
	std::string strPath;
	strPath = RPATH::getDataGLogicPath();
	strPath += RINFO::getc( EMRINFO_PARTY_FINDER_FILE_NAME );

	CIniLoader cFile;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFile.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	std::string strSep( ",[]\t" );
	cFile.reg_sep( strSep );

	CleanData();

	if( !cFile.open( strPath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		RLOG::Log( RINFO::getc( EMRINFO_PARTY_FINDER_ERROR_FILE ), strPath.c_str() );
		return;
	}

	DWORD dwNum = cFile.GetKeySize( "PARTY_FINDER", "MAP" );
	for ( DWORD i=0; i<dwNum; ++i )
	{
		SPARTY_FINDER_MAP_DATA sData;

		cFile.getflag( i, "PARTY_FINDER", "MAP", 0, 4, sData.dwIndex );
		cFile.getflag( i, "PARTY_FINDER", "MAP", 1, 4, sData.sMapID.wMainID );
		cFile.getflag( i, "PARTY_FINDER", "MAP", 2, 4, sData.sMapID.wSubID );
		cFile.getflag( i, "PARTY_FINDER", "MAP", 3, 4, sData.strMapName );

		m_mapPartyFinderMap.insert( std::make_pair( sData.dwIndex, sData ) );
	}
}

SPARTY_FINDER_MAP_DATA* GLPartyFinder::GetData( DWORD dwID )
{
	if ( dwID == SPARTY_FINDER_SEARCH_INDEX_NULL )		return NULL;

	SPARTY_FINDER_MAP_DATA_MAP_ITER it = m_mapPartyFinderMap.find( dwID );
	if( it == m_mapPartyFinderMap.end() )	return NULL;

	return &(*it).second;
}