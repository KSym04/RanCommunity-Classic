
// PatchBuilderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./PatchBuilder.h"
#include "./PatchBuilderDlg.h"

#include "./RPatcher.h"
#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Engine/Hash/CHash.h"
#include "../Lib_Engine/Common/SerialFile.h"
#include "./MIN_CAB_UTIL.h"
#include "./CMinFtp.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CPatchBuilderDlg::setup_app_path()
{
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	rpatcher_global::app_path = RPATH::getAppPath();

	TCHAR path[MAX_PATH] = {0};
	StringCchCopy( path, MAX_PATH, rpatcher_global::app_path.c_str() );
	StringCchCat( path, MAX_PATH, rpatcher_settings::source_folder.c_str() );
	rpatcher_global::source_path = path;

	StringCchCopy( path, MAX_PATH, rpatcher_global::app_path.c_str() );
	StringCchCat( path, MAX_PATH, rpatcher_settings::output_folder.c_str() );
	rpatcher_global::output_path = path;

	std::transform ( rpatcher_global::source_path.begin(), rpatcher_global::source_path.end(), rpatcher_global::source_path.begin(), std::tolower );
	std::transform ( rpatcher_global::output_path.begin(), rpatcher_global::output_path.end(), rpatcher_global::output_path.begin(), std::tolower );
}

void CPatchBuilderDlg::show_path_info()
{
	SetWin_Text( this, IDC_EDIT_PROGRAM_PATH, rpatcher_global::app_path.c_str() );
	SetWin_Text( this, IDC_EDIT_SOURCE_PATH, rpatcher_global::source_path.c_str() );
	SetWin_Text( this, IDC_EDIT_OUTPUT_PATH, rpatcher_global::output_path.c_str() );

	CreateDirectory(rpatcher_global::source_path.c_str(), NULL);
	CreateDirectory(rpatcher_global::output_path.c_str(), NULL);
}

void CPatchBuilderDlg::logs_add( const char *szFormat, ... )
{
	va_list argList;
	char szBuffer[256] = "";
	va_start(argList, szFormat);
	StringCbVPrintf( szBuffer, 256, szFormat, argList );
	va_end(argList);

	CString text( szBuffer );
	if ( text[text.GetLength()-1] == '\n' )
	{
		text = text.Left ( text.GetLength()-1 );
		if ( text[text.GetLength()-1] == '\r' )
		{
			text = text.Left ( text.GetLength()-1 );
		}
	}

	int idx = m_ctrl_list_logs.AddString( text );
	m_ctrl_list_logs.SetCurSel( idx );
}

void CPatchBuilderDlg::update_progress()
{
	static const ULONGLONG UL100 = 100UL;
	static const ULONGLONG UL1 = 1UL;

	ULONGLONG pos = m_cur_pos;
	ULONGLONG end = m_end_pos;

	CString text;
	text.Format( "%I64u / %I64u", pos, end );
	m_ctrl_progress_static.SetWindowText( text.GetString() );

	end = ( !end ) ? UL1 : end;
	int	cur_percent = int( (pos*UL100) / end );	

	m_ctrl_progress_total.SetPos ( cur_percent );
}

void CPatchBuilderDlg::set_button( BOOL b )
{
	SetWin_Enable( this, IDC_BUTTON_SCAN, b );
	SetWin_Enable( this, IDC_BUTTON_BUILD, b );
	SetWin_Enable( this, IDC_BUTTON_UPLOAD_PATCH, b );
}

void CPatchBuilderDlg::scan_path_recurse ( std::string &dir )
{
	CFileFind finder;

	//	build a string with wildcards
	std::string strWildcard ( dir );
	strWildcard += _T("\\*.*");

	//	start working for files
	BOOL bWorking = finder.FindFile ( strWildcard.c_str() );

	while ( bWorking )
	{
		bWorking = finder.FindNextFile ();

		//	skip . and .. files; otherwise, we'd
		//	recur infinitely!
		if ( finder.IsDots() ) continue;

		//	if it's a directory, recursively search it
		if ( finder.IsDirectory() )
		{
			std::string strPath = finder.GetFilePath();
			scan_path_recurse ( strPath );
		}
		else
		{
			PATCH_BUILDER_FILE_DATA file_data;
			file_data.file_name = finder.GetFileName().GetString();
			std::transform ( file_data.file_name.begin(), file_data.file_name.end(), file_data.file_name.begin(), std::tolower );
			file_data.file_path_full = finder.GetFilePath().GetString();
			std::transform ( file_data.file_path_full.begin(), file_data.file_path_full.end(), file_data.file_path_full.begin(), std::tolower );

			file_data.file_path_sub = file_data.file_path_full;

			std::string full_path = rpatcher_global::source_path;
			std::transform ( full_path.begin(), full_path.end(), full_path.begin(), std::tolower );

			bool valid_subpath = false;
			std::string::size_type pos = file_data.file_path_sub.find(full_path);
			if (pos != std::string::npos)
			{
				file_data.file_path_sub.erase(pos, full_path.length() -1 );

				pos = file_data.file_path_sub.find(file_data.file_name);
				if (pos != std::string::npos)
				{
					file_data.file_path_sub.erase(pos, file_data.file_name.length() );
					valid_subpath = true;
				}
			}

			if ( !valid_subpath )
				file_data.file_path_sub = "\\";
			
			m_map_file_scan.insert ( std::make_pair(file_data.file_path_full,file_data) );
		}
	}

	finder.Close();
}


BOOL CPatchBuilderDlg::read_patch_list()
{
	std::string file_path = rpatcher_global::app_path;
	file_path += "\\";
	file_path += rpatcher_settings::patch_file_list_server;

	CSerialFile	serial_file;
	if ( !serial_file.OpenFile ( FOT_READ, file_path.c_str() ) ) return FALSE;

	char file_type[FILETYPESIZE] = "";
	DWORD version = 0;
	serial_file.GetFileType( file_type, FILETYPESIZE, version );


	if ( version == PATCH_FILE_LIST_VERSION )
	{
		DWORD num = 0;
		serial_file >> num;

		for ( DWORD i=0; i<num; ++i )
		{
			PATCH_BUILDER_FILE_DATA file_data;
			serial_file >> file_data.file_name;
			serial_file >> file_data.file_path_full;
			serial_file >> file_data.file_path_sub;
			serial_file >> file_data.file_hash;

			m_map_patch_list.insert ( std::make_pair(file_data.file_path_full,file_data) );
		}
	}
	else
	{
		logs_add( "file list invalid version:%d %x", version, version );
		return FALSE;
	}

	return TRUE;
}

BOOL CPatchBuilderDlg::save_patch_list()
{
	std::string file_path = rpatcher_global::app_path;
	file_path += "\\";
	file_path += rpatcher_settings::patch_file_list_server;

	CSerialFile	serial_file;
	std::string file_type = "[file_list_server]";
	serial_file.SetFileType ( file_type.c_str(), PATCH_FILE_LIST_VERSION );

	if ( !serial_file.OpenFile ( FOT_WRITE, file_path.c_str() ) ) return FALSE;

	serial_file << (DWORD)m_map_patch_list.size();

	PATCH_BUILDER_FILE_DATA_MAP_ITER it = m_map_patch_list.begin();
	PATCH_BUILDER_FILE_DATA_MAP_ITER end = m_map_patch_list.end();

	for ( ; it!=end; ++it )
	{
		PATCH_BUILDER_FILE_DATA& file_data = (*it).second;
		serial_file << file_data.file_name;
		serial_file << file_data.file_path_full;
		serial_file << file_data.file_path_sub;
		serial_file << file_data.file_hash;
	}

	return save_patch_list_client();
}

BOOL CPatchBuilderDlg::save_patch_list_client()
{
	std::string file_path = rpatcher_global::output_path;
	file_path += rpatcher_settings::patch_file_list_client;

	CSerialFile	serial_file;
	std::string file_type = "[file_list_client]";
	serial_file.SetFileType ( file_type.c_str(), PATCH_FILE_LIST_VERSION );

	if ( !serial_file.OpenFile ( FOT_WRITE, file_path.c_str() ) ) return FALSE;

	serial_file.SetEncodeType( EMRCRYPT_BYTE_TYPE_LAUNCHER_PATCH_LIST_VER_1 );

	serial_file << (DWORD)m_map_patch_list.size();

	PATCH_BUILDER_FILE_DATA_MAP_ITER it = m_map_patch_list.begin();
	PATCH_BUILDER_FILE_DATA_MAP_ITER end = m_map_patch_list.end();

	for ( ; it!=end; ++it )
	{
		PATCH_BUILDER_FILE_DATA& file_data = (*it).second;
		serial_file << file_data.file_name;
		//serial_file << file_data.file_path_full;
		serial_file << file_data.file_path_sub;
		serial_file << file_data.file_hash;
	}

	return TRUE;
}

void CPatchBuilderDlg::action_scan_start()
{
	DWORD dwThreadId;

	HANDLE hThread = (HANDLE) ::_beginthreadex(
		NULL, 
		0, 
		scan_start,
		this,
		0,
		(unsigned int*) &dwThreadId);
}

unsigned int WINAPI CPatchBuilderDlg::scan_start( void* pDlg )
{
	CPatchBuilderDlg* pDLG = (CPatchBuilderDlg*) pDlg;

	pDLG->logs_add( "scanning.." );
	pDLG->m_map_file_scan.clear();
	pDLG->scan_path_recurse( rpatcher_global::source_path );

	unsigned int total = pDLG->m_map_file_scan.size();
	pDLG->logs_add( "scan finished, files found:%u",total  );


	pDLG->scan_source();
	
	pDLG->set_button( TRUE );

	return 0;
}

void CPatchBuilderDlg::action_build_start()
{
	DWORD dwThreadId;

	HANDLE hThread = (HANDLE) ::_beginthreadex(
		NULL, 
		0, 
		build_start,
		this,
		0,
		(unsigned int*) &dwThreadId);
}

void CPatchBuilderDlg::action_upload_start()
{
	DWORD dwThreadId;

	HANDLE hThread = (HANDLE) ::_beginthreadex(
		NULL, 
		0, 
		upload_start,
		this,
		0,
		(unsigned int*) &dwThreadId);
}

unsigned int WINAPI CPatchBuilderDlg::build_start( void* pDlg )
{
	CPatchBuilderDlg* pDLG = (CPatchBuilderDlg*) pDlg;

	pDLG->logs_add( "reading existing patch list.." );

	pDLG->m_map_patch_list.clear();
	pDLG->m_map_update_list.clear();
	pDLG->m_map_upload_list.clear();

	BOOL read = pDLG->read_patch_list();
	if ( !read )
	{
		pDLG->logs_add( "patch list read failed, going to rebuild" );

		pDLG->m_map_patch_list = pDLG->m_map_file_scan;
		pDLG->m_map_update_list = pDLG->m_map_patch_list;

		CreateDirectory(rpatcher_global::output_path.c_str(), NULL);

		BOOL save = pDLG->save_patch_list();
		if ( !save )
		{
			pDLG->logs_add( "patch list save failed, abort operation!" );
			pDLG->set_button( TRUE );
			return 0;
		}

		pDLG->build_patch();
	}
	else
	{
		pDLG->compare_hash();

		CreateDirectory(rpatcher_global::output_path.c_str(), NULL);

		BOOL save = pDLG->save_patch_list();
		if ( !save )
		{
			pDLG->logs_add( "patch list save failed, abort operation!" );
			pDLG->set_button( TRUE );
			return 0;
		}

		pDLG->build_patch();
	}


	pDLG->set_button( TRUE );

	return 0;
}


unsigned int WINAPI CPatchBuilderDlg::upload_start( void* pDlg )
{
	CPatchBuilderDlg* pDLG = (CPatchBuilderDlg*) pDlg;

	pDLG->upload_patch();

	pDLG->set_button( TRUE );

	return 0;
}

void CPatchBuilderDlg::scan_source()
{
	logs_add( "hash start" );
	DWORD time_start = ::GetTickCount();

	unsigned int total = m_map_file_scan.size();

	m_cur_pos = 0;
	m_end_pos = total;

	unsigned int hash = 0;

	PATCH_BUILDER_FILE_DATA_MAP_ITER it = m_map_file_scan.begin();
	PATCH_BUILDER_FILE_DATA_MAP_ITER end = m_map_file_scan.end();

	for ( ; it!=end; ++it )
	{
		PATCH_BUILDER_FILE_DATA& file_data = (*it).second;

		HASH::CHash g_hashObj;
		g_hashObj.SetHashAlgorithm( HASH::MD5 );
		g_hashObj.SetHashOperation( HASH::FILE_HASH );
		g_hashObj.SetHashFormat( HASH::UPPERCASE_NOSPACES );

		g_hashObj.SetHashFile( file_data.file_path_full.c_str() );
		file_data.file_hash = g_hashObj.DoHash().GetString();

		if ( file_data.file_hash.empty() )
		{
			logs_add( "hash failed file:%s dir:%s", file_data.file_name.c_str(), file_data.file_path_full.c_str() );
			continue;
		}

		hash++;

		m_cur_pos = hash;
		m_end_pos = total;
	}

	m_cur_pos = total;
	m_end_pos = total;

	DWORD time_run = ::GetTickCount() - time_start;

	if ( time_run < 1000 )
		logs_add( "hash finished, total:%u time:%u msec", hash, time_run );
	else
		logs_add( "hash finished, total:%u time:%u sec", hash, time_run/1000 );
}

void CPatchBuilderDlg::build_patch()
{
	DWORD time_start = ::GetTickCount();

	logs_add( "build patch start" );

	unsigned int total = m_map_update_list.size();

	m_cur_pos = 0;
	m_end_pos = total;

	PATCH_BUILDER_FILE_DATA_MAP_ITER it = m_map_update_list.begin();
	PATCH_BUILDER_FILE_DATA_MAP_ITER end = m_map_update_list.end();

	logs_add( "create folders" );
	int build = 0;
	for ( ; it!=end; ++it )
	{
		PATCH_BUILDER_FILE_DATA& file_data = (*it).second;
		
		std::string save_dir = rpatcher_global::output_path;
		save_dir.erase( save_dir.size() - 1 );
		save_dir += file_data.file_path_sub;

		unsigned int pos = 0;
		do
		{
			pos = save_dir.find_first_of("\\/", pos + 1);
			CreateDirectory(save_dir.substr(0, pos).c_str(), NULL);
		} while (pos != std::string::npos);

		build++;

		m_cur_pos = build;
		m_end_pos = total;
	}

	logs_add( "checking folders" );
	
	m_cur_pos = 0;
	m_end_pos = total;

	int check = 0;
	for ( ; it!=end; ++it )
	{
		PATCH_BUILDER_FILE_DATA& file_data = (*it).second;

		std::string save_dir = rpatcher_global::output_path;
		save_dir.erase( save_dir.size() - 1 );
		save_dir += file_data.file_path_sub;

		unsigned int pos = 0;
		do
		{
			pos = save_dir.find_first_of("\\/", pos + 1);
			save_dir = save_dir.substr(0, pos).c_str();
			if ( INVALID_FILE_ATTRIBUTES == GetFileAttributes(save_dir.c_str()) )
			{
				logs_add( "folder invalid:%s", save_dir.c_str() );
			}
		} while (pos != std::string::npos);

		check++;

		m_cur_pos = check;
		m_end_pos = total;
	}



	it = m_map_update_list.begin();
	end = m_map_update_list.end();

	logs_add( "compressing files" );
	
	m_cur_pos = 0;
	m_end_pos = total;

	int compress = 0;
	for ( ; it!=end; ++it )
	{
		PATCH_BUILDER_FILE_DATA& file_data = (*it).second;

		std::string save_dir = rpatcher_global::output_path;
		save_dir.erase( save_dir.size() - 1 );
		save_dir += file_data.file_path_sub;

		if (CAB_UTIL_MIN::MinMakeCab( file_data.file_path_full.c_str(), save_dir.c_str() ) == TRUE)
		{ 
		}
		else
		{
			logs_add( "failed to compress file:%s source:%s output:%s", file_data.file_name.c_str(), file_data.file_path_full.c_str(), save_dir.c_str() );
			continue;
		}

		compress++;

		m_cur_pos = compress;
		m_end_pos = total;
	}

	update_upload_list();

	DWORD time_run = ::GetTickCount() - time_start;

	if ( time_run < 1000 )
		logs_add( "compress finished, total:%u time:%u msec", compress, time_run );
	else
		logs_add( "compress finished, total:%u time:%u sec", compress, time_run/1000 );
}

void CPatchBuilderDlg::compare_hash()
{
	DWORD time_start = ::GetTickCount();

	logs_add( "start comparing list" );

	unsigned int total = m_map_file_scan.size();

	m_cur_pos = 0;
	m_end_pos = total;

	PATCH_BUILDER_FILE_DATA_MAP_ITER scan_it = m_map_file_scan.begin();
	PATCH_BUILDER_FILE_DATA_MAP_ITER scan_end = m_map_file_scan.end();

	unsigned int compare = 0;
	for ( ; scan_it!=scan_end; ++scan_it )
	{
		PATCH_BUILDER_FILE_DATA& file_data = (*scan_it).second;

		PATCH_BUILDER_FILE_DATA_MAP_ITER list_it = m_map_patch_list.find( file_data.file_path_full );
		if ( list_it == m_map_patch_list.end() )
		{
			//new file
			m_map_update_list.insert ( std::make_pair(file_data.file_path_full,file_data) );
			m_map_patch_list.insert ( std::make_pair(file_data.file_path_full,file_data) );
		}
		else
		{
			PATCH_BUILDER_FILE_DATA& file_data2 = (*list_it).second;
			if ( file_data.file_hash != file_data2.file_hash )
			{
				//file mismatch
				file_data2.file_hash = file_data.file_hash;
				m_map_update_list.insert ( std::make_pair(file_data.file_path_full,file_data) );
			}
		}

		compare ++;

		m_cur_pos = compare;
		m_end_pos = total;
	}

	DWORD time_run = ::GetTickCount() - time_start;

	unsigned int update = m_map_update_list.size();

	if ( time_run < 1000 )
		logs_add( "compare finished, to update files:%u time:%u msec", update, time_run );
	else
		logs_add( "compare finished, to update files:%u time:%u sec", update, time_run/1000 );
}

void CPatchBuilderDlg::upload_patch()
{
	logs_add( "ftp sort folders" );

	DWORD time_start = ::GetTickCount();

	m_map_upload_list.clear();
	read_upload_list();

	unsigned int total = m_map_upload_list.size();

	m_cur_pos = 0;
	m_end_pos = total;

	PATCH_BUILDER_FILE_DATA_MAP_ITER it = m_map_upload_list.begin();
	PATCH_BUILDER_FILE_DATA_MAP_ITER end = m_map_upload_list.end();

	std::vector<std::string> vec_file_dir;

	int check = 0;
	for ( ; it!=end; ++it )
	{
		PATCH_BUILDER_FILE_DATA& file_data = (*it).second;
		std::string save_dir = file_data.file_path_sub;

		unsigned int pos = 0;
		do
		{
			pos = save_dir.find_first_of("\\/", pos + 1);

			std::string create_dir = save_dir.substr(0, pos).c_str();

			bool exist = false;
			for ( unsigned int n=0; n<vec_file_dir.size(); ++n )
			{
				if ( vec_file_dir[n] == create_dir )	exist = true;
			}

			if ( !exist )
				vec_file_dir.push_back( create_dir );

		} while (pos != std::string::npos);

		check ++;

		m_cur_pos = check;
		m_end_pos = total;

		Sleep( 0 );
	}

	DWORD time_run = ::GetTickCount() - time_start;

	if ( time_run < 1000 )
		logs_add( "ftp sort folders done, folders:%u time:%u msec", vec_file_dir.size(), time_run );
	else
		logs_add( "ftp sort folders done, folders:%u time:%u sec", vec_file_dir.size(), time_run/1000 );




	logs_add( "start ftp" );
	std::string ftp_ip = GetWin_Text( this, IDC_EDIT_FTP_IP );
	int ftp_port = GetWin_Num_int( this, IDC_EDIT_FTP_PORT );
	std::string ftp_user = GetWin_Text( this, IDC_EDIT_FTP_USER );
	std::string ftp_pass = GetWin_Text( this, IDC_EDIT_FTP_PASS );

	CMinFtp* pFtp = new CMinFtp();

	int nRetCode = pFtp->Connect( ftp_ip.c_str(), ftp_port, ftp_user.c_str(), ftp_pass.c_str() );
	if (nRetCode == NET_ERROR)
	{
		SAFE_DELETE( pFtp );

		logs_add( "ftp connect failure" );
		return;
	}
	else
	{
		logs_add( "ftp connected" );
	}

	CString strFirstDir;

	if (pFtp->m_pConnect->GetCurrentDirectory(strFirstDir) == 0)
	{
		pFtp->DisConnect();
		
		SAFE_DELETE( pFtp )

		logs_add( "ftp failure  connect" );
		return;
	}

	
	logs_add( "ftp create folders" );

	m_cur_pos = 0;
	m_end_pos = total;

	time_start = ::GetTickCount();

	//recreate directory 5 times
	//a funny hack to make sure folders exist in ftp
	int build = 0;
	int total_build = (int)vec_file_dir.size() * 5;
	for ( int i=0; i<5; ++i )
	{
		for ( unsigned int n=0; n<vec_file_dir.size(); ++n )
		{
			std::string create_dir = vec_file_dir[n];

			CString create_directory = create_dir.c_str();
			create_directory.Replace('\\', '/');

			pFtp->CreateDirectory(create_directory);
			pFtp->SetCurrentDirectory(create_directory);

			build ++;

			m_cur_pos = build;
			m_end_pos = total_build;

			Sleep( 5 );
		}
	}

	pFtp->SetCurrentDirectory(strFirstDir);        

	time_run = ::GetTickCount() - time_start;

	if ( time_run < 1000 )
		logs_add( "ftp create folders done, time:%u msec", time_run );
	else
		logs_add( "ftp create folders done, time:%u sec", time_run/1000 );





	logs_add( "ftp upload files" );

	m_cur_pos = 0;
	m_end_pos = total;

	time_start = ::GetTickCount();

	it = m_map_upload_list.begin();
	end = m_map_upload_list.end();

	int upload = 0;
	for ( ; it!=end; ++it )
	{
		PATCH_BUILDER_FILE_DATA& file_data = (*it).second;

		std::string file_name = file_data.file_name;
		file_name += ".cab";

		std::string full_path = rpatcher_global::output_path;
		full_path.erase( full_path.size() - 1 );
		full_path += file_data.file_path_sub;
		full_path += file_name;

		CString create_directory = full_path.c_str();
		create_directory.Replace('\\', '/');

		CString create_file_name = file_name.c_str();
		create_file_name.Replace('\\', '/');

		CString current_directory = file_data.file_path_sub.c_str();
		current_directory.Replace('\\', '/');

		pFtp->SetCurrentDirectory( current_directory );

		if (pFtp->PutFile(create_directory, create_file_name ) == NET_ERROR)
		{
			if (pFtp->PutFile(create_directory, create_file_name ) == NET_ERROR)
			{
				logs_add( "ftp upload failed: %s %s", create_directory.GetString(), create_file_name.GetString() );	
				Sleep( 5 );
				continue;
			}
		}

		upload++;

		m_cur_pos = upload;
		m_end_pos = total;

		Sleep( 5 );
	}
	
	pFtp->SetCurrentDirectory(strFirstDir); 

	std::string file_name = rpatcher_settings::patch_file_list_client;
	std::string full_path = rpatcher_global::output_path;
	full_path += file_name;

	if (pFtp->PutFile(full_path.c_str(), file_name.c_str() ) == NET_ERROR)
	{
		if (pFtp->PutFile(full_path.c_str(), file_name.c_str() ) == NET_ERROR)
		{
			logs_add( "ftp upload patch list failed: %s %s", full_path.c_str(), file_name.c_str() );	
		}
	}

	pFtp->SetCurrentDirectory(strFirstDir); 

	pFtp->DisConnect();
	SAFE_DELETE( pFtp );

	m_map_upload_list.clear();
	save_upload_list();

	time_run = ::GetTickCount() - time_start;

	if ( time_run < 1000 )
		logs_add( "ftp upload files done, total:%d time:%u msec", upload, time_run );
	else
		logs_add( "ftp upload files done, total:%d time:%u sec", upload, time_run/1000 );
}

void CPatchBuilderDlg::update_upload_list()
{
	logs_add( "updating upload list" );

	m_map_upload_list.clear();

	read_upload_list();

	PATCH_BUILDER_FILE_DATA_MAP_ITER it = m_map_update_list.begin();
	PATCH_BUILDER_FILE_DATA_MAP_ITER end = m_map_update_list.end();
	for ( ; it!=end; ++it )
	{
		PATCH_BUILDER_FILE_DATA& file_data = (*it).second;
		m_map_upload_list.insert ( std::make_pair(file_data.file_path_full,file_data) );
	}
	
	save_upload_list();

	logs_add( "upload list updated" );
}

void CPatchBuilderDlg::read_upload_list()
{
	std::string file_path = rpatcher_global::app_path;
	file_path += "\\";
	file_path += rpatcher_settings::patch_file_upload_list;

	CSerialFile	serial_file;
	if ( serial_file.OpenFile ( FOT_READ, file_path.c_str() ) ) 
	{
		char file_type[FILETYPESIZE] = "";
		DWORD version = 0;
		serial_file.GetFileType( file_type, FILETYPESIZE, version );

		if ( version == PATCH_FILE_LIST_VERSION )
		{
			DWORD num = 0;
			serial_file >> num;

			for ( DWORD i=0; i<num; ++i )
			{
				PATCH_BUILDER_FILE_DATA file_data;
				serial_file >> file_data.file_name;
				serial_file >> file_data.file_path_full;
				serial_file >> file_data.file_path_sub;
				serial_file >> file_data.file_hash;

				m_map_upload_list.insert ( std::make_pair(file_data.file_path_full,file_data) );
			}
		}
		else
		{
			logs_add( "file list invalid version:%d %x", version, version );
		}
	}
}

void CPatchBuilderDlg::save_upload_list()
{
	std::string file_path = rpatcher_global::app_path;
	file_path += "\\";
	file_path += rpatcher_settings::patch_file_upload_list;

	CSerialFile	serial_file;
	std::string file_type = "[file_list_server]";
	serial_file.SetFileType ( file_type.c_str(), PATCH_FILE_LIST_VERSION );

	if ( serial_file.OpenFile ( FOT_WRITE, file_path.c_str() ) )
	{
		serial_file << (DWORD)m_map_upload_list.size();

		PATCH_BUILDER_FILE_DATA_MAP_ITER it = m_map_upload_list.begin();
		PATCH_BUILDER_FILE_DATA_MAP_ITER end = m_map_upload_list.end();

		for ( ; it!=end; ++it )
		{
			PATCH_BUILDER_FILE_DATA& file_data = (*it).second;
			serial_file << file_data.file_name;
			serial_file << file_data.file_path_full;
			serial_file << file_data.file_path_sub;
			serial_file << file_data.file_hash;
		}
	}
}