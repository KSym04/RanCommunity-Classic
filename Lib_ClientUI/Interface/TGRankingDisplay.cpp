#include "StdAfx.h"
#include "TGRankingDisplay.h"
#include "TGRankingDisplaySlot.h"
#include "./BasicTextBox.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "UITextControl.h"
#include "GameTextControl.h"
#include "BasicLineBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTGRankingDisplay::CTGRankingDisplay()
	: m_pRankText(NULL)
	, m_pClubText(NULL)
	, m_pScoreText(NULL)
	, m_pSlotOwn(NULL)
{
	for( int i=0; i<TYRANNY_GUILD_RANKING_NUM; ++i )
		m_pSlot[i] = NULL;
}

CTGRankingDisplay::~CTGRankingDisplay()
{
}

void CTGRankingDisplay::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CDM_RANKING_LINEBOX" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody( "PVP_CDM_RANKING_LINEBOX_HEAD" );
	RegisterControl ( pLineBox );	

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody( "PVP_CDM_RANKING_LINEBOX_HEAD" );
	RegisterControl ( pLineBox );	

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody( "PVP_CDM_RANKING_LINEBOX_LOWER" );
	RegisterControl ( pLineBox );

	m_pRankText = new CBasicTextBox;
	m_pRankText->CreateSub ( this, "PVP_CDM_RANKING_TEXT_BASE_RANK" );
	m_pRankText->SetFont ( pFont );
	m_pRankText->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pRankText->AddText( ID2GAMEWORD("PVP_CDM_RANKING_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pRankText );	

	m_pClubText = new CBasicTextBox;
	m_pClubText->CreateSub ( this, "PVP_CDM_RANKING_TEXT_BASE_CLUB" );
	m_pClubText->SetFont ( pFont );
	m_pClubText->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pClubText->AddText( "Name", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pClubText );	

	m_pScoreText = new CBasicTextBox;
	m_pScoreText->CreateSub ( this, "PVP_CDM_RANKING_TEXT_BASE_SCORE" );
	m_pScoreText->SetFont ( pFont );
	m_pScoreText->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pScoreText->AddText( ID2GAMEWORD("PVP_CDM_RANKING_TEXT", 2 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pScoreText );	

	std::string strSlots[TYRANNY_GUILD_RANKING_NUM] = 
	{
		"PVP_CDM_RANKING_SLOT_0",
		"PVP_CDM_RANKING_SLOT_1",
		"PVP_CDM_RANKING_SLOT_2",
		"PVP_CDM_RANKING_SLOT_3",
		"PVP_CDM_RANKING_SLOT_4",
		"PVP_CDM_RANKING_SLOT_5",
		"PVP_CDM_RANKING_SLOT_6",
		"PVP_CDM_RANKING_SLOT_7",
		"PVP_CDM_RANKING_SLOT_8",
		"PVP_CDM_RANKING_SLOT_9",
	};
	
	for( int i=0; i<TYRANNY_GUILD_RANKING_NUM; ++i )
	{
		m_pSlot[i] = new CTGRankingDisplaySlot;
		m_pSlot[i]->CreateSub( this, strSlots[i].c_str(), UI_FLAG_DEFAULT, CDM_RANKING_SLOT_0 + i );
		m_pSlot[i]->CreateSubControl ();
		RegisterControl ( m_pSlot[i] );
	}

	m_pSlotOwn = new CTGRankingDisplaySlot;
	m_pSlotOwn->CreateSub( this, "PVP_CDM_RANKING_SLOT_OWN", UI_FLAG_DEFAULT, CDM_RANKING_SLOT_OWN );
	m_pSlotOwn->CreateSubControl ();
	RegisterControl ( m_pSlotOwn );
}

void CTGRankingDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CTGRankingDisplay::RefreshTGRanking()
{
	for( int i=0; i<TYRANNY_GUILD_RANKING_NUM; ++i )
	{
		if ( m_pSlot[i] )	m_pSlot[i]->ResetData();
	}

	const STG_RANK_EX& sMyRank = GLGaeaClient::GetInstance().m_sMyTGRank;

	if ( m_pSlotOwn )
	{
		m_pSlotOwn->ResetData();
		m_pSlotOwn->SetData( sMyRank.wClubRanking, sMyRank.szClubName, sMyRank.wKillNum, sMyRank.wDeathNum );
	}

	int nRankNum = GLGaeaClient::GetInstance().m_vecTGRank.size();
	CString strRanking;

	TG_RANK_VEC& vecTGRank = GLGaeaClient::GetInstance().m_vecTGRank;

	for ( int i=0; i<nRankNum; ++i )
	{
		if ( i >= TYRANNY_GUILD_RANKING_NUM )
			continue;;

		if ( m_pSlot[i] )	
			m_pSlot[i]->SetData( vecTGRank[i].wClubRanking, vecTGRank[i].szClubName, vecTGRank[i].wKillNum, vecTGRank[i].wDeathNum );
	}
}