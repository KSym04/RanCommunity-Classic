#include "StdAfx.h"
#include "./PVPClubDeathMatchRankingPage.h"
#include "./PVPClubDeathMatchRankingPageSlot.h"

#include "./BasicLineBox.h"
#include "./BasicLineBoxEx.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicTextBox.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Client/G-Logic/PVPClubDeathMatchClient.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubDeathMatchRankingPage::CPVPClubDeathMatchRankingPage ()
	: m_nType(0)
	, m_nStart(0)
	, m_nTotal(0)
	, m_pTextRank(NULL)
	, m_pTextClubName(NULL)
	, m_pTextClubMaster(NULL)
	, m_pTextPoint(NULL)
	, m_pTextKill(NULL)
	, m_pTextDeath(NULL)
	, m_pTextResu(NULL)
	, m_pTextReward(NULL)
	, m_pScrollBar(NULL)
	, m_pSlotRankSelf(NULL)
{
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANKING_PAGE_MAX_SLOT; ++i )
		m_pSlotRank[i] = NULL;
}

CPVPClubDeathMatchRankingPage::~CPVPClubDeathMatchRankingPage ()
{
}

void CPVPClubDeathMatchRankingPage::CreateSubControl ( int nType )
{
	m_nType = nType;

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_CTF_RESULT_RANKING", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxCTFResultRankingLineBox ( "RNCDM_RANK_LINEBOX" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_CTF_RESULT_RANKING", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxCTFResultRankingLineBox ( "RNCDM_RANK_LINEBOX_HEAD" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RNCDM_RANK__MYRANK_LINE_HEAD" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RNCDM_RANK__MYRANK_LINE_TAIL" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RNCDM_RANK_SCROLLBAR_BACKGROUND" );
	RegisterControl ( pLineBox );

	m_pTextRank = CreateUITextBox( "RNCDM_RANK_TEXTBOX", pFont9, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextClubName = CreateUITextBox( "RNCDM_CLUB_NAME_TEXTBOX", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextClubMaster = CreateUITextBox( "RNCDM_CLUB_MASTER_TEXTBOX", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPoint = CreateUITextBox( "RNCDM_GET_POINT_TEXTBOX", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextKill = CreateUITextBox( "RNCDM_KINN_NUM_TEXTBOX", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextDeath = CreateUITextBox( "RNCDM_DEATH_NUM_TEXTBOX", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextResu = CreateUITextBox( "RNCDM_REBIRTH_TEXTBOX", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextReward = CreateUITextBox( "RNCDM_REWARD_TEXTBOX", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );

	m_pTextRank->AddText( ID2GAMEWORD( "RN_CMD_RANK_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextClubName->AddText( ID2GAMEWORD( "RN_CDM_CLUB_NAME_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextClubMaster->AddText( ID2GAMEWORD( "RN_CDM_CLUB_MASTER_NAME_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextPoint->AddText( ID2GAMEWORD( "RN_CDM_GET_POINT_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextKill->AddText( ID2GAMEWORD( "RN_CDM_KILL_NUM_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextDeath->AddText( ID2GAMEWORD( "RN_CDM_DEATH_NUM_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextResu->AddText( ID2GAMEWORD( "RN_CDM_REBIRTH_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextReward->AddText( ID2GAMEWORD( "RN_CDM_REWARD_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );

	std::string strslots[PVP_CLUB_DEATH_MATCH_RANKING_PAGE_MAX_SLOT] = 
	{
		"PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_0",
		"PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_1",
		"PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_2",
		"PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_3",
		"PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_4",
		"PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_5",
		"PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_6",
		"PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_7",
		"PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_8",
		"PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_9",
	};

	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANKING_PAGE_MAX_SLOT; ++i )
	{
		m_pSlotRank[i] = new CPVPClubDeathMatchRankingPageSlot;
		m_pSlotRank[i]->CreateSub ( this, strslots[i].c_str() );
		m_pSlotRank[i]->CreateSubControl ( FALSE );
		m_pSlotRank[i]->SetVisibleSingle( TRUE );
		RegisterControl ( m_pSlotRank[i] );	
	}

	m_pSlotRankSelf = new CPVPClubDeathMatchRankingPageSlot;
	m_pSlotRankSelf->CreateSub ( this, "PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_SLOT_10" );
	m_pSlotRankSelf->CreateSubControl ( TRUE );
	m_pSlotRankSelf->SetVisibleSingle( TRUE );
	RegisterControl ( m_pSlotRankSelf );	

	m_pScrollBar = new CBasicScrollBarEx;
	m_pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, PVP_CLUB_DEATH_MATCH_RANKING_PAGE_SCROLLBAR );
	m_pScrollBar->CreateBaseScrollBar ( "RNCDM_RANK_SCROLLBAR" );
	m_pScrollBar->GetThumbFrame()->SetState ( 1, PVP_CLUB_DEATH_MATCH_RANKING_PAGE_MAX_SLOT );
	RegisterControl ( m_pScrollBar );
}

CBasicTextBox* CPVPClubDeathMatchRankingPage::CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( nTextAlign );
	RegisterControl ( pTextBox );
	return pTextBox;
}

void CPVPClubDeathMatchRankingPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_nTotal = (int)PVPClubDeathMatchClient::GetInstance().m_vecRanking.size();

	if ( m_pScrollBar )
	{
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal();

			if( nTotal != m_nTotal )
				m_pScrollBar->GetThumbFrame()->SetState ( m_nTotal, PVP_CLUB_DEATH_MATCH_RANKING_PAGE_MAX_SLOT );	
		}
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal ();
			if ( nTotal <= PVP_CLUB_DEATH_MATCH_RANKING_PAGE_MAX_SLOT ) return ;
			const int nViewPerPage = pThumbFrame->GetViewPerPage ();

			if ( nViewPerPage < nTotal )
			{
				int nCurPos = 0;
				const int nMovableLine = nTotal - nViewPerPage;
				float fPercent = pThumbFrame->GetPercent ();
				nCurPos = (int)floor(fPercent * nMovableLine);
				if ( nCurPos < 0 ) nCurPos = 0;
				if ( m_nStart == nCurPos ) return;
				m_nStart = nCurPos;
				RenderView();
			}
		}
	}
}

void CPVPClubDeathMatchRankingPage::RefreshRanking()
{
	m_nTotal = (int)PVPClubDeathMatchClient::GetInstance().m_vecRanking.size();

	if ( m_pSlotRankSelf )
	{
		m_pSlotRankSelf->SetVisibleSingle( FALSE );

		DWORD dwGuildID = GLGaeaClient::GetInstance().GetCharacter()->m_dwGuild;

		PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK& sRank = PVPClubDeathMatchClient::GetInstance().m_sMyClubRank;
		if ( sRank.wRank != PVP_CLUB_DEATH_MATCH_RANK_NONE && sRank.dwClubID != PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL )
		{
			m_pSlotRankSelf->SetVisibleSingle( TRUE );
			m_pSlotRankSelf->SetData( &sRank );
		}
	}

	RenderView();
}

void CPVPClubDeathMatchRankingPage::RenderView()
{
	for ( int i=0; i < PVP_CLUB_DEATH_MATCH_RANKING_PAGE_MAX_SLOT; ++ i )
	{
		if ( m_pSlotRank[i] )
		{
			m_pSlotRank[i]->ResetData();
			m_pSlotRank[i]->SetVisibleSingle( FALSE );
		}
	}

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK_VEC& vecRank = PVPClubDeathMatchClient::GetInstance().m_vecRanking;

	int nEnd = m_nStart + PVP_CLUB_DEATH_MATCH_RANKING_PAGE_MAX_SLOT;
	int nSlot = 0;

	for ( int i = m_nStart; i < nEnd; ++ i )
	{
		if ( i >= (int)vecRank.size() )	continue;
		if ( m_pSlotRank[nSlot] )
		{
			m_pSlotRank[nSlot]->SetVisibleSingle( TRUE );
			m_pSlotRank[nSlot]->SetData( &vecRank[i] );
			nSlot ++ ;
		}
	}
}
