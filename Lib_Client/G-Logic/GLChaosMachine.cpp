#include "stdafx.h"
#include "./GLChaosMachine.h"

#include "../../Lib_Engine/Common/SUBPATH.h"
#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/Glogic.h"
#include "../../Lib_Engine/Core/NSRLog.h"
#include "./GLogicData.h"
#include "./GLItemMan.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GLChaosMachine& GLChaosMachine::GetInstance ()
{
	static GLChaosMachine Instance;
	return Instance;
}


GLChaosMachine::GLChaosMachine()
{

}

GLChaosMachine::~GLChaosMachine()
{
	ClearData();
}

void GLChaosMachine::ClearData()
{
	m_mapData.clear();
}

void GLChaosMachine::LoadData()
{
	ClearData();

	std::string strPath;
	strPath = GLOGIC::GetPath();
	strPath += "ChaosMachine.ini";

	CIniLoader file;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		file.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	std::string separator( ",[]\t" );
	file.reg_sep( separator );

	if( !file.open( strPath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ErrorFile( "GLChaosMachine::load_file", strPath.c_str() );
		return;
	}

	DWORD dwNum = file.GetKeySize( "CONFIG", "DATA" );
	for ( DWORD i=0; i<dwNum; ++i )
	{
		SCHAOS_MACHINE_DATA sData;

		file.getflag( i, "CONFIG", "DATA", 0, 14, sData.dwRequireGroupID );
		file.getflag( i, "CONFIG", "DATA", 1, 14, sData.dwResultGroupID );

		file.getflag( i, "CONFIG", "DATA", 2, 14, sData.llCost );
		file.getflag( i, "CONFIG", "DATA", 3, 14, sData.fChance );

		file.getflag( i, "CONFIG", "DATA", 4, 14, sData.sMaterial[0].sItemID.wMainID );
		file.getflag( i, "CONFIG", "DATA", 5, 14, sData.sMaterial[0].sItemID.wSubID );
		file.getflag( i, "CONFIG", "DATA", 6, 14, sData.sMaterial[0].wNum );

		file.getflag( i, "CONFIG", "DATA", 7, 14, sData.sMaterial[1].sItemID.wMainID );
		file.getflag( i, "CONFIG", "DATA", 8, 14, sData.sMaterial[1].sItemID.wSubID );
		file.getflag( i, "CONFIG", "DATA", 9, 14, sData.sMaterial[1].wNum );

		file.getflag( i, "CONFIG", "DATA", 10, 14, sData.sMaterial[2].sItemID.wMainID );
		file.getflag( i, "CONFIG", "DATA", 11, 14, sData.sMaterial[2].sItemID.wSubID );
		file.getflag( i, "CONFIG", "DATA", 12, 14, sData.sMaterial[2].wNum );

		file.getflag( i, "CONFIG", "DATA", 13, 14, sData.bConsumeMainItem );
		
		if ( sData.dwRequireGroupID == CHAOS_MACHINE_ID_NULL )	continue;;
		if ( sData.dwResultGroupID == CHAOS_MACHINE_ID_NULL )	continue;

		m_mapData.insert( std::make_pair( sData.dwRequireGroupID, sData ) );
	}
}

void GLChaosMachine::CopyItemResult()
{
	for ( WORD i=0; i<GLItemMan::MAX_MID; ++i )
	{
		for ( WORD j=0; j<GLItemMan::MAX_SID; ++j )
		{
			SITEM* pItem = GLItemMan::GetInstance().GetItem( i, j );
			if ( pItem )
			{
				if ( pItem->sBasicOp.GetChaosMachineID() == CHAOS_MACHINE_ID_NULL )	continue;

				SCHAOS_MACHINE_DATA_MAP_ITER it_s = m_mapData.begin();
				SCHAOS_MACHINE_DATA_MAP_ITER it_b = m_mapData.end();
				for ( ; it_s != it_b; ++it_s )
				{
					SCHAOS_MACHINE_DATA& sData = (*it_s).second;

					if ( sData.dwResultGroupID == pItem->sBasicOp.GetChaosMachineID() )
					{
						sData.vecResultItem.push_back( pItem->sBasicOp.sNativeID );
					}	
				}			
			}
		}
	}

	SCHAOS_MACHINE_DATA_MAP_ITER it_s = m_mapData.begin();
	SCHAOS_MACHINE_DATA_MAP_ITER it_b = m_mapData.end();
	for ( ; it_s != it_b; ++it_s )
	{
		SCHAOS_MACHINE_DATA& sData = (*it_s).second;

		sData.wResultIndex = 0;

		if ( sData.vecResultItem.size() )
		{
			std::random_shuffle( sData.vecResultItem.begin(), sData.vecResultItem.end() );
		}
	}
}

SCHAOS_MACHINE_DATA* GLChaosMachine::GetData( DWORD dwID )
{
	if ( dwID == CHAOS_MACHINE_ID_NULL )	return NULL;

	SCHAOS_MACHINE_DATA_MAP_ITER it = m_mapData.find( dwID );
	if ( it != m_mapData.end() )
		return &(*it).second;

	return NULL;
}