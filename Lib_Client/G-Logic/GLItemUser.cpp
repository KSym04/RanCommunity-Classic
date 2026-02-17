#include "stdafx.h"
#include "./GLItemUser.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"
#include "./GLogicData.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "../../Lib_Engine/Common/IniLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GLItemUser& GLItemUser::GetInstance()
{
	static GLItemUser Instance;
	return Instance;
}

GLItemUser::GLItemUser () 
{
	CleanUp();
}

GLItemUser::~GLItemUser ()
{
}

void GLItemUser::LoadFile()
{
	CleanUp();
	LoadUser();
	LoadChar();
	LoadClub();
}

void GLItemUser::LoadUser()
{
	std::string filepath( GLOGIC::GetPath() );
	filepath += "itemuser.ini";

	CIniLoader ini;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		ini.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	if( !ini.open( filepath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ErrorFile( "GLItemUser::LoadUser", filepath.c_str() );
		return;
	}

	unsigned int cnt = ini.GetKeySize( "ITEM_USER", "ITEM" );
	for( unsigned int i=0; i<cnt; ++i )
	{
		SNATIVEID id;
		ini.getflag( i, "ITEM_USER", "ITEM", 0, 3, id.wMainID );
		ini.getflag( i, "ITEM_USER", "ITEM", 1, 3, id.wSubID );

		DWORD user;
		ini.getflag( i, "ITEM_USER", "ITEM", 2, 3, user );
		
		MAP_ITEM_USER_ITER it = m_mapItemUser.find( id.dwID );
		if ( it != m_mapItemUser.end() )
		{
			SET_ITEM_USER& set = (*it).second;
			set.insert( user );
		}
		else
		{
			SET_ITEM_USER set;
			set.insert( user );
			m_mapItemUser.insert( std::make_pair( id.dwID, set ) );
		}
	}
}


void GLItemUser::LoadChar()
{
	std::string filepath( GLOGIC::GetPath() );
	filepath += "itemchar.ini";

	CIniLoader ini;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		ini.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	if( !ini.open( filepath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ErrorFile( "GLItemUser::LoadChar", filepath.c_str() );
		return;
	}

	unsigned int cnt = ini.GetKeySize( "ITEM_CHAR", "ITEM" );
	for( unsigned int i=0; i<cnt; ++i )
	{
		SNATIVEID id;
		ini.getflag( i, "ITEM_CHAR", "ITEM", 0, 3, id.wMainID );
		ini.getflag( i, "ITEM_CHAR", "ITEM", 1, 3, id.wSubID );

		DWORD user;
		ini.getflag( i, "ITEM_CHAR", "ITEM", 2, 3, user );


		MAP_ITEM_USER_ITER it = m_mapItemChar.find( id.dwID );
		if ( it != m_mapItemChar.end() )
		{
			SET_ITEM_USER& set = (*it).second;
			set.insert( user );
		}
		else
		{
			SET_ITEM_USER set;
			set.insert( user );
			m_mapItemChar.insert( std::make_pair( id.dwID, set ) );
		}
	}
}


void GLItemUser::LoadClub()
{
	std::string filepath( GLOGIC::GetPath() );
	filepath += "itemclub.ini";

	CIniLoader ini;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		ini.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	if( !ini.open( filepath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ErrorFile( "GLItemUser::LoadClub", filepath.c_str() );
		return;
	}

	unsigned int cnt = ini.GetKeySize( "ITEM_CLUB", "ITEM" );
	for( unsigned int i=0; i<cnt; ++i )
	{
		SNATIVEID id;
		ini.getflag( i, "ITEM_CLUB", "ITEM", 0, 3, id.wMainID );
		ini.getflag( i, "ITEM_CLUB", "ITEM", 1, 3, id.wSubID );

		DWORD user;
		ini.getflag( i, "ITEM_CLUB", "ITEM", 2, 3, user );


		MAP_ITEM_USER_ITER it = m_mapItemClub.find( id.dwID );
		if ( it != m_mapItemClub.end() )
		{
			SET_ITEM_USER& set = (*it).second;
			set.insert( user );
		}
		else
		{
			SET_ITEM_USER set;
			set.insert( user );
			m_mapItemClub.insert( std::make_pair( id.dwID, set ) );
		}
	}
}

BOOL GLItemUser::CharTest( DWORD item_id, DWORD user_id, DWORD char_id, DWORD club_id )
{
	if ( item_id >= NATIVEID_NULL().dwID )	return TRUE;
	
	if ( !CharTestUserID( item_id, user_id ) )	return FALSE;
	if ( !CharTestCharID( item_id, char_id ) )	return FALSE;
	if ( !CharTestClubID( item_id, club_id ) )	return FALSE;

	return TRUE;
}

BOOL GLItemUser::CharTestUserID( DWORD item_id, DWORD user_id )
{
	if ( user_id >= UINT_MAX )			return TRUE;

	MAP_ITEM_USER_ITER it = m_mapItemUser.find( item_id );
	if ( it == m_mapItemUser.end() )	return TRUE;

	SET_ITEM_USER& set = (*it).second;
	SET_ITEM_USER_ITER set_it = set.find( user_id );
	if ( set_it != set.end() )			return TRUE;

	return FALSE;
}

BOOL GLItemUser::CharTestCharID( DWORD item_id, DWORD char_id )
{
	if ( char_id >= UINT_MAX )			return TRUE;

	MAP_ITEM_USER_ITER it = m_mapItemChar.find( item_id );
	if ( it == m_mapItemChar.end() )	return TRUE;

	SET_ITEM_USER& set = (*it).second;
	SET_ITEM_USER_ITER set_it = set.find( char_id );
	if ( set_it != set.end() )			return TRUE;

	return FALSE;
}

BOOL GLItemUser::CharTestClubID( DWORD item_id, DWORD club_id )
{
	if ( club_id >= UINT_MAX )			return TRUE;

	MAP_ITEM_USER_ITER it = m_mapItemClub.find( item_id );
	if ( it == m_mapItemClub.end() )	return TRUE;

	SET_ITEM_USER& set = (*it).second;
	SET_ITEM_USER_ITER set_it = set.find( club_id );
	if ( set_it != set.end() )			return TRUE;

	return FALSE;
}


void GLItemUser::CleanUp()
{
	m_mapItemUser.clear();
	m_mapItemChar.clear();
	m_mapItemClub.clear();
}