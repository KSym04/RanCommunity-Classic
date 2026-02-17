#include "StdAfx.h"
#include "./PartyFinderSlot.h"

#include "./BasicLineBoxEx.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLParty.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPartyFinderSlot::CPartyFinderSlot ()
	: m_pTextLeaderInfo1(NULL)
	, m_pTextLeaderInfo2(NULL)
	, m_pTextLeaderInfo3(NULL)
	, m_pTextLeaderInfo4(NULL)
	, m_pTextLeaderInfo5(NULL)
	, m_pSelectImage(NULL)
	, m_pButtonJoin(NULL)
	, m_dwPartyID(PARTY_NULL)
{
	for( int i=0; i<GLSCHOOL_NUM; ++i )
	{
		m_pSchoolImage[i] = NULL;
	}

	for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
	{
		m_pClassImage[i] = NULL;
	}
}

CPartyFinderSlot::~CPartyFinderSlot ()
{
}

void CPartyFinderSlot::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "PARTY_FINDER_SLOT_LINEBOX" );
	RegisterControl ( pBasicLineBox );

	m_pSelectImage = new CBasicLineBox;
	m_pSelectImage->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pSelectImage->CreateBaseBoxWhiteBlankBody( "PARTY_FINDER_SLOT_IMAGE_SELECT" );
	RegisterControl ( m_pSelectImage );

	m_pTextLeaderInfo1 = new CBasicTextBox;
	m_pTextLeaderInfo1->CreateSub ( this, "PARTY_FINDER_SLOT_TEXT_INFO_1" );
	m_pTextLeaderInfo1->SetFont ( pFont9 );
	m_pTextLeaderInfo1->SetTextAlign( TEXT_ALIGN_LEFT );
	//m_pTextLeaderInfo1->SetText( "leader name/level" );
	RegisterControl ( m_pTextLeaderInfo1 );

	m_pTextLeaderInfo2 = new CBasicTextBox;
	m_pTextLeaderInfo2->CreateSub ( this, "PARTY_FINDER_SLOT_TEXT_INFO_2" );
	m_pTextLeaderInfo2->SetFont ( pFont9 );
	m_pTextLeaderInfo2->SetTextAlign( TEXT_ALIGN_LEFT );
	//m_pTextLeaderInfo2->SetText( "leader class/school" );
	RegisterControl ( m_pTextLeaderInfo2 );

	m_pTextLeaderInfo3 = new CBasicTextBox;
	m_pTextLeaderInfo3->CreateSub ( this, "PARTY_FINDER_SLOT_TEXT_INFO_3" );
	m_pTextLeaderInfo3->SetFont ( pFont9 );
	m_pTextLeaderInfo3->SetTextAlign( TEXT_ALIGN_LEFT );
	//m_pTextLeaderInfo3->SetText( "location map" );
	RegisterControl ( m_pTextLeaderInfo3 );

	m_pTextLeaderInfo4 = new CBasicTextBox;
	m_pTextLeaderInfo4->CreateSub ( this, "PARTY_FINDER_SLOT_TEXT_INFO_4" );
	m_pTextLeaderInfo4->SetFont ( pFont9 );
	m_pTextLeaderInfo4->SetTextAlign( TEXT_ALIGN_LEFT );
	//m_pTextLeaderInfo4->SetText( "party option" );
	RegisterControl ( m_pTextLeaderInfo4 );

	m_pTextLeaderInfo5 = new CBasicTextBox;
	m_pTextLeaderInfo5->CreateSub ( this, "PARTY_FINDER_SLOT_TEXT_INFO_5" );
	m_pTextLeaderInfo5->SetFont ( pFont9 );
	m_pTextLeaderInfo5->SetTextAlign( TEXT_ALIGN_LEFT );
	//m_pTextLeaderInfo5->SetText( "party members/total" );
	RegisterControl ( m_pTextLeaderInfo5 );

	std::string strSchool[GLSCHOOL_NUM] = 
	{
		"PARTY_FINDER_SLOT_SCHOOL_00",
		"PARTY_FINDER_SLOT_SCHOOL_01",
		"PARTY_FINDER_SLOT_SCHOOL_02",
	};

	for( int i=0; i<GLSCHOOL_NUM; ++i )
	{
		m_pSchoolImage[i] = new CUIControl;
		m_pSchoolImage[i]->CreateSub ( this, strSchool[i].c_str() );	
		m_pSchoolImage[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolImage[i] );
	}

	//m_pSchoolImage[0]->SetVisibleSingle ( TRUE );

	std::string strClass[GLCI_NUM_8CLASS] = 
	{
		"PARTY_FINDER_SLOT_CLASS_BRAWLER_MALE",
		"PARTY_FINDER_SLOT_CLASS_SWORDMAN_MALE",
		"PARTY_FINDER_SLOT_CLASS_ARCHER_FEMALE",
		"PARTY_FINDER_SLOT_CLASS_SHAMAN_FEMALE",
		"PARTY_FINDER_SLOT_CLASS_EXTREME_MALE",
		"PARTY_FINDER_SLOT_CLASS_EXTREME_FEMALE",
		"PARTY_FINDER_SLOT_CLASS_BRAWLER_FEMALE",
		"PARTY_FINDER_SLOT_CLASS_SWORDMAN_FEMALE",
		"PARTY_FINDER_SLOT_CLASS_ARCHER_MALE",
		"PARTY_FINDER_SLOT_CLASS_SHAMAN_MALE",
		"PARTY_FINDER_SLOT_CLASS_SCIENTIST_MALE",
		"PARTY_FINDER_SLOT_CLASS_SCIENTIST_FEMALE",
		"PARTY_FINDER_SLOT_CLASS_ASSASSIN_MALE",
		"PARTY_FINDER_SLOT_CLASS_ASSASSIN_FEMALE",
		"PARTY_FINDER_SLOT_CLASS_TRICKER_MALE",
		"PARTY_FINDER_SLOT_CLASS_TRICKER_FEMALE",
	};

	for( int i=0; i<GLCI_NUM_8CLASS; ++i )
	{
		m_pClassImage[i] = new CUIControl;
		m_pClassImage[i]->CreateSub ( this, strClass[i].c_str() );	
		m_pClassImage[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pClassImage[i] );
	}

	//m_pClassImage[0]->SetVisibleSingle ( TRUE );


	m_pButtonJoin = new CBasicTextButton;
	m_pButtonJoin->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, PARTY_FINDER_SLOT_JOIN_BUTTON );
	m_pButtonJoin->CreateBaseButton ( "PARTY_FINDER_SLOT_BUTTON_JOIN", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PARTY_FINDER_TEXT", 3) );
	RegisterControl ( m_pButtonJoin );
}

void CPartyFinderSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case PARTY_FINDER_SLOT_JOIN_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					GLGaeaClient::GetInstance().GetCharacter()->PartyFinderJoinRequest( m_dwPartyID );
				}
			}
		}break;
	};
}

void CPartyFinderSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pButtonJoin )
	{
		float fPartyFinderJoinTimer = GLGaeaClient::GetInstance().GetCharacter()->m_fPartyFinderJoinDelayTimer;
		BOOL bCooldown = BOOL( fPartyFinderJoinTimer < RPARAM::fPartyFinderJoinDelay );

		m_pButtonJoin->SetVisibleSingle( !bCooldown );
	}
}

void CPartyFinderSlot::DataSet( const SPARTY_FINDER_RESULT* pParty )
{
	DataReset();

	if ( !pParty )	return;

	CString strText;
	int nIndex;

	if ( m_pTextLeaderInfo1 )
	{
		nIndex  = m_pTextLeaderInfo1->AddText( ID2GAMEWORD("PARTY_FINDER_SLOT_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
		m_pTextLeaderInfo1->AddString( nIndex, pParty->szLeaderName, NS_UITEXTCOLOR::BRIGHTGREEN );
	}

	if ( m_pTextLeaderInfo2 )
	{
		if ( pParty->wLeaderClass < GLCI_NUM_8CLASS )
		{
			nIndex = m_pTextLeaderInfo2->AddText( COMMENT::CHARCLASS[pParty->wLeaderClass].c_str(), NS_UITEXTCOLOR::WHITE );
		}

		m_pTextLeaderInfo2->AddString( nIndex, " / ", NS_UITEXTCOLOR::WHITE );

		if ( pParty->wLeaderSchool < GLSCHOOL_NUM )
		{
			m_pTextLeaderInfo2->AddString( nIndex, COMMENT::EMSCHOOL[pParty->wLeaderSchool].c_str(), NS_UITEXTCOLOR::WHITE );
		}
	}

	if ( m_pTextLeaderInfo3 )
	{
		nIndex = m_pTextLeaderInfo3->AddText( ID2GAMEWORD("PARTY_FINDER_SLOT_TEXT", 1 ), NS_UITEXTCOLOR::WHITE );
		m_pTextLeaderInfo3->AddString( nIndex, pParty->szMapName, NS_UITEXTCOLOR::ORNAGERED );
	}

	if ( m_pTextLeaderInfo4 )
	{
		if ( pParty->wPartyItemOption < EMPTYITEMOPT_SIZE && pParty->wPartyMoneyOption < EMPTYMONEYOPT_SIZE )
		{
			nIndex = m_pTextLeaderInfo4->AddText( ID2GAMEWORD("PARTY_FINDER_SLOT_TEXT", 3 ), NS_UITEXTCOLOR::WHITE );
			m_pTextLeaderInfo4->AddString( nIndex, ID2GAMEWORD("PARTY_MODAL_ITEM_OPTION", pParty->wPartyItemOption), NS_UITEXTCOLOR::GOLD );
			m_pTextLeaderInfo4->AddString( nIndex, "/", NS_UITEXTCOLOR::WHITE );
			m_pTextLeaderInfo4->AddString( nIndex, ID2GAMEWORD("PARTY_FINDER_SLOT_TEXT", 4 ), NS_UITEXTCOLOR::WHITE );
			m_pTextLeaderInfo4->AddString( nIndex, ID2GAMEWORD("PARTY_MODAL_MONEY_OPTION", pParty->wPartyMoneyOption), NS_UITEXTCOLOR::GOLD );
		}
	}

	if ( m_pTextLeaderInfo5 )
	{
		strText.Format( "%s %u/%u", ID2GAMEWORD("PARTY_FINDER_SLOT_TEXT", 2 ), pParty->dwPartyMembers, MAXPARTY );
		m_pTextLeaderInfo5->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
	}
		
	if ( pParty->wLeaderSchool < GLSCHOOL_NUM )
	{
		if ( m_pSchoolImage[pParty->wLeaderSchool] )
			m_pSchoolImage[pParty->wLeaderSchool]->SetVisibleSingle( TRUE );
	}

	if ( pParty->wLeaderClass < GLCI_NUM_8CLASS )
	{
		if ( m_pClassImage[pParty->wLeaderClass] )
			m_pClassImage[pParty->wLeaderClass]->SetVisibleSingle( TRUE );
	}

	m_dwPartyID = pParty->dwPartyID;
}

void CPartyFinderSlot::DataReset()
{
	if ( m_pTextLeaderInfo1 )	m_pTextLeaderInfo1->ClearText();
	if ( m_pTextLeaderInfo2 )	m_pTextLeaderInfo2->ClearText();
	if ( m_pTextLeaderInfo3 )	m_pTextLeaderInfo3->ClearText();
	if ( m_pTextLeaderInfo4 )	m_pTextLeaderInfo4->ClearText();
	if ( m_pTextLeaderInfo5 )	m_pTextLeaderInfo5->ClearText();

	for( int i=0; i<GLSCHOOL_NUM; ++i )
	{
		if ( m_pSchoolImage[i] )
			m_pSchoolImage[i]->SetVisibleSingle( FALSE );
	}

	for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
	{
		if ( m_pClassImage[i] )
			m_pClassImage[i]->SetVisibleSingle( FALSE );
	}

	if ( m_pSelectImage )	
		m_pSelectImage->SetVisibleSingle( FALSE );

	m_dwPartyID = PARTY_NULL;
}

void CPartyFinderSlot::SlotSelect( BOOL bSelect )
{
	if ( m_pSelectImage )
		m_pSelectImage->SetVisibleSingle( bSelect );
}