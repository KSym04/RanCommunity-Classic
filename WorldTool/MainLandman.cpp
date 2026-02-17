// MainLandman.cpp : implementation file
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./MainLandman.h"
#include "./MainTab.h"
#include "./LandmanTab.h"
#include "./WorldToolView.h"
#include "../Lib_Helper/EtcFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainLandman dialog

IMPLEMENT_DYNAMIC(CMainLandman, CPropertyPage)

CMainLandman::CMainLandman(LOGFONT logfont)
	: CPropertyPage(CMainLandman::IDD)
	, m_pFont(NULL)
	, m_pToolTab(NULL)
	, m_pData(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CMainLandman::~CMainLandman()
{
	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pToolTab);
}

void CMainLandman::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainLandman, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_WLD_LOAD, &CMainLandman::OnBnClickedBtnWldLoad)
	ON_BN_CLICKED(IDC_BTN_WLD_SAVE, &CMainLandman::OnBnClickedBtnWldSave)
END_MESSAGE_MAP()


// CMainLandman message handlers
BOOL CMainLandman::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CMainTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	CRect SheetRect;
	CWnd *pWnd;
	pWnd = GetDlgItem ( IDC_STATIC_TOOL_PAGE_WLD );
	pWnd->GetWindowRect ( SheetRect );
	ScreenToClient ( &SheetRect );
	LOGFONT logfont = {0};

	CFont* pFont = GetFont();
	if ( pFont ) pFont->GetLogFont( &logfont );

	SAFE_DELETE ( m_pToolTab );
	m_pToolTab = new CLandmanTab (SheetRect, logfont, this );
	m_pToolTab->Create( this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0 );
	m_pToolTab->ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_pToolTab->InitPages();


	return TRUE;  
}

void CMainLandman::ResetData()
{
	if ( m_pToolTab )
		m_pToolTab->ResetData();

	m_pData = NULL;
}

void CMainLandman::OnBnClickedBtnWldLoad()
{
	CString szFilter = "WLD|*.wld|";
	CFileDialog dlg ( TRUE,".WLD", DxLandMan::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = DxLandMan::GetPath ();
	if ( dlg.DoModal() != IDOK )	return;

	std::string strFile = dlg.GetFileName().GetString();

	BOOL bOK = CWorldToolView::GetView()->WLDLoad( strFile.c_str() );
	if ( !bOK )
		CDebugSet::MsgBox( GetSafeHwnd(), "WLD load failed :%s", dlg.GetPathName().GetString() );

	SetWin_Text( this, IDC_EDT_WLD_FILENAME, dlg.GetPathName().GetString() );

	m_pData = CWorldToolView::GetView()->m_pLand;

	if ( m_pToolTab )
		m_pToolTab->SetData( m_pData );
}

void CMainLandman::OnBnClickedBtnWldSave()
{
	if ( !m_pData )	return;

	CString szFilter = "WLD|*.wld|";
	std::string strfilename = DxLandMan::GetPath ();
	strfilename += CWorldToolView::GetView()->m_strFileName;
	CFileDialog dlg ( false,".WLD", strfilename.c_str(),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = DxLandMan::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		BOOL bOk = CWorldToolView::GetView()->WLDSave( dlg.GetFileName().GetString() );
		if ( bOk )
			CDebugSet::MsgBox( "WLD save Done: %s", dlg.GetPathName().GetString() );
		else
			CDebugSet::MsgBox( "WLD save failed: %s", dlg.GetPathName().GetString() );

		SetWin_Text( this, IDC_EDT_WLD_FILENAME, dlg.GetPathName().GetString() );

		m_pData = CWorldToolView::GetView()->m_pLand;
		
		if ( m_pToolTab )
			m_pToolTab->SetData( m_pData );
	}
}
