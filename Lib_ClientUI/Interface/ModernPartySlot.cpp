#include "StdAfx.h"
#include "./ModernPartySlot.h"

#include "./BasicTextBox.h"
#include "./BasicTextButton.h"
#include "./BasicLineBox.h"

#include "./ModernPartySlotText.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "./ModalWindow.h"
#include "./ModalCallerID.h"

#include "../../Lib_Client/G-Logic/GLFriendClient.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernPartySlot::CModernPartySlot () :
	m_pClassSelect( NULL ),
	m_pSlotText( NULL ),
	m_pSlotMaster( NULL ),
	m_pSlotSubMaster( NULL ),
	m_pSlotNumText( NULL ),
	m_pClassDIE( NULL ),
	m_dwGaeaID(GAEAID_NULL)
{
	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		m_pClassLive[i] = NULL;
		m_pClassDead[i] = NULL;
	}
}

CModernPartySlot::~CModernPartySlot ()
{
}


void CModernPartySlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
	int nTextAlign = TEXT_ALIGN_CENTER_X;

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_PARTY_INFO_SLOT_NUM" );
	RegisterControl ( pBasicLineBox );

	CString strClassLive[GLCI_NUM_8CLASS] = 
	{
		"MODERN_PARTY_INFO_BRAWLER_MALE_LIVE",
		"MODERN_PARTY_INFO_SWORDSMAN_MALE_LIVE",
		"MODERN_PARTY_INFO_ARCHER_FEMALE_LIVE",
		"MODERN_PARTY_INFO_SHAMAN_FEMALE_LIVE",
		"MODERN_PARTY_INFO_EXTREME_MALE_LIVE",
		"MODERN_PARTY_INFO_EXTREME_FEMALE_LIVE",
		"MODERN_PARTY_INFO_BRAWLER_FEMALE_LIVE",
		"MODERN_PARTY_INFO_SWORDSMAN_FEMALE_LIVE",
		"MODERN_PARTY_INFO_ARCHER_MALE_LIVE",
		"MODERN_PARTY_INFO_SHAMAN_MALE_LIVE",
		"MODERN_PARTY_INFO_SCIENTIST_MALE_LIVE",
		"MODERN_PARTY_INFO_SCIENTIST_FEMALE_LIVE",
		"MODERN_PARTY_INFO_ASSASSIN_MALE_LIVE",
		"MODERN_PARTY_INFO_ASSASSIN_FEMALE_LIVE",
		"MODERN_PARTY_INFO_TRICKER_MALE_LIVE",
		"MODERN_PARTY_INFO_TRICKER_FEMALE_LIVE",
	};

	CString strClassDead[GLCI_NUM_8CLASS] = 
	{
		"MODERN_PARTY_INFO_BRAWLER_MALE_DIE",
		"MODERN_PARTY_INFO_SWORDSMAN_MALE_DIE",
		"MODERN_PARTY_INFO_ARCHER_FEMALE_DIE",
		"MODERN_PARTY_INFO_SHAMAN_FEMALE_DIE",
		"MODERN_PARTY_INFO_EXTREME_MALE_DIE",
		"MODERN_PARTY_INFO_EXTREME_FEMALE_DIE",
		"MODERN_PARTY_INFO_BRAWLER_FEMALE_DIE",
		"MODERN_PARTY_INFO_SWORDSMAN_FEMALE_DIE",
		"MODERN_PARTY_INFO_ARCHER_MALE_DIE",
		"MODERN_PARTY_INFO_SHAMAN_MALE_DIE",
		"MODERN_PARTY_INFO_SCIENTIST_MALE_DIE",
		"MODERN_PARTY_INFO_SCIENTIST_FEMALE_DIE",
		"MODERN_PARTY_INFO_ASSASSIN_MALE_DIE",
		"MODERN_PARTY_INFO_ASSASSIN_FEMALE_DIE",
		"MODERN_PARTY_INFO_TRICKER_MALE_DIE",
		"MODERN_PARTY_INFO_TRICKER_FEMALE_DIE",
	};

	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		m_pClassLive[i] = CreateControl ( strClassLive[i].GetString() );
		m_pClassDead[i] = CreateControl ( strClassDead[i].GetString() );
	}

	m_pSlotNumText = new CBasicTextBox;
	m_pSlotNumText->CreateSub ( this, "MODERN_PARTY_INFO_SLOT_NUM_TEXT" );
	m_pSlotNumText->SetFont ( pFont8 );
	m_pSlotNumText->SetTextAlign ( nTextAlign );	
	RegisterControl ( m_pSlotNumText );

	m_pSlotText = new CModernPartySlotText;
	m_pSlotText->CreateSub ( this, "MODERN_PARTY_INFO_SLOT_TEXT" );
	m_pSlotText->CreateSubControl ();
	RegisterControl ( m_pSlotText );

	m_pSlotMaster = CreateControl ( "MODERN_PARTY_INFO_SLOT_MASTER" );
	m_pSlotSubMaster = CreateControl ( "MODERN_PARTY_INFO_SLOT_SUB_MASTER" );
	m_pClassDIE = CreateControl ( "MODERN_PARTY_INFO_DIE" );
	m_pClassSelect = CreateControl ( "MODERN_PARTY_INFO_SLOT_SELECT" );

	m_pSlotMaster->SetVisibleSingle( FALSE );
	m_pSlotSubMaster->SetVisibleSingle( FALSE );
	m_pClassDIE->SetVisibleSingle( FALSE );
	m_pClassSelect->SetVisibleSingle( FALSE );

	
}

CUIControl*	CModernPartySlot::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

void CModernPartySlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CModernPartySlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernPartySlot::SetClass ( int nClassType, BOOL bSameMap )
{
	if ( nClassType < GLCI_BRAWLER_M || nClassType > GLCI_TRICKER_W )
	{
		GASSERT ( 0 && "클래스타입의 인덱스범위가 경계를 벗어납니다." );
		return ;
	}

	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		m_pClassLive[i]->SetVisibleSingle ( FALSE );
		m_pClassDead[i]->SetVisibleSingle ( FALSE );
	}

	m_pClassLive[nClassType]->SetVisibleSingle ( bSameMap );
	m_pClassDead[nClassType]->SetVisibleSingle ( !bSameMap );
}

void CModernPartySlot::SetSchool ( int nSchoolType )
{
	if ( m_pSlotText )
		m_pSlotText->SetSchool( nSchoolType );
}

void CModernPartySlot::SetLevel( CString strLevel )
{
	if ( m_pSlotText )
		m_pSlotText->SetLevel( strLevel.GetString() );
}

void CModernPartySlot::SetPlayerName ( CString strName )
{
	m_strName = strName.GetString();

	if ( m_pSlotText )
		m_pSlotText->SetPlayerName( strName.GetString() );
}

void CModernPartySlot::SetMapName ( CString strMapName )
{
	if ( m_pSlotText )
		m_pSlotText->SetMapName( strMapName.GetString() );
}

void CModernPartySlot::SetPlayerPos ( CString strPos )
{
	if ( m_pSlotText )
		m_pSlotText->SetPlayerPos( strPos.GetString() );
}

void CModernPartySlot::SetMaster( bool bMaster, int nSlotNumber )
{
	m_pSlotMaster->SetVisibleSingle( bMaster );

	CString strNum;
	strNum.Format( "%d", nSlotNumber );
	m_pSlotNumText->SetText( bMaster? "":strNum );
}

void CModernPartySlot::SetSelect ( bool bSelect )
{
	m_pClassSelect->SetVisibleSingle( bSelect );

	if ( m_pSlotText )
		m_pSlotText->RefreshText( bSelect );
}

void CModernPartySlot::SetDie( bool bDIE )
{
	m_pClassDIE->SetVisibleSingle( bDIE );

	if ( m_pSlotText )
		m_pSlotText->SetDie( bDIE );
}



