
// PatchBuilderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./PatchBuilder.h"
#include "./PatchBuilderDlg.h"
#include "./RPatcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPatchBuilderDlg dialog


CPatchBuilderDlg::CPatchBuilderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchBuilderDlg::IDD, pParent)
	, m_cur_pos(0)
	, m_end_pos(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	
}

void CPatchBuilderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOGS, m_ctrl_list_logs);
	DDX_Control(pDX, IDC_PROGRESS_ACTIVE, m_ctrl_progress_total);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_ctrl_progress_static);
}

BEGIN_MESSAGE_MAP(CPatchBuilderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CPatchBuilderDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CPatchBuilderDlg::OnBnClickedButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_BUILD, &CPatchBuilderDlg::OnBnClickedButtonBuild)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD_PATCH, &CPatchBuilderDlg::OnBnClickedButtonUploadPatch)
END_MESSAGE_MAP()


// CPatchBuilderDlg message handlers

BOOL CPatchBuilderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
	setup_app_path();
	show_path_info();
	
	SetTimer( PATCH_BUILDER_TIMER_PROGRESS, PATCH_BUILDER_PROGRESS_UPDATE_TIME, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPatchBuilderDlg::OnPaint()
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
HCURSOR CPatchBuilderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPatchBuilderDlg::OnTimer(UINT nIDEvent)
{
	switch ( nIDEvent )
	{
	case PATCH_BUILDER_TIMER_PROGRESS:
		{
			update_progress();
		}break;

	case PATCH_BUILDER_TIMER_SCAN:
		{
			KillTimer(PATCH_BUILDER_TIMER_SCAN);
			action_scan_start();
		}break;

	case PATCH_BUILDER_TIMER_BUILD:
		{
			KillTimer(PATCH_BUILDER_TIMER_BUILD);
			action_build_start();
		}break;

	case PATCH_BUILDER_TIMER_UPLOAD:
		{
			KillTimer(PATCH_BUILDER_TIMER_UPLOAD);
			action_upload_start();
		}break;
	}


	CDialog::OnTimer(nIDEvent);
}

BOOL CPatchBuilderDlg::PreTranslateMessage(MSG* pMsg)
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
				return TRUE;
			}
		}break;


	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPatchBuilderDlg::OnBnClickedCancel()
{
	KillTimer(PATCH_BUILDER_TIMER_PROGRESS);

	CDialog::OnCancel();
}

void CPatchBuilderDlg::OnBnClickedButtonScan()
{
	SetTimer( PATCH_BUILDER_TIMER_SCAN, 1000, NULL);
	set_button(FALSE);
}

void CPatchBuilderDlg::OnBnClickedButtonBuild()
{
	SetTimer( PATCH_BUILDER_TIMER_BUILD, 1000, NULL);
	set_button(FALSE);
}

void CPatchBuilderDlg::OnBnClickedButtonUploadPatch()
{
	SetTimer( PATCH_BUILDER_TIMER_UPLOAD, 1000, NULL);
	set_button(FALSE);
}
