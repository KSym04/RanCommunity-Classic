// EditorBar.cpp : implementation file
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./EditorBar.h"
#include "./MainTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEditorBar dialog

IMPLEMENT_DYNAMIC(CEditorBar, CDialogBar)

CEditorBar::CEditorBar()
	: m_pTab( NULL )
{

}

CEditorBar::~CEditorBar()
{
	SAFE_DELETE ( m_pTab );
}

void CEditorBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditorBar, CDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
END_MESSAGE_MAP()


// CEditorBar message handlers
LONG CEditorBar::OnInitDialog ( UINT wParam, LONG lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
	}

	CRect SheetRect;
	CWnd *pWnd;
	pWnd = GetDlgItem ( IDC_STATIC_TOOL );
	pWnd->GetWindowRect ( SheetRect );
	ScreenToClient ( &SheetRect );
	LOGFONT logfont = {0};

	CFont* pFont = GetFont();
	if ( pFont ) pFont->GetLogFont( &logfont );

	m_pTab = new CMainTab ( SheetRect, logfont, this );
	m_pTab->Create( this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0 );
	m_pTab->ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_pTab->InitPages();

	return bRet;
}

