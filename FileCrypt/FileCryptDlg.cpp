
// FileCryptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./FileCrypt.h"
#include "./FileCryptDlg.h"
#include "./FileCryptGlobal.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Common/RCryptByte.h"

#include "../Lib_Helper/EtcFunction.h"


#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileCryptDlg dialog


CFileCryptDlg::CFileCryptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileCryptDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strInputFolder = "";
	m_strOutputFolder = "";
}

void CFileCryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, m_cEdit);
}

BEGIN_MESSAGE_MAP(CFileCryptDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_CMB_FILETYPE, OnCbnSelchangeCmbFiletype)
	ON_BN_CLICKED(IDC_BTN_INPUT, OnBnClickedBtnInput)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, OnBnClickedBtnOutput)
	ON_BN_CLICKED(IDC_BTN_INPLACE, OnBnClickedBtnInplace)
	ON_BN_CLICKED(IDC_BTN_ENCRYPT, OnBnClickedBtnEncrypt)
	ON_BN_CLICKED(IDC_BTN_DECRYPT, OnBnClickedBtnDecrypt)
END_MESSAGE_MAP()


// CFileCryptDlg message handlers

BOOL CFileCryptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);
	RPATH::SetPath( szPath );
	RLOG::Init();
	CDebugSet::OneTimeSceneInit( RPATH::getAppPath(), FALSE );

	SetWin_Combo_Init( this, IDC_CMB_FILETYPE, FILE_CRYPT_GLOBAL::file_crypt_type, EMCRYPT_FILETYPE_SIZE );
	SetWin_Combo_Init( this, IDC_CMB_BYPASSHEADER, FILE_CRYPT_GLOBAL::file_crypt_bypass_type, EMCRYPT_BYPASSHEADER_SIZE );
	SetWin_Combo_Init( this, IDC_CMB_CRYPTTYPE, FILE_CRYPT_GLOBAL::byte_crypt_type, EMRCRYPT_BYTE_TYPE_SIZE );
	SetWin_Combo_Del( this, IDC_CMB_CRYPTTYPE, FILE_CRYPT_GLOBAL::byte_crypt_type[EMRCRYPT_BYTE_TYPE_RAN2].c_str() );

#ifndef _DEBUG
	SetWin_Combo_Del( this, IDC_CMB_FILETYPE, FILE_CRYPT_GLOBAL::file_crypt_type[EMCRYPT_FILETYPE_STREAMFILE].c_str() );
#endif

	SettingChange();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileCryptDlg::OnPaint()
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
HCURSOR CFileCryptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFileCryptDlg::InfoAdd( const char *szFormat, ... )
{
	va_list argList;
	char szBuffer[256] = "";
	va_start(argList, szFormat);
	StringCbVPrintf( szBuffer, 256, szFormat, argList );
	va_end(argList);

	int nlen = m_cEdit.GetWindowTextLength();

	if ( m_cEdit.GetWindowTextLength() >= (int)m_cEdit.GetLimitText() )
	{
		m_cEdit.SetSel( 0 , m_cEdit.GetWindowTextLength() );
	}else{
		m_cEdit.SetSel( nlen, nlen );
	}

	m_cEdit.ReplaceSel( szBuffer );	

	nlen = m_cEdit.GetWindowTextLength();
	m_cEdit.SetSel( nlen, nlen );
	m_cEdit.ReplaceSel( "\r\n" );
}

void CFileCryptDlg::InfoReset()
{
	m_cEdit.SetWindowTextA( "" );
}

void CFileCryptDlg::SettingChange()
{
	EMCRYPT_FILETYPE emFileType = static_cast<EMCRYPT_FILETYPE>( GetWin_Combo_Sel( this, IDC_CMB_FILETYPE) );
	BOOL bStreamFile = ( emFileType == EMCRYPT_FILETYPE_STREAMFILE );
	BOOL bStreamFileR2 = ( emFileType == EMCRYPT_FILETYPE_STREAMFILE_R2 );
	BOOL bStreamFileOrig = ( emFileType == EMCRYPT_FILETYPE_STREAMFILE_ORIG );
	BOOL bStreamAnimation = ( emFileType == EMCRYPT_FILETYPE_ANIMFILE );
	BOOL bEGP = ( emFileType == EMCRYPT_FILETYPE_EGP_TO_NEW || emFileType == EMCRYPT_FILETYPE_R2_EGP_TO_NEW );
	BOOL bCPS = ( emFileType == EMCRYPT_FILETYPE_CPS_TO_NEW || emFileType == EMCRYPT_FILETYPE_R2_CPS_TO_NEW );

	BOOL bDecryptOnly = bStreamFileR2 || bStreamFileOrig;
	BOOL bEncryptOnly = bStreamAnimation || bEGP || bCPS;

	SetWin_Enable( this, IDC_STATIC_ENCODETYPE, bStreamFile );
	SetWin_Enable( this, IDC_STATIC_BYPASSHEADER, bStreamFile || bStreamFileOrig );
	SetWin_Enable( this, IDC_CMB_BYPASSHEADER, bStreamFile || bStreamFileOrig );
	SetWin_Enable( this, IDC_CMB_CRYPTTYPE, bStreamFile );
	SetWin_Enable( this, IDC_CHECK_CONVERT_ENCRYPT, bStreamFileR2 );

	SetWin_Text( this, IDC_EDIT_INPUT, m_strInputFolder.c_str() );
	SetWin_Text( this, IDC_EDIT_OUTPUT, m_strOutputFolder.c_str() );

	SetWin_Enable( this, IDC_BTN_INPLACE, BOOL( m_strInputFolder.size() > 0 ) );
	SetWin_Enable( this, IDC_BTN_ENCRYPT, BOOL( m_strInputFolder.size() > 0 && m_strOutputFolder.size() > 0 && !bDecryptOnly ) );
	SetWin_Enable( this, IDC_BTN_DECRYPT, BOOL( m_strInputFolder.size() > 0 && m_strOutputFolder.size() > 0 && !bEncryptOnly ) );
}

void CFileCryptDlg::OnCbnSelchangeCmbFiletype()
{
	SettingChange();
}

void CFileCryptDlg::OnBnClickedBtnInput()
{
	BROWSEINFO biInput;
	ZeroMemory(&biInput, sizeof(BROWSEINFO));
	biInput.hwndOwner = m_hWnd;
	biInput.lpszTitle = "Select Input Folder";
	biInput.pidlRoot = NULL;
	biInput.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_NONEWFOLDERBUTTON | BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pidlFolderInput = SHBrowseForFolder(&biInput);
	if (pidlFolderInput == NULL) return;
	TCHAR szPathInput[_MAX_PATH] = {0};
	SHGetPathFromIDList(pidlFolderInput, szPathInput);
	m_strInputFolder = szPathInput;

	if ( m_strInputFolder.size() >= 250 )
	{
		InfoAdd( "Input path length too long!" );
		m_strInputFolder = "";
	}

	SettingChange();
}

void CFileCryptDlg::OnBnClickedBtnOutput()
{
	BROWSEINFO biOutput;
	ZeroMemory(&biOutput, sizeof(BROWSEINFO));
	biOutput.hwndOwner = m_hWnd;
	biOutput.lpszTitle = "Select Output Folder"; 
	biOutput.pidlRoot = NULL;
	biOutput.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_NONEWFOLDERBUTTON | BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pidlFolderOutput = SHBrowseForFolder(&biOutput);
	if (pidlFolderOutput == NULL) return;
	TCHAR szPathOutput[_MAX_PATH] = {0};
	SHGetPathFromIDList(pidlFolderOutput, szPathOutput);
	m_strOutputFolder = szPathOutput;

	if ( m_strOutputFolder.size() >= 220 )
	{
		InfoAdd( "Output path length too long!" );
		m_strOutputFolder = "";
	}

	SettingChange();
}

void CFileCryptDlg::OnBnClickedBtnInplace()
{
	m_strOutputFolder = m_strInputFolder.c_str();
	SettingChange();
}

void CFileCryptDlg::OnBnClickedBtnEncrypt()
{
	DoCrypt( TRUE );
}

void CFileCryptDlg::OnBnClickedBtnDecrypt()
{
	DoCrypt( FALSE );
}
