#include "StdAfx.h"
#include "./PVPClubDeathMatchTopRankings.h"
#include "./PVPClubDeathMatchTopRankingsSlot.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "./BasicTextBox.h"
#include "./BasicLineBox.h"
#include "./BasicTextButton.h"

#include "../../Lib_Client/G-Logic/PVPClubDeathMatchClient.h"
#include "./InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubDeathMatchTopRankings::CPVPClubDeathMatchTopRankings ()
	: m_pButtonClose(NULL)
	, m_pRankText(NULL)
	, m_pClubText(NULL)
	, m_pPointText(NULL)
{
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
		m_pSlot[i] = NULL;
}

CPVPClubDeathMatchTopRankings::~CPVPClubDeathMatchTopRankings ()
{
}

void CPVPClubDeathMatchTopRankings::CreateSubControl ()
{
	CD3DFontPar * pFont9 = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CLUB_DEATH_MATCH_TOP_RANKING_LINEBOX" );
	RegisterControl ( pLineBox );
	
	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody( "PVP_CLUB_DEATH_MATCH_TOP_RANKING_LINEBOX_HEAD" );
	RegisterControl ( pLineBox );	

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody( "PVP_CLUB_DEATH_MATCH_TOP_RANKING_LINEBOX_HEAD" );
	RegisterControl ( pLineBox );	

	m_pRankText = CreateUITextBox( "PVP_CLUB_DEATH_MATCH_TOP_RANKING_TEXT_BASE_RANK", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pClubText = CreateUITextBox( "PVP_CLUB_DEATH_MATCH_TOP_RANKING_TEXT_BASE_CLUB_NAME", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pPointText = CreateUITextBox( "PVP_CLUB_DEATH_MATCH_TOP_RANKING_TEXT_BASE_POINTS", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );

	m_pRankText->AddText( ID2GAMEWORD("RN_CDM_RANKING_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pClubText->AddText( ID2GAMEWORD("RN_CDM_CLUB_NAME_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pPointText->AddText( ID2GAMEWORD("RN_CDM_POINT_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );

	std::string strSlots[PVP_CLUB_DEATH_MATCH_RANK_TOP] = 
	{
		"PVP_CLUB_DEATH_MATCH_TOP_RANKING_SLOT_0",
		"PVP_CLUB_DEATH_MATCH_TOP_RANKING_SLOT_1",
		"PVP_CLUB_DEATH_MATCH_TOP_RANKING_SLOT_2",
		"PVP_CLUB_DEATH_MATCH_TOP_RANKING_SLOT_3",
		"PVP_CLUB_DEATH_MATCH_TOP_RANKING_SLOT_4",
		"PVP_CLUB_DEATH_MATCH_TOP_RANKING_SLOT_5",
		"PVP_CLUB_DEATH_MATCH_TOP_RANKING_SLOT_6",
		"PVP_CLUB_DEATH_MATCH_TOP_RANKING_SLOT_7",
		"PVP_CLUB_DEATH_MATCH_TOP_RANKING_SLOT_8",
		"PVP_CLUB_DEATH_MATCH_TOP_RANKING_SLOT_9",
	};

	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
	{
		m_pSlot[i] = new CPVPClubDeathMatchTopRankingsSlot;
		m_pSlot[i]->CreateSub( this, strSlots[i].c_str(), UI_FLAG_DEFAULT, PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_0 + i );
		m_pSlot[i]->CreateSubControl ();
		RegisterControl ( m_pSlot[i] );
	}

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_BUTTON_CLOSE );
	m_pButtonClose->CreateBaseButton ( "PVP_CLUB_DEATH_MATCH_TOP_RANKING_BUTTON_CLOSE", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PVP_CLUB_DEATH_MATCH_TOP_RANKING", 0 ) );
	m_pButtonClose->SetFlip ( TRUE );
	RegisterControl ( m_pButtonClose );

}

CBasicTextBox* CPVPClubDeathMatchTopRankings::CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( nTextAlign );
	RegisterControl ( pTextBox );
	return pTextBox;
}

void CPVPClubDeathMatchTopRankings::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		UpdateRankings();
	}
}

void CPVPClubDeathMatchTopRankings::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_BUTTON_CLOSE:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;
	};
}

void CPVPClubDeathMatchTopRankings::UpdateRankings()
{
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
	{
		if ( m_pSlot[i] )
			m_pSlot[i]->SetVisibleSingle( FALSE );
	}

	int nSlot = 0;
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK& sRank = PVPClubDeathMatchClient::GetInstance().m_sClubTop[i];
		if( sRank.dwClubID == PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL )	continue;
		if( sRank.wRank == PVP_CLUB_DEATH_MATCH_RANK_NONE )			continue;

		if ( m_pSlot[nSlot] )
		{
			m_pSlot[nSlot]->SetVisibleSingle( TRUE );
			m_pSlot[nSlot]->SetData( &sRank );
			nSlot ++ ;
		}
	}
}