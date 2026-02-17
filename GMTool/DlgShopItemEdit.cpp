// DlgShopItemEdit.cpp : implementation file
//

#include "stdafx.h"
#include "./GMTool.h"
#include "./DlgShopItemEdit.h"

#include "./Logic/GMToolData.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Client/G-Logic/GLItemMan.h"
#include "./DlgItem.h"
#include "./Logic/GMToolGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDlgShopItemEdit dialog

IMPLEMENT_DYNAMIC(CDlgShopItemEdit, CDialog)

CDlgShopItemEdit::CDlgShopItemEdit(CWnd* pParent /*=NULL*/, SGMTOOL_SHOP_ITEM* pDATA /*= NULL*/)
	: CDialog(CDlgShopItemEdit::IDD, pParent)
	, m_pData( NULL )
{
	m_pData = pDATA;
}

CDlgShopItemEdit::~CDlgShopItemEdit()
{
}

void CDlgShopItemEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgShopItemEdit, CDialog)
	ON_BN_CLICKED(IDC_SHOPITEM_BUTTON_EDIT_ITEM, OnBnClickedShopitemButtonEditItem)
	ON_BN_CLICKED(IDC_SHOPITEM_BUTTON_USE_DEFAULT, OnBnClickedShopitemButtonUseDefault)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_EN_CHANGE(IDC_SHOPITEM_EDIT_ITEM_MID, OnEnChangeShopitemEditItemMid)
	ON_EN_CHANGE(IDC_SHOPITEM_EDIT_ITEM_SID, OnEnChangeShopitemEditItemSid)
END_MESSAGE_MAP()


// CDlgShopItemEdit message handlers
BOOL CDlgShopItemEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	DataShow();

	return TRUE;
}

void CDlgShopItemEdit::DataShow()
{
	if ( !m_pData )	return;

	SetWin_Num_int( this, IDC_SHOPITEM_EDIT_PRODUCTNUM, m_pData->dwProductNum );
	SetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_MID, m_pData->wItemMain );
	SetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_SID, m_pData->wItemSub );
	SetWin_Text( this, IDC_SHOPITEM_EDIT_ITEM_NAME, m_pData->szItemName );
	SetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_CATEGORY, m_pData->wType );
	SetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_STOCK, m_pData->wItemStock );
	SetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_MONEY, m_pData->dwPrice );
	SetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_CURRENCY, m_pData->wCurrency );
	SetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_ISSALE, m_pData->wIsSale );
}

BOOL CDlgShopItemEdit::DataSave()
{
	if ( !m_pData )	return FALSE;

	WORD wMID = GetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_MID );
	WORD wSID = GetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_SID );
	SITEM* pItem = GLItemMan::GetInstance().GetItem( wMID, wSID );
	if ( !pItem )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Invalid Item( %d~%d )", wMID, wSID );
		return FALSE;
	}

	std::string strItemName = GetWin_Text( this, IDC_SHOPITEM_EDIT_ITEM_NAME );
	if ( strItemName.size() <= 0 )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "ItemName Cannot be empty!" );
		return FALSE;
	}

	StringCchCopy( m_pData->szItemName, SHOP_ITEMNAME_LENGTH, strItemName.c_str() );


	m_pData->wItemMain = wMID;
	m_pData->wItemSub = wSID;
	m_pData->wType = GetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_CATEGORY );
	m_pData->wItemStock = GetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_STOCK );
	m_pData->dwPrice = GetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_MONEY );
	m_pData->wCurrency = GetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_CURRENCY );
	m_pData->wIsSale = GetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_ISSALE );

	return TRUE;
}

void CDlgShopItemEdit::OnBnClickedShopitemButtonEditItem()
{
	CDlgItem dlg( this );
	if ( dlg.DoModal() != IDOK ){
		return;
	}

	SNATIVEID sidITEM = dlg.m_SELECTED;
	SITEM* pITEM = GLItemMan::GetInstance().GetItem ( sidITEM );
	if ( !pITEM ) {
		CDebugSet::MsgBox( GetSafeHwnd(), "Invalid Item! ( %d %d )", 
			sidITEM.wMainID,
			sidITEM.wSubID );
		return;
	}

	SetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_MID, sidITEM.wMainID );
	SetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_SID, sidITEM.wSubID );
}

void CDlgShopItemEdit::OnBnClickedShopitemButtonUseDefault()
{
	WORD wMID = GetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_MID );
	WORD wSID = GetWin_Num_int( this, IDC_SHOPITEM_EDIT_ITEM_SID );
	SITEM* pItem = GLItemMan::GetInstance().GetItem( wMID, wSID );
	if ( pItem )
	{
		SetWin_Text( this, IDC_SHOPITEM_EDIT_ITEM_NAME, pItem->GetName() );
	}	
}

void CDlgShopItemEdit::OnBnClickedOk()
{
	if ( DataSave() ){
		CDialog::OnOK();
	}
}

void CDlgShopItemEdit::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void CDlgShopItemEdit::OnEnChangeShopitemEditItemMid()
{
	GMTOOL_GLOBAL::DataCheckItem( this, 
		IDC_SHOPITEM_EDIT_ITEM_MID,
		IDC_SHOPITEM_EDIT_ITEM_SID,
		IDC_SHOPITEM_EDIT_ITEMNAME );
}

void CDlgShopItemEdit::OnEnChangeShopitemEditItemSid()
{
	GMTOOL_GLOBAL::DataCheckItem( this, 
		IDC_SHOPITEM_EDIT_ITEM_MID,
		IDC_SHOPITEM_EDIT_ITEM_SID,
		IDC_SHOPITEM_EDIT_ITEMNAME );
}
