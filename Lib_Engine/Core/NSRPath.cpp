#include "stdafx.h"
#include "./NSRPath.h"

#include "shlobj.h"
#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RPATH
{
	TCHAR m_szAppPath[MAX_PATH]					= "Path";
	TCHAR m_szAppName[MAX_PATH]					= "Path";
	TCHAR m_szAppNameNoExt[MAX_PATH]			= "Path";

	TCHAR m_szSavePath[MAX_PATH]				= "Path";
	TCHAR m_szSaveProfilePath[MAX_PATH]			= "Path";
	TCHAR m_szSaveScreenShotPath[MAX_PATH]		= "Path";

	TCHAR m_szCachePath[MAX_PATH]				= "Path";
	TCHAR m_szLogsPath[MAX_PATH]				= "Path";

	TCHAR m_szServerConfigPath[MAX_PATH]		= "Path";

	TCHAR m_szDataPath[MAX_PATH]				= "Path";
	TCHAR m_szDataGLogicPath[MAX_PATH]			= "Path";
	TCHAR m_szDataGLogicServerPath[MAX_PATH]	= "Path";

	TCHAR m_szSoundsPath[MAX_PATH]				= "Path";
	TCHAR m_szTexturesPath[MAX_PATH]			= "Path";

	
	
	void SetPath( const TCHAR* szAppPath, BOOL bServer /*= FALSE*/ )
	{
		if ( !szAppPath )	return;

		boost::filesystem::path fsAppPath = szAppPath;

		StringCchCopy( m_szAppPath, MAX_PATH, fsAppPath.parent_path().string().c_str() );
		StringCchCopy( m_szAppName, MAX_PATH, fsAppPath.filename().string().c_str() );
		StringCchCopy( m_szAppNameNoExt, MAX_PATH, fsAppPath.stem().string().c_str() );

		//CDebugSet::MsgBox( "%s %s %s", m_szAppPath, m_szAppName, m_szAppNameNoExt );

		StringCchCopy( m_szSavePath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szSavePath, MAX_PATH, _T("\\Ran Online\\") );
		CreateDirectory ( m_szSavePath, NULL );

		StringCchCopy( m_szSaveProfilePath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szSaveProfilePath, MAX_PATH, _T("\\Ran Online\\Profile\\") );
		CreateDirectory ( m_szSaveProfilePath, NULL );

		StringCchCopy( m_szSaveScreenShotPath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szSaveScreenShotPath, MAX_PATH, _T("\\Ran Online\\ScreenShot\\") );
		CreateDirectory ( m_szSaveScreenShotPath, NULL );

		

		StringCchCopy( m_szCachePath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szCachePath, MAX_PATH, _T("\\cache\\") );

		StringCchCopy( m_szLogsPath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szLogsPath, MAX_PATH, _T("\\Logs\\") );
		CreateDirectory ( m_szLogsPath, NULL );

		StringCchCopy( m_szServerConfigPath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szServerConfigPath, MAX_PATH, _T("\\cfg\\") );

		if ( bServer )
			CreateDirectory ( m_szServerConfigPath, NULL );
		
		StringCchCopy( m_szDataPath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szDataPath, MAX_PATH, _T("\\Data\\") );

		StringCchCopy( m_szDataGLogicPath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szDataGLogicPath, MAX_PATH, _T("\\Data\\GLogic\\") );

		StringCchCopy( m_szDataGLogicServerPath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szDataGLogicServerPath, MAX_PATH, _T("\\Data\\GLogicServer\\") );

		StringCchCopy( m_szSoundsPath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szSoundsPath, MAX_PATH, _T("\\Sounds\\") );

		StringCchCopy( m_szTexturesPath, MAX_PATH, m_szAppPath );
		StringCchCat( m_szTexturesPath, MAX_PATH, _T("\\Textures\\") );

		
	};


	const TCHAR* getAppPath()					{ return m_szAppPath; }
	const TCHAR* getAppName()					{ return m_szAppName; }
	const TCHAR* getAppNameNoExt()				{ return m_szAppNameNoExt; }

	const TCHAR* getSavePath()					{ return m_szSavePath; }
	const TCHAR* getSaveProfilePath()			{ return m_szSaveProfilePath; }
	const TCHAR* getSaveScreenShotPath()		{ return m_szSaveScreenShotPath; }

	const TCHAR* getCachePath()					{ return m_szCachePath; }
	const TCHAR* getLogsPath()					{ return m_szLogsPath; }

	const TCHAR* getServerConfigPath()			{ return m_szServerConfigPath; }

	const TCHAR* getDataPath()					{ return m_szDataPath; }
	const TCHAR* getDataGLogicPath()			{ return m_szDataGLogicPath; }
	const TCHAR* getDataGLogicServerPath()		{ return m_szDataGLogicServerPath; }

	const TCHAR* getSoundsPath()				{ return m_szSoundsPath; }
	const TCHAR* getTexturesPath()				{ return m_szTexturesPath; }

};