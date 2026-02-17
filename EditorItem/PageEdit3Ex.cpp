#include "stdafx.h"
#include "./EditorItem.h"
#include "./PageEdit3.h"

#include "./SheetWithTab.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Client/G-Logic/GLStringTable.h"
#include "../Lib_Client/G-Logic/GLItem.h"
#include "../Lib_Client/G-Logic/GLItemMan.h"
#include "../Lib_Client/G-Logic/GLItemDef.h"
#include "../Lib_Client/G-Logic/VipDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CPageEdit3::DataDefault ()
{
	SetWin_Combo_Init( this, IDC_COMBO_VIP, COMMENT::CHAR_VIP, EMVIP_LEVEL_SIZE );

	return TRUE;
}

BOOL CPageEdit3::DataShow ()
{
	/*item color, Juver, 2018/01/05 */
	SetWin_Check ( this, IDC_ITEM_COLOR_USE, m_pDummyItem->sBasicOp.bItemColor);
	SetWin_Num_int ( this, IDC_EDIT_ITEM_COLOR_1, m_pDummyItem->sBasicOp.wItemColor1 );
	SetWin_Num_int ( this, IDC_EDIT_ITEM_COLOR_2, m_pDummyItem->sBasicOp.wItemColor2 );

	/* chaos machine, Juver, 2021/07/09 */
	SetWin_Num_int ( this, IDC_EDIT_CHAOS_MACHINE_ID, m_pDummyItem->sBasicOp.dwChaosMachineID);

	/* Item Reform use limit, Juver, 2021/07/23 */
	SetWin_Num_int( this, IDC_EDIT_MAX_ITEM_REFORM, m_pDummyItem->sBasicOp.wMaxItemReform );

	/* item refines ID, Juver, 2021/07/30 */
	SetWin_Num_int( this, IDC_EDIT_REFINES_ID, m_pDummyItem->sBasicOp.wItemRefinesID );

	/* set item option, Juver, 2021/08/03 */
	SetWin_Text( this, IDC_EDIT_SET_OPTION_GEN, m_pDummyItem->sBasicOp.strGenSetOption.c_str() );

	SetWin_Text( this, IDC_EDIT_EXCHANGE_ITEM, m_pDummyItem->sBasicOp.strExchangeItem.c_str() );

	SetWin_Combo_Sel( this, IDC_COMBO_VIP, COMMENT::CHAR_VIP[m_pDummyItem->sBasicOp.emVIPLevel] );

	SetWin_Num_int( this, IDC_EDIT_REBORN_REQ, m_pDummyItem->sBasicOp.dwRebornReq );

	SetWin_Num_int ( this, IDC_EDIT_SUIT_DUMMY_1, m_pDummyItem->sSuitOp.bItemColor);
	SetWin_Num_int ( this, IDC_EDIT_SUIT_DUMMY_2, m_pDummyItem->sSuitOp.dummy_var_1);
	SetWin_Num_int ( this, IDC_EDIT_SUIT_DUMMY_3, m_pDummyItem->sSuitOp.dummy_var_2);
	SetWin_Num_int ( this, IDC_EDIT_SUIT_DUMMY_4, m_pDummyItem->sBasicOp.unknown_var_1);
	SetWin_Text( this, IDC_EDIT_SUIT_DUMMY_5, m_pDummyItem->sBasicOp.unknown_var_2.c_str() );
	SetWin_Text( this, IDC_EDIT_SUIT_DUMMY_6, m_pDummyItem->sBasicOp.unknown_var_3.c_str() );

	SetWin_Check( this, IDC_CHECK_BOX_DUMMY_1, m_pDummyItem->sBox.bData );

	SetWin_Text( this, IDC_EDIT_ETC_1_1, m_pDummyItem->etc_1.Data );
	SetWin_Num_int( this, IDC_EDIT_ETC_2_1, m_pDummyItem->etc_2.unknown_var_1 );
	SetWin_Num_float( this, IDC_EDIT_ETC_2_2, m_pDummyItem->etc_2.unknown_var_2 );
	SetWin_Num_int( this, IDC_EDIT_ETC_2_3, m_pDummyItem->etc_2.unknown_var_3 );
	SetWin_Num_int( this, IDC_EDIT_ETC_2_4, m_pDummyItem->etc_2.unknown_var_4 );




	return TRUE;
}

BOOL CPageEdit3::DataSave()
{
	/*item color, Juver, 2018/01/05 */
	m_pDummyItem->sBasicOp.bItemColor = GetWin_Check ( this, IDC_ITEM_COLOR_USE )? true:false;
	m_pDummyItem->sBasicOp.wItemColor1 = GetWin_Num_int ( this, IDC_EDIT_ITEM_COLOR_1 );
	m_pDummyItem->sBasicOp.wItemColor2 = GetWin_Num_int ( this, IDC_EDIT_ITEM_COLOR_2 );


	/* chaos machine, Juver, 2021/07/09 */
	m_pDummyItem->sBasicOp.dwChaosMachineID = GetWin_Num_int ( this, IDC_EDIT_CHAOS_MACHINE_ID );

	/* Item Reform use limit, Juver, 2021/07/23 */
	m_pDummyItem->sBasicOp.wMaxItemReform = GetWin_Num_int( this, IDC_EDIT_MAX_ITEM_REFORM );

	/* item refines ID, Juver, 2021/07/30 */
	m_pDummyItem->sBasicOp.wItemRefinesID = GetWin_Num_int( this, IDC_EDIT_REFINES_ID );

	/* set item option, Juver, 2021/08/03 */
	m_pDummyItem->sBasicOp.strGenSetOption = GetWin_Text( this, IDC_EDIT_SET_OPTION_GEN ).GetString();

	m_pDummyItem->sBasicOp.strExchangeItem = GetWin_Text( this, IDC_EDIT_EXCHANGE_ITEM ).GetString();

	m_pDummyItem->sBasicOp.emVIPLevel = static_cast<EMVIP_LEVEL> ( GetWin_Combo_Sel( this, IDC_COMBO_VIP ) );

	m_pDummyItem->sBasicOp.dwRebornReq = GetWin_Num_int( this, IDC_EDIT_REBORN_REQ );

	m_pDummyItem->sSuitOp.bItemColor = GetWin_Num_int ( this, IDC_EDIT_SUIT_DUMMY_1 );
	m_pDummyItem->sSuitOp.dummy_var_1 = GetWin_Num_int ( this, IDC_EDIT_SUIT_DUMMY_2 );
	m_pDummyItem->sSuitOp.dummy_var_2 = GetWin_Num_int ( this, IDC_EDIT_SUIT_DUMMY_3 );

	m_pDummyItem->sBasicOp.unknown_var_1 = GetWin_Num_int ( this, IDC_EDIT_SUIT_DUMMY_4 );
	m_pDummyItem->sBasicOp.unknown_var_2 = GetWin_Text( this, IDC_EDIT_SUIT_DUMMY_5 ).GetString();
	m_pDummyItem->sBasicOp.unknown_var_3 = GetWin_Text( this, IDC_EDIT_SUIT_DUMMY_6 ).GetString();

	m_pDummyItem->sBox.bData = GetWin_Check( this, IDC_CHECK_BOX_DUMMY_1 )? true:false;

	std::string text = GetWin_Text( this, IDC_EDIT_ETC_1_1 );
	StringCchCopy( m_pDummyItem->etc_1.Data, ITEM::ETC_1::DATA_SIZE, text.c_str() );
	m_pDummyItem->etc_2.unknown_var_1 = GetWin_Num_int( this, IDC_EDIT_ETC_2_1 );
	m_pDummyItem->etc_2.unknown_var_2 = GetWin_Num_float( this, IDC_EDIT_ETC_2_2 );
	m_pDummyItem->etc_2.unknown_var_3 = GetWin_Num_int( this, IDC_EDIT_ETC_2_3 );
	m_pDummyItem->etc_2.unknown_var_4 = GetWin_Num_int( this, IDC_EDIT_ETC_2_4 );

	return TRUE;
}
