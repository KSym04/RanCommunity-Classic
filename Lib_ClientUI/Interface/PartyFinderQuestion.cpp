#include "StdAfx.h"
#include "./PartyFinderQuestion.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPartyFinderQuestion::CPartyFinderQuestion ()
	: m_CallerID ( NO_ID )
	, m_dwCharID(GAEAID_NULL)
	, m_dwPartyID(PARTY_NULL)
	, m_pTextBoxTitle(NULL)
	, m_pTextBoxInfo1(NULL)
	, m_pTextBoxInfo2(NULL)
	, m_pTextBoxInfo3(NULL)
	, m_pButtonAccept(NULL)
	, m_pButtonCancel(NULL)
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

CPartyFinderQuestion::~CPartyFinderQuestion ()
{
}

void CPartyFinderQuestion::CreateBaseModal ( char* szWindowKeyword )
{
	CreateTitle ( "MODAL_WINDOW_TITLE", "MODAL_WINDOW_TITLE_LEFT", "MODAL_WINDOW_TITLE_MID", "MODAL_WINDOW_TITLE_RIGHT", "MODAL_WINDOW_TEXTBOX", NULL );
	CreateBody ( "MODAL_WINDOW_BODY", "MODAL_WINDOW_BODY_LEFT", "MODAL_WINDOW_BODY_UP", "MODAL_WINDOW_BODY_MAIN", "MODAL_WINDOW_BODY_DOWN", "MODAL_WINDOW_BODY_RIGHT" );

	ResizeControl ( szWindowKeyword );
}

void CPartyFinderQuestion::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "PARTY_FINDER_QUESTION_LINEBOX" );
	RegisterControl ( pBasicLineBox );

	m_pTextBoxTitle = new CBasicTextBox;
	m_pTextBoxTitle->CreateSub ( this, "PARTY_FINDER_QUESTION_TEXT_TITLE" );
	m_pTextBoxTitle->SetFont ( pFont9 );
	m_pTextBoxTitle->SetTextAlign( TEXT_ALIGN_CENTER_X );
	//m_pTextBoxTitle->SetText( ID2GAMEWORD( "PARTY_FINDER_QUESTION_TEXT", 0  ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextBoxTitle );

	m_pTextBoxInfo1 = new CBasicTextBox;
	m_pTextBoxInfo1->CreateSub ( this, "PARTY_FINDER_QUESTION_TEXT_INFO_1" );
	m_pTextBoxInfo1->SetFont ( pFont9 );
	m_pTextBoxInfo1->SetTextAlign( TEXT_ALIGN_LEFT );
	//m_pTextBoxInfo1->SetText( "name" );
	RegisterControl ( m_pTextBoxInfo1 );

	m_pTextBoxInfo2 = new CBasicTextBox;
	m_pTextBoxInfo2->CreateSub ( this, "PARTY_FINDER_QUESTION_TEXT_INFO_2" );
	m_pTextBoxInfo2->SetFont ( pFont9 );
	m_pTextBoxInfo2->SetTextAlign( TEXT_ALIGN_LEFT );
	//m_pTextBoxInfo2->SetText( "level" );
	RegisterControl ( m_pTextBoxInfo2 );

	m_pTextBoxInfo3 = new CBasicTextBox;
	m_pTextBoxInfo3->CreateSub ( this, "PARTY_FINDER_QUESTION_TEXT_INFO_3" );
	m_pTextBoxInfo3->SetFont ( pFont9 );
	m_pTextBoxInfo3->SetTextAlign( TEXT_ALIGN_LEFT );
	//m_pTextBoxInfo3->SetText( "clas/school" );
	RegisterControl ( m_pTextBoxInfo3 );

	std::string strSchool[GLSCHOOL_NUM] = 
	{
		"PARTY_FINDER_QUESTION_SCHOOL_00",
		"PARTY_FINDER_QUESTION_SCHOOL_01",
		"PARTY_FINDER_QUESTION_SCHOOL_02",
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
		"PARTY_FINDER_QUESTION_CLASS_BRAWLER_MALE",
		"PARTY_FINDER_QUESTION_CLASS_SWORDMAN_MALE",
		"PARTY_FINDER_QUESTION_CLASS_ARCHER_FEMALE",
		"PARTY_FINDER_QUESTION_CLASS_SHAMAN_FEMALE",
		"PARTY_FINDER_QUESTION_CLASS_EXTREME_MALE",
		"PARTY_FINDER_QUESTION_CLASS_EXTREME_FEMALE",
		"PARTY_FINDER_QUESTION_CLASS_BRAWLER_FEMALE",
		"PARTY_FINDER_QUESTION_CLASS_SWORDMAN_FEMALE",
		"PARTY_FINDER_QUESTION_CLASS_ARCHER_MALE",
		"PARTY_FINDER_QUESTION_CLASS_SHAMAN_MALE",
		"PARTY_FINDER_QUESTION_CLASS_SCIENTIST_MALE",
		"PARTY_FINDER_QUESTION_CLASS_SCIENTIST_FEMALE",
		"PARTY_FINDER_QUESTION_CLASS_ASSASSIN_MALE",
		"PARTY_FINDER_QUESTION_CLASS_ASSASSIN_FEMALE",
		"PARTY_FINDER_QUESTION_CLASS_TRICKER_MALE",
		"PARTY_FINDER_QUESTION_CLASS_TRICKER_FEMALE",
	};

	for( int i=0; i<GLCI_NUM_8CLASS; ++i )
	{
		m_pClassImage[i] = new CUIControl;
		m_pClassImage[i]->CreateSub ( this, strClass[i].c_str() );	
		m_pClassImage[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pClassImage[i] );
	}

	//m_pClassImage[0]->SetVisibleSingle ( TRUE );


	const int nBUTTONSIZE = CBasicTextButton::SIZE14;
	m_pButtonAccept = new CBasicTextButton;
	m_pButtonAccept->CreateSub ( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, MODAL_BUTTON_ACCEPT );
	m_pButtonAccept->CreateBaseButton ( "PARTY_FINDER_QUESTION_BUTTON_ACCEPT", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "PARTY_FINDER_QUESTION_TEXT", 3 ) );
	RegisterControl ( m_pButtonAccept );
	m_pButtonAccept->SetShortcutKey ( DIK_RETURN, DIK_NUMPADENTER );

	m_pButtonCancel = new CBasicTextButton;
	m_pButtonCancel->CreateSub ( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, MODAL_BUTTON_DECLINE );
	m_pButtonCancel->CreateBaseButton ( "PARTY_FINDER_QUESTION_BUTTON_DECLINE", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "PARTY_FINDER_QUESTION_TEXT", 4 ) );
	RegisterControl ( m_pButtonCancel );
	m_pButtonCancel->SetShortcutKey ( DIK_ESCAPE );
}

void CPartyFinderQuestion::ResizeControl ( char* szWindowKeyword )
{
    CUIControl TempControl;
	TempControl.Create ( 1, szWindowKeyword );
	const UIRECT& rcParentOldPos = GetLocalPos ();
	const UIRECT& rcParentNewPos = TempControl.GetLocalPos ();
	AlignSubControl ( rcParentOldPos, rcParentNewPos );

	SetLocalPos ( D3DXVECTOR2 ( rcParentNewPos.left, rcParentNewPos.top ) );
}

void CPartyFinderQuestion::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CPartyFinderQuestion::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindow::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case MODAL_BUTTON_ACCEPT:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				AddMessageEx ( UIMSG_MODAL_OK );
				CInnerInterface::GetInstance().HideGroup ( GetWndID () );
			}
		}break;

	case MODAL_BUTTON_DECLINE:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				AddMessageEx ( UIMSG_MODAL_CANCEL );
				CInnerInterface::GetInstance().HideGroup ( GetWndID () );
			}
		}break;
	}
}

void CPartyFinderQuestion::SetQuestionData( SPARTY_FINDER_QUESTION* pData )
{
	ResetQuestionData();

	if ( !pData )	return;

	int nIndex = 0;
	CString strText;

	if ( m_pTextBoxTitle )
	{
		m_pTextBoxTitle->AddText( ID2GAMEWORD("PARTY_FINDER_QUESTION_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextBoxInfo1 )
	{
		int nIndex  = m_pTextBoxInfo1->AddText( ID2GAMEWORD("PARTY_FINDER_QUESTION_TEXT", 1 ), NS_UITEXTCOLOR::WHITE );
		m_pTextBoxInfo1->AddString( nIndex, pData->szName, NS_UITEXTCOLOR::BRIGHTGREEN );
	}

	if ( m_pTextBoxInfo2 )
	{
		int nIndex  = m_pTextBoxInfo2->AddText( ID2GAMEWORD("PARTY_FINDER_QUESTION_TEXT", 2 ), NS_UITEXTCOLOR::WHITE );

		strText.Format( "%u", pData->wLevel );
		m_pTextBoxInfo2->AddString( nIndex, strText.GetString(), NS_UITEXTCOLOR::GOLD );
	}

	if ( m_pTextBoxInfo3 )
	{
		if ( pData->wClass < GLCI_NUM_8CLASS && pData->wSchool < GLSCHOOL_NUM )
		{
			nIndex = m_pTextBoxInfo3->AddText( COMMENT::CHARCLASS[pData->wClass].c_str(), NS_UITEXTCOLOR::WHITE );
			m_pTextBoxInfo3->AddString( nIndex, " / ", NS_UITEXTCOLOR::WHITE );
			m_pTextBoxInfo3->AddString( nIndex, COMMENT::EMSCHOOL[pData->wSchool].c_str(), NS_UITEXTCOLOR::WHITE );
		}
	}

	if ( pData->wSchool < GLSCHOOL_NUM )
	{
		if ( m_pSchoolImage[pData->wSchool] )
			m_pSchoolImage[pData->wSchool]->SetVisibleSingle( TRUE );
	}

	if ( pData->wClass < GLCI_NUM_8CLASS )
	{
		if ( m_pClassImage[pData->wClass] )
			m_pClassImage[pData->wClass]->SetVisibleSingle( TRUE );
	}

	m_dwCharID = pData->dwCharID;
	m_dwPartyID = pData->dwPartyID;
}

void CPartyFinderQuestion::ResetQuestionData()
{
	if ( m_pTextBoxTitle )	m_pTextBoxTitle->ClearText();
	if ( m_pTextBoxInfo1 )	m_pTextBoxInfo1->ClearText();
	if ( m_pTextBoxInfo2 )	m_pTextBoxInfo2->ClearText();
	if ( m_pTextBoxInfo3 )	m_pTextBoxInfo3->ClearText();
	
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

	m_dwCharID = GAEAID_NULL;
	m_dwPartyID = PARTY_NULL;
}