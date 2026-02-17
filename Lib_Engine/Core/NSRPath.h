#ifndef NSRPATH_H__INCLUDED
#define NSRPATH_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace RPATH
{
	extern TCHAR m_szAppPath[MAX_PATH];
	extern TCHAR m_szAppName[MAX_PATH];
	extern TCHAR m_szAppNameNoExt[MAX_PATH];

	extern TCHAR m_szSavePath[MAX_PATH];
	extern TCHAR m_szSaveProfilePath[MAX_PATH];
	extern TCHAR m_szSaveScreenShotPath[MAX_PATH];

	extern TCHAR m_szCachePath[MAX_PATH];
	extern TCHAR m_szLogsPath[MAX_PATH];

	extern TCHAR m_szServerConfigPath[MAX_PATH];

	extern TCHAR m_szDataPath[MAX_PATH];
	extern TCHAR m_szDataGLogicPath[MAX_PATH];
	extern TCHAR m_szDataGLogicServerPath[MAX_PATH];

	extern TCHAR m_szSoundsPath[MAX_PATH];
	extern TCHAR m_szTexturesPath[MAX_PATH];

	

	void SetPath( const TCHAR* szAppPath, BOOL bServer = FALSE );

	const TCHAR* getAppPath();
	const TCHAR* getAppName();
	const TCHAR* getAppNameNoExt();

	const TCHAR* getSavePath();
	const TCHAR* getSaveProfilePath();
	const TCHAR* getSaveScreenShotPath();

	const TCHAR* getCachePath();
	const TCHAR* getLogsPath();

	const TCHAR* getServerConfigPath();

	const TCHAR* getDataPath();
	const TCHAR* getDataGLogicPath();
	const TCHAR* getDataGLogicServerPath();

	const TCHAR* getSoundsPath();
	const TCHAR* getTexturesPath();

	

};

#endif // NSRPATH_H__INCLUDED
