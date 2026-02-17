// PageEditGate.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorLevel.h"
#include "./PageEditGate.h"
#include "./SheetWithTab.h"
#include "../Lib_Helper/EtcFunction.h"
#include "./EditorLevelView.h"
#include "./GenerateName.h"
#include "./DlgFindMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPageEditGate dialog

IMPLEMENT_DYNAMIC(CPageEditGate, CPropertyPage)

CPageEditGate::CPageEditGate(LOGFONT logfont)
	: CPropertyPage(CPageEditGate::IDD)
	, m_pFont ( NULL )
	, m_pGLLand( NULL )
	, m_pEdit( NULL )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CPageEditGate::~CPageEditGate()
{
	SAFE_DELETE ( m_pFont );
}

void CPageEditGate::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageEditGate, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_REGNEW, OnBnClickedButtonRegnew)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnBnClickedButtonCheck)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_MOUSEPOS, OnBnClickedButtonMousepos)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)

	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_0, &CPageEditGate::OnBnClickedButtonToMap0)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_1, &CPageEditGate::OnBnClickedButtonToMap1)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_2, &CPageEditGate::OnBnClickedButtonToMap2)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_3, &CPageEditGate::OnBnClickedButtonToMap3)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_4, &CPageEditGate::OnBnClickedButtonToMap4)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_5, &CPageEditGate::OnBnClickedButtonToMap5)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_6, &CPageEditGate::OnBnClickedButtonToMap6)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_7, &CPageEditGate::OnBnClickedButtonToMap7)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_MID_0, &CPageEditGate::OnEnChangeEditToMapMid0)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_SID_0, &CPageEditGate::OnEnChangeEditToMapSid0)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_MID_1, &CPageEditGate::OnEnChangeEditToMapMid1)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_SID_1, &CPageEditGate::OnEnChangeEditToMapSid1)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_MID_2, &CPageEditGate::OnEnChangeEditToMapMid2)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_SID_2, &CPageEditGate::OnEnChangeEditToMapSid2)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_MID_3, &CPageEditGate::OnEnChangeEditToMapMid3)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_SID_3, &CPageEditGate::OnEnChangeEditToMapSid3)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_MID_4, &CPageEditGate::OnEnChangeEditToMapMid4)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_SID_4, &CPageEditGate::OnEnChangeEditToMapSid4)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_MID_5, &CPageEditGate::OnEnChangeEditToMapMid5)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_SID_5, &CPageEditGate::OnEnChangeEditToMapSid5)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_MID_6, &CPageEditGate::OnEnChangeEditToMapMid6)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_SID_6, &CPageEditGate::OnEnChangeEditToMapSid6)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_MID_7, &CPageEditGate::OnEnChangeEditToMapMid7)
	ON_EN_CHANGE(IDC_EDIT_TO_MAP_SID_7, &CPageEditGate::OnEnChangeEditToMapSid7)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_RESET_0, &CPageEditGate::OnBnClickedButtonToMapReset0)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_RESET_1, &CPageEditGate::OnBnClickedButtonToMapReset1)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_RESET_2, &CPageEditGate::OnBnClickedButtonToMapReset2)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_RESET_3, &CPageEditGate::OnBnClickedButtonToMapReset3)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_RESET_4, &CPageEditGate::OnBnClickedButtonToMapReset4)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_RESET_5, &CPageEditGate::OnBnClickedButtonToMapReset5)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_RESET_6, &CPageEditGate::OnBnClickedButtonToMapReset6)
	ON_BN_CLICKED(IDC_BUTTON_TO_MAP_RESET_7, &CPageEditGate::OnBnClickedButtonToMapReset7)
	
END_MESSAGE_MAP()


// CPageEditGate message handlers
BOOL CPageEditGate::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pSheetTab->ChangeDialogFont( this, m_pFont, CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	return TRUE;  
}

void CPageEditGate::SetData( GLLandMan* pLand, PDXLANDGATE pEdit )
{
	m_pGLLand = pLand;
	m_pEdit = pEdit;

	if ( m_pGLLand && m_pEdit )
	{
		InitUI( TRUE );
		DataShow();
		JumpTo( m_pEdit->GetMax(), m_pEdit->GetMin() );
	}else if ( m_pGLLand ){
		InitUI( FALSE );
		SetWin_Num_int( this, IDC_EDIT_GATE_ID, GateNewID( m_pGLLand ) );
		SetWin_Text( this, IDC_EDIT_GATE_NAME, GateNameNew( m_pGLLand ).c_str() );
	}
}

void CPageEditGate::ResetData()
{
	m_pEdit = NULL;
	m_pGLLand = NULL;
}

void CPageEditGate::InitUI( BOOL bENABLE )
{
	SetWin_Enable( this, IDC_CHECK_GATE_IN, bENABLE );
	SetWin_Enable( this, IDC_CHECK_GATE_OUT, bENABLE );
	
	SetWin_Enable( this, IDC_EDIT_GATE_GATEINTERVAL_X, bENABLE );
	SetWin_Enable( this, IDC_EDIT_GATE_GATEINTERVAL_Y, bENABLE );

	SetWin_Enable( this, IDC_CHECK_GATE_NEW_SYSTEM, bENABLE );

	SetWin_Enable( this, IDC_BUTTON_TO_MAP_0, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_1, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_2, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_3, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_4, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_5, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_6, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_7, bENABLE );

	SetWin_Enable( this, IDC_EDIT_TO_MAP_MID_0, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_MID_1, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_MID_2, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_MID_3, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_MID_4, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_MID_5, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_MID_6, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_MID_7, bENABLE );

	SetWin_Enable( this, IDC_EDIT_TO_MAP_SID_0, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_SID_1, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_SID_2, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_SID_3, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_SID_4, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_SID_5, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_SID_6, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_MAP_SID_7, bENABLE );

	SetWin_Enable( this, IDC_EDIT_TO_GATE_0, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_GATE_1, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_GATE_2, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_GATE_3, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_GATE_4, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_GATE_5, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_GATE_6, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TO_GATE_7, bENABLE );

	SetWin_Enable( this, IDC_EDIT_NAME_KEY_0, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_KEY_1, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_KEY_2, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_KEY_3, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_KEY_4, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_KEY_5, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_KEY_6, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_KEY_7, bENABLE );

	SetWin_Enable( this, IDC_EDIT_NAME_INDEX_0, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_INDEX_1, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_INDEX_2, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_INDEX_3, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_INDEX_4, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_INDEX_5, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_INDEX_6, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NAME_INDEX_7, bENABLE );

	SetWin_Enable( this, IDC_BUTTON_TO_MAP_RESET_0, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_RESET_1, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_RESET_2, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_RESET_3, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_RESET_4, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_RESET_5, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_RESET_6, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_TO_MAP_RESET_7, bENABLE );


	SetWin_Enable( this, IDC_BUTTON_MOUSEPOS, bENABLE );

	SetWin_Enable( this, IDC_BUTTON_REGNEW, !bENABLE );
	SetWin_Enable( this, IDC_BUTTON_SAVE, bENABLE );
	SetWin_Enable( this, IDC_BUTTON_APPLY, bENABLE );


	


	SetWin_Num_int( this, IDC_EDIT_GATE_ID, 0 );
	SetWin_Text( this, IDC_EDIT_GATE_NAME, "" );
	SetWin_Text( this, IDC_EDIT_GATE_INFO, "" );

	SetWin_Check( this, IDC_CHECK_GATE_IN, FALSE );
	SetWin_Check( this, IDC_CHECK_GATE_OUT, FALSE );
	
	SetWin_Num_float( this, IDC_EDIT_GATE_GATEINTERVAL_X, 0.0f );
	SetWin_Num_float( this, IDC_EDIT_GATE_GATEINTERVAL_Y, 0.0f );

	SetWin_Check( this, IDC_CHECK_GATE_NEW_SYSTEM, FALSE );
	
	SNATIVEID sid(false);
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_0, sid.wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_0, sid.wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_1, sid.wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_1, sid.wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_2, sid.wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_2, sid.wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_3, sid.wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_3, sid.wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_4, sid.wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_4, sid.wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_5, sid.wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_5, sid.wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_6, sid.wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_6, sid.wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_7, sid.wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_7, sid.wSubID );

	SetWin_Num_int( this, IDC_EDIT_TO_GATE_0, 0 );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_1, 0 );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_2, 0 );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_3, 0 );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_4, 0 );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_5, 0 );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_6, 0 );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_7, 0 );

	SetWin_Text( this, IDC_EDIT_NAME_KEY_0, "" );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_1, "" );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_2, "" );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_3, "" );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_4, "" );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_5, "" );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_6, "" );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_7, "" );

	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_0, -1 );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_1, -1 );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_2, -1 );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_3, -1 );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_4, -1 );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_5, -1 );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_6, -1 );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_7, -1 );

	SetWin_Num_int( this, IDC_EDIT_TO_MAP_NUM, 0 );
}

void CPageEditGate::DataShow()
{
	if ( !m_pGLLand )	return;
	if ( !m_pEdit )		return;

	SetWin_Num_int( this, IDC_EDIT_GATE_ID, m_pEdit->GetGateID() );
	SetWin_Text( this, IDC_EDIT_GATE_NAME, m_pEdit->GetName() );
	SetWin_Text( this, IDC_EDIT_GATE_INFO, m_pEdit->GetInfo() );
	SetWin_Check( this, IDC_CHECK_GATE_IN, m_pEdit->GetFlags() & DxLandGate::GATE_IN );
	SetWin_Check( this, IDC_CHECK_GATE_OUT, m_pEdit->GetFlags() & DxLandGate::GATE_OUT );

	SetWin_Num_float( this, IDC_EDIT_GATE_GATEINTERVAL_X, m_pEdit->GetDiv().x );
	SetWin_Num_float( this, IDC_EDIT_GATE_GATEINTERVAL_Y, m_pEdit->GetDiv().y );

	SetWin_Check( this, IDC_CHECK_GATE_NEW_SYSTEM, m_pEdit->GetNewSystem() );

	SetWin_Num_int( this, IDC_EDIT_TO_MAP_NUM, m_pEdit->GetToMapNum() );


	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_0, m_pEdit->GetToMapID(0).wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_0, m_pEdit->GetToMapID(0).wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_0, m_pEdit->GetToGateID(0) );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_0, m_pEdit->GetNameKEY(0) );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_0, m_pEdit->GetNameINDEX(0) );

	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_1, m_pEdit->GetToMapID(1).wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_1, m_pEdit->GetToMapID(1).wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_1, m_pEdit->GetToGateID(1) );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_1, m_pEdit->GetNameKEY(1) );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_1, m_pEdit->GetNameINDEX(1) );

	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_2, m_pEdit->GetToMapID(2).wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_2, m_pEdit->GetToMapID(2).wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_2, m_pEdit->GetToGateID(2) );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_2, m_pEdit->GetNameKEY(2) );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_2, m_pEdit->GetNameINDEX(2) );

	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_3, m_pEdit->GetToMapID(3).wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_3, m_pEdit->GetToMapID(3).wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_3, m_pEdit->GetToGateID(3) );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_3, m_pEdit->GetNameKEY(3) );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_3, m_pEdit->GetNameINDEX(3) );

	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_4, m_pEdit->GetToMapID(4).wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_4, m_pEdit->GetToMapID(4).wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_4, m_pEdit->GetToGateID(4) );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_4, m_pEdit->GetNameKEY(4) );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_4, m_pEdit->GetNameINDEX(4) );

	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_5, m_pEdit->GetToMapID(5).wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_5, m_pEdit->GetToMapID(5).wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_5, m_pEdit->GetToGateID(5) );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_5, m_pEdit->GetNameKEY(5) );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_5, m_pEdit->GetNameINDEX(5) );

	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_6, m_pEdit->GetToMapID(6).wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_6, m_pEdit->GetToMapID(6).wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_6, m_pEdit->GetToGateID(6) );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_6, m_pEdit->GetNameKEY(6) );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_6, m_pEdit->GetNameINDEX(6) );

	SetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_7, m_pEdit->GetToMapID(7).wMainID );
	SetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_7, m_pEdit->GetToMapID(7).wSubID );
	SetWin_Num_int( this, IDC_EDIT_TO_GATE_7, m_pEdit->GetToGateID(7) );
	SetWin_Text( this, IDC_EDIT_NAME_KEY_7, m_pEdit->GetNameKEY(7) );
	SetWin_Num_int( this, IDC_EDIT_NAME_INDEX_7, m_pEdit->GetNameINDEX(7) );


	CEditorLevelView::GetView()->EditBoxSet( TRUE );
	CEditorLevelView::GetView()->m_DxEditBox.SetBox( m_pEdit->GetMax(), m_pEdit->GetMin() );
}

BOOL CPageEditGate::DataSave()
{
	if ( !m_pGLLand )	return FALSE;
	if ( !m_pEdit )		return FALSE;

	DWORD gateid = GetWin_Num_int( this, IDC_EDIT_GATE_ID );
	std::string gatename = GetWin_Text( this, IDC_EDIT_GATE_NAME );
	std::string gatedesc = GetWin_Text( this, IDC_EDIT_GATE_INFO );

	if ( gatename.size() <= 0 )
	{
		CDebugSet::MsgBox( "Name Empty!" );
		return FALSE;
	}

	if ( strcmp( m_pEdit->GetName(), gatename.c_str() ) )
	{
		if ( GateNameCheck( m_pGLLand, gatename.c_str() ) )
		{
			CDebugSet::MsgBox( "Name: %s is already used! Create New name!", gatename.c_str() );
			return FALSE;
		}
	}

	if ( m_pEdit->GetGateID() != gateid )
	{
		if ( GateIDCheck( m_pGLLand, gateid ) )
		{
			CDebugSet::MsgBox( "Gate ID: %d is already used, Create New ID!", gateid );
			return FALSE;
		}
	}

	SNATIVEID sMapID(false);
	DWORD dwGateID(0);
	std::string strNameKEY("");
	DWORD dwNameINDEX(-1);

	//
	sMapID.wMainID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_0 );
	sMapID.wSubID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_0 );
	dwGateID = GetWin_Num_int( this, IDC_EDIT_TO_GATE_0 );
	strNameKEY = GetWin_Text( this, IDC_EDIT_NAME_KEY_0 );
	dwNameINDEX = GetWin_Num_int( this, IDC_EDIT_NAME_INDEX_0 );

	m_pEdit->SetToMapID(0, sMapID);
	m_pEdit->SetToGateID(0, dwGateID);
	m_pEdit->SetNameKEY(0, strNameKEY.c_str() );
	m_pEdit->SetNameINDEX(0, dwNameINDEX );

	//
	sMapID.wMainID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_1 );
	sMapID.wSubID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_1 );
	dwGateID = GetWin_Num_int( this, IDC_EDIT_TO_GATE_1 );
	strNameKEY = GetWin_Text( this, IDC_EDIT_NAME_KEY_1 );
	dwNameINDEX = GetWin_Num_int( this, IDC_EDIT_NAME_INDEX_1 );

	m_pEdit->SetToMapID(1, sMapID);
	m_pEdit->SetToGateID(1, dwGateID);
	m_pEdit->SetNameKEY(1, strNameKEY.c_str() );
	m_pEdit->SetNameINDEX(1, dwNameINDEX );

	//
	sMapID.wMainID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_2 );
	sMapID.wSubID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_2 );
	dwGateID = GetWin_Num_int( this, IDC_EDIT_TO_GATE_2 );
	strNameKEY = GetWin_Text( this, IDC_EDIT_NAME_KEY_2 );
	dwNameINDEX = GetWin_Num_int( this, IDC_EDIT_NAME_INDEX_2 );

	m_pEdit->SetToMapID(2, sMapID);
	m_pEdit->SetToGateID(2, dwGateID);
	m_pEdit->SetNameKEY(2, strNameKEY.c_str() );
	m_pEdit->SetNameINDEX(2, dwNameINDEX );

	//
	sMapID.wMainID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_3 );
	sMapID.wSubID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_3 );
	dwGateID = GetWin_Num_int( this, IDC_EDIT_TO_GATE_3 );
	strNameKEY = GetWin_Text( this, IDC_EDIT_NAME_KEY_3 );
	dwNameINDEX = GetWin_Num_int( this, IDC_EDIT_NAME_INDEX_3 );

	m_pEdit->SetToMapID(3, sMapID);
	m_pEdit->SetToGateID(3, dwGateID);
	m_pEdit->SetNameKEY(3, strNameKEY.c_str() );
	m_pEdit->SetNameINDEX(3, dwNameINDEX );

	//
	sMapID.wMainID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_4 );
	sMapID.wSubID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_4 );
	dwGateID = GetWin_Num_int( this, IDC_EDIT_TO_GATE_4 );
	strNameKEY = GetWin_Text( this, IDC_EDIT_NAME_KEY_4 );
	dwNameINDEX = GetWin_Num_int( this, IDC_EDIT_NAME_INDEX_4 );

	m_pEdit->SetToMapID(4, sMapID);
	m_pEdit->SetToGateID(4, dwGateID);
	m_pEdit->SetNameKEY(4, strNameKEY.c_str() );
	m_pEdit->SetNameINDEX(4, dwNameINDEX );

	//
	sMapID.wMainID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_5 );
	sMapID.wSubID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_5 );
	dwGateID = GetWin_Num_int( this, IDC_EDIT_TO_GATE_5 );
	strNameKEY = GetWin_Text( this, IDC_EDIT_NAME_KEY_5 );
	dwNameINDEX = GetWin_Num_int( this, IDC_EDIT_NAME_INDEX_5 );

	m_pEdit->SetToMapID(5, sMapID);
	m_pEdit->SetToGateID(5, dwGateID);
	m_pEdit->SetNameKEY(5, strNameKEY.c_str() );
	m_pEdit->SetNameINDEX(5, dwNameINDEX );

	//
	sMapID.wMainID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_6 );
	sMapID.wSubID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_6 );
	dwGateID = GetWin_Num_int( this, IDC_EDIT_TO_GATE_6 );
	strNameKEY = GetWin_Text( this, IDC_EDIT_NAME_KEY_6 );
	dwNameINDEX = GetWin_Num_int( this, IDC_EDIT_NAME_INDEX_6 );

	m_pEdit->SetToMapID(6, sMapID);
	m_pEdit->SetToGateID(6, dwGateID);
	m_pEdit->SetNameKEY(6, strNameKEY.c_str() );
	m_pEdit->SetNameINDEX(6, dwNameINDEX );

	//
	sMapID.wMainID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_MID_7 );
	sMapID.wSubID = GetWin_Num_int( this, IDC_EDIT_TO_MAP_SID_7 );
	dwGateID = GetWin_Num_int( this, IDC_EDIT_TO_GATE_7 );
	strNameKEY = GetWin_Text( this, IDC_EDIT_NAME_KEY_7 );
	dwNameINDEX = GetWin_Num_int( this, IDC_EDIT_NAME_INDEX_7 );

	m_pEdit->SetToMapID(7, sMapID);
	m_pEdit->SetToGateID(7, dwGateID);
	m_pEdit->SetNameKEY(7, strNameKEY.c_str() );
	m_pEdit->SetNameINDEX(7, dwNameINDEX );

	

	DWORD flag=0;
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_GATE_IN ), flag, DxLandGate::GATE_IN );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_GATE_OUT ), flag, DxLandGate::GATE_OUT );

	D3DXVECTOR2 div;
	div.x = GetWin_Num_float( this, IDC_EDIT_GATE_GATEINTERVAL_X );
	div.y = GetWin_Num_float( this, IDC_EDIT_GATE_GATEINTERVAL_Y );

	BOOL bNewSystem = GetWin_Check( this, IDC_CHECK_GATE_NEW_SYSTEM );
	
	D3DXVECTOR3 vmin = CEditorLevelView::GetView()->m_DxEditBox.m_vMin;
	D3DXVECTOR3 vmax = CEditorLevelView::GetView()->m_DxEditBox.m_vMax;

	m_pEdit->SetName( gatename.c_str() );
	m_pEdit->SetInfo( gatedesc.c_str() );
	m_pEdit->SetGate( flag, gateid, div );
	m_pEdit->SetBox( vmax, vmin );
	m_pEdit->MakeStartPos( m_pGLLand->GetNavi() );
	m_pEdit->SetNewSystem( bNewSystem );
	m_pEdit->CountToMap();
	
	return TRUE;
}

void CPageEditGate::JumpTo( D3DXVECTOR3 vMax, D3DXVECTOR3 vMin )
{
	D3DXVECTOR3	vec3Center;
	vec3Center = ((vMin + vMax ) / 2);
	DxViewPort::GetInstance().CameraJump( vec3Center );
}

void CPageEditGate::OnBnClickedButtonSave()
{
	if ( DataSave() )
	{
		if ( m_pSheetTab )
			m_pSheetTab->PageActive( LEVELEDIT_PAGE_GATELIST );
	}
}

void CPageEditGate::OnBnClickedButtonClose()
{
	if ( m_pSheetTab )
		m_pSheetTab->PageActive( LEVELEDIT_PAGE_GATELIST );
}

void CPageEditGate::OnBnClickedButtonRegnew()
{
	if ( !m_pGLLand )	return;

	D3DXVECTOR3 vPOS = CEditorLevelView::GetView()->m_vPos;

	std::string strName = GetWin_Text( this, IDC_EDIT_GATE_NAME );
	DWORD dwGateID = GetWin_Num_int( this, IDC_EDIT_GATE_ID );

	if ( strName.size() <= 0 )
	{
		CDebugSet::MsgBox( "Name Empty!" );
		return;
	}

	if ( GateNameCheck( m_pGLLand, strName.c_str() ) )
	{
		CDebugSet::MsgBox( "Name: %s is already used! Create New name!", strName.c_str() );
		return;
	}

	if ( GateIDCheck( m_pGLLand, dwGateID ) )
	{
		CDebugSet::MsgBox( "Gate ID: %d is already used, Create New ID!", dwGateID );
		return;
	}

	PDXLANDGATE pNewGate = new DxLandGate;
	pNewGate->SetGateID( dwGateID );
	pNewGate->SetName( strName.c_str() );
	pNewGate->SetPos( vPOS );
	pNewGate->MakeStartPos( m_pGLLand->GetNavi() );

	m_pGLLand->GetLandGateMan().AddLandGate( pNewGate );
	m_pEdit = m_pGLLand->GetLandGateMan().FindLandGate( pNewGate->GetGateID() );

	if ( m_pGLLand && m_pEdit )
	{
		m_pGLLand->GetLandGateMan().BuildTree();

		InitUI( TRUE );
		DataShow();
		JumpTo( m_pEdit->GetMax(), m_pEdit->GetMin() );
	}
	else
	{
		m_pGLLand->GetLandGateMan().DelLandGate( pNewGate->GetGateID() );
		CDebugSet::MsgBox( "Error Register New Gate: (%d) %s", dwGateID, strName.c_str() );
		return;
	}
}

void CPageEditGate::OnBnClickedButtonCheck()
{
	if ( !m_pGLLand )	return;
	if ( !m_pEdit )		return;

	std::string strName = GetWin_Text( this, IDC_EDIT_GATE_NAME );
	DWORD dwGateID = GetWin_Num_int( this, IDC_EDIT_GATE_ID );

	if ( strcmp( m_pEdit->GetName(), strName.c_str() ) )
	{
		if ( GateNameCheck( m_pGLLand, strName.c_str() ) )
		{
			CDebugSet::MsgBox( "Name: %s is already used! Create New name!", strName.c_str() );
		}else{
			CDebugSet::MsgBox( "Name: %s is Valid!", strName.c_str() );
		}
	}

	if ( m_pEdit->GetGateID() != dwGateID )
	{
		if ( GateIDCheck( m_pGLLand, dwGateID ) )
		{
			CDebugSet::MsgBox( "Gate ID: %d is already used, Create New ID!", dwGateID );
		}else{
			CDebugSet::MsgBox( "Gate ID: %d is Valid!", dwGateID );
		}
	}
}

void CPageEditGate::OnBnClickedButtonNew()
{
	if ( !m_pGLLand )	return;

	SetWin_Num_int( this, IDC_EDIT_GATE_ID, GateNewID( m_pGLLand ) );
	SetWin_Text( this, IDC_EDIT_GATE_NAME, GateNameNew( m_pGLLand ).c_str() );
}

void CPageEditGate::OnBnClickedButtonMousepos()
{
	if ( !m_pGLLand )	return;
	if ( !m_pEdit )		return;

	//rebuild the gate pos to current mouse pos
	D3DXVECTOR3 vPOS = CEditorLevelView::GetView()->m_vPos;
	m_pEdit->SetPos( vPOS );
	m_pEdit->MakeStartPos( m_pGLLand->GetNavi() );
	CEditorLevelView::GetView()->EditBoxSet( TRUE );
	CEditorLevelView::GetView()->m_DxEditBox.SetBox( m_pEdit->GetMax(), m_pEdit->GetMin() );
}

void CPageEditGate::OnBnClickedButtonApply()
{
	DataSave();

	SetWin_Num_int( this, IDC_EDIT_TO_MAP_NUM, m_pEdit->GetToMapNum() );
}

void CPageEditGate::CheckMap( int MID, int SID, int NAME )
{
	WORD wMID = GetWin_Num_int( this, MID );
	WORD wSID = GetWin_Num_int( this, SID );

	std::string strMAPNAME = "Unknown Map";
	SMAPNODE* pMap = CEditorLevelView::GetView()->m_sMap.FindMapNode( SNATIVEID( wMID, wSID ) );
	if ( pMap )
		strMAPNAME = pMap->strMapName.c_str();

	SetWin_Text( this, NAME, strMAPNAME.c_str() );
}

void CPageEditGate::FindMap( int MID, int SID )
{
	CDlgFindMap dlg( this );
	if ( dlg.DoModal() == IDOK )
	{
		SMAPNODE* pMap = CEditorLevelView::GetView()->m_sMap.FindMapNode( dlg.m_sSELECTED.sMAPID );
		if ( pMap )
		{
			SetWin_Num_int( this, MID, pMap->sNativeID.wMainID );
			SetWin_Num_int( this, SID, pMap->sNativeID.wSubID );
		}
	}
}

void CPageEditGate::ResetMap( int MID, int SID, int GATE, int KEY, int INDEX )
{
	SNATIVEID sid(false);
	SetWin_Num_int( this, MID, sid.wMainID );
	SetWin_Num_int( this, SID, sid.wMainID );
	SetWin_Num_int( this, GATE, 0 );
	SetWin_Text( this, KEY, "" );
	SetWin_Num_int( this, INDEX, -1 );
}

void CPageEditGate::OnBnClickedButtonToMap0()
{
	FindMap( IDC_EDIT_TO_MAP_MID_0, IDC_EDIT_TO_MAP_SID_0 );
}

void CPageEditGate::OnBnClickedButtonToMap1()
{
	FindMap( IDC_EDIT_TO_MAP_MID_1, IDC_EDIT_TO_MAP_SID_1 );
}

void CPageEditGate::OnBnClickedButtonToMap2()
{
	FindMap( IDC_EDIT_TO_MAP_MID_2, IDC_EDIT_TO_MAP_SID_2 );
}

void CPageEditGate::OnBnClickedButtonToMap3()
{
	FindMap( IDC_EDIT_TO_MAP_MID_3, IDC_EDIT_TO_MAP_SID_3 );
}

void CPageEditGate::OnBnClickedButtonToMap4()
{
	FindMap( IDC_EDIT_TO_MAP_MID_4, IDC_EDIT_TO_MAP_SID_4 );
}

void CPageEditGate::OnBnClickedButtonToMap5()
{
	FindMap( IDC_EDIT_TO_MAP_MID_5, IDC_EDIT_TO_MAP_SID_5 );
}

void CPageEditGate::OnBnClickedButtonToMap6()
{
	FindMap( IDC_EDIT_TO_MAP_MID_6, IDC_EDIT_TO_MAP_SID_6 );
}

void CPageEditGate::OnBnClickedButtonToMap7()
{
	FindMap( IDC_EDIT_TO_MAP_MID_7, IDC_EDIT_TO_MAP_SID_7 );
}

void CPageEditGate::OnEnChangeEditToMapMid0()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_0, IDC_EDIT_TO_MAP_SID_0, IDC_EDIT_TO_MAP_NAME_0 );
}

void CPageEditGate::OnEnChangeEditToMapSid0()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_0, IDC_EDIT_TO_MAP_SID_0, IDC_EDIT_TO_MAP_NAME_0 );
}

void CPageEditGate::OnEnChangeEditToMapMid1()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_1, IDC_EDIT_TO_MAP_SID_1, IDC_EDIT_TO_MAP_NAME_1 );
}

void CPageEditGate::OnEnChangeEditToMapSid1()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_1, IDC_EDIT_TO_MAP_SID_1, IDC_EDIT_TO_MAP_NAME_1 );
}

void CPageEditGate::OnEnChangeEditToMapMid2()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_2, IDC_EDIT_TO_MAP_SID_2, IDC_EDIT_TO_MAP_NAME_2 );
}

void CPageEditGate::OnEnChangeEditToMapSid2()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_2, IDC_EDIT_TO_MAP_SID_2, IDC_EDIT_TO_MAP_NAME_2 );
}

void CPageEditGate::OnEnChangeEditToMapMid3()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_3, IDC_EDIT_TO_MAP_SID_3, IDC_EDIT_TO_MAP_NAME_3 );
}

void CPageEditGate::OnEnChangeEditToMapSid3()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_3, IDC_EDIT_TO_MAP_SID_3, IDC_EDIT_TO_MAP_NAME_3 );
}

void CPageEditGate::OnEnChangeEditToMapMid4()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_4, IDC_EDIT_TO_MAP_SID_4, IDC_EDIT_TO_MAP_NAME_4 );
}

void CPageEditGate::OnEnChangeEditToMapSid4()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_4, IDC_EDIT_TO_MAP_SID_4, IDC_EDIT_TO_MAP_NAME_4 );
}

void CPageEditGate::OnEnChangeEditToMapMid5()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_5, IDC_EDIT_TO_MAP_SID_5, IDC_EDIT_TO_MAP_NAME_5 );
}

void CPageEditGate::OnEnChangeEditToMapSid5()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_5, IDC_EDIT_TO_MAP_SID_5, IDC_EDIT_TO_MAP_NAME_5 );
}

void CPageEditGate::OnEnChangeEditToMapMid6()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_6, IDC_EDIT_TO_MAP_SID_6, IDC_EDIT_TO_MAP_NAME_6 );
}

void CPageEditGate::OnEnChangeEditToMapSid6()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_6, IDC_EDIT_TO_MAP_SID_6, IDC_EDIT_TO_MAP_NAME_6 );
}

void CPageEditGate::OnEnChangeEditToMapMid7()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_7, IDC_EDIT_TO_MAP_SID_7, IDC_EDIT_TO_MAP_NAME_7 );
}

void CPageEditGate::OnEnChangeEditToMapSid7()
{
	CheckMap( IDC_EDIT_TO_MAP_MID_7, IDC_EDIT_TO_MAP_SID_7, IDC_EDIT_TO_MAP_NAME_7 );
}

void CPageEditGate::OnBnClickedButtonToMapReset0()
{
	ResetMap( IDC_EDIT_TO_MAP_MID_0, IDC_EDIT_TO_MAP_SID_0, IDC_EDIT_TO_GATE_0, IDC_EDIT_NAME_KEY_0, IDC_EDIT_NAME_INDEX_0 );
}

void CPageEditGate::OnBnClickedButtonToMapReset1()
{
	ResetMap( IDC_EDIT_TO_MAP_MID_1, IDC_EDIT_TO_MAP_SID_1, IDC_EDIT_TO_GATE_1, IDC_EDIT_NAME_KEY_1, IDC_EDIT_NAME_INDEX_1 );
}

void CPageEditGate::OnBnClickedButtonToMapReset2()
{
	ResetMap( IDC_EDIT_TO_MAP_MID_2, IDC_EDIT_TO_MAP_SID_2, IDC_EDIT_TO_GATE_2, IDC_EDIT_NAME_KEY_2, IDC_EDIT_NAME_INDEX_2 );
}

void CPageEditGate::OnBnClickedButtonToMapReset3()
{
	ResetMap( IDC_EDIT_TO_MAP_MID_3, IDC_EDIT_TO_MAP_SID_3, IDC_EDIT_TO_GATE_3, IDC_EDIT_NAME_KEY_3, IDC_EDIT_NAME_INDEX_3 );
}

void CPageEditGate::OnBnClickedButtonToMapReset4()
{
	ResetMap( IDC_EDIT_TO_MAP_MID_4, IDC_EDIT_TO_MAP_SID_4, IDC_EDIT_TO_GATE_4, IDC_EDIT_NAME_KEY_4, IDC_EDIT_NAME_INDEX_4 );
}

void CPageEditGate::OnBnClickedButtonToMapReset5()
{
	ResetMap( IDC_EDIT_TO_MAP_MID_5, IDC_EDIT_TO_MAP_SID_5, IDC_EDIT_TO_GATE_5, IDC_EDIT_NAME_KEY_5, IDC_EDIT_NAME_INDEX_5 );
}

void CPageEditGate::OnBnClickedButtonToMapReset6()
{
	ResetMap( IDC_EDIT_TO_MAP_MID_6, IDC_EDIT_TO_MAP_SID_6, IDC_EDIT_TO_GATE_6, IDC_EDIT_NAME_KEY_6, IDC_EDIT_NAME_INDEX_6 );
}

void CPageEditGate::OnBnClickedButtonToMapReset7()
{
	ResetMap( IDC_EDIT_TO_MAP_MID_7, IDC_EDIT_TO_MAP_SID_7, IDC_EDIT_TO_GATE_7, IDC_EDIT_NAME_KEY_7, IDC_EDIT_NAME_INDEX_7 );
}


