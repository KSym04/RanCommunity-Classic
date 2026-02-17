// PageEGP.cpp : implementation file
//

#include "stdafx.h"
#include "./EffectTool.h"
#include "./PageMain.h"
#include "./SheetWithTab.h"
#include "../Lib_Helper/EtcFunction.h"
#include "./EffectToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPageEGP dialog

IMPLEMENT_DYNAMIC(CPageMain, CPropertyPage)

CPageMain::CPageMain(LOGFONT logfont)
	: CPropertyPage(CPageMain::IDD)
	, m_pFont ( NULL )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CPageMain::~CPageMain()
{
	SAFE_DELETE ( m_pFont );
}

void CPageMain::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFFECT_TREE, m_ctrlTree);
}


BEGIN_MESSAGE_MAP(CPageMain, CPropertyPage)
	
END_MESSAGE_MAP()


// CPageEGP message handlers
BOOL CPageMain::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pSheetTab->ChangeDialogFont( this, m_pFont, CDF_TOPLEFT );
	m_bDlgInit = TRUE;


	return TRUE;
}

void CPageMain::DataReset()
{
	m_ctrlTree.DeleteAllItems ();

}

void CPageMain::DataShow()
{
	EFF_PROPGROUP* pData = CEffectToolView::GetView()->m_pPropGroup;
	if ( !pData )	return;


	m_ctrlTree.DeleteAllItems ();
	m_ctrlTree.SetRedraw( FALSE );

	if ( pData->m_pPropRoot )
	{
		HTREEITEM h = m_ctrlTree.InsertItem ( "Root" );
		InsertEffect( h, pData->m_pPropRoot );
	}

	m_ctrlTree.SetRedraw( TRUE );
}

void CPageMain::InsertEffect( HTREEITEM h, EFF_PROPERTY* pProp )
{
	HTREEITEM hret = m_ctrlTree.InsertItem( pProp->GetName(), h );
	m_ctrlTree.SetItemData ( hret, (DWORD_PTR)pProp );

	if ( pProp->m_pChild )
		InsertEffect( hret, pProp->m_pChild );

	if ( pProp->m_pSibling )
		InsertEffect( h, pProp->m_pSibling );
}
