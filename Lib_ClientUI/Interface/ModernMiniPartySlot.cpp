#include "StdAfx.h"
#include "./ModernMiniPartySlot.h"

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

CModernMiniPartySlot::CModernMiniPartySlot () :
	m_pPlayerName( NULL ),
	m_pPlayerLevel( NULL ),
	m_pPlayerSlotText( NULL ),
	m_pSlotMaster( NULL ),
	m_pSlotSubMaster( NULL ),
	m_pSelect( FALSE ),
	m_bDie( false ),
	m_bSel( false )
{
	for ( int i = 0; i < 3; ++ i )
	{
		m_pSchool[i] = NULL;
	}

	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		m_pHP[i] = NULL;
	}

	m_strName = "";
	m_strLevel = "";
}

CModernMiniPartySlot::~CModernMiniPartySlot ()
{
}

void CModernMiniPartySlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
	int nTextAlign = TEXT_ALIGN_CENTER_X;
	D3DCOLOR dwFontColor = NS_UITEXTCOLOR::DEFAULT;
	D3DCOLOR dwNameColor = NS_UITEXTCOLOR::PARTYNAME;

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_MINIPARTY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList( "MODERN_MINI_PARTY_EXPEDITION_INFO_MEMBER_SLOT_NUM" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_MINIPARTY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList( "MODERN_MINI_PARTY_EXPEDITION_INFO_MEMBER_INFORMATION" );
	RegisterControl ( pLineBox );

	std::string strHpOver[GLCI_NUM_8CLASS] = 
	{
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_FIGHTER",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_ARMS",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_ARCHER",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_SPIRIT",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_EXTREME",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_EXTREME",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_FIGHTER",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_ARMS",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_ARCHER",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_SPIRIT",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_SCIENTIST",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_SCIENTIST",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_ASSASSIN",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_ASSASSIN",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_TRICKER",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_HP_OVERIMAGE_TRICKER",
	};

	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		CBasicProgressBar*	pProgressBar = new CBasicProgressBar;
		pProgressBar->CreateSub ( this, "MODERN_MINI_PARTY_EXPEDITION_INFO_HP_IMAGE" );
		pProgressBar->CreateOverImage ( (char*)strHpOver[i].c_str() );
		pProgressBar->SetPercent ( 0.3f );
		pProgressBar->SetVisibleSingle( FALSE );
		RegisterControl ( pProgressBar );
		m_pHP[i]= pProgressBar;
	}

	CString strSchool[3] = 
	{
		"MODERN_MINI_PARTY_EXPEDITION_INFO_SCHOOL_SM",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_SCHOOL_HA",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_SCHOOL_BH"
	};

	for ( int i = 0; i < 3; ++ i )
	{
		m_pSchool[i] = CreateControl( strSchool[i].GetString() );
	}

	m_pPlayerName = CreateStaticControl ( "MODERN_MINI_PARTY_EXPEDITION_INFO_MEMBER_NAME", pFont8, dwNameColor, nTextAlign );
	m_pPlayerLevel = CreateStaticControl ( "MODERN_MINI_PARTY_EXPEDITION_INFO_MEMBER_LEVEL", pFont8, dwNameColor, nTextAlign );
	m_pPlayerSlotText = CreateStaticControl ( "MODERN_MINI_PARTY_EXPEDITION_INFO_MEMBER_SLOT_NUM_TEXT", pFont8, dwNameColor, nTextAlign );

	m_pSlotMaster = CreateControl ( "MODERN_MINI_PARTY_EXPEDITION_INFO_MEMBER_SLOT_MASTER" );
	m_pSlotSubMaster = CreateControl ( "MODERN_MINI_PARTY_EXPEDITION_INFO_MEMBER_SLOT_SUB_MASTER" );
	m_pSlotMaster->SetVisibleSingle( FALSE );
	m_pSlotSubMaster->SetVisibleSingle( FALSE );

	m_pSelect = new CBasicLineBox;
	m_pSelect->CreateSub ( this, "MODERN_MINI_PARTY_EXPEDITION_INFO_SLOT_LINE_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pSelect->CreateBaseBoxModernMiniPartyExpedition( "MODERN_MINI_PARTY_EXPEDITION_INFO_SLOT_SELECT" );
	RegisterControl ( m_pSelect );

}

CUIControl*	CModernMiniPartySlot::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

CBasicTextBox* CModernMiniPartySlot::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );
	return pStaticText;
}

void CModernMiniPartySlot::SetNumber ( int nSlotNumber )
{
	if ( nSlotNumber < 0 && MAXPARTY <= nSlotNumber )
	{
		GASSERT ( 0 && "클래스타입의 인덱스범위가 경계를 벗어납니다." );
		return ;
	}

	CString strTemp;
	strTemp.Format( "%d", nSlotNumber );
	m_pPlayerSlotText->SetText( strTemp.GetString() );

	SetMaster( nSlotNumber == 0 );

}

void CModernMiniPartySlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	ApplyText();
}

void CModernMiniPartySlot::SetHP ( float fPercent, int nClassType )
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

void CModernMiniPartySlot::SetSchool ( int nSchoolType )
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

void CModernMiniPartySlot::ApplyText()
{
	if ( m_pPlayerName )	m_pPlayerName->SetText( "" );
	if ( m_pPlayerLevel )	m_pPlayerLevel->SetText( "" );

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

	if ( m_pPlayerName )
		m_pPlayerName->SetText( m_strName.GetString(), dwCOLOR );

	if ( m_pPlayerLevel )
		m_pPlayerLevel->SetText( m_strLevel.GetString(), dwCOLOR );
}

void CModernMiniPartySlot::SetSelect( bool b )
{
	m_pSelect->SetVisibleSingle( b );
	m_bSel = b;
	ApplyText();
}

void CModernMiniPartySlot::SetDie( bool b )
{
	m_bDie = b;
	ApplyText();
}

void CModernMiniPartySlot::SetMaster( bool bMaster )
{
	m_pSlotMaster->SetVisibleSingle( bMaster );
}