#include "StdAfx.h"
#include "./LauncherText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLauncherTextMan& CLauncherTextMan::GetInstance()
{
	static CLauncherTextMan Instance;
	return Instance;
}

CLauncherTextMan::CLauncherTextMan ()
{
	memset( m_szPath, 0, sizeof( m_szPath ) );
}

CLauncherTextMan::~CLauncherTextMan ()
{
    Clear ();
}

void CLauncherTextMan::Clear ()
{
	NS_TEXT_LOADER::ClearText ( m_LauncherText.mapFlags );
}

BOOL CLauncherTextMan::LoadText ( const char* szFileName,BOOL bXML )
{
	CString strPath(m_szPath);
	strPath += szFileName;

	if( !NS_TEXT_LOADER::LoadText( strPath.GetString(), SLAUNCHERTEXT::progVersion, m_LauncherText.mapFlags, bXML ) )
	{
		DefaultSetText();
	}

	return TRUE;
}

BOOL CLauncherTextMan::DefaultSetText()
{
	return TRUE;
}


const CString & CLauncherTextMan::GetLauncherText ( std::string strKey, int Index )
{
	return GetText ( strKey, Index);
}

const CString &	CLauncherTextMan::GetText ( std::string strKey, int Index)
{
	return NS_TEXT_LOADER::GetText ( strKey, Index, m_LauncherText.mapFlags );
}

int	CLauncherTextMan::GetNumber ( std::string strKey, int Index)
{
	return NS_TEXT_LOADER::GetNumber ( strKey, Index, m_LauncherText.mapFlags );
}

const char * ID2LAUNCHERTEXT( const char* szKeyword, int nIndex )
{
	return CLauncherTextMan::GetInstance().GetText( szKeyword, nIndex);
}