// PageEdit3.cpp : implementation file
//

#include "stdafx.h"
#include "EditorMobNpc.h"
#include "PageEdit3.h"
#include "afxdialogex.h"

#include "./SheetWithTab.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Engine/G-Logic/GLOGIC.h"

// CPageEdit3 dialog

IMPLEMENT_DYNAMIC(CPageEdit3, CPropertyPage)

CPageEdit3::CPageEdit3(LOGFONT logfont)
	: CPropertyPage(CPageEdit3::IDD)
	, m_pFont ( NULL )
	, m_CallPage(0)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);

	m_pCrow = NULL;
	m_pDummyCrow = new SCROWDATA;
}

CPageEdit3::~CPageEdit3()
{
	SAFE_DELETE ( m_pFont );
	SAFE_DELETE ( m_pDummyCrow );
}

void CPageEdit3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageEdit3, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_BACK2, &CPageEdit3::OnBnClickedButtonPageBack2)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_CANCEL, &CPageEdit3::OnBnClickedButtonPageCancel)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_SAVE, &CPageEdit3::OnBnClickedButtonPageSave)
	ON_BN_CLICKED(IDC_BUTTON_WARDEN_ITEM_GEN_1, &CPageEdit3::OnBnClickedButtonWardenItemGen1)
	ON_BN_CLICKED(IDC_BUTTON_WARDEN_ITEM_GEN_2, &CPageEdit3::OnBnClickedButtonWardenItemGen2)
	ON_BN_CLICKED(IDC_BUTTON_WARDEN_ITEM_GEN_3, &CPageEdit3::OnBnClickedButtonWardenItemGen3)
	ON_BN_CLICKED(IDC_BUTTON_WARDEN_ITEM_GEN_4, &CPageEdit3::OnBnClickedButtonWardenItemGen4)
END_MESSAGE_MAP()


// CPageEdit3 message handlers
BOOL CPageEdit3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pSheetTab->ChangeDialogFont( this, m_pFont, CDF_TOPLEFT );
	m_bDlgInit = TRUE;


	DataDefault();
	DataShow();		

	return TRUE;  
}

BOOL CPageEdit3::CrowSet ( PCROWDATA pCROW )
{
	if ( m_pCrow )	return FALSE;
	m_pCrow = pCROW;
	m_pDummyCrow->Assign( *m_pCrow );

	if ( m_bDlgInit ) 
	{
		DataDefault();
		DataShow();	
	}

	return TRUE;
}

void CPageEdit3::CrowClear()
{
	m_pCrow = NULL;
}

void CPageEdit3::OnBnClickedButtonPageBack2()
{
	if ( !DataSave() )	return;

	SCROWDATA sCROW;
	sCROW.Assign( *m_pDummyCrow );
	GLCrowDataMan::GetInstance().InsertCrowData ( m_pDummyCrow->m_sBasic.sNativeID, &sCROW );

	m_pSheetTab->ActivePageEdit2( m_CallPage, m_pCrow );
	m_pCrow = NULL;
}

void CPageEdit3::OnBnClickedButtonPageCancel()
{
	m_pCrow = NULL;
	if ( m_pSheetTab )
		m_pSheetTab->ActivePageTree( FALSE );
}

void CPageEdit3::OnBnClickedButtonPageSave()
{
	if ( !DataSave() )	return;

	SCROWDATA sCROW;
	sCROW.Assign( *m_pDummyCrow );
	GLCrowDataMan::GetInstance().InsertCrowData ( m_pDummyCrow->m_sBasic.sNativeID, &sCROW );

	m_pCrow = NULL;
	m_pSheetTab->ActivePageTree( FALSE );
}



void CPageEdit3::OnBnClickedButtonWardenItemGen1()
{
	CString szFilter = "Item Gen File (*.genitem)|*.GENITEM|";
	CFileDialog dlg( TRUE, ".genitem", GLOGIC::GetServerPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetServerPath ();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_WARDEN_GEN_ITEM_1, dlg.GetFileName().GetString() );
}

void CPageEdit3::OnBnClickedButtonWardenItemGen2()
{
	CString szFilter = "Item Gen File (*.genitem)|*.GENITEM|";
	CFileDialog dlg( TRUE, ".genitem", GLOGIC::GetServerPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetServerPath ();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_WARDEN_GEN_ITEM_2, dlg.GetFileName().GetString() );
}

void CPageEdit3::OnBnClickedButtonWardenItemGen3()
{
	CString szFilter = "Item Gen File (*.genitem)|*.GENITEM|";
	CFileDialog dlg( TRUE, ".genitem", GLOGIC::GetServerPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetServerPath ();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_WARDEN_GEN_ITEM_3, dlg.GetFileName().GetString() );
}

void CPageEdit3::OnBnClickedButtonWardenItemGen4()
{
	CString szFilter = "Item Gen File (*.genitem)|*.GENITEM|";
	CFileDialog dlg( TRUE, ".genitem", GLOGIC::GetServerPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetServerPath ();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_WARDEN_GEN_ITEM_4, dlg.GetFileName().GetString() );
}
