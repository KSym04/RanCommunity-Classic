// EditorRandomOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorRandomOption.h"
#include "./EditorRandomOptionDlg.h"

#include "../Lib_Engine/DxResponseMan.h"

#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Engine/G-Logic/GLOGIC.h"

#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Helper/HLibDataConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CEditorRandomOptionDlg::DataShow()
{
	m_List.DeleteAllItems();
	m_List.SetRedraw( FALSE );

	SRANDOM_OPTION_GEN_MAP &map_random_option = GLRandomOptionManager::GetInstance().m_mapRandomOption;
	int i=0;
	for( SRANDOM_OPTION_GEN_MAP_ITER iter = map_random_option.begin(); iter != map_random_option.end(); iter++, i++ )
	{
		SRANDOM_OPTION_GEN &sdata = (*iter).second;
		m_List.InsertItem( i, _HLIB::cstring_dword(i).GetString() );
		m_List.SetItemText( i, 1, sdata.strFileName.c_str() );

		if ( sdata.CheckData() )
			m_List.SetItemText( i, 2, "check" );

		m_List.SetItemData( i, i );
	}

	m_List.SetRedraw( TRUE );

	m_RandomOptionGen.Reset();
	RandomOptionGenShow( FALSE, FALSE );

	SetWin_Num_int( this, IDC_EDIT_RANDOM_OPTION_NUM, i );

	return TRUE;
}

void CEditorRandomOptionDlg::RandomOptionGenShow( BOOL bEnable, BOOL bAdd )
{
	m_bAddGen = bAdd;

	SetWin_Text( this, IDC_EDIT_GEN_NAME, m_RandomOptionGen.strFileName.c_str() );
	SetWin_Num_float( this, IDC_EDIT_GEN_RATE, m_RandomOptionGen.fRATE );
	SetWin_Num_float( this, IDC_EDIT_GEN_DPOINT, m_RandomOptionGen.fD_point );
	SetWin_Num_float( this, IDC_EDIT_GEN_SVALUE, m_RandomOptionGen.fS_value );

	SetWin_Num_int( this, IDC_EDIT_GEN_MIN_GAIN, m_RandomOptionGen.wMinGain );
	SetWin_Num_int( this, IDC_EDIT_GEN_MAX_GAIN, m_RandomOptionGen.wMaxGain );

	SetWin_Num_int( this, IDC_EDIT_GEN_MAX_GEN_D, m_RandomOptionGen.wMaxGenD );
	SetWin_Num_int( this, IDC_EDIT_GEN_MAX_GEN_R, m_RandomOptionGen.wMaxGenR );

	ShowListSet();
	ShowListSetR();

	SetWin_Enable( this, IDC_EDIT_GEN_NAME, bEnable && m_bAddGen );
	SetWin_Enable( this, IDC_EDIT_GEN_RATE, bEnable );
	SetWin_Enable( this, IDC_EDIT_GEN_DPOINT, bEnable );
	SetWin_Enable( this, IDC_EDIT_GEN_SVALUE, bEnable );

	SetWin_Enable( this, IDC_EDIT_GEN_MIN_GAIN, bEnable );
	SetWin_Enable( this, IDC_EDIT_GEN_MAX_GAIN, bEnable );

	SetWin_Enable( this, IDC_EDIT_GEN_MAX_GEN_D, bEnable );
	SetWin_Enable( this, IDC_EDIT_GEN_MAX_GEN_R, bEnable );

	SetWin_Enable( this, IDC_LIST_GEN_SET, bEnable );
	SetWin_Enable( this, IDC_LIST_GEN_SETR, bEnable );

	SetWin_Enable( this, IDC_BUTTON_GEN_SET_ADD1, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_SET_EDIT1, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_SET_DEL1, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_SET_CLEAR1, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_SET_COPY1, bEnable );

	SetWin_Enable( this, IDC_BUTTON_GEN_SET_ADD2, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_SET_EDIT2, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_SET_DEL2, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_SET_CLEAR2, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_SET_COPY2, bEnable );

	SetWin_Enable( this, IDC_BUTTON_GEN_SAVE, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_COPY_SET_TO_SETR, bEnable );
}

void CEditorRandomOptionDlg::ShowListSet()
{
	m_ListSet.DeleteAllItems();
	m_ListSet.SetRedraw( FALSE );

	int i=0;
	for( SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSET.begin(); iter != m_RandomOptionGen.mapSET.end(); iter++, i++ )
	{
		SRANDOM_OPTION_SET &sdata = (*iter).second;
		m_ListSet.InsertItem( i, _HLIB::cstringformat( "(%02d) %s", sdata.emTYPE, COMMENT::ITEM_RANDOM_OPT[sdata.emTYPE].c_str() ).GetString() );
		m_ListSet.SetItemText( i, 1, _HLIB::cstring_floatg( sdata.fRATE ) );
		m_ListSet.SetItemText( i, 2, _HLIB::cstring_floatg( sdata.fLOW ) );
		m_ListSet.SetItemText( i, 3, _HLIB::cstring_floatg( sdata.fHIGH ) );
		m_ListSet.SetItemData( i, (DWORD)sdata.emTYPE );
	}

	m_ListSet.SetRedraw( TRUE );

	m_RandomOptionSet.Reset();
	RandomOptionSetShow( FALSE, FALSE, FALSE );
}

void CEditorRandomOptionDlg::ShowListSetR()
{
	m_ListSetR.DeleteAllItems();
	m_ListSetR.SetRedraw( FALSE );

	int i=0;
	for( SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSETR.begin(); iter != m_RandomOptionGen.mapSETR.end(); iter++, i++ )
	{
		SRANDOM_OPTION_SET &sdata = (*iter).second;
		m_ListSetR.InsertItem( i, _HLIB::cstringformat( "(%02d) %s", sdata.emTYPE, COMMENT::ITEM_RANDOM_OPT[sdata.emTYPE].c_str() ).GetString() );
		m_ListSetR.SetItemText( i, 1, _HLIB::cstring_floatg( sdata.fRATE ) );
		m_ListSetR.SetItemText( i, 2, _HLIB::cstring_floatg( sdata.fLOW ) );
		m_ListSetR.SetItemText( i, 3, _HLIB::cstring_floatg( sdata.fHIGH ) );
		m_ListSetR.SetItemData( i, (DWORD)sdata.emTYPE );
	}

	m_ListSetR.SetRedraw( TRUE );

	m_RandomOptionSet.Reset();
	RandomOptionSetShow( FALSE, FALSE, TRUE );
}

void CEditorRandomOptionDlg::RandomOptionSetShow( BOOL bEnable, BOOL bAdd, BOOL bSetR )
{
	m_bAddSet = bAdd;
	m_bSetR = bSetR;

	SetWin_Combo_Sel( this, IDC_COMBO_SET_TYPE, m_RandomOptionSet.emTYPE );
	SetWin_Num_float( this, IDC_EDIT_SET_RATE, m_RandomOptionSet.fRATE );
	SetWin_Num_float( this, IDC_EDIT_SET_LOW, m_RandomOptionSet.fLOW );
	SetWin_Num_float( this, IDC_EDIT_SET_HIGH, m_RandomOptionSet.fHIGH );

	ShowListSection();

	SetWin_Enable( this, IDC_COMBO_SET_TYPE, bEnable && m_bAddSet );
	SetWin_Enable( this, IDC_EDIT_SET_RATE, bEnable );
	SetWin_Enable( this, IDC_EDIT_SET_LOW, bEnable );
	SetWin_Enable( this, IDC_EDIT_SET_HIGH, bEnable );

	SetWin_Enable( this, IDC_LIST_SET_SECTION, bEnable );

	SetWin_Enable( this, IDC_BUTTON_SET_GENERATE, bEnable );

	SetWin_Enable( this, IDC_BUTTON_SET_SECTION_ADD, bEnable );
	SetWin_Enable( this, IDC_BUTTON_SET_SECTION_EDIT, bEnable );
	SetWin_Enable( this, IDC_BUTTON_SET_SECTION_DEL, bEnable );
	SetWin_Enable( this, IDC_BUTTON_SET_SECTION_CLEAR, bEnable );

	SetWin_Enable( this, IDC_BUTTON_SET_SAVE, bEnable );
}

void CEditorRandomOptionDlg::ShowListSection()
{
	m_ListSection.DeleteAllItems();
	m_ListSection.SetRedraw( FALSE );

	for ( int i=0; i<(int)m_RandomOptionSet.vecSECT.size(); ++i )
	{
		m_ListSection.InsertItem( i, _HLIB::cstring_floatg( m_RandomOptionSet.vecSECT[i].fRATE ) );
		m_ListSection.SetItemText( i, 1, _HLIB::cstring_floatg( m_RandomOptionSet.vecSECT[i].fLOW ) );
		m_ListSection.SetItemText( i, 2, _HLIB::cstring_floatg( m_RandomOptionSet.vecSECT[i].fHIGH ) );
		m_ListSection.SetItemData( i, i );
	}

	SetWin_Num_float( this, IDC_EDIT_SET_TOTAL, m_RandomOptionSet.GetTotalRate() );

	m_ListSection.SetRedraw( TRUE );

	m_RandomOptionSection.Reset();
	RandomOptionSectionShow( FALSE, FALSE );
}

void CEditorRandomOptionDlg::RandomOptionSectionShow( BOOL bEnable, BOOL bAdd )
{
	m_bAddSection = bAdd;

	SetWin_Num_float( this, IDC_EDIT_SECTION_RATE, m_RandomOptionSection.fRATE );
	SetWin_Num_float( this, IDC_EDIT_SECTION_LOW, m_RandomOptionSection.fLOW );
	SetWin_Num_float( this, IDC_EDIT_SECTION_HIGH, m_RandomOptionSection.fHIGH );

	SetWin_Enable( this, IDC_EDIT_SECTION_RATE, bEnable  );
	SetWin_Enable( this, IDC_EDIT_SECTION_LOW, bEnable );
	SetWin_Enable( this, IDC_EDIT_SECTION_HIGH, bEnable );

	SetWin_Enable( this, IDC_BUTTON_SECTION_SAVE, bEnable );
}