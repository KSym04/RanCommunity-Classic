
// ServerSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./ServerSetting.h"
#include "./ServerSettingDlg.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSRParam.h"

#include "../Lib_Helper/EtcFunction.h"

#include "../Lib_Network/s_CCfg.h"

#include "./ServerSettingTab.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerSettingDlg dialog


CServerSettingDlg::CServerSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerSettingDlg::IDD, pParent)
	, m_pTab(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void CServerSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CServerSettingDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BTN_LOAD, &CServerSettingDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CServerSettingDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CServerSettingDlg::OnBnClickedBtnImport)
END_MESSAGE_MAP()


// CServerSettingDlg message handlers

BOOL CServerSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath, TRUE );
	RLOG::Init();

	RPARAM::Init();

	CRect SheetRect;
	CWnd *pWnd;
	pWnd = GetDlgItem ( IDC_STATIC_TOOL );
	pWnd->GetWindowRect ( SheetRect );
	ScreenToClient ( &SheetRect );
	LOGFONT logfont = {0};

	CFont* pFont = GetFont();
	if ( pFont ) pFont->GetLogFont( &logfont );

	SAFE_DELETE ( m_pTab );
	m_pTab = new CServerSettingTab (SheetRect, logfont, this );
	m_pTab->Create( this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0 );
	m_pTab->ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_pTab->InitPages();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerSettingDlg::OnPaint()
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
HCURSOR CServerSettingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerSettingDlg::PostNcDestroy()
{
	SAFE_DELETE ( m_pTab );

	CDialog::PostNcDestroy();
}

void CServerSettingDlg::OnBnClickedBtnLoad()
{
//#ifndef _DEBUG
//	CDebugSet::MsgBox( GetSafeHwnd(), "active build type does not allow editing, contact developer" );
//#else
	CString szFilter = "scfg|*.scfg|";
	CFileDialog dlg ( TRUE,".scfg", RPATH::getServerConfigPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = RPATH::getServerConfigPath();
	if ( dlg.DoModal() != IDOK )	return;

	CCfg::ReleaseInstance();

	std::string filename = dlg.GetFileName().GetString();
	BOOL loaded = CCfg::GetInstance()->Load_File_Tool( filename.c_str() );
	if ( !loaded )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Server CFG load failed :%s", dlg.GetPathName().GetString() );
		return;
	}

	SetWin_Text( this, IDC_EDIT_FILENAME, dlg.GetPathName().GetString() );

	if ( m_pTab )
		m_pTab->ShowData();

//#endif
}

void CServerSettingDlg::OnBnClickedBtnSave()
{
//#ifndef _DEBUG
//	CDebugSet::MsgBox( GetSafeHwnd(), "active build type does not allow editing, contact developer" );
//#else
	CString szFilter = "scfg|*.scfg|";
	CString strFile = RPATH::getServerConfigPath();
	strFile += CCfg::GetInstance()->GetFileName();
	CFileDialog dlg ( false,".scfg", strFile.GetString(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = RPATH::getServerConfigPath();
	if ( dlg.DoModal() == IDOK )
	{
		std::string filename = dlg.GetFileName().GetString();
		BOOL saved = CCfg::GetInstance()->Save_File( filename.c_str() );
		if ( !saved )
			CDebugSet::MsgBox( GetSafeHwnd(), "Server CFG save failed :%s", dlg.GetPathName().GetString() );


		SetWin_Text( this, IDC_EDIT_FILENAME, dlg.GetPathName().GetString() );
	}
//#endif
}

void CServerSettingDlg::OnBnClickedBtnImport()
{
//#ifndef _DEBUG
//	CDebugSet::MsgBox( GetSafeHwnd(), "active build type does not allow editing, contact developer" );
//#else
	CString szFilter = "(*.cfg)|*.cfg|(*.ini)|*.ini|";
	CFileDialog dlg ( TRUE,".cfg", RPATH::getServerConfigPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = RPATH::getServerConfigPath();
	if ( dlg.DoModal() != IDOK )	return;

	CCfg::ReleaseInstance();

	std::string filepath = dlg.GetPathName().GetString();
	int ret = CCfg::GetInstance()->Import( filepath.c_str() );
	if ( ret != 0 )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Import failed :%s", filepath.c_str() );
		return;
	}

	boost::filesystem::path p = filepath;
	p.replace_extension( ".scfg" );
	StringCchCopy(  CCfg::GetInstance()->m_szFileName, MAX_PATH, p.filename().string().c_str() );

	if ( m_pTab )
		m_pTab->ShowData();
//#endif
}
