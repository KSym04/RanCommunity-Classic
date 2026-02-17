#include "stdafx.h"
#include "./GLAgentServer.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GLAgentServer::KickOutListLoad()
{
	std::string strFilePath = RPATH::getAppPath();
	strFilePath += "\\KOList.ini";

	FILE *oFile = NULL;

	if ((oFile = ::_tfopen(strFilePath.c_str(), _T("r"))) == NULL)	return;

	TCHAR line[300];

	m_mapKickOut.clear();

	if ( m_pConsoleMsg )
		m_pConsoleMsg->Write( _T("KickOut Load Start") );

	while (::_fgetts(line, 200, oFile))
	{
		TCHAR seps[]   = _T(" ,\t\n");

		TCHAR* token = _tcstok(line, seps);
		while (token != NULL)
		{
			if (_tcscmp(token, _T("//")) == 0) 
			{	
				break;
			}
			else if (_tcscmp(token, "user") == 0)
			{
				SKICKOUT_DATA sData;

				//user num
				token = ::_tcstok(NULL, seps);
				if (token) sData.dwUserNum = (DWORD)_tstoi(token);
				else       break;
				
				//end time
				token = ::_tcstok(NULL, seps);
				if (token) sData.fTime = (float)_tstol(token);
				else       break;
				
				m_mapKickOut.insert( std::make_pair( sData.dwUserNum, sData ) );

				if ( m_pConsoleMsg )
					m_pConsoleMsg->Write( _T("KickOut Added %u %g"), sData.dwUserNum, sData.fTime );
			}
			
			token = ::_tcstok( NULL, seps );
		}
	}

	::fclose( oFile );

	if ( m_pConsoleMsg )
		m_pConsoleMsg->Write( _T("KickOut Load End") );
}

void GLAgentServer::KickOutListSave()
{
	std::string strFilePath = RPATH::getAppPath();
	strFilePath += "\\KOList.ini";

	FILE *oFile = NULL;

	if ((oFile = ::_tfopen(strFilePath.c_str(), _T("w"))) == NULL)	return;

	CString strText;

	KICKOUT_DATA_MAP_ITER it_b = m_mapKickOut.begin();
	KICKOUT_DATA_MAP_ITER it_e = m_mapKickOut.end();

	for( ; it_b != it_e; ++it_b )
	{
		SKICKOUT_DATA& sData = (*it_b).second;

		CString strFormat;
		strFormat.Format( "user %u %g", sData.dwUserNum, sData.fTime );
		strText += strFormat.GetString();
		strText += "\n";
	}

	if (strText.GetLength())
		::fwrite ( strText.GetBuffer(), strText.GetLength(), 1, oFile );

	::fclose( oFile );


	if ( m_pConsoleMsg )
		m_pConsoleMsg->Write( _T("KickOut Save List") );
}