// StringFile.cpp: implementation of the CStringFile class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "./CompByte.h"
#include "./StringFileGS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStringFileGS::CStringFileGS ( UINT nBufSize ) :
	m_dwMasterIndex(0),
	m_fFile(NULL)
{
	m_dwRead = nBufSize;
	m_nBufferSize = nBufSize;
	m_pBuffer = new BYTE[nBufSize];
}

CStringFileGS::~CStringFileGS ()
{
	delete[] m_pBuffer;
	Close ();
}

//BOOL CStringFileGS::OpenAddPastVer ( LPCSTR szFile, BOOL bDecode )
//{
//	GASSERT(m_fFile==NULL&&"CStringFile::Open ()");
//
//	m_fFile = fopen ( szFile, "rb" );
//	if ( m_fFile==NULL )	return FALSE;
//
//	fseek ( m_fFile, 0, SEEK_END );
//	m_nMaxSize = ftell ( m_fFile );
//
//	fseek ( m_fFile, 0, SEEK_SET );
//
//	m_bDecode = bDecode;
//	StringCchCopy( m_szFileName, MAX_PATH, szFile );
//
//	if( m_bDecode )
//	{
//		int nVersion;
//		fread ( &nVersion, sizeof(int), 1, m_fFile );
//		if( nVersion > 0 && CRijndael::VERSION >= nVersion )
//		{
//			m_nVersion = nVersion;
//			m_oRijndael.Initialize( CRijndael::sm_Version[nVersion-1], CRijndael::DEFAULT_KEY_LENGTH );
//		}
//		else
//		{
//			fseek ( m_fFile, 0, SEEK_SET );
//		}
//	}
//	return TRUE;
//}

BOOL CStringFileGS::Open ( LPCSTR szFile, BOOL bDecode )
{
	GASSERT(m_fFile==NULL&&"CStringFileGS::Open ()");

	m_fFile = fopen ( szFile, "rb" );
	if ( m_fFile==NULL )	return FALSE;

	fseek ( m_fFile, 0, SEEK_END );
	m_nMaxSize = ftell ( m_fFile );

	fseek ( m_fFile, 0, SEEK_SET );

	m_bDecode = bDecode;
	StringCchCopy( m_szFileName, MAX_PATH, szFile );

	char fname[_MAX_FNAME]={0,};
	char ext[_MAX_EXT]={0,};
	_splitpath( szFile, NULL, NULL, fname, ext );

	bool bPastLoad = FALSE;
	if( stricmp( fname, "param" ) == 0 )
	{
		bPastLoad = TRUE;
	}

	if( m_bDecode )
	{
		int nVersion;
		fread ( &nVersion, sizeof(int), 1, m_fFile );
		if( nVersion > 0 && CRijndaelGS::VERSION >= nVersion )
		{
			m_nVersion = nVersion;
			m_nVersionDate = nVersion >= CRijndaelGS::VERSION ? CRijndaelGS::VERSION_DATE : 0;
			m_oRijndael.Initialize( 
				CRijndaelGS::sm_Version[m_nVersion-1].c_str(), 
				CRijndaelGS::sm_KeyLength[m_nVersion-1], 
				m_nVersionDate, 
				m_nVersion,
				CRijndaelGS::sm_chain0, 
				CRijndaelGS::sm_KeyLength[m_nVersion-1] );
		}
		else
		{
			fseek ( m_fFile, 0, SEEK_SET );
			return FALSE;
		}
	}

	return TRUE;
}

void CStringFileGS::Close ()
{
	if( m_fFile )
	{
		fclose ( m_fFile );
		m_fFile = NULL;
	} // if( m_fFile )

	m_dwIndex = 0;
	m_dwLine = 0;

	m_dwMasterIndex = 0;
	m_dwIndex = 0;
	m_dwLine = 0;
}

DWORD CStringFileGS::GetNextLine ( CString &strString )
{
	if ( m_fFile==NULL )	return 0;

	char *szString = strString.GetBuffer ( m_nBufferSize );
	DWORD dwReturn = this->GetNextLine ( szString, m_nBufferSize );
	if ( dwReturn == 0 )	strString.Empty();

	strString.ReleaseBuffer ();

	return dwReturn;
}

DWORD CStringFileGS::GetNextLine ( PSTR &szString )
{
	if ( m_fFile==NULL )	return 0;

	SAFE_DELETE(szString);

	szString = new CHAR[m_nBufferSize];
	DWORD dwReturn = this->GetNextLine ( szString, m_nBufferSize );

	SAFE_DELETE(szString);
	
	return dwReturn;
}

DWORD CStringFileGS::GetNextLine ( LPSTR szLine, UINT iLineSize )
{
	if ( m_fFile==NULL )	return 0;

	BYTE	*chTemp;
	BOOL	bStop=FALSE;
	int		nOut;

	chTemp = (LPBYTE) szLine;
	*chTemp = 0;
	nOut = 0;

	while ( !bStop )
	{
		if ( !m_dwLine || m_dwIndex==m_dwRead )
		{
			m_dwMasterIndex = ftell ( m_fFile );
			
			if( m_bDecode )
			{
				if( m_nVersion != -1 )
				{
					int nLen = m_oRijndael.GetEncryptLength( m_nBufferSize );
					m_dwRead = (DWORD)fread ( m_pBuffer, sizeof(BYTE), nLen, m_fFile );

					if( m_dwRead > 0 )
						m_oRijndael.DecryptEx( (char*)m_pBuffer, nLen );
				}
				else
				{
					m_dwRead = (DWORD)fread ( m_pBuffer, sizeof(BYTE), m_nBufferSize, m_fFile );

					if( m_dwRead > 0 )
						compbyte::decode_old ( m_pBuffer, sizeof(BYTE)*m_dwRead );
				}
			}
			else
			{
				m_dwRead = (DWORD)fread ( m_pBuffer, sizeof(BYTE), m_nBufferSize, m_fFile );
			}

			m_dwIndex = 0;
			if ( m_dwRead == 0 )
			{
				bStop = TRUE; // Error during readfile or END-OF-FILE encountered

				if ( nOut>0 )
				{
					chTemp[nOut++] = (char) 0;
					return m_dwLine;	
				}
				else return m_dwLine = 0; //nix gelezen
			}
			else
			{
				if ( m_dwRead != (DWORD)m_nBufferSize )
					bStop = TRUE;	//END-OF-FILE
			}
		}

		for ( ; m_dwIndex<m_dwRead; m_dwIndex++ )
		{
			if ( (nOut+1) == iLineSize )
			{
				TCHAR szError[260] = {0};
				StringCchPrintf( szError, 260, "CStringFileGS::m_pBuffer Not enough. File:%s, Line:%u(Over Char Number:%d)", m_szFileName, ++m_dwLine, iLineSize );
				::MessageBox ( NULL, szError, "Warning !", MB_ICONEXCLAMATION|MB_OK );
				chTemp[nOut] = '\0';
				return m_dwLine;
			}

			switch ( m_pBuffer[m_dwIndex] )
			{
			case 0x0d://End of Line encountered
			case 0x0a:
				if ( (m_dwIndex+1) < m_dwRead ) // Check we're not on end of m_pBuffer ???
				{
					if ( m_pBuffer[m_dwIndex+1] == '\n' || m_pBuffer[m_dwIndex+1] == '\r' )
					{
						if ( !*chTemp )	m_dwLine++;
						m_dwIndex++;
					}
				}

				if ( *chTemp )
				{
					chTemp[nOut++] = '\0';
					m_dwLine++;

					return m_dwLine;
				}

				break;

			default:
				chTemp[nOut++] = m_pBuffer[m_dwIndex];
				break;
			};
		}
	}

	if ( nOut > 0 )
	{
		chTemp[nOut++] = '\0';
		
		if ( m_dwLine==0 )	m_dwLine++;

		return m_dwLine;
	}

	return m_dwLine = 0; //nix gelezen
}

void CStringFileGS::Reset ()
{
	if ( m_fFile==NULL )	return;

	m_dwIndex = 0;
	m_dwLine = 0;

	fseek ( m_fFile, SEEK_SET, 0 );
}
