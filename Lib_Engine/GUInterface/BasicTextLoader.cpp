#include "StdAfx.h"
#include "./BasicTextLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicTextLoader& CBasicTextLoader::GetInstance()
{
	static CBasicTextLoader Instance;
	return Instance;
}

CBasicTextLoader::CBasicTextLoader()
{
	memset( m_szPath, 0, sizeof( m_szPath ) );
}

CBasicTextLoader::~CBasicTextLoader()
{
    Clear ();
}

void CBasicTextLoader::Clear ()
{
	NS_TEXT_LOADER::ClearText ( m_LauncherText.mapFlags );
}

BOOL CBasicTextLoader::LoadText ( const char* szFileName,BOOL bXML )
{
	CString strPath(m_szPath);
	strPath += szFileName;

	return NS_TEXT_LOADER::LoadText(strPath.GetString(), SBASIC_TEXT_DATA::progVersion, m_LauncherText.mapFlags, bXML);
}

const CString & CBasicTextLoader::GetLauncherText ( std::string strKey, int Index )
{
	return GetText ( strKey, Index);
}

const CString &	CBasicTextLoader::GetText ( std::string strKey, int Index)
{
	return NS_TEXT_LOADER::GetText ( strKey, Index, m_LauncherText.mapFlags );
}

int	CBasicTextLoader::GetNumber ( std::string strKey, int Index)
{
	return NS_TEXT_LOADER::GetNumber ( strKey, Index, m_LauncherText.mapFlags );
}

const char * ID2BASICTEXT(const char* szKeyword, int nIndex)
{
	return CBasicTextLoader::GetInstance().GetText(szKeyword, nIndex);
}