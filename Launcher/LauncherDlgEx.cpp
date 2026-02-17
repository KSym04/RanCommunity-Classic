
// LauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./Launcher.h"
#include "./LauncherDlg.h"

#include "./RLauncher.h"
#include "./LauncherText.h"
#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Common/SUBPATH.h"
#include "../Lib_Network/s_CHttpPatch.h"
#include "../Lib_Engine/Common/SerialFile.h"
#include "./AutoPatchThread.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CLauncherDlg::setup_app_path()
{
	//app path
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	rlauncher_global::app_path =  RPATH::getAppPath();

	//profile path
	TCHAR profile[MAX_PATH] = {0};
	SHGetSpecialFolderPath( NULL, profile, CSIDL_PERSONAL, FALSE );	
	rlauncher_global::profile_path = profile;
}

void CLauncherDlg::load_configuration()
{
	TCHAR path[MAX_PATH] = {0};

	rlauncher_setting::load_file( rlauncher_global::app_path.c_str() );

	
	StringCchCopy( path, MAX_PATH, rlauncher_global::app_path.c_str() );
	StringCchCat( path, MAX_PATH, "\\" );

	CLauncherTextMan::GetInstance().SetPath(path);
	CLauncherTextMan::GetInstance().LoadText(rlauncher_setting::launcher_text_file.c_str(), TRUE );

	StringCchCopy( path, MAX_PATH, rlauncher_global::app_path.c_str() );
	StringCchCat( path, MAX_PATH, rlauncher_setting::patch_temp_folder.c_str() );
	rlauncher_global::temp_path = path;
}

void CLauncherDlg::set_dialog_text()
{
	SetWindowText ( ID2LAUNCHERTEXT("IDC_TITLE_STATIC" ) );
	SetDlgItemText(IDC_STATIC_TITLE, ID2LAUNCHERTEXT("IDC_TITLE_STATIC" ) );
	SetDlgItemText(IDC_BUTTON_START, ID2LAUNCHERTEXT("IDC_BUTTON_START" ) );
}

void CLauncherDlg::logs_add( const CString & _text )
{
	CString text( _text );
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

void CLauncherDlg::logs_add( UINT id )
{
	CString text;
	text.LoadString( id );

	int idx = m_ctrl_list_logs.AddString ( text );
	m_ctrl_list_logs.SetCurSel ( idx );
}

void CLauncherDlg::logs_add( const PCHAR _text )
{
	if ( !_text )	return;

	CString text( _text );
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

void CLauncherDlg::update_progress_bar()
{
	static const ULONGLONG UL100 = 100UL;
	static const ULONGLONG UL1 = 1UL;

	ULONGLONG pos = 0;
	ULONGLONG end = 0;

	int	cur_percent	= 0;
	int	all_percent = 0;	

	rlauncher_log_control::get_process_cur_position ( &pos, &end );
	end = ( !end ) ? UL1 : end;
	cur_percent = int( (pos*UL100) / end );	

	rlauncher_log_control::get_process_all_position ( &pos, &end );	
	end = ( !end )? UL1 : end;
	all_percent = int( (pos*UL100) / end);

	m_ctrl_progress_active.SetPos ( cur_percent );
	m_ctrl_progress_total.SetPos ( all_percent );
}

void CLauncherDlg::login_block()
{
	m_ctrl_progress_active.SetPos ( 0 );
	m_ctrl_progress_total.SetPos ( 0 );

	m_ctrl_button_start.EnableWindow( FALSE );
}

void CLauncherDlg::login_enable()
{
	m_ctrl_progress_active.SetPos ( 100 );
	m_ctrl_progress_total.SetPos ( 100 );

	m_ctrl_button_start.EnableWindow( TRUE );
}

void CLauncherDlg::thread_begin( LAUNCHER_THREAD_TYPE emtype, LPCLPATCHTHREAD & pThread, LPVOID pParam, const CString & strParam )
{
	if( pThread != NULL )	return;

	switch( emtype )
	{
	case LAUNCHER_THREAD_TYPE_PATCH:
		pThread = new CAutoPatchThread( pParam, AfxGetThread()->m_nThreadID );
		break;

	default:
		return;
	}

	if( pThread == NULL )
		return;

	ASSERT_VALID(pThread);
	pThread->m_pThreadParams = NULL;

	if ( !pThread->CreateThread(CREATE_SUSPENDED) )
	{
		delete pThread;
		return;
	}

	VERIFY(pThread->SetThreadPriority(THREAD_PRIORITY_IDLE));
	pThread->ResumeThread();
}

void CLauncherDlg::thread_end( LPCLPATCHTHREAD & pThread )
{
	if( pThread != NULL )
	{
		pThread->KillThread();
		pThread = NULL;
	}
}