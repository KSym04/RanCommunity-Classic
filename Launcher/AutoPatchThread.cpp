#include "StdAfx.h"
#include "./AutoPatchThread.h"
#include "./Resource.h"

#include "../Lib_Network/s_CHttpPatch.h"
#include "../Lib_Helper/CompactFdi.h"
#include "../Lib_Engine/Common/StringUtils.h"
#include "./LauncherText.h"
#include "../Lib_Engine/Common/SUBPATH.h"
#include "./RLauncher.h"
#include "./RLauncherData.h"
#include "../Lib_Engine/Hash/CHash.h"
#include "../Lib_Engine/Common/SerialFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CAutoPatchThread, CHttpThread)

CAutoPatchThread::CAutoPatchThread( LPVOID pParam, DWORD nThreadID )
{
	VERIFY( pParam != NULL);
	m_pPatchThreadParam = (S_PATCH_THREAD_PARAM *)pParam;

	m_nDlgThreadID = nThreadID;
}

void CAutoPatchThread::ThreadMain()
{
	DWORD time_start = ::GetTickCount();
	DWORD time_run = ::GetTickCount();

	CHttpPatch* pHttpPatch	= m_pPatchThreadParam->pHttpPatch;
	
	CString cFileList, sFileList, str, strTemp;
	BOOL bInstalling(FALSE);

	CreateDirectory ( rlauncher_global::temp_path.c_str(), NULL );

	rlauncher_log_control::set_process_all_position ( 0, 100 );

	if ( IsForceTerminate() ) return ;


	::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 1 ), 0 );

	if ( !GETFILE_USEHTTP ( pHttpPatch, "\\", rlauncher_setting::patch_file_list.c_str(), "" ) )
	{	
		if ( !IsForceTerminate() )
		{
			::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 2 ), 0 );
		}

		goto LFail;
	}

	if ( IsForceTerminate() ) goto LFail;


	::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 4 ), 0 );
	if ( !LoadList() )
	{
		::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 5 ), 0 );
		goto LFail;
	}

	if ( IsForceTerminate() ) goto LFail;

	rlauncher_log_control::set_process_all_position ( 4, 100 );

	::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 6 ), 0 );
	if ( !CompareFileList () )
	{
		::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 7 ), 0 );
		goto LFail;
	}    

	if ( IsForceTerminate() ) goto LFail;

	rlauncher_log_control::set_process_all_position ( 8, 100 );

	::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 9 ), 0 );
	if ( !CreateFolders () )
	{
		::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 10 ), 0 );
		goto LFail;
	}    

	rlauncher_log_control::set_process_all_position ( 10, 100 );

	if ( IsForceTerminate() ) goto LFail;

	if ( !DownloadFilesByHttp ( pHttpPatch ) )
	{
		if ( !IsForceTerminate() )
		{
			::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 8 ), 0 );
		}

		goto LFail;
	}

	rlauncher_log_control::set_process_all_position ( 90, 100 );

	if ( IsForceTerminate() ) goto LFail;

	DeleteList();

	rlauncher_log_control::set_process_all_position ( 100, 100 );

	Destroy ();

	time_run = ::GetTickCount() - time_start;

	if ( time_run )
	{
		DWORD milliseconds = time_run % 1000;
		time_run /= 1000;
		DWORD seconds = time_run % 60;
		time_run /= 60;
		DWORD minutes = time_run % 60;
		time_run /= 60;
		DWORD hours = time_run; // may exceed 24 hours.

		CHAR * szListMessage = new CHAR[256];
		wsprintf( szListMessage, "%d:%02d:%02d.%03d\n", hours, minutes, seconds, milliseconds);
		::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, 0, (LPARAM)szListMessage );
	}
	

	return;

LFail:

	Destroy ();
	SetFail();

	time_run = ::GetTickCount() - time_start;

	if ( time_run )
	{
		DWORD milliseconds = time_run % 1000;
		time_run /= 1000;
		DWORD seconds = time_run % 60;
		time_run /= 60;
		DWORD minutes = time_run % 60;
		time_run /= 60;
		DWORD hours = time_run; // may exceed 24 hours.

		CHAR * szListMessage = new CHAR[256];
		wsprintf( szListMessage, "%d:%02d:%02d.%03d\n", hours, minutes, seconds, milliseconds);
		::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, 0, (LPARAM)szListMessage );
	}

	return ;
}

BOOL CAutoPatchThread::LoadList()
{
	m_map_patch_list.clear();

	std::string file_path = rlauncher_global::temp_path;
	file_path += rlauncher_setting::patch_file_list;

	CSerialFile	serial_file;
	if ( !serial_file.OpenFile ( FOT_READ, file_path.c_str() ) ) return FALSE;

	char file_type[FILETYPESIZE] = "";
	DWORD version = 0;
	serial_file.GetFileType( file_type, FILETYPESIZE, version );

	serial_file.SetEncodeType( EMRCRYPT_BYTE_TYPE_LAUNCHER_PATCH_LIST_VER_1 );

	if ( version == PATCH_FILE_LIST_VERSION )
	{
		DWORD num = 0;
		serial_file >> num;

		for ( DWORD i=0; i<num; ++i )
		{
			GAME_LAUNCHER_FILE_DATA file_data;
			serial_file >> file_data.file_name;
			serial_file >> file_data.file_path_sub;
			serial_file >> file_data.file_hash;

			file_data.file_path_full = file_data.file_path_sub + file_data.file_name;
			m_map_patch_list.insert ( std::make_pair(file_data.file_path_full,file_data) );
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void CAutoPatchThread::DeleteList()
{
	std::string file_path = rlauncher_global::temp_path;
	file_path += rlauncher_setting::patch_file_list;
	DeleteFile( file_path.c_str() );
}

BOOL CAutoPatchThread::CreateFolders()
{
	GAME_LAUNCHER_FILE_DATA_MAP_ITER it = m_map_patch_list.begin();
	GAME_LAUNCHER_FILE_DATA_MAP_ITER end  = m_map_patch_list.end();

	for ( ; it!=end; ++it )
	{
		GAME_LAUNCHER_FILE_DATA& file_data = (*it).second;

		if ( IsForceTerminate() ) return FALSE;

		std::string full_path = rlauncher_global::app_path;
		full_path += file_data.file_path_sub;

		unsigned int pos = 0;
		do
		{
			pos = full_path.find_first_of("\\/", pos + 1);
			CreateDirectory(full_path.substr(0, pos).c_str(), NULL);
		} while (pos != std::string::npos);

		Sleep( 0 );
	}

	return TRUE;
}

BOOL CAutoPatchThread::CompareFileList()
{
	m_map_download_list.clear();

	GAME_LAUNCHER_FILE_DATA_MAP_ITER it = m_map_patch_list.begin();
	GAME_LAUNCHER_FILE_DATA_MAP_ITER end  = m_map_patch_list.end();

	for ( ; it!=end; ++it )
	{
		GAME_LAUNCHER_FILE_DATA& file_data = (*it).second;

		if ( IsForceTerminate() ) return FALSE;

		HASH::CHash g_hashObj;
		g_hashObj.SetHashAlgorithm( HASH::MD5 );
		g_hashObj.SetHashOperation( HASH::FILE_HASH );
		g_hashObj.SetHashFormat( HASH::UPPERCASE_NOSPACES );

		std::string full_path = rlauncher_global::app_path;
		full_path += file_data.file_path_full;
		g_hashObj.SetHashFile( full_path.c_str() );

		std::string file_hash = g_hashObj.DoHash().GetString();
		if ( file_data.file_hash != file_hash )
		{
			GAME_LAUNCHER_FILE_DATA new_file_data = file_data;
			new_file_data.file_name += ".cab"; 

			m_map_download_list.insert ( std::make_pair(new_file_data.file_path_full,new_file_data) );
		}

		Sleep( 0 );
	}

	return TRUE;
}

BOOL CAutoPatchThread::DownloadFilesByHttp( CHttpPatch* pHttpPatch )
{
	int DownCount = 0;
	int ErrorDownload = 0;
	int ErrorExtract = 0;

	int nVectorNewFileSize = (int)m_map_download_list.size();

	GAME_LAUNCHER_FILE_DATA_MAP_ITER it = m_map_download_list.begin();
	GAME_LAUNCHER_FILE_DATA_MAP_ITER end  = m_map_download_list.end();

	for ( ; it!=end; ++it )
	{
		GAME_LAUNCHER_FILE_DATA& file_data = (*it).second;

		if ( IsForceTerminate() ) return FALSE;

		::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)file_data.file_name.c_str(), 0 );

		ULONGLONG TotalPos = 10 + (++DownCount * 80) / nVectorNewFileSize;			
		rlauncher_log_control::set_process_all_position( TotalPos, 100 );

		if ( !GETFILE_USEHTTP ( pHttpPatch, file_data.file_path_sub.c_str(), file_data.file_name.c_str(), "" ) )
		{
			ErrorDownload ++;
		}

		Sleep( 30 );

		CString	DownloadDir;
		DownloadDir.Format( "%s%s", rlauncher_global::temp_path.c_str(), file_data.file_name.c_str() );
		
		CString	DestFile;
		DestFile.Format( "%s%s", rlauncher_global::app_path.c_str(), file_data.file_path_full.c_str() );

		CString FullPath;
		FullPath.Format( "%s%s", rlauncher_global::app_path.c_str(), file_data.file_path_sub.c_str() );
	
		SetFileAttributes( DestFile.GetString(), FILE_ATTRIBUTE_NORMAL );
		Sleep( 5 );

		DeleteFile( DestFile.GetString() );
		Sleep( 5 );

		if( !Extract ( FullPath.GetString(), DownloadDir.GetString() ) )
		{
			ErrorExtract ++;
		}

		Sleep( 5 );
		SetFileAttributes ( DestFile.GetString(), FILE_ATTRIBUTE_NORMAL );

		Sleep( 5 );
		SetFileAttributes( DownloadDir.GetString(), FILE_ATTRIBUTE_NORMAL );

		Sleep( 5 );
		DeleteFile( DownloadDir.GetString() );

		if( IsForceTerminate() ) return FALSE;

		Sleep( 0 );
	}
	
	if ( ErrorDownload )
		::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 11 ), 0 );

	if ( ErrorExtract )
		::PostThreadMessage( m_nDlgThreadID, WM_LISTADDSTRING, (WPARAM)ID2LAUNCHERTEXT("IDS_MESSAGE", 12 ), 0 );

	return TRUE;
}

void CAutoPatchThread::Destroy()
{
	m_map_patch_list.clear();
	m_map_download_list.clear();
}

BEGIN_MESSAGE_MAP(CAutoPatchThread, CHttpThread)
END_MESSAGE_MAP()