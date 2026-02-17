#include "stdafx.h"
#include "GLTransformSkinSet.h"

#include "./GLogicData.h"
#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"

#include "../../Lib_Helper/StringTrim.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLTransformSkinSet::GLTransformSkinSet()
{
};

GLTransformSkinSet::~GLTransformSkinSet()
{
	cleanup();
};

GLTransformSkinSet& GLTransformSkinSet::GetInstance ()
{
	static GLTransformSkinSet Instance;
	return Instance;
}

void GLTransformSkinSet::cleanup()
{
	m_map_skin_set.clear();
}

void GLTransformSkinSet::load_file( std::string file_name )
{
	cleanup();

	std::string strPath;
	strPath = GLOGIC::GetPath();
	strPath += file_name;

	CIniLoader file;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		file.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	std::string separator( ",[]\t" );
	file.reg_sep( separator );

	if( !file.open( strPath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ErrorFile( "GLTransformSkinSet::load_file", file_name.c_str() );
		return;
	}

	DWORD num = file.GetKeySize( "TRANSFORM_SKIN_SET", "SKIN" );
	for ( DWORD i=0; i<num; ++i )
	{
		DWORD index(0);
		std::string skin("");

		file.getflag( i, "TRANSFORM_SKIN_SET", "SKIN", 0, 2, index );
		file.getflag( i, "TRANSFORM_SKIN_SET", "SKIN", 1, 2, skin );

		_HLIB::trim( skin );

		m_map_skin_set.insert( std::make_pair( index, skin ) );
	}
};

std::string GLTransformSkinSet::get_skin( DWORD id )
{
	TRANSFORM_SKIN_SET_MAP_ITER it = m_map_skin_set.find( id );
	if ( it == m_map_skin_set.end() )	return "";

	return (*it).second;
}