#pragma	once

#include "./GameTextLoader.h"

struct SBASIC_TEXT_DATA
{
	static const WORD progVersion = 0x0001;

	NS_TEXT_LOADER::STRFLAG		mapFlags;
};

const char * ID2BASICTEXT ( const char* szKeyword, int nIndex = 0 );

class CBasicTextLoader
{
public:
	CBasicTextLoader();
	virtual ~CBasicTextLoader();

public:
	const char* GetPath ()				{ return m_szPath; }
	void SetPath( const char* szPath )	{ StringCchCopy(m_szPath,MAX_PATH,szPath); }

public:
	const CString & GetLauncherText( std::string strKey, int Index = 0 );
	const CString &	GetText( std::string strKey, int Index);
	int	GetNumber( std::string strKey, int Index);

public:
	void Clear ();
	BOOL LoadText ( const char* szFileName,BOOL bXML = FALSE );	

private:
	char m_szPath[MAX_PATH];

private:
	SBASIC_TEXT_DATA	m_LauncherText;

public:
	static CBasicTextLoader& GetInstance();
};