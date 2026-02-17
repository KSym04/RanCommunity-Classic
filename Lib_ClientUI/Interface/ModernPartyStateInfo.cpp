#include "StdAfx.h"
#include "./ModernPartyStateInfo.h"

#include "./BasicLineBox.h"
#include "./BasicProgressBar.h"
#include "./BasicTextBox.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernPartyStateInfo::CModernPartyStateInfo () :
	m_pTextName( NULL ),
	m_pTextHP( NULL ),
	m_bDie( false )
{
	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		m_pHP[i] = NULL;
	}

	for ( int i = 0; i < 3; ++ i )
	{
		m_pSchool[i] = NULL;
	}

	m_strName = "";
	m_strHP = "";
}

CModernPartyStateInfo::~CModernPartyStateInfo ()
{
}

void CModernPartyStateInfo::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_MEMBER_STATE_SLOT_INFORMATION_NAME_BOX" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_MEMBER_STATE_SLOT_INFORMATION_HP_BOX" );
	RegisterControl ( pBasicLineBox );

	std::string strHpOver[GLCI_NUM_8CLASS] = 
	{
		"MODERN_PARTY_STATE_HP_OVERIMAGE_FIGHTER",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_ARMS",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_ARCHER",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_SPIRIT",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_EXTREME",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_EXTREME",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_FIGHTER",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_ARMS",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_ARCHER",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_SPIRIT",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_SCIENTIST",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_SCIENTIST",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_ASSASSIN",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_ASSASSIN",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_TRICKER",
		"MODERN_PARTY_STATE_HP_OVERIMAGE_TRICKER",
	};

	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		CBasicProgressBar*	pProgressBar = new CBasicProgressBar;
		pProgressBar->CreateSub ( this, "MODERN_PARTY_STATE_HP_IMAGE" );
		pProgressBar->CreateOverImage ( (char*)strHpOver[i].c_str() );
		pProgressBar->SetPercent ( 0.3f );
		pProgressBar->SetVisibleSingle( FALSE );
		RegisterControl ( pProgressBar );
		m_pHP[i]= pProgressBar;
	}

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "MODERN_MEMBER_STATE_SLOT_INFORMATION_NAME" );
	m_pTextName->SetFont ( pFont );
	m_pTextName->SetTextAlign ( TEXT_ALIGN_LEFT );	
	RegisterControl ( m_pTextName );

	m_pTextHP = new CBasicTextBox;
	m_pTextHP->CreateSub ( this, "MODERN_MEMBER_STATE_SLOT_INFORMATION_HP" );
	m_pTextHP->SetFont ( pFont );
	m_pTextHP->SetTextAlign ( TEXT_ALIGN_LEFT );	
	RegisterControl ( m_pTextHP );

	CString strSchool[3] = 
	{
		"MODERN_MEMBER_STATE_SCHOOL_SM",
		"MODERN_MEMBER_STATE_SCHOOL_HA",
		"MODERN_MEMBER_STATE_SCHOOL_BH"
	};

	for ( int i = 0; i < 3; ++ i )
	{
		m_pSchool[i] = CreateControl( strSchool[i].GetString() );
	}
}

CUIControl*	CModernPartyStateInfo::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

void CModernPartyStateInfo::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	ApplyText();
}

void CModernPartyStateInfo::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CModernPartyStateInfo::SetSchool ( int nSchoolType )
{
	for ( int i = 0; i < 3; ++ i )
	{
		if ( m_pSchool[i] )
			m_pSchool[i]->SetVisibleSingle( FALSE );
	}

	if ( nSchoolType < 0 || nSchoolType >=3 )
		return;

	if ( m_pSchool[nSchoolType] )
		m_pSchool[nSchoolType]->SetVisibleSingle( TRUE );
}

void CModernPartyStateInfo::SetPlayerName ( CString strName )
{
	m_strName = strName.GetString();
}

void CModernPartyStateInfo::SetDie( bool bDIE )
{
	if ( m_bDie != bDIE )
	{
		m_bDie = bDIE;
	}
}

void CModernPartyStateInfo::SetHP ( float fPercent, int nClassType )
{
	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		if ( i != nClassType )
			m_pHP[i]->SetVisibleSingle(FALSE);
	}

	if ( m_pHP[nClassType] ) 
	{
		if ( !m_pHP[nClassType]->IsVisible() )
		{
			m_pHP[nClassType]->SetVisibleSingle(TRUE);
		}

		m_pHP[nClassType]->SetPercent ( fPercent );
	}
}

void CModernPartyStateInfo::SetHP ( CString str )
{
	m_strHP = str.GetString();
}

void CModernPartyStateInfo::ApplyText()
{
	if ( m_pTextName )	m_pTextName->SetText( "" );
	if ( m_pTextHP )	m_pTextHP->SetText( "" );
	
	DWORD dwCOLOR = NS_UITEXTCOLOR::WHITE;

	if ( m_bDie )
	{
		dwCOLOR = NS_UITEXTCOLOR::RED;
	}

	if ( m_pTextName )
		m_pTextName->SetText( m_strName.GetString(), dwCOLOR );

	if ( m_pTextHP )	
		m_pTextHP->SetText( m_strHP.GetString(), dwCOLOR );

}