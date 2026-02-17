#include "stdafx.h"
#include "./MapDropFilter.h"

#include "./GLogicData.h"

#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"
#include "../../Lib_Engine/Core/NSRPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMapDropFilter& CMapDropFilter::GetInstance ()
{
	static CMapDropFilter Instance;
	return Instance;
}

CMapDropFilter::CMapDropFilter()
{

}

CMapDropFilter::~CMapDropFilter()
{
	
}

void CMapDropFilter::CleanUp()
{
	m_mapData.clear();
}

void CMapDropFilter::Load(const std::string& strFileName)
{
	std::string strPath;
	strPath = RPATH::getDataGLogicPath();
	strPath += strFileName;

	CIniLoader cFile;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFile.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	std::string strSep( ",[]\t" );
	cFile.reg_sep( strSep );

	CleanUp();

	if( !cFile.open( strPath, true, GLOGIC::bGLOGIC_PACKFILE ) )
		return;

	DWORD dwNum = cFile.GetKeySize( "LIST", "DATA" );
	for ( DWORD i=0; i<dwNum; ++i )
	{
		
		SNATIVEID mapID;
		SNATIVEID itemID;
		cFile.getflag( i, "LIST", "DATA", 0, 4, mapID.wMainID );
		cFile.getflag( i, "LIST", "DATA", 1, 4, mapID.wSubID);
		cFile.getflag( i, "LIST", "DATA", 2, 4, itemID.wMainID );
		cFile.getflag( i, "LIST", "DATA", 3, 4, itemID.wSubID );

		MDF_MAP_MAP_ITER it = m_mapData.find(mapID.dwID);
		if ( it != m_mapData.end() )
		{
			//map data exist
			//just insert the item 
			SMDF_MAP& smdf_map = (*it).second;

			SMDF_ITEM smdf_item;
			smdf_item.sItemID = itemID;

			smdf_map.mapItem.insert(std::make_pair(smdf_item.sItemID.dwID,smdf_item));
		}
		else
		{
			//create new entry

			SMDF_MAP smdf_map;
			smdf_map.sMapID = mapID;

			SMDF_ITEM smdf_item;
			smdf_item.sItemID = itemID;

			smdf_map.mapItem.insert(std::make_pair(smdf_item.sItemID.dwID,smdf_item));

			m_mapData.insert(std::make_pair(smdf_map.sMapID.dwID,smdf_map));
		}
	}
}

//returns true if itemID is found
bool CMapDropFilter::Find(const SNATIVEID& mapID, const SNATIVEID& itemID)
{
	MDF_MAP_MAP_CITER cit = m_mapData.find(mapID.dwID);
	if ( cit != m_mapData.end() )
	{
		const SMDF_MAP& smdf_map = (*cit).second;

		MDF_ITEM_MAP_CITER cit2 = smdf_map.mapItem.find(itemID.dwID);
		if ( cit2 != smdf_map.mapItem.end() )
			return true;
	}

	return false;
}