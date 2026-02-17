
// LauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./Launcher.h"
#include "./LauncherDlg.h"

#include "./RLauncher.h"
#include "./LauncherText.h"
#include "../Lib_Network/s_CHttpPatch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLauncherDlg dialog


CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLauncherDlg::IDD, pParent)
	, m_bmouse_down(FALSE)
	, m_bforce_terminate(FALSE)
	, m_phttp_patch(NULL)
	, m_ppatch_thread(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BROWSER, m_ctrl_brower_main);
	DDX_Control(pDX, IDC_BUTTON_START, m_ctrl_button_start);
	DDX_Control(pDX, IDC_LIST_LOGS, m_ctrl_list_logs);
	DDX_Control(pDX, IDC_PROGRESS_ACTIVE, m_ctrl_progress_active);
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_ctrl_progress_total);
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CLauncherDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDCANCEL, &CLauncherDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLauncherDlg message handlers

BOOL CLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//attempt to change icon
	const HANDLE hIconS = ::LoadImage( ::GetModuleHandle(0), "game.ico", IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
	if (hIconS)
		::SendMessage(GetSafeHwnd(), WM_SETICON, ICON_SMALL, (LPARAM)hIconS);

	const HANDLE hIconB = ::LoadImage( ::GetModuleHandle(0), "game.ico", IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE );
	if (hIconB)
		::SendMessage(GetSafeHwnd(), WM_SETICON, ICON_BIG, (LPARAM)hIconB);


	setup_app_path();
	load_configuration();

	set_dialog_text();
	
	login_block();
	
	SetTimer( TIMER_BROWSER, LAUNCHER_BROWSER_START_TIME, NULL );

	m_phttp_patch = new CHttpPatch;
	if( NULL == m_phttp_patch ) EndDialog(IDOK);
	
	m_phttp_patch->SetForceTerminateToggle ( &m_bforce_terminate );

	ULONGLONG* pCurPos = NULL;
	ULONGLONG* pCurEnd = NULL;
	rlauncher_log_control::get_process_cur_position_addr ( &pCurPos, &pCurEnd );	
	m_phttp_patch->SetProgressValue ( pCurPos, pCurEnd );

	if ( rlauncher_setting::patch_function )
	{
		static S_PATCH_THREAD_PARAM sParam;
		sParam.pHttpPatch = m_phttp_patch;

		thread_begin( LAUNCHER_THREAD_TYPE_PATCH, m_ppatch_thread, &sParam );
		SetTimer ( TIMER_PATCH_THREAD, LAUNCHER_PATCH_THREAD_START_TIME, NULL );
	}
	else
	{
		//patch function not enabled, allow game start right away.
		logs_add( ID2LAUNCHERTEXT("IDS_MESSAGE", 0 ) );
		login_enable();
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLauncherDlg::PostNcDestroy()
{
	m_bforce_terminate = TRUE;

	thread_end( m_ppatch_thread );

	SAFE_DELETE( m_phttp_patch );

	CDialog::PostNcDestroy();
}

BOOL CLauncherDlg::PreTranslateMessage(MSG* pMsg)
{
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		{
			switch ( pMsg->wParam )
			{
			case VK_RETURN:
			case VK_ESCAPE:
				return TRUE;
			}
		}break;

	case WM_SYSKEYDOWN:
		{
			if ( pMsg->wParam == VK_F4 )
			{
				OnBnClickedCancel ();
				return TRUE;
			}
		}break;

	
	case WM_LISTADDSTRING:
		{
			if( pMsg->wParam == 0)
			{
				logs_add( (PCHAR)pMsg->lParam );
				delete [] (PCHAR)pMsg->lParam;
			}
			else
			{
				logs_add( (LPCTSTR) pMsg->wParam );
			}

			return TRUE;
		}break;

	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CLauncherDlg::OnTimer(UINT nIDEvent)
{
	switch ( nIDEvent )
	{

	case TIMER_BROWSER:
		{
			m_ctrl_brower_main.Navigate( rlauncher_setting::url_news.c_str(), NULL, NULL, NULL, NULL );
			KillTimer( TIMER_BROWSER );
		}break;

	case TIMER_PATCH_THREAD:
		{
			update_progress_bar();

			if( m_ppatch_thread )
			{
				if( m_ppatch_thread->IsForceTerminate() )
				{
					KillTimer ( TIMER_PATCH_THREAD );
					login_enable();
					break;
				}

				if( !m_ppatch_thread->IsRunning() )
				{
					KillTimer ( TIMER_PATCH_THREAD );
					if( m_ppatch_thread->IsFail() )
					{
						login_enable();
					}
					else // Succeed
					{
						login_enable();
						logs_add( ID2LAUNCHERTEXT("IDS_MESSAGE", 0 ) );
					}
				}
			}
			else
			{
				KillTimer ( TIMER_PATCH_THREAD );
				login_enable();
			}
		}break;

	

	}

	CDialog::OnTimer(nIDEvent);
}

void CLauncherDlg::OnLButtonDown(UINT nFlags, CPoint point)
{    
	m_mouse_click_pos = point;
	m_bmouse_down = TRUE;
	SetCapture();

	CDialog::OnLButtonDown(nFlags, point);
}

void CLauncherDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( !m_bmouse_down ) return;

	RECT rect;
	GetWindowRect ( &rect );

	CPoint move_pos;
	move_pos.x = m_mouse_click_pos.x - point.x;
	move_pos.y = m_mouse_click_pos.y - point.y;

	rect.left -= move_pos.x;
	rect.top -= move_pos.y;
	rect.right -= move_pos.x;
	rect.bottom -= move_pos.y;

	MoveWindow ( &rect );

	CDialog::OnMouseMove(nFlags, point);
}

void CLauncherDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ( m_bmouse_down )
	{
		ReleaseCapture ();
		m_bmouse_down = FALSE;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CLauncherDlg::OnBnClickedButtonStart()
{
	if ( rlauncher_setting::game_minimize_other )
	{
		HWND hTrayWnd = NULL;
		hTrayWnd = ::FindWindow("Shell_TrayWnd", NULL);

		if ( hTrayWnd )
			::SendMessage( hTrayWnd, WM_COMMAND, 419, 0 );		
	}
	
	CString parameter;
	parameter.Format ( "/%s", rlauncher_setting::game_parameter.c_str() );	

	CString target;
	target.Format ( "\"%s\\%s\"", rlauncher_global::app_path.c_str(), rlauncher_setting::game_filename.c_str() );	

	ShellExecute( m_hWnd , "open", target.GetString(), parameter.GetString(), NULL, SW_SHOW);

	OnOK(); 
}

void CLauncherDlg::OnBnClickedCancel()
{
	m_bforce_terminate = TRUE;
	thread_end( m_ppatch_thread );

	CDialog::OnCancel();
}
