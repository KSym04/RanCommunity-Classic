#include "StdAfx.h"
#include "GLBattlePass.h"

#include "./GLogicData.h"

#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-9-14, Battle Pass - CNDev*/

const char* GLBattlePass::_FILEHEAD = "GLBATTLEPASS";

GLBattlePass& GLBattlePass::GetInstance ()
{
	static GLBattlePass _instance;
	return _instance;
}

GLBattlePass::GLBattlePass()
{
	memset(m_szFileName, 0, sizeof(char) * (MAX_PATH));
}

GLBattlePass::~GLBattlePass()
{
}

void GLBattlePass::CleanUp()
{
	m_mapBattlePass.clear();
}

HRESULT GLBattlePass::LoadFile ( const char* szFile, BOOL bServer )
{
	StringCchCopy(m_szFileName,MAX_PATH,szFile);

	char szFullPath[MAX_PATH] = {0};
	StringCchCopy ( szFullPath, MAX_PATH, GLOGIC::GetPath() );
	StringCchCat ( szFullPath, MAX_PATH, m_szFileName );

	std::auto_ptr<basestream> pBStream( GLOGIC::openfile_basestream(GLOGIC::bGLOGIC_ZIPFILE, 
		GLOGIC::strGLOGIC_ZIPFILE.c_str(),
		szFullPath, 
		szFile,
		EMRCRYPT_BYTE_TYPE_NONE,
		GLOGIC::bGLOGIC_PACKFILE ) );

	if ( !pBStream.get() )
	{
		if ( strlen( szFile ) > 0 )
			CDebugSet::ErrorFile( "GLBattlePass::LoadFile", szFile );

		return E_FAIL;
	}

	basestream &SFile = *pBStream;

	DWORD dwFILEVER;
	char szFILETYPE[_MAX_FNAME];
	SFile.GetFileType( szFILETYPE, _MAX_FNAME, dwFILEVER );

	if ( dwFILEVER>=VERSION_ENCODE_2024 )
		SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_BATTLEPASS );

	CleanUp();

	DWORD dwnum(0);
	SFile >> dwnum;

	for ( DWORD i=0; i<dwnum; i++ )
	{
		SBATTLEPASS_FILE_DATA sdata;
		BOOL bLoad = sdata.LOAD( SFile, bServer );
		if ( !bLoad )	continue;

		SBATTLEPASS_FILE_DATA* pdata = GetBattlePass( sdata.dwBattlePassID );
		if ( pdata )	continue;

		m_mapBattlePass.insert( std::make_pair( sdata.dwBattlePassID, sdata ) );
	}

	return S_OK;
}

HRESULT GLBattlePass::SaveFile ( const char* szFile )
{
	StringCchCopy(m_szFileName,MAX_PATH,szFile);

	char szFullPath[MAX_PATH];
	StringCchCopy ( szFullPath, MAX_PATH, GetPath().c_str() );
	StringCchCat ( szFullPath, MAX_PATH, m_szFileName );

	CSerialFile SFile;
	SFile.SetFileType ( _FILEHEAD, VERSION_ENCODE_2024 );
	if ( !SFile.OpenFile ( FOT_WRITE, szFullPath ) )	return E_FAIL;
	SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_BATTLEPASS );

	SFile << (DWORD)m_mapBattlePass.size();

	SBATTLEPASS_FILE_DATA_MAP_ITER iterpos = m_mapBattlePass.begin();
	for( ; iterpos != m_mapBattlePass.end(); ++iterpos )
	{
		SBATTLEPASS_FILE_DATA &sdata = (*iterpos).second;
		sdata.SAVE( SFile );
	}

	return S_OK;
}

SBATTLEPASS_FILE_DATA* GLBattlePass::GetBattlePass( DWORD dwID )
{
	if ( dwID >= UINT_MAX )	return NULL;

	SBATTLEPASS_FILE_DATA_MAP_ITER iter = m_mapBattlePass.find( dwID );
	if ( iter == m_mapBattlePass.end() )	return NULL;

	return &(*iter).second;
}
