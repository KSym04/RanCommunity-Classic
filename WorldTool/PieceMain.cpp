// PieceMain.cpp : implementation file
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./PieceMain.h"
#include "./PieceTab.h"
#include "./WorldToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPieceMain dialog

IMPLEMENT_DYNAMIC(CPieceMain, CPropertyPage)

CPieceMain::CPieceMain(LOGFONT logfont)
	: CPropertyPage(CPieceMain::IDD)
	, m_pFont(NULL)
	, m_pData(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CPieceMain::~CPieceMain()
{
	SAFE_DELETE( m_pFont );
}

void CPieceMain::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPieceMain, CPropertyPage)
END_MESSAGE_MAP()


// CPieceMain message handlers
BOOL CPieceMain::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CPieceTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	return TRUE;  
}

void CPieceMain::SetData( DxPieceEdit* pData )
{
	m_pData = pData;

}

void CPieceMain::ResetData()
{
	m_pData = NULL;
}