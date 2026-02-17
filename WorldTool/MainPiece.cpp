// MainPiece.cpp : implementation file
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./MainPiece.h"
#include "./MainTab.h"
#include "./PieceTab.h"
#include "./WorldToolView.h"
#include "../Lib_Helper/EtcFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainPiece dialog

IMPLEMENT_DYNAMIC(CMainPiece, CPropertyPage)

CMainPiece::CMainPiece(LOGFONT logfont)
	: CPropertyPage(CMainPiece::IDD)
	, m_pFont(NULL)
	, m_pToolTab(NULL)
	, m_pData(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CMainPiece::~CMainPiece()
{
	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pToolTab)
}

void CMainPiece::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainPiece, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_PIE_LOAD, &CMainPiece::OnBnClickedBtnPieLoad)
	ON_BN_CLICKED(IDC_BTN_PIE_SAVE, &CMainPiece::OnBnClickedBtnPieSave)
END_MESSAGE_MAP()


// CMainPiece message handlers
BOOL CMainPiece::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CMainTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	CRect SheetRect;
	CWnd *pWnd;
	pWnd = GetDlgItem ( IDC_STATIC_TOOL_PAGE_PIE );
	pWnd->GetWindowRect ( SheetRect );
	ScreenToClient ( &SheetRect );
	LOGFONT logfont = {0};

	CFont* pFont = GetFont();
	if ( pFont ) pFont->GetLogFont( &logfont );

	SAFE_DELETE ( m_pToolTab );
	m_pToolTab = new CPieceTab (SheetRect, logfont, this );
	m_pToolTab->Create( this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0 );
	m_pToolTab->ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_pToolTab->InitPages();

	return TRUE;  
}

void CMainPiece::ResetData()
{
	if ( m_pToolTab )
		m_pToolTab->ResetData();

	m_pData = NULL;
}

void CMainPiece::OnBnClickedBtnPieLoad()
{
	CString szFilter = "Piece (*.pie,*.pie15)|*.pie;*.pie15|";
	CFileDialog dlg ( TRUE,".", DxPieceContainer::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = DxPieceContainer::GetInstance().GetPath();
	if ( dlg.DoModal() != IDOK )	return;

	std::string strFile = dlg.GetFileName().GetString();

	BOOL bOK = CWorldToolView::GetView()->PIELoad( strFile.c_str() );
	if ( !bOK )
		CDebugSet::MsgBox( GetSafeHwnd(), "PIE load failed :%s", dlg.GetPathName().GetString() );

	SetWin_Text( this, IDC_EDT_PIE_FILENAME, dlg.GetPathName().GetString() );

	m_pData = CWorldToolView::GetView()->m_pPiece;
}

void CMainPiece::OnBnClickedBtnPieSave()
{
	if ( !m_pData )	return;

	CString szFilter = "Piece (*.pie,*.pie15)|*.pie;*.pie15|";
	std::string strfilename = DxPieceContainer::GetInstance().GetPath();
	strfilename += CWorldToolView::GetView()->m_strFileName;
	CFileDialog dlg ( false,".", strfilename.c_str(),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = DxPieceContainer::GetInstance().GetPath();
	if ( dlg.DoModal() == IDOK )
	{
		BOOL bOk = CWorldToolView::GetView()->PIESave( dlg.GetFileName().GetString() );
		if ( bOk )
			CDebugSet::MsgBox( "PIE save Done: %s", dlg.GetPathName().GetString() );
		else
			CDebugSet::MsgBox( "PIE save failed: %s", dlg.GetPathName().GetString() );

		SetWin_Text( this, IDC_EDT_PIE_FILENAME, dlg.GetPathName().GetString() );
	}
}
