#include "stdafx.h"

#include "../../Lib_Engine/G-Logic/GLogic.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/IniLoader.h"

#include "./GLSkillPurchase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLSkillPurchase::GLSkillPurchase()
{

}

GLSkillPurchase::~GLSkillPurchase()
{
	
}

GLSkillPurchase& GLSkillPurchase::GetInstance ()
{
	static GLSkillPurchase Instance;
	return Instance;
}

void GLSkillPurchase::CleanUp()
{
	m_mapSkillPurchase.clear();
}

void GLSkillPurchase::LoadFile( std::string strFileName )
{
	if ( strFileName.empty() )
		return;

	std::string strPath;
	strPath = GLOGIC::GetPath();
	strPath += strFileName;

	CIniLoader cFILE;
	std::string strSep( ",[]\t" );
	cFILE.reg_sep( strSep );

	if( !cFILE.open( strPath, true ) )
	{
		CDebugSet::ToLogFile ( "ERROR : GLSkillPurchase::LoadFile(), File Open %s", strPath.c_str() );
		return;
	}

	CleanUp();


	DWORD dwNUM = cFILE.GetKeySize( "SKILL_LIST", "SKILL" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		SSKILL_PURCHASE sData;
		cFILE.getflag( i, "SKILL_LIST", "SKILL", 0, 3, sData.sSkillID.wMainID );
		cFILE.getflag( i, "SKILL_LIST", "SKILL", 1, 3, sData.sSkillID.wSubID );
		cFILE.getflag( i, "SKILL_LIST", "SKILL", 2, 3, sData.dwCost );

		m_mapSkillPurchase.insert( std::make_pair( sData.sSkillID.dwID, sData ) );
	}
}

SSKILL_PURCHASE* GLSkillPurchase::getSkillPurchase( SNATIVEID sSkillID )
{
	SKILL_PURCHASE_MAP_ITER it = m_mapSkillPurchase.find( sSkillID.dwID );
	if ( it == m_mapSkillPurchase.end() )
		return NULL;

	return &(*it).second;
}