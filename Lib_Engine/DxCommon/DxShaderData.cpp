#include "stdafx.h"
#include "./DxShaderData.h"

#include "../Common/StlFunctions.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DxShaderData& DxShaderData::GetInstance()
{
	static DxShaderData Instance;
	return Instance;
}

DxShaderData::DxShaderData()
	: m_strPATH("")
{
}

DxShaderData::~DxShaderData()
{
}

BOOL DxShaderData::LoadFile( std::string strfile )
{
	std::string strfilepath = m_strPATH;
	strfilepath += strfile;

	CSerialFile sfile;
	if ( !sfile.OpenFile ( FOT_READ, strfilepath.c_str() ) ) return FALSE;	

	CleanUp();

	DWORD version, datasize;
	sfile >> version;
	sfile >> datasize;

	if ( version >= VERSION_ENCODE )
		sfile.SetEncodeType( EMRCRYPT_BYTE_TYPE_MAP_SHADER_VER_1 );

	if ( version == SHADER_DATA_VERSION )
	{
		DWORD num;
		sfile >> num;

		for ( DWORD i=0; i<num; ++i )
		{
			std::string key;
			std::string body;

			sfile >> key;
			sfile >> body;

			m_mapShader.insert( std::make_pair( key, body ) );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxShaderData::LoadFile", sfile.GetFileName(), version );
		DWORD dwCur = sfile.GetfTell();
		sfile.SetOffSet( dwCur+datasize );

		return FALSE;
	}

	return TRUE;
}

BOOL DxShaderData::SaveFile ( std::string strfile )
{
	std::string strfilepath = m_strPATH;
	strfilepath += strfile;

	CSerialFile sfile;
	if ( !sfile.OpenFile ( FOT_WRITE, strfilepath.c_str() ) )	return FALSE;
	
	sfile << (DWORD)SHADER_DATA_VERSION;
	sfile.BeginBlock();

	sfile.SetEncodeType( EMRCRYPT_BYTE_TYPE_MAP_SHADER_VER_1 );

	sfile << (DWORD)m_mapShader.size();

	for( SHADER_DATA_MAP_ITER it = m_mapShader.begin(); it != m_mapShader.end(); ++it )
	{
		sfile << (*it).first;
		sfile << (*it).second;
	}

	sfile.SetEncodeType( EMRCRYPT_BYTE_TYPE_NONE );

	sfile.EndBlock();

	return TRUE;
}

void DxShaderData::CleanUp()
{
	m_mapShader.clear();
}