#include "StdAfx.h"
#include "./httpthread.h"
#include "./Resource.h"

#include "../Lib_Network/s_CHttpPatch.h"
#include "./LauncherText.h"
#include "../Lib_Engine/Common/SUBPATH.h"
#include "./RLauncher.h"
#include "../Lib_Engine/Common/SerialFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CHttpThread, CLPatchThread)

BOOL CHttpThread::GETFILE_USEHTTP( CHttpPatch* pHttpPatch, std::string strRemoteSubPath, std::string strFileName, CString strTempDir )
{
	if ( !pHttpPatch )			return FALSE;
	if ( !strFileName.size () )	return FALSE;

	std::string strSubPath( strRemoteSubPath );

	std::replace ( strSubPath.begin(), strSubPath.end(), '\\','/' );
	strSubPath += strFileName;

	std::string strLocalFullPath;
	strLocalFullPath += rlauncher_global::temp_path;	
	strLocalFullPath += strTempDir.GetString();
	strLocalFullPath += strFileName;

	static int nTRY = 0;
	int nTRY_FILESIZE_CHECK = 0;
	int nERROR_RETRY = 0;
	int nADDRESS_NULL_COUNT = 0;
	while ( nTRY_FILESIZE_CHECK < 3 )
	{
		if ( IsForceTerminate () ) return FALSE;		

		if ( nADDRESS_NULL_COUNT == LAUNCHER_TOTAL_SERVER ) return FALSE;

		if ( LAUNCHER_TOTAL_SERVER == nTRY ) nTRY = 0;
		if ( nERROR_RETRY == 5 ) return FALSE;		

		static const CString strHTTP = "http://";

		CString strRealAddress = rlauncher_setting::patch_server_url[nTRY].c_str();
		if ( !strRealAddress.GetLength () )
		{
			nADDRESS_NULL_COUNT++;
			nTRY++;
			continue;
		}

		nADDRESS_NULL_COUNT = 0;

		CString strHttpAddress = strHTTP + strRealAddress; 

		if ( NET_ERROR == pHttpPatch->SetBaseURL ( strHttpAddress ) )
		{			
			nTRY++;
			nERROR_RETRY++;
			continue;
		}

		if ( NET_ERROR == pHttpPatch->GetFile ( strSubPath.c_str(), strLocalFullPath.c_str() ) )
		{
			CString	strMsg;
			strMsg = ID2LAUNCHERTEXT("IDS_MESSAGE", 1 );
			CHAR * szListMessage = new CHAR[256];
			wsprintf( szListMessage, "%s %s", strMsg.GetString(), strFileName.c_str() );
			::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, 0, (LPARAM)szListMessage );

			nTRY ++;
			nERROR_RETRY++;
			continue;
		}

		if ( IsForceTerminate () ) return FALSE;	

		ULONGLONG ulRECEIVED, ulTOTALSIZE;
		rlauncher_log_control::get_process_cur_position ( &ulRECEIVED, &ulTOTALSIZE );

		if ( ulRECEIVED != ulTOTALSIZE )
		{
			nTRY_FILESIZE_CHECK++;
			::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 3 ), 0 );
			continue;
		}
		else
		{
			return TRUE;
		}

		Sleep( 0 );
	}

	return FALSE;
}

BEGIN_MESSAGE_MAP(CHttpThread, CLPatchThread)
END_MESSAGE_MAP()