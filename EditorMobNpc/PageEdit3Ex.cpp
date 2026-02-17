#include "stdafx.h"
#include "./EditorMobNpc.h"
#include "./PageEdit3.h"

#include "../Lib_Helper/EtcFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL CPageEdit3::DataDefault()
{
	return TRUE;
}

BOOL CPageEdit3::DataShow()
{
	SetWin_Text( this, IDC_EDIT_WARDEN_GEN_ITEM_1, m_pDummyCrow->m_sCrowWarden.strGenerateItem[0].c_str() );
	SetWin_Text( this, IDC_EDIT_WARDEN_GEN_ITEM_2, m_pDummyCrow->m_sCrowWarden.strGenerateItem[1].c_str() );
	SetWin_Text( this, IDC_EDIT_WARDEN_GEN_ITEM_3, m_pDummyCrow->m_sCrowWarden.strGenerateItem[2].c_str() );
	SetWin_Text( this, IDC_EDIT_WARDEN_GEN_ITEM_4, m_pDummyCrow->m_sCrowWarden.strGenerateItem[3].c_str() );

	SetWin_Num_int( this, IDC_EDIT_WARDEN_GEN_MONEY, m_pDummyCrow->m_sCrowWarden.dwGenerateMoney );

	SetWin_Num_int( this, IDC_EDIT_WARDEN_DATA_1, m_pDummyCrow->m_sCrowWarden.dwData1 );
	SetWin_Num_int( this, IDC_EDIT_WARDEN_DATA_2, m_pDummyCrow->m_sCrowWarden.dwData2 );
	

	return TRUE;
}

BOOL CPageEdit3::DataSave()
{
	m_pDummyCrow->m_sCrowWarden.strGenerateItem[0] = GetWin_Text( this, IDC_EDIT_WARDEN_GEN_ITEM_1 ).GetString();
	m_pDummyCrow->m_sCrowWarden.strGenerateItem[1] = GetWin_Text( this, IDC_EDIT_WARDEN_GEN_ITEM_2 ).GetString();
	m_pDummyCrow->m_sCrowWarden.strGenerateItem[2] = GetWin_Text( this, IDC_EDIT_WARDEN_GEN_ITEM_3 ).GetString();
	m_pDummyCrow->m_sCrowWarden.strGenerateItem[3] = GetWin_Text( this, IDC_EDIT_WARDEN_GEN_ITEM_4 ).GetString();

	m_pDummyCrow->m_sCrowWarden.dwGenerateMoney = GetWin_Num_int( this, IDC_EDIT_WARDEN_GEN_MONEY );

	m_pDummyCrow->m_sCrowWarden.dwData1 = GetWin_Num_int( this, IDC_EDIT_WARDEN_DATA_1 );
	m_pDummyCrow->m_sCrowWarden.dwData2 = GetWin_Num_int( this, IDC_EDIT_WARDEN_DATA_2 );

	return TRUE;
}