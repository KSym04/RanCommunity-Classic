#include "StdAfx.h"
#include "ClubWarRankingDisplay.h"
#include "ClubWarRankingDisplaySlot.h"
#include "ClubWarRankingDisplayPlayerSlot.h"
#include "ClubWarRankingDisplayResuScoreSlot.h"
#include "./BasicTextBox.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Interface/UITextControl.h"
#include "../Interface/GameTextControl.h"
#include "BasicLineBox.h"
#include "../Interface/BasicTextButton.h"
#include "./BasicButton.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "./BasicLineBox.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_Client/DxGlobalStage.h"
#include "../Lib_Client/G-Logic/GLCharData.h"
#include "../Lib_Client/G-Logic/GLCharacter.h"
#include "../Lib_Client/G-Logic/GLChar.h"
#include "../Lib_Client/G-Logic/GLCharClient.h"
#include "../Lib_Client/G-Logic/GLogicData.h"
#include "UIInfoLoader.h"
#include "../Lib_Client/G-Logic/glcrowrenlist.h"
#include "../Lib_Client/G-Logic/GLStringTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CClubWarRankingDisplay::CClubWarRankingDisplay()
	: m_pSlotOwn(NULL)
	, m_pPlayerSlotOwn(NULL)
	, m_pResuScoreSlotOwn(NULL)

	, m_pButtonGuild(NULL)
	, m_pButtonPlayer(NULL)
	, m_pButtonResuScore(NULL)


	, m_pButtonShow(NULL)
{
	for( int i=0; i<RANKING_NUM_CW; ++i )
		m_pSlot[i] = NULL;

	for( int i=0; i<RANKING_NUM_CW; ++i )
		m_pPlayerSlot[i] = NULL;

	for( int i=0; i<RANKING_NUM_CW; ++i )
		m_pResuScoreSlot[i] = NULL;
}

CClubWarRankingDisplay::~CClubWarRankingDisplay()
{
}

void CClubWarRankingDisplay::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBoxBodyWhite = new CBasicLineBox;
	pBasicLineBoxBodyWhite->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxBodyWhite->CreateBaseBoxOuterBlankWhite ( "CLUBWAR_RANKING_DISPLAY_LINEBOX_BODY_WHITE" );
	RegisterControl ( pBasicLineBoxBodyWhite );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string strSlots[RANKING_NUM_CW] = 
	{
		"CLUBWAR_RANKING_GUILD_SLOT_0",
		"CLUBWAR_RANKING_GUILD_SLOT_1",
		"CLUBWAR_RANKING_GUILD_SLOT_2",
		"CLUBWAR_RANKING_GUILD_SLOT_3",
		"CLUBWAR_RANKING_GUILD_SLOT_4",
		"CLUBWAR_RANKING_GUILD_SLOT_5",
		"CLUBWAR_RANKING_GUILD_SLOT_6",
		"CLUBWAR_RANKING_GUILD_SLOT_7",
		"CLUBWAR_RANKING_GUILD_SLOT_8",
		"CLUBWAR_RANKING_GUILD_SLOT_9",
	};

	for( int i=0; i<RANKING_NUM_CW; ++i )
	{
		m_pSlot[i] = new CClubWarRankingDisplaySlot;
		m_pSlot[i]->CreateSub( this, strSlots[i].c_str(), UI_FLAG_DEFAULT, CLUBWAR_RANKING_GUILD_SLOT_0 + i );
		m_pSlot[i]->CreateSubControl ();
		RegisterControl ( m_pSlot[i] );
	}

	m_pSlotOwn = new CClubWarRankingDisplaySlot;
	m_pSlotOwn->CreateSub( this, "CLUBWAR_RANKING_GUILD_SLOT_OWN", UI_FLAG_DEFAULT, CLUBWAR_RANKING_GUILD_SLOT_OWN );
	m_pSlotOwn->CreateSubControl ();
	RegisterControl ( m_pSlotOwn );

	/////////////////////////////////////////////////////////////////////////////
	std::string strPlayerSlots[RANKING_NUM_CW] = 
	{
		"CLUBWAR_RANKING_PLAYER_SLOT_0",
		"CLUBWAR_RANKING_PLAYER_SLOT_1",
		"CLUBWAR_RANKING_PLAYER_SLOT_2",
		"CLUBWAR_RANKING_PLAYER_SLOT_3",
		"CLUBWAR_RANKING_PLAYER_SLOT_4",
		"CLUBWAR_RANKING_PLAYER_SLOT_5",
		"CLUBWAR_RANKING_PLAYER_SLOT_6",
		"CLUBWAR_RANKING_PLAYER_SLOT_7",
		"CLUBWAR_RANKING_PLAYER_SLOT_8",
		"CLUBWAR_RANKING_PLAYER_SLOT_9",
	};

	for( int i=0; i<RANKING_NUM_CW; ++i )
	{
		m_pPlayerSlot[i] = new CClubWarRankingDisplayPlayerSlot;
		m_pPlayerSlot[i]->CreateSub( this, strPlayerSlots[i].c_str(), UI_FLAG_DEFAULT, CLUBWAR_RANKING_PLAYER_SLOT_0 + i );
		m_pPlayerSlot[i]->CreateSubControl ();
		RegisterControl ( m_pPlayerSlot[i] );
	}

	m_pPlayerSlotOwn = new CClubWarRankingDisplayPlayerSlot;
	m_pPlayerSlotOwn->CreateSub( this, "CLUBWAR_RANKING_PLAYER_SLOT_OWN", UI_FLAG_DEFAULT, CLUBWAR_RANKING_PLAYER_SLOT_OWN );
	m_pPlayerSlotOwn->CreateSubControl ();
	RegisterControl ( m_pPlayerSlotOwn );
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	std::string strResuScoreSlots[RANKING_NUM_CW] = 
	{
		"CLUBWAR_RANKING_RESU_SCORE_SLOT_0",
		"CLUBWAR_RANKING_RESU_SCORE_SLOT_1",
		"CLUBWAR_RANKING_RESU_SCORE_SLOT_2",
		"CLUBWAR_RANKING_RESU_SCORE_SLOT_3",
		"CLUBWAR_RANKING_RESU_SCORE_SLOT_4",
		"CLUBWAR_RANKING_RESU_SCORE_SLOT_5",
		"CLUBWAR_RANKING_RESU_SCORE_SLOT_6",
		"CLUBWAR_RANKING_RESU_SCORE_SLOT_7",
		"CLUBWAR_RANKING_RESU_SCORE_SLOT_8",
		"CLUBWAR_RANKING_RESU_SCORE_SLOT_9",
	};

	for( int i=0; i<RANKING_NUM_CW; ++i )
	{
		m_pResuScoreSlot[i] = new CClubWarRankingDisplayResuScoreSlot;
		m_pResuScoreSlot[i]->CreateSub( this, strResuScoreSlots[i].c_str(), UI_FLAG_DEFAULT, CLUBWAR_RANKING_RESU_SCORE_SLOT_0 + i );
		m_pResuScoreSlot[i]->CreateSubControl ();
		RegisterControl ( m_pResuScoreSlot[i] );
	}

	m_pResuScoreSlotOwn = new CClubWarRankingDisplayResuScoreSlot;
	m_pResuScoreSlotOwn->CreateSub( this, "CLUBWAR_RANKING_RESU_SCORE_SLOT_OWN", UI_FLAG_DEFAULT, CLUBWAR_RANKING_RESU_SCORE_SLOT_OWN );
	m_pResuScoreSlotOwn->CreateSubControl ();
	RegisterControl ( m_pResuScoreSlotOwn );
	/////////////////////////////////////////////////////////////////////////////


	CBasicTextButton* pButtonGuild = new CBasicTextButton;
	pButtonGuild->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CLUBWAR_BUTTON_GUILD );
	pButtonGuild->CreateBaseButton ( "PVP_CLUBWAR_BUTTON_GUILD", CBasicTextButton::SIZE18, CBasicButton::RADIO_FLIP, ID2GAMEWORD("PVP_CLUBWAR_RANKING_TEXT_BUTTON", 0 ) );
	pButtonGuild->SetFlip( TRUE );
	pButtonGuild->SetVisibleSingle(TRUE);
	RegisterControl ( pButtonGuild );
	m_pButtonGuild = pButtonGuild;

	CBasicTextButton* pButtonPlayer = new CBasicTextButton;
	pButtonPlayer->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CLUBWAR_BUTTON_PLAYER );
	pButtonPlayer->CreateBaseButton ( "PVP_CLUBWAR_BUTTON_PLAYER", CBasicTextButton::SIZE18, CBasicButton::RADIO_FLIP, ID2GAMEWORD("PVP_CLUBWAR_RANKING_TEXT_BUTTON", 1 ) );
	pButtonPlayer->SetFlip( FALSE );
	RegisterControl ( pButtonPlayer );
	m_pButtonPlayer = pButtonPlayer;

	CBasicTextButton* pButtonResuScore = new CBasicTextButton;
	pButtonResuScore->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CLUBWAR_BUTTON_RESU );
	pButtonResuScore->CreateBaseButton ( "PVP_CLUBWAR_BUTTON_RESU", CBasicTextButton::SIZE18, CBasicButton::RADIO_FLIP, ID2GAMEWORD("PVP_CLUBWAR_RANKING_TEXT_BUTTON", 2 ) );
	pButtonResuScore->SetFlip( FALSE );
	RegisterControl ( pButtonResuScore );
	m_pButtonResuScore = pButtonResuScore;

	m_pButtonShow = new CBasicTextButton;
	m_pButtonShow->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, CLUBWAR_BUTTON_SHOW );
	m_pButtonShow->CreateBaseButton ( "PVP_CLUBWAR_BUTTON_SHOW", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PVP_CLUBWAR_RANKING_TEXT_BUTTON", 3 ) );
	m_pButtonShow->SetFlip ( TRUE );
	m_pButtonShow->SetVisibleSingle( FALSE );
	RegisterControl ( m_pButtonShow );

	OpenPage(0);
}

void CClubWarRankingDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CClubWarRankingDisplay::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case CLUBWAR_BUTTON_SHOW:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD("PVP_CLUBWAR_RANKING_TEXT_BUTTON", 3 ), NS_UITEXTCOLOR::WHITE );
				if ( UIMSG_LB_UP & dwMsg )
				{
					ShowAll();
				}

				AddMessageEx ( UIMSG_CWRANKDISPLAY_BUTTON_SHOW_MOUSEIN );
			}
		}break;

	case CLUBWAR_BUTTON_GUILD:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD("PVP_CLUBWAR_RANKING_TEXT_BUTTON", 0 ), NS_UITEXTCOLOR::GOLD );
				if ( UIMSG_LB_UP & dwMsg )
				{
					OpenPage( 0 );
				}

				AddMessageEx ( UIMSG_CWRANKDISPLAY_BUTTON_GUILD_MOUSEIN );
			}
		}break;

	case CLUBWAR_BUTTON_PLAYER:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD("PVP_CLUBWAR_RANKING_TEXT_BUTTON", 1 ), NS_UITEXTCOLOR::PALEGREEN );
				if ( UIMSG_LB_UP & dwMsg )
				{
					OpenPage( 1 );
				}

				AddMessageEx ( UIMSG_CWRANKDISPLAY_BUTTON_PLAYER_MOUSEIN );
			}
		}break;

	case CLUBWAR_BUTTON_RESU:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD("PVP_CLUBWAR_RANKING_TEXT_BUTTON", 2 ), NS_UITEXTCOLOR::HOTPINK );
				if ( UIMSG_LB_UP & dwMsg )
				{
					OpenPage( 2 );
				}

				AddMessageEx ( UIMSG_CWRANKDISPLAY_BUTTON_RESU_SCORE_MOUSEIN );
			}
		}break;
	};

	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
}

void CClubWarRankingDisplay::ShowAll()
{
	if ( m_pButtonGuild )			m_pButtonGuild->SetVisibleSingle( TRUE );
	if ( m_pButtonPlayer )			m_pButtonPlayer->SetVisibleSingle( TRUE );
	if ( m_pButtonResuScore )		m_pButtonResuScore->SetVisibleSingle( TRUE );

	m_pButtonShow->SetVisibleSingle(FALSE);

	OpenPage(0);
}

void CClubWarRankingDisplay::OpenPage( int nPage )
{
	if ( m_pButtonGuild )		m_pButtonGuild->SetFlip( FALSE );
	if ( m_pButtonPlayer )		m_pButtonPlayer->SetFlip( FALSE );
	if ( m_pButtonResuScore )		m_pButtonResuScore->SetFlip( FALSE );

	for( int i=0; i<RANKING_NUM_CW; ++i )
	{
		if ( m_pSlot[i] )	m_pSlot[i]->SetVisibleSingle(FALSE);
		if ( m_pPlayerSlot[i] )	m_pPlayerSlot[i]->SetVisibleSingle(FALSE);
		if ( m_pResuScoreSlot[i] )	m_pResuScoreSlot[i]->SetVisibleSingle(FALSE);
	}

	if ( m_pSlotOwn )
		m_pSlotOwn->SetVisibleSingle(FALSE);

	if ( m_pPlayerSlotOwn )
		m_pPlayerSlotOwn->SetVisibleSingle(FALSE);

	if ( m_pResuScoreSlotOwn )
		m_pResuScoreSlotOwn->SetVisibleSingle(FALSE);

	CBasicTextButton* pVisibleButton = NULL;

	switch( nPage )
	{
	case 0:
		{
			pVisibleButton = m_pButtonGuild;

			for( int i=0; i<RANKING_NUM_CW; ++i ){
				if ( m_pSlot[i] )	m_pSlot[i]->SetVisibleSingle(TRUE);
			}

			if ( m_pSlotOwn )
				m_pSlotOwn->SetVisibleSingle(TRUE);

		}break;

	case 1:
		{
			pVisibleButton = m_pButtonPlayer;

			for( int i=0; i<RANKING_NUM_CW; ++i ){
				if ( m_pPlayerSlot[i] )	m_pPlayerSlot[i]->SetVisibleSingle(TRUE);
			}

			if ( m_pPlayerSlotOwn )
				m_pPlayerSlotOwn->SetVisibleSingle(TRUE);

		}break;

	case 2:
		{
			pVisibleButton = m_pButtonResuScore;

			for( int i=0; i<RANKING_NUM_CW; ++i ){
				if ( m_pResuScoreSlot[i] )	m_pResuScoreSlot[i]->SetVisibleSingle(TRUE);
			}

			if (m_pResuScoreSlotOwn )
				m_pResuScoreSlotOwn->SetVisibleSingle(TRUE);
		}break;
	};

	pVisibleButton->SetFlip ( TRUE );
}

void CClubWarRankingDisplay::RefreshCWRanking()
{
	for( int i=0; i<RANKING_NUM_CW; ++i )
	{
		if ( m_pSlot[i] )	m_pSlot[i]->ResetData();
		if ( m_pPlayerSlot[i] )	m_pPlayerSlot[i]->ResetData();
		if ( m_pResuScoreSlot[i] )	m_pResuScoreSlot[i]->ResetData();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Top - Guild
	const SCWGUILD_RANK_EX& m_sMyCWGuildRank = GLGaeaClient::GetInstance().m_sMyCWGuildRank;

	if ( m_pSlotOwn )
	{
		m_pSlotOwn->ResetData();
		m_pSlotOwn->SetData( m_sMyCWGuildRank.wClubRanking, m_sMyCWGuildRank.szClubName, m_sMyCWGuildRank.wKillNum, m_sMyCWGuildRank.wDeathNum
			, m_sMyCWGuildRank.wGuNum
			, m_sMyCWGuildRank.wGuMarkVer );
	}

	int nRankNum = GLGaeaClient::GetInstance().m_vecCWGuildRank.size();

	CWGUILD_RANK_VEC& m_vecCWGuildRank = GLGaeaClient::GetInstance().m_vecCWGuildRank;

	for ( int i=0; i<nRankNum; ++i )
	{
		if ( i >= RANKING_NUM_CW )	continue;;

		if ( m_pSlot[i] )	
			m_pSlot[i]->SetData( m_vecCWGuildRank[i].wClubRanking, m_vecCWGuildRank[i].szClubName, m_vecCWGuildRank[i].wKillNum, m_vecCWGuildRank[i].wDeathNum
			, m_vecCWGuildRank[i].wGuNum
			, m_vecCWGuildRank[i].wGuMarkVer );
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Top - PLAYER
	const SCWPLAYER_RANK_EX& m_sMyCWPlayerRank = GLGaeaClient::GetInstance().m_sMyCWPlayerRank;

	if ( m_pPlayerSlotOwn )
	{
		m_pPlayerSlotOwn->ResetData();
		m_pPlayerSlotOwn->SetData( m_sMyCWPlayerRank.wPlayerRanking, m_sMyCWPlayerRank.szCharName, m_sMyCWPlayerRank.wKillNum, m_sMyCWPlayerRank.wDeathNum 
			, m_sMyCWPlayerRank.wSchool
			, m_sMyCWPlayerRank.nChaClass
			, m_sMyCWPlayerRank.wGuNum
			, m_sMyCWPlayerRank.wGuMarkVer );
	}

	int nRankPlayerNum = GLGaeaClient::GetInstance().m_vecCWPlayerRank.size();

	CWPLAYER_RANK_VEC& m_vecCWPlayerRank = GLGaeaClient::GetInstance().m_vecCWPlayerRank;

	for ( int i=0; i<nRankPlayerNum; ++i )
	{
		if ( i >= RANKING_NUM_CW )	continue;;

		if ( m_pPlayerSlot[i] )	
			m_pPlayerSlot[i]->SetData( m_vecCWPlayerRank[i].wPlayerRanking, m_vecCWPlayerRank[i].szCharName, m_vecCWPlayerRank[i].wKillNum, m_vecCWPlayerRank[i].wDeathNum
			, m_vecCWPlayerRank[i].wSchool
			, m_vecCWPlayerRank[i].nChaClass
			, m_vecCWPlayerRank[i].wGuNum
			, m_vecCWPlayerRank[i].wGuMarkVer );
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// Top - RESU
	const SCWPLAYER_RANK_RESU_EX& m_sMyCWPlayerRankResuScore = GLGaeaClient::GetInstance().m_sMyCWPlayerRankResuScore;

	if (m_pResuScoreSlotOwn)
	{
		m_pResuScoreSlotOwn->ResetData();
		m_pResuScoreSlotOwn->SetData(
			m_sMyCWPlayerRankResuScore.wPlayerRankingResu,
			m_sMyCWPlayerRankResuScore.szCharName,
			m_sMyCWPlayerRankResuScore.wResuNum,
			m_sMyCWPlayerRankResuScore.wSchool,
			m_sMyCWPlayerRankResuScore.nChaClass,
			m_sMyCWPlayerRankResuScore.wGuNum,
			m_sMyCWPlayerRankResuScore.wGuMarkVer
		);
	}
	int nRanksResuNum = GLGaeaClient::GetInstance().m_vecCWPlayerRankResuScore.size();

	CWPLAYER_RANK_RESU_VEC& m_vecCWPlayerRankResuScore = GLGaeaClient::GetInstance().m_vecCWPlayerRankResuScore;

	for ( int i=0; i<nRanksResuNum; ++i )
	{
		if ( i >= RANKING_NUM_CW )	continue;;

		if ( m_pResuScoreSlot[i] )	
			m_pResuScoreSlot[i]->SetData( m_vecCWPlayerRankResuScore[i].wPlayerRankingResu, m_vecCWPlayerRankResuScore[i].szCharName, m_vecCWPlayerRankResuScore[i].wResuNum
			, m_vecCWPlayerRankResuScore[i].wSchool 
			, m_vecCWPlayerRankResuScore[i].nChaClass
			, m_vecCWPlayerRankResuScore[i].wGuNum
			, m_vecCWPlayerRankResuScore[i].wGuMarkVer );
	}
}