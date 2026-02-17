#include "StdAfx.h"
#include "./PVPPBGRankingPage.h"
#include "./PVPPBGRankingPageSlot.h"

#include "./BasicLineBox.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicTextBox.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Client/G-Logic/PVPPartyBattleGroundsData.h"
#include "../../Lib_Client/G-Logic/PVPPartyBattleGroundsClient.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPPBGRankingPage::CPVPPBGRankingPage ()
	: m_nStart(0)
	, m_nTotal(0)
	, m_pTextRank(NULL)
	, m_pTextSchool(NULL)
	, m_pTextClass(NULL)
	, m_pTextName(NULL)
	, m_pTextKill(NULL)
	, m_pTextDeath(NULL)
	, m_pTextResu(NULL)
	, m_pTextHeal(NULL)
	, m_pTextScore(NULL)
	, m_pTextPoint(NULL)
	, m_pScrollBar(NULL)
	, m_pSlotRankSelf(NULL)
{
	for( int i=0; i<RANKING_PAGE_MAX_SLOT; ++i )
		m_pSlotRank[i] = NULL;
}

CPVPPBGRankingPage::~CPVPPBGRankingPage ()
{
}

void CPVPPBGRankingPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_PBG_RESULT_RANKING_LINEBOX" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_PBG_RESULT_MYRANK_LINE_HEAD" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_PBG_RESULT_MYRANK_LINE_TAIL" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_PBG_RESULT_RANK_SCROLLBAR_BACKGROUND" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "PVP_PBG_RESULT_RANKING_LINEBOX" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "PVP_PBG_RESULT_RANKING_LINEBOX_HEAD" );
	RegisterControl ( pLineBox );

	m_pTextRank = CreateUITextBox( "PVP_PBG_RESULT_TEXT_BASE_RANK", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextSchool = CreateUITextBox( "PVP_PBG_RESULT_TEXT_BASE_SCHOOL", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextClass = CreateUITextBox( "PVP_PBG_RESULT_TEXT_BASE_CLASS", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextName = CreateUITextBox( "PVP_PBG_RESULT_TEXT_BASE_NAME", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextKill = CreateUITextBox( "PVP_PBG_RESULT_TEXT_BASE_KILL", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextDeath = CreateUITextBox( "PVP_PBG_RESULT_TEXT_BASE_DEATH", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextResu = CreateUITextBox( "PVP_PBG_RESULT_TEXT_BASE_REBIRTH", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextHeal = CreateUITextBox( "PVP_PBG_RESULT_TEXT_BASE_HEAL", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextScore = CreateUITextBox( "PVP_PBG_RESULT_TEXT_BASE_SCORE", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPoint = CreateUITextBox( "PVP_PBG_RESULT_TEXT_BASE_CONTRIBUTION", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );

	m_pTextRank->AddText( ID2GAMEWORD( "PVP_PBG_RESULT_TEXT_BASE", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextSchool->AddText( ID2GAMEWORD( "PVP_PBG_RESULT_TEXT_BASE", 1 ), NS_UITEXTCOLOR::WHITE );
	m_pTextClass->AddText( ID2GAMEWORD( "PVP_PBG_RESULT_TEXT_BASE", 2 ), NS_UITEXTCOLOR::WHITE );
	m_pTextName->AddText( ID2GAMEWORD( "PVP_PBG_RESULT_TEXT_BASE", 3 ), NS_UITEXTCOLOR::WHITE );
	m_pTextKill->AddText( ID2GAMEWORD( "PVP_PBG_RESULT_TEXT_BASE", 4 ), NS_UITEXTCOLOR::WHITE );
	m_pTextDeath->AddText( ID2GAMEWORD( "PVP_PBG_RESULT_TEXT_BASE", 5 ), NS_UITEXTCOLOR::WHITE );
	m_pTextResu->AddText( ID2GAMEWORD( "PVP_PBG_RESULT_TEXT_BASE", 7 ), NS_UITEXTCOLOR::WHITE );
	m_pTextHeal->AddText( ID2GAMEWORD( "PVP_PBG_RESULT_TEXT_BASE", 6 ), NS_UITEXTCOLOR::WHITE );
	m_pTextScore->AddText( ID2GAMEWORD( "PVP_PBG_RESULT_TEXT_BASE", 9 ), NS_UITEXTCOLOR::WHITE );
	m_pTextPoint->AddText( ID2GAMEWORD( "PVP_PBG_RESULT_TEXT_BASE", 8 ), NS_UITEXTCOLOR::WHITE );

	std::string strslots[RANKING_PAGE_MAX_SLOT] = 
	{
		"PVP_PBG_RESULT_RANKSLOT_SLOT_0",
		"PVP_PBG_RESULT_RANKSLOT_SLOT_1",
		"PVP_PBG_RESULT_RANKSLOT_SLOT_2",
		"PVP_PBG_RESULT_RANKSLOT_SLOT_3",
		"PVP_PBG_RESULT_RANKSLOT_SLOT_4",
		"PVP_PBG_RESULT_RANKSLOT_SLOT_5",
		"PVP_PBG_RESULT_RANKSLOT_SLOT_6",
		"PVP_PBG_RESULT_RANKSLOT_SLOT_7",
		"PVP_PBG_RESULT_RANKSLOT_SLOT_8",
		"PVP_PBG_RESULT_RANKSLOT_SLOT_9",
	};

	for( int i=0; i<RANKING_PAGE_MAX_SLOT; ++i )
	{
		m_pSlotRank[i] = new CPVPPBGRankingPageSlot;
		m_pSlotRank[i]->CreateSub ( this, strslots[i].c_str() );
		m_pSlotRank[i]->CreateSubControl ( FALSE );
		m_pSlotRank[i]->SetVisibleSingle( TRUE );
		RegisterControl ( m_pSlotRank[i] );	
	}

	m_pSlotRankSelf = new CPVPPBGRankingPageSlot;
	m_pSlotRankSelf->CreateSub ( this, "PVP_PBG_RESULT_RANKSLOT_SLOT_10" );
	m_pSlotRankSelf->CreateSubControl ( TRUE );
	m_pSlotRankSelf->SetVisibleSingle( TRUE );
	RegisterControl ( m_pSlotRankSelf );	

	m_pScrollBar = new CBasicScrollBarEx;
	m_pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, RANKING_PAGE_SCROLLBAR );
	m_pScrollBar->CreateBaseScrollBar ( "PVP_PBG_RESULT_RANK_SCROLLBAR" );
	m_pScrollBar->GetThumbFrame()->SetState ( 1, RANKING_PAGE_MAX_SLOT );
	RegisterControl ( m_pScrollBar );
}

CBasicTextBox* CPVPPBGRankingPage::CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( nTextAlign );
	RegisterControl ( pTextBox );
	return pTextBox;
}

void CPVPPBGRankingPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_nTotal = (int)PVPPBG::ManagerClient::GetInstance().GetRankingSize();

	if ( m_pScrollBar )
	{
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal();

			if( nTotal != m_nTotal )
				m_pScrollBar->GetThumbFrame()->SetState ( m_nTotal, RANKING_PAGE_MAX_SLOT );	
		}
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal ();
			if ( nTotal <= RANKING_PAGE_MAX_SLOT )
				return ;

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

void CPVPPBGRankingPage::RefreshRanking()
{
	m_nTotal = (int)PVPPBG::ManagerClient::GetInstance().GetRankingSize();

	if ( m_pSlotRankSelf )
		m_pSlotRankSelf->SetVisibleSingle( FALSE );

	PVPPBG::PLAYER_DATA* pOwnRank = PVPPBG::ManagerClient::GetInstance().GetOwnRanking();
	if ( pOwnRank && m_pSlotRankSelf )
	{
		m_pSlotRankSelf->SetVisibleSingle( TRUE );
		m_pSlotRankSelf->SetData( pOwnRank );
	}

	RenderView();
}

void CPVPPBGRankingPage::RenderView()
{
	for ( int i=0; i < RANKING_PAGE_MAX_SLOT; ++ i )
	{
		if ( m_pSlotRank[i] )
		{
			m_pSlotRank[i]->ResetData();
			m_pSlotRank[i]->SetVisibleSingle( FALSE );
		}
	}

	PVPPBG::PLAYER_DATA_VEC vecRank = PVPPBG::ManagerClient::GetInstance().GetRankingVector();

	int nEnd = m_nStart + RANKING_PAGE_MAX_SLOT;
	int nSlot = 0;

	for ( int i = m_nStart; i < nEnd; ++ i )
	{
		if ( i >= (int)vecRank.size() )	continue;
		if ( m_pSlotRank[nSlot] )
		{
			PVPPBG::PLAYER_DATA sRANK = vecRank[i];
			m_pSlotRank[nSlot]->SetVisibleSingle( TRUE );
			m_pSlotRank[nSlot]->SetData( &sRANK );
			nSlot ++ ;
		}
	}
}
