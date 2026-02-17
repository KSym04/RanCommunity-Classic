// DlgSetOptionEdit.cpp : implementation file
//

#include "stdafx.h"
#include "EditorSetOption.h"
#include "DlgSetOptionEdit.h"
#include "afxdialogex.h"

#include "DlgFindItem.h"

#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Helper/HLibDataConvert.h"

// CDlgSetOptionEdit dialog

IMPLEMENT_DYNAMIC(CDlgSetOptionEdit, CDialog)

CDlgSetOptionEdit::CDlgSetOptionEdit(CWnd* pParent, SET_OPTION_DATA* pOptionData)
	: CDialog(CDlgSetOptionEdit::IDD, pParent)
	, m_pOptionData(pOptionData)
{

}

CDlgSetOptionEdit::~CDlgSetOptionEdit()
{
}

void CDlgSetOptionEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSetOptionEdit, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSetOptionEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD0, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd0)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD1, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd1)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD2, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd2)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD3, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd3)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD4, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd4)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD5, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd5)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD6, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd6)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD7, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd7)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD8, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd8)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD9, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd9)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD10, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd10)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD11, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd11)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD12, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd12)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_ADD13, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd13)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL0, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel0)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL1, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel1)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL2, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel2)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL3, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel3)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL4, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel4)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL5, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel5)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL6, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel6)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL7, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel7)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL8, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel8)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL9, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel9)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL10, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel10)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL11, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel11)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL12, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel12)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_SLOT_DEL13, &CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel13)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID0, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid0)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID0, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid0)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID1, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid1)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID1, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid1)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID2, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid2)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID2, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid2)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID3, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid3)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID3, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid3)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID4, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid4)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID4, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid4)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID5, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid5)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID5, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid5)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID6, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid6)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID6, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid6)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID7, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid7)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID7, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid7)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID8, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid8)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID8, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid8)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID9, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid9)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID9, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid9)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID10, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid10)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID10, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid10)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID11, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid11)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID11, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid11)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID12, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid12)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID12, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid12)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_MID13, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid13)
	ON_EN_CHANGE(IDC_EDIT_ITEM_SLOT_ITEM_SID13, &CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid13)
END_MESSAGE_MAP()


// CDlgSetOptionEdit message handlers

BOOL CDlgSetOptionEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWin_Combo_Init( this, IDC_COMBO_SET_OPTION_TYPE0, COMMENT::SET_OPTION_TYPE, SET_OPTION_TYPE_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_SET_OPTION_TYPE1, COMMENT::SET_OPTION_TYPE, SET_OPTION_TYPE_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_SET_OPTION_TYPE2, COMMENT::SET_OPTION_TYPE, SET_OPTION_TYPE_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_SET_OPTION_TYPE3, COMMENT::SET_OPTION_TYPE, SET_OPTION_TYPE_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_SET_OPTION_TYPE4, COMMENT::SET_OPTION_TYPE, SET_OPTION_TYPE_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_SET_OPTION_TYPE5, COMMENT::SET_OPTION_TYPE, SET_OPTION_TYPE_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_SET_OPTION_TYPE6, COMMENT::SET_OPTION_TYPE, SET_OPTION_TYPE_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_SET_OPTION_TYPE7, COMMENT::SET_OPTION_TYPE, SET_OPTION_TYPE_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_SET_OPTION_TYPE8, COMMENT::SET_OPTION_TYPE, SET_OPTION_TYPE_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_SET_OPTION_TYPE9, COMMENT::SET_OPTION_TYPE, SET_OPTION_TYPE_SIZE );

	if ( m_pOptionData )
	{
		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_ID, m_pOptionData->dwID );
		SetWin_Text( this, IDC_EDIT_SET_OPTION_NAME, m_pOptionData->strName.c_str() );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID0, m_pOptionData->sItemSet[SET_OPTION_SLOT_HEAD].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID0, m_pOptionData->sItemSet[SET_OPTION_SLOT_HEAD].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID1, m_pOptionData->sItemSet[SET_OPTION_SLOT_UPPER].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID1, m_pOptionData->sItemSet[SET_OPTION_SLOT_UPPER].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID2, m_pOptionData->sItemSet[SET_OPTION_SLOT_LOWER].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID2, m_pOptionData->sItemSet[SET_OPTION_SLOT_LOWER].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID3, m_pOptionData->sItemSet[SET_OPTION_SLOT_HAND].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID3, m_pOptionData->sItemSet[SET_OPTION_SLOT_HAND].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID4, m_pOptionData->sItemSet[SET_OPTION_SLOT_FOOT].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID4, m_pOptionData->sItemSet[SET_OPTION_SLOT_FOOT].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID5, m_pOptionData->sItemSet[SET_OPTION_SLOT_WEAPON].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID5, m_pOptionData->sItemSet[SET_OPTION_SLOT_WEAPON].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID6, m_pOptionData->sItemSet[SET_OPTION_SLOT_NECK].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID6, m_pOptionData->sItemSet[SET_OPTION_SLOT_NECK].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID7, m_pOptionData->sItemSet[SET_OPTION_SLOT_WRIST].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID7, m_pOptionData->sItemSet[SET_OPTION_SLOT_WRIST].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID8, m_pOptionData->sItemSet[SET_OPTION_SLOT_FINGER].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID8, m_pOptionData->sItemSet[SET_OPTION_SLOT_FINGER].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID9, m_pOptionData->sItemSet[SET_OPTION_SLOT_VEHICLE].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID9, m_pOptionData->sItemSet[SET_OPTION_SLOT_VEHICLE].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID10, m_pOptionData->sItemSet[SET_OPTION_SLOT_BELT].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID10, m_pOptionData->sItemSet[SET_OPTION_SLOT_BELT].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID11, m_pOptionData->sItemSet[SET_OPTION_SLOT_EARRING].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID11, m_pOptionData->sItemSet[SET_OPTION_SLOT_EARRING].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID12, m_pOptionData->sItemSet[SET_OPTION_SLOT_ACCESSORY].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID12, m_pOptionData->sItemSet[SET_OPTION_SLOT_ACCESSORY].wSubID );

		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID13, m_pOptionData->sItemSet[SET_OPTION_SLOT_DECORATION].wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID13, m_pOptionData->sItemSet[SET_OPTION_SLOT_DECORATION].wSubID );

		SetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE0, m_pOptionData->Option[0].emType );
		SetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE1, m_pOptionData->Option[1].emType );
		SetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE2, m_pOptionData->Option[2].emType );
		SetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE3, m_pOptionData->Option[3].emType );
		SetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE4, m_pOptionData->Option[4].emType );
		SetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE5, m_pOptionData->Option[5].emType );
		SetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE6, m_pOptionData->Option[6].emType );
		SetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE7, m_pOptionData->Option[7].emType );
		SetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE8, m_pOptionData->Option[8].emType );
		SetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE9, m_pOptionData->Option[9].emType );

		SetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL0, m_pOptionData->Option[0].fValue );
		SetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL1, m_pOptionData->Option[1].fValue );
		SetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL2, m_pOptionData->Option[2].fValue );
		SetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL3, m_pOptionData->Option[3].fValue );
		SetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL4, m_pOptionData->Option[4].fValue );
		SetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL5, m_pOptionData->Option[5].fValue );
		SetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL6, m_pOptionData->Option[6].fValue );
		SetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL7, m_pOptionData->Option[7].fValue );
		SetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL8, m_pOptionData->Option[8].fValue );
		SetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL9, m_pOptionData->Option[9].fValue );

		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM0, m_pOptionData->Option[0].dwSetCount );
		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM1, m_pOptionData->Option[1].dwSetCount );
		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM2, m_pOptionData->Option[2].dwSetCount );
		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM3, m_pOptionData->Option[3].dwSetCount );
		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM4, m_pOptionData->Option[4].dwSetCount );
		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM5, m_pOptionData->Option[5].dwSetCount );
		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM6, m_pOptionData->Option[6].dwSetCount );
		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM7, m_pOptionData->Option[7].dwSetCount );
		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM8, m_pOptionData->Option[8].dwSetCount );
		SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM9, m_pOptionData->Option[9].dwSetCount );
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSetOptionEdit::OnBnClickedOk()
{
	if ( m_pOptionData )
	{
		m_pOptionData->strName = GetWin_Text( this, IDC_EDIT_SET_OPTION_NAME ).GetString();

		m_pOptionData->sItemSet[SET_OPTION_SLOT_HEAD].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID0 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_HEAD].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID0 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_UPPER].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID1 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_UPPER].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID1 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_LOWER].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID2 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_LOWER].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID2 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_HAND].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID3 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_HAND].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID3 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_FOOT].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID4 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_FOOT].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID4 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_WEAPON].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID5 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_WEAPON].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID5 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_NECK].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID6 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_NECK].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID6 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_WRIST].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID7 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_WRIST].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID7 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_FINGER].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID8 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_FINGER].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID8 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_VEHICLE].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID9 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_VEHICLE].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID9 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_BELT].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID10 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_BELT].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID10 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_EARRING].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID11 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_EARRING].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID11 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_ACCESSORY].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID12 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_ACCESSORY].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID12 );

		m_pOptionData->sItemSet[SET_OPTION_SLOT_DECORATION].wMainID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID13 );
		m_pOptionData->sItemSet[SET_OPTION_SLOT_DECORATION].wSubID = GetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID13 );


		m_pOptionData->Option[0].emType = static_cast<SET_OPTION_TYPE>(GetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE0 ));
		m_pOptionData->Option[1].emType = static_cast<SET_OPTION_TYPE>(GetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE1 ));
		m_pOptionData->Option[2].emType = static_cast<SET_OPTION_TYPE>(GetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE2 ));
		m_pOptionData->Option[3].emType = static_cast<SET_OPTION_TYPE>(GetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE3 ));
		m_pOptionData->Option[4].emType = static_cast<SET_OPTION_TYPE>(GetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE4 ));
		m_pOptionData->Option[5].emType = static_cast<SET_OPTION_TYPE>(GetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE5 ));
		m_pOptionData->Option[6].emType = static_cast<SET_OPTION_TYPE>(GetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE6 ));
		m_pOptionData->Option[7].emType = static_cast<SET_OPTION_TYPE>(GetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE7 ));
		m_pOptionData->Option[8].emType = static_cast<SET_OPTION_TYPE>(GetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE8 ));
		m_pOptionData->Option[9].emType = static_cast<SET_OPTION_TYPE>(GetWin_Combo_Sel( this, IDC_COMBO_SET_OPTION_TYPE9 ));

		m_pOptionData->Option[0].fValue = GetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL0 );
		m_pOptionData->Option[1].fValue = GetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL1 );
		m_pOptionData->Option[2].fValue = GetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL2 );
		m_pOptionData->Option[3].fValue = GetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL3 );
		m_pOptionData->Option[4].fValue = GetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL4 );
		m_pOptionData->Option[5].fValue = GetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL5 );
		m_pOptionData->Option[6].fValue = GetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL6 );
		m_pOptionData->Option[7].fValue = GetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL7 );
		m_pOptionData->Option[8].fValue = GetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL8 );
		m_pOptionData->Option[9].fValue = GetWin_Num_float( this, IDC_EDIT_SET_OPTION_VAL9 );

		m_pOptionData->Option[0].dwSetCount = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM0 );
		m_pOptionData->Option[1].dwSetCount = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM1 );
		m_pOptionData->Option[2].dwSetCount = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM2 );
		m_pOptionData->Option[3].dwSetCount = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM3 );
		m_pOptionData->Option[4].dwSetCount = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM4 );
		m_pOptionData->Option[5].dwSetCount = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM5 );
		m_pOptionData->Option[6].dwSetCount = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM6 );
		m_pOptionData->Option[7].dwSetCount = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM7 );
		m_pOptionData->Option[8].dwSetCount = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM8 );
		m_pOptionData->Option[9].dwSetCount = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM9 );


	}

	CDialog::OnOK();
}

void CDlgSetOptionEdit::UpdateItem(int nID_MID, int nID_SID, int nID_NAME)
{
	SNATIVEID sID;
	sID.wMainID = GetWin_Num_int( this, nID_MID );
	sID.wSubID = GetWin_Num_int( this, nID_SID );

	SITEM* pItem = GLItemMan::GetInstance().GetItem( sID );
	if ( pItem )
		SetWin_Text( this, nID_NAME, pItem->GetName() );
	else
		SetWin_Text( this, nID_NAME, "unknown item" );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd0()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID0, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID0, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd1()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID1, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID1, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd2()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID2, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID2, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd3()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID3, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID3, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd4()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID4, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID4, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd5()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID5, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID5, dlg.m_sSELECTED.wSubID );
		
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd6()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID6, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID6, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd7()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID7, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID7, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd8()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID8, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID8, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd9()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID9, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID9, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd10()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID10, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID10, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd11()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID11, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID11, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd12()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID12, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID12, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotAdd13()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID13, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID13, dlg.m_sSELECTED.wSubID );
	}
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel0()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID0, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID0, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel1()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID1, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID1, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel2()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID2, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID2, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel3()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID3, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID3, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel4()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID4, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID4, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel5()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID5, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID5, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel6()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID6, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID6, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel7()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID7, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID7, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel8()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID8, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID8, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel9()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID9, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID9, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel10()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID10, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID10, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel11()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID11, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID11, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel12()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID12, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID12, sID.wSubID );
}

void CDlgSetOptionEdit::OnBnClickedButtonItemSlotDel13()
{
	SNATIVEID sID(false);
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_MID13, sID.wMainID );
	SetWin_Num_int(this, IDC_EDIT_ITEM_SLOT_ITEM_SID13, sID.wSubID );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid0()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID0, IDC_EDIT_ITEM_SLOT_ITEM_SID0, IDC_EDIT_ITEM_SLOT_ITEM_NAME0 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid0()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID0, IDC_EDIT_ITEM_SLOT_ITEM_SID0, IDC_EDIT_ITEM_SLOT_ITEM_NAME0 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid1()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID1, IDC_EDIT_ITEM_SLOT_ITEM_SID1, IDC_EDIT_ITEM_SLOT_ITEM_NAME1 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid1()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID1, IDC_EDIT_ITEM_SLOT_ITEM_SID1, IDC_EDIT_ITEM_SLOT_ITEM_NAME1 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid2()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID2, IDC_EDIT_ITEM_SLOT_ITEM_SID2, IDC_EDIT_ITEM_SLOT_ITEM_NAME2 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid2()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID2, IDC_EDIT_ITEM_SLOT_ITEM_SID2, IDC_EDIT_ITEM_SLOT_ITEM_NAME2 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid3()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID3, IDC_EDIT_ITEM_SLOT_ITEM_SID3, IDC_EDIT_ITEM_SLOT_ITEM_NAME3 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid3()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID3, IDC_EDIT_ITEM_SLOT_ITEM_SID3, IDC_EDIT_ITEM_SLOT_ITEM_NAME3 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid4()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID4, IDC_EDIT_ITEM_SLOT_ITEM_SID4, IDC_EDIT_ITEM_SLOT_ITEM_NAME4 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid4()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID4, IDC_EDIT_ITEM_SLOT_ITEM_SID4, IDC_EDIT_ITEM_SLOT_ITEM_NAME4 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid5()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID5, IDC_EDIT_ITEM_SLOT_ITEM_SID5, IDC_EDIT_ITEM_SLOT_ITEM_NAME5 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid5()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID5, IDC_EDIT_ITEM_SLOT_ITEM_SID5, IDC_EDIT_ITEM_SLOT_ITEM_NAME5 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid6()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID6, IDC_EDIT_ITEM_SLOT_ITEM_SID6, IDC_EDIT_ITEM_SLOT_ITEM_NAME6 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid6()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID6, IDC_EDIT_ITEM_SLOT_ITEM_SID6, IDC_EDIT_ITEM_SLOT_ITEM_NAME6 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid7()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID7, IDC_EDIT_ITEM_SLOT_ITEM_SID7, IDC_EDIT_ITEM_SLOT_ITEM_NAME7 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid7()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID7, IDC_EDIT_ITEM_SLOT_ITEM_SID7, IDC_EDIT_ITEM_SLOT_ITEM_NAME7 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid8()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID8, IDC_EDIT_ITEM_SLOT_ITEM_SID8, IDC_EDIT_ITEM_SLOT_ITEM_NAME8 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid8()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID8, IDC_EDIT_ITEM_SLOT_ITEM_SID8, IDC_EDIT_ITEM_SLOT_ITEM_NAME8 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid9()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID9, IDC_EDIT_ITEM_SLOT_ITEM_SID9, IDC_EDIT_ITEM_SLOT_ITEM_NAME9 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid9()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID9, IDC_EDIT_ITEM_SLOT_ITEM_SID9, IDC_EDIT_ITEM_SLOT_ITEM_NAME9 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid10()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID10, IDC_EDIT_ITEM_SLOT_ITEM_SID10, IDC_EDIT_ITEM_SLOT_ITEM_NAME10 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid10()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID10, IDC_EDIT_ITEM_SLOT_ITEM_SID10, IDC_EDIT_ITEM_SLOT_ITEM_NAME10 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid11()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID11, IDC_EDIT_ITEM_SLOT_ITEM_SID11, IDC_EDIT_ITEM_SLOT_ITEM_NAME11 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid11()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID11, IDC_EDIT_ITEM_SLOT_ITEM_SID11, IDC_EDIT_ITEM_SLOT_ITEM_NAME11 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid12()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID12, IDC_EDIT_ITEM_SLOT_ITEM_SID12, IDC_EDIT_ITEM_SLOT_ITEM_NAME12 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid12()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID12, IDC_EDIT_ITEM_SLOT_ITEM_SID12, IDC_EDIT_ITEM_SLOT_ITEM_NAME12 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemMid13()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID13, IDC_EDIT_ITEM_SLOT_ITEM_SID13, IDC_EDIT_ITEM_SLOT_ITEM_NAME13 );
}

void CDlgSetOptionEdit::OnEnChangeEditItemSlotItemSid13()
{
	UpdateItem( IDC_EDIT_ITEM_SLOT_ITEM_MID13, IDC_EDIT_ITEM_SLOT_ITEM_SID13, IDC_EDIT_ITEM_SLOT_ITEM_NAME13 );
}
