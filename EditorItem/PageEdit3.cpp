// PageEdit3.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorItem.h"
#include "./PageEdit3.h"
#include "afxdialogex.h"

#include "./SheetWithTab.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Client/G-Logic/GLStringTable.h"
#include "../Lib_Client/G-Logic/GLItem.h"
#include "../Lib_Client/G-Logic/GLItemMan.h"
#include "../Lib_Client/G-Logic/GLItemDef.h"

#include "./SelColorDlg.h"
#include "../Lib_Helper/HLibColorValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPageEdit3 dialog

IMPLEMENT_DYNAMIC(CPageEdit3, CPropertyPage)

CPageEdit3::CPageEdit3(LOGFONT logfont)
	: CPropertyPage(CPageEdit3::IDD),
	m_pFont ( NULL )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);

	m_pItem = NULL;
	m_pDummyItem = new SITEM;
}

CPageEdit3::~CPageEdit3()
{
	SAFE_DELETE ( m_pFont );
	SAFE_DELETE ( m_pDummyItem );
}

void CPageEdit3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageEdit3, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_PREV, &CPageEdit3::OnBnClickedButtonPagePrev)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_NEXT, &CPageEdit3::OnBnClickedButtonPageNext)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_CANCEL, &CPageEdit3::OnBnClickedButtonPageCancel)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_SAVE, &CPageEdit3::OnBnClickedButtonPageSave)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_DEFAULT_COLOR_1, &CPageEdit3::OnBnClickedButtonItemDefaultColor1)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_DEFAULT_COLOR_2, &CPageEdit3::OnBnClickedButtonItemDefaultColor2)
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

BOOL CPageEdit3::ItemSet ( SITEM* pItem )
{
	if ( m_pItem )	return FALSE;
	m_pItem = pItem;
	*m_pDummyItem = *m_pItem;

	if ( m_bDlgInit ) 
	{
		DataDefault();
		DataShow();		
	}

	return TRUE;
}

void CPageEdit3::ItemClear ()
{
	m_pItem = NULL;
}

void CPageEdit3::OnBnClickedButtonPagePrev()
{
	if ( !DataSave() )	return;

	CItemNode sItemNode;
	sItemNode.m_sItem = *m_pDummyItem;
	GLItemMan::GetInstance().InsertItem ( m_pDummyItem->sBasicOp.sNativeID, &sItemNode );

	m_pSheetTab->ActivePageEdit2( m_CallPage, m_pItem );
	m_pItem = NULL;
}

void CPageEdit3::OnBnClickedButtonPageNext()
{
	// TODO: Add your control notification handler code here
}

void CPageEdit3::OnBnClickedButtonPageCancel()
{
	m_pItem = NULL;

	if ( m_pSheetTab )
		m_pSheetTab->ActivePageTree( FALSE );
}

void CPageEdit3::OnBnClickedButtonPageSave()
{
	if ( !DataSave() )	return;

	CItemNode sItemNode;
	sItemNode.m_sItem = *m_pDummyItem;

	GLItemMan::GetInstance().InsertItem ( m_pDummyItem->sBasicOp.sNativeID, &sItemNode );

	m_pItem = NULL;
	m_pSheetTab->ActivePageTree( FALSE );
}



void CPageEdit3::OnBnClickedButtonItemDefaultColor1()
{
	/*item color, Juver, 2018/01/05 */
	WORD wColor = GetWin_Num_int( this, IDC_EDIT_ITEM_COLOR_1 );
	DWORD dwColor = _HLIB::color_dword_word( wColor );

	CSelColorDlg dlg( BYTE( _HLIB::color_get_int_r( dwColor) ), 
		BYTE( _HLIB::color_get_int_g( dwColor ) ), 
		BYTE( _HLIB::color_get_int_b( dwColor ) ), 
		this );
	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		SetWin_Num_int( this, IDC_EDIT_ITEM_COLOR_1, _HLIB::color_word_dword( _HLIB::color_argb_rgb(crNewColor) ) );
	}
}

void CPageEdit3::OnBnClickedButtonItemDefaultColor2()
{
	/*item color, Juver, 2018/01/05 */
	WORD wColor = GetWin_Num_int( this, IDC_EDIT_ITEM_COLOR_2 );
	DWORD dwColor = _HLIB::color_dword_word( wColor );

	CSelColorDlg dlg( BYTE( _HLIB::color_get_int_r( dwColor) ), 
		BYTE( _HLIB::color_get_int_g( dwColor ) ), 
		BYTE( _HLIB::color_get_int_b( dwColor ) ), 
		this );
	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		SetWin_Num_int( this, IDC_EDIT_ITEM_COLOR_2, _HLIB::color_word_dword( _HLIB::color_argb_rgb(crNewColor) ) );
	}
}
