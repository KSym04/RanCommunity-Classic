#include "StdAfx.h"
#include "./ModernPartySlotText.h"

#include "./BasicTextBox.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernPartySlotText::CModernPartySlotText () :
	m_fUpdateTime(0.0f),
	m_pTextName ( NULL ),
	m_pTextLevel( NULL ),
	m_pTextPos( NULL ),
	m_pTextMap( NULL ),
	m_bDie( false ),
	m_bSel( false )
{
	for ( int i = 0; i < 3; ++ i )
	{
		m_pSchool[i] = NULL;
	}

	m_strName = "";
	m_strPos = "";
	m_strMap = "";
	m_strLevel = "";
}

CModernPartySlotText::~CModernPartySlotText ()
{
}

void CModernPartySlotText::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
	int nTextAlign = TEXT_ALIGN_CENTER_X;

	CString strSchool[3] = 
	{
		"MODERN_PARTY_INFO_SCHOOL_SM",
		"MODERN_PARTY_INFO_SCHOOL_HA",
		"MODERN_PARTY_INFO_SCHOOL_BH"
	};

	for ( int i = 0; i < 3; ++ i )
	{
		m_pSchool[i] = CreateControl( strSchool[i].GetString() );
	}

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "MODERN_PARTY_INFO_SLOT_NAME" );
	m_pTextName->SetFont ( pFont8 );
	m_pTextName->SetTextAlign ( nTextAlign );	
	RegisterControl ( m_pTextName );

	m_pTextLevel = new CBasicTextBox;
	m_pTextLevel->CreateSub ( this, "MODERN_PARTY_INFO_SLOT_LEVEL" );
	m_pTextLevel->SetFont ( pFont8 );
	m_pTextLevel->SetTextAlign ( nTextAlign );	
	RegisterControl ( m_pTextLevel );

	m_pTextPos = new CBasicTextBox;
	m_pTextPos->CreateSub ( this, "MODERN_PARTY_INFO_SLOT_POSITION" );
	m_pTextPos->SetFont ( pFont8 );
	m_pTextPos->SetTextAlign ( nTextAlign );	
	RegisterControl ( m_pTextPos );

	m_pTextMap = new CBasicTextBox;
	m_pTextMap->CreateSub ( this, "MODERN_PARTY_INFO_SLOT_LOCATE" );
	m_pTextMap->SetFont ( pFont8 );
	m_pTextMap->SetTextAlign ( nTextAlign );	
	RegisterControl ( m_pTextMap );
}

CUIControl*	CModernPartySlotText::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

void CModernPartySlotText::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fUpdateTime += fElapsedTime;
	if( m_fUpdateTime >= MODERN_PARTY_SLOT_TEXT_UPDATE_TIME )
	{
		ApplyText();
		m_fUpdateTime = 0.0f;
	}
}

void CModernPartySlotText::SetSchool( int nSchool )
{
	for ( int i = 0; i < 3; ++ i )
	{
		if ( m_pSchool[i] )
			m_pSchool[i]->SetVisibleSingle( FALSE );
	}

	if ( nSchool < 0 || nSchool >=3 )
		return;

	if ( m_pSchool[nSchool] )
		m_pSchool[nSchool]->SetVisibleSingle( TRUE );
}

void CModernPartySlotText::SetPlayerName ( CString strName )
{
	m_strName = strName.GetString();
}

void CModernPartySlotText::SetPlayerPos ( CString strPos )
{
	m_strPos = strPos.GetString();
}

void CModernPartySlotText::SetMapName ( CString strMapName )
{
	m_strMap = strMapName.GetString();
}

void CModernPartySlotText::SetLevel( CString strLevel )
{
	m_strLevel = strLevel.GetString();
}

void CModernPartySlotText::ApplyText()
{
	if ( m_pTextName )	m_pTextName->SetText( "" );
	if ( m_pTextPos )	m_pTextPos->SetText( "" );
	if ( m_pTextMap )	m_pTextMap->SetText( "" );
	if ( m_pTextLevel )	m_pTextLevel->SetText( "" );

	DWORD dwCOLOR = NS_UITEXTCOLOR::WHITE;

	if ( m_bDie && m_bSel )
	{
		dwCOLOR = NS_UITEXTCOLOR::YELLOW;
	}
	else if ( m_bDie )
	{
		dwCOLOR = NS_UITEXTCOLOR::RED;
	}
	else if ( m_bSel )
	{
		dwCOLOR = NS_UITEXTCOLOR::YELLOW;
	}

	if ( m_pTextName )
		m_pTextName->SetText( m_strName.GetString(), dwCOLOR );

	if ( m_pTextPos )
		m_pTextPos->SetText( m_strPos.GetString(), dwCOLOR );

	if ( m_pTextMap )
		m_pTextMap->SetText( m_strMap.GetString(), dwCOLOR );

	if ( m_pTextLevel )
		m_pTextLevel->SetText( m_strLevel.GetString(), dwCOLOR );
}

void CModernPartySlotText::RefreshText( bool bSel )
{
	if ( m_bSel != bSel )
	{
		m_bSel = bSel;
	}
}

void CModernPartySlotText::SetDie( bool bDIE )
{
	if ( m_bDie != bDIE )
	{
		m_bDie = bDIE;
	}
}

void CModernPartySlotText::Reset()
{
	for ( int i = 0; i < 3; ++ i )
	{
		if ( m_pSchool[i] )		
			m_pSchool[i]->SetVisibleSingle( FALSE );
	}

	m_bDie = false;
	m_bSel = false;

	m_strName = "";
	m_strPos = "";
	m_strMap = "";
	m_strLevel = "";

	if ( m_pTextName )	m_pTextName->SetText( "" );
	if ( m_pTextPos )	m_pTextPos->SetText( "" );
	if ( m_pTextMap )	m_pTextMap->SetText( "" );
	if ( m_pTextLevel )	m_pTextLevel->SetText( "" );
}