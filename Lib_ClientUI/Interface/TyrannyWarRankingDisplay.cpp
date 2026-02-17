#include "StdAfx.h"
#include "TyrannyWarRankingDisplay.h"
#include "TyrannyWarRankingDisplayGuildSlot.h"
#include "TyrannyWarRankingDisplayPlayerSlot.h"
#include "TyrannyWarRankingDisplayPlayerResuScoreSlot.h"
#include "../Lib_Client/G-Logic/GLPVPTyrannyClient.h"
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
#include "../Lib_Client/G-Logic/GLCrowRenList.h"
#include "../Lib_Client/G-Logic/GLStringTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCTyrannyWarRankingDisplay::CCTyrannyWarRankingDisplay()
	: m_pSlotOwn(NULL)
	, m_pPlayerSlotOwn(NULL)
	, m_pResuScoreSlotOwn(NULL)

	, m_pButtonGuild(NULL)
	, m_pButtonPlayer(NULL)
	, m_pButtonResuScore(NULL)

	, m_pButtonShow(NULL)
{
	///////////////////////////////////////////
	/* rsilent, top guild */
	for( int i=0; i<RANKING_NUM_TW; ++i )
		m_pSlot[i] = NULL;
	///////////////////////////////////////////

	///////////////////////////////////////////
	/* rsilent, top player */
	for( int i=0; i<RANKING_NUM_TW; ++i )
		m_pPlayerSlot[i] = NULL;
	///////////////////////////////////////////

	///////////////////////////////////////////
	/* rsilent, top resu */
	for( int i=0; i<RANKING_NUM_TW; ++i )
		m_pResuScoreSlot[i] = NULL;
	///////////////////////////////////////////
}

CCTyrannyWarRankingDisplay::~CCTyrannyWarRankingDisplay()
{
}

void CCTyrannyWarRankingDisplay::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	CBasicLineBox* pBasicLineBoxBodyWhite = new CBasicLineBox;
	pBasicLineBoxBodyWhite->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxBodyWhite->CreateBaseBoxOuterBlankWhite ( "TYRANNY_RANKING_DISPLAY_LINEBOX_BODY_WHITE" );
	RegisterControl ( pBasicLineBoxBodyWhite );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TOP - GUILD
	std::string strSlots[RANKING_NUM_TW] = 
	{
		"TYRANNY_RANKING_GUILD_SLOT_0",
		"TYRANNY_RANKING_GUILD_SLOT_1",
		"TYRANNY_RANKING_GUILD_SLOT_2",
		"TYRANNY_RANKING_GUILD_SLOT_3",
		"TYRANNY_RANKING_GUILD_SLOT_4",
		"TYRANNY_RANKING_GUILD_SLOT_5",
		"TYRANNY_RANKING_GUILD_SLOT_6",
		"TYRANNY_RANKING_GUILD_SLOT_7",
		"TYRANNY_RANKING_GUILD_SLOT_8",
		"TYRANNY_RANKING_GUILD_SLOT_9",
	};

	for( int i=0; i<RANKING_NUM_TW; ++i )
	{
		m_pSlot[i] = new CCTyrannyWarRankingDisplayGuildSlot;
		m_pSlot[i]->CreateSub( this, strSlots[i].c_str(), UI_FLAG_DEFAULT, TYRANNY_RANKING_GUILD_SLOT_0 + i );
		m_pSlot[i]->CreateSubControl ();
		RegisterControl ( m_pSlot[i] );
	}

	m_pSlotOwn = new CCTyrannyWarRankingDisplayGuildSlot;
	m_pSlotOwn->CreateSub( this, "TYRANNY_RANKING_GUILD_SLOT_OWN", UI_FLAG_DEFAULT, TYRANNY_RANKING_GUILD_SLOT_OWN );
	m_pSlotOwn->CreateSubControl ();
	RegisterControl ( m_pSlotOwn );

	/////////////////////////////////////////////////////////////////////////////
	// TOP - PLAYER
	std::string strPlayerSlots[RANKING_NUM_TW] = 
	{
		"TYRANNY_RANKING_PLAYER_SLOT_0",
		"TYRANNY_RANKING_PLAYER_SLOT_1",
		"TYRANNY_RANKING_PLAYER_SLOT_2",
		"TYRANNY_RANKING_PLAYER_SLOT_3",
		"TYRANNY_RANKING_PLAYER_SLOT_4",
		"TYRANNY_RANKING_PLAYER_SLOT_5",
		"TYRANNY_RANKING_PLAYER_SLOT_6",
		"TYRANNY_RANKING_PLAYER_SLOT_7",
		"TYRANNY_RANKING_PLAYER_SLOT_8",
		"TYRANNY_RANKING_PLAYER_SLOT_9",
	};

	for( int i=0; i<RANKING_NUM_TW; ++i )
	{
		m_pPlayerSlot[i] = new CCTyrannyWarRankingDisplayPlayerSlot;
		m_pPlayerSlot[i]->CreateSub( this, strPlayerSlots[i].c_str(), UI_FLAG_DEFAULT, TYRANNY_RANKING_PLAYER_SLOT_0 + i );
		m_pPlayerSlot[i]->CreateSubControl ();
		RegisterControl ( m_pPlayerSlot[i] );
	}

	m_pPlayerSlotOwn = new CCTyrannyWarRankingDisplayPlayerSlot;
	m_pPlayerSlotOwn->CreateSub( this, "TYRANNY_RANKING_PLAYER_SLOT_OWN", UI_FLAG_DEFAULT, TYRANNY_RANKING_PLAYER_SLOT_OWN );
	m_pPlayerSlotOwn->CreateSubControl ();
	RegisterControl ( m_pPlayerSlotOwn );
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	// TOP - RESU
	std::string strResuScoreSlots[RANKING_NUM_TW] = 
	{
		"TYRANNY_RANKING_RESU_SCORE_SLOT_0",
		"TYRANNY_RANKING_RESU_SCORE_SLOT_1",
		"TYRANNY_RANKING_RESU_SCORE_SLOT_2",
		"TYRANNY_RANKING_RESU_SCORE_SLOT_3",
		"TYRANNY_RANKING_RESU_SCORE_SLOT_4",
		"TYRANNY_RANKING_RESU_SCORE_SLOT_5",
		"TYRANNY_RANKING_RESU_SCORE_SLOT_6",
		"TYRANNY_RANKING_RESU_SCORE_SLOT_7",
		"TYRANNY_RANKING_RESU_SCORE_SLOT_8",
		"TYRANNY_RANKING_RESU_SCORE_SLOT_9",
	};

	for( int i=0; i<RANKING_NUM_TW; ++i )
	{
		m_pResuScoreSlot[i] = new CCTyrannyWarRankingDisplayPlayerResuScoreSlot;
		m_pResuScoreSlot[i]->CreateSub( this, strResuScoreSlots[i].c_str(), UI_FLAG_DEFAULT, TYRANNY_RANKING_RESU_SCORE_SLOT_0 + i );
		m_pResuScoreSlot[i]->CreateSubControl ();
		RegisterControl ( m_pResuScoreSlot[i] );
	}

	m_pResuScoreSlotOwn = new CCTyrannyWarRankingDisplayPlayerResuScoreSlot;
	m_pResuScoreSlotOwn->CreateSub( this, "TYRANNY_RANKING_RESU_SCORE_SLOT_OWN", UI_FLAG_DEFAULT, TYRANNY_RANKING_RESU_SCORE_SLOT_OWN );
	m_pResuScoreSlotOwn->CreateSubControl ();
	RegisterControl ( m_pResuScoreSlotOwn );

	CBasicTextButton* pButtonGuild = new CBasicTextButton;
	pButtonGuild->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, TYRANNY_WAR_BUTTON_GUILD );
	pButtonGuild->CreateBaseButton ( "PVP_TYRANNY_BUTTON_GUILD", CBasicTextButton::SIZE18, CBasicButton::RADIO_FLIP, ID2GAMEWORD("PVP_TYRANNY_RANKING_TEXT_BUTTON", 0 ) );
	pButtonGuild->SetFlip( TRUE );
	RegisterControl ( pButtonGuild );
	m_pButtonGuild = pButtonGuild;

	CBasicTextButton* pButtonPlayer = new CBasicTextButton;
	pButtonPlayer->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, TYRANNY_WAR_BUTTON_PLAYER );
	pButtonPlayer->CreateBaseButton ( "PVP_TYRANNY_BUTTON_PLAYER", CBasicTextButton::SIZE18, CBasicButton::RADIO_FLIP, ID2GAMEWORD("PVP_TYRANNY_RANKING_TEXT_BUTTON", 1 ) );
	pButtonPlayer->SetFlip( FALSE );
	RegisterControl ( pButtonPlayer );
	m_pButtonPlayer = pButtonPlayer;

	CBasicTextButton* pButtonResuScore = new CBasicTextButton;
	pButtonResuScore->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, TYRANNY_WAR_BUTTON_RESU );
	pButtonResuScore->CreateBaseButton ( "PVP_TYRANNY_BUTTON_RESU", CBasicTextButton::SIZE18, CBasicButton::RADIO_FLIP, ID2GAMEWORD("PVP_TYRANNY_RANKING_TEXT_BUTTON", 2 ) );
	pButtonResuScore->SetFlip( FALSE );
	RegisterControl ( pButtonResuScore );
	m_pButtonResuScore = pButtonResuScore;

	m_pButtonShow = new CBasicTextButton;
	m_pButtonShow->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, TYRANNY_WAR_BUTTON_SHOW );
	m_pButtonShow->CreateBaseButton ( "PVP_TYRANNY_WAR_BUTTON_SHOW", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PVP_TYRANNY_RANKING_TEXT_BUTTON", 3 ) );
	m_pButtonShow->SetFlip ( TRUE );
	m_pButtonShow->SetVisibleSingle( FALSE );
	RegisterControl ( m_pButtonShow );

	OpenPage(0);
}

void CCTyrannyWarRankingDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CCTyrannyWarRankingDisplay::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch ( ControlID )
	{
	case TYRANNY_WAR_BUTTON_SHOW:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD("PVP_TYRANNY_RANKING_TEXT_BUTTON", 3 ), NS_UITEXTCOLOR::WHITE );
				if ( UIMSG_LB_UP & dwMsg )
				{
					ShowAll();
				}

				AddMessageEx ( UIMSG_TWRANKDISPLAY_BUTTON_SHOW_MOUSEIN );
			}
		}break;

	case TYRANNY_WAR_BUTTON_GUILD:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD("PVP_TYRANNY_RANKING_TEXT_BUTTON", 0 ), NS_UITEXTCOLOR::GOLD );
				if ( UIMSG_LB_UP & dwMsg )
				{
					OpenPage( 0 );
				}

				AddMessageEx ( UIMSG_TWRANKDISPLAY_GUILD_BUTTON_MOUSEIN );
			}
		}break;

	case TYRANNY_WAR_BUTTON_PLAYER:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD("PVP_TYRANNY_RANKING_TEXT_BUTTON", 1 ), NS_UITEXTCOLOR::PALEGREEN );
				if ( UIMSG_LB_UP & dwMsg )
				{
					OpenPage( 1 );
				}

				AddMessageEx ( UIMSG_TWRANKDISPLAY_PLAYER_BUTTON_MOUSEIN );
			}
		}break;

	case TYRANNY_WAR_BUTTON_RESU:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD("PVP_TYRANNY_RANKING_TEXT_BUTTON", 2 ), NS_UITEXTCOLOR::HOTPINK );
				if ( UIMSG_LB_UP & dwMsg )
				{
					OpenPage( 2 );
				}

				AddMessageEx ( UIMSG_TWRANKDISPLAY_RESU_BUTTON_MOUSEIN );
			}
		}break;
	};

	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
}

void CCTyrannyWarRankingDisplay::ShowAll()
{
	if ( m_pButtonGuild )			m_pButtonGuild->SetVisibleSingle( TRUE );
	if ( m_pButtonPlayer )			m_pButtonPlayer->SetVisibleSingle( TRUE );
	if ( m_pButtonResuScore )		m_pButtonResuScore->SetVisibleSingle( TRUE );

	m_pButtonShow->SetVisibleSingle(FALSE);

	OpenPage(0);
}

void CCTyrannyWarRankingDisplay::OpenPage( int nPage )
{
	if ( m_pButtonGuild )			m_pButtonGuild->SetFlip( FALSE );
	if ( m_pButtonPlayer )			m_pButtonPlayer->SetFlip( FALSE );
	if ( m_pButtonResuScore )		m_pButtonResuScore->SetFlip( FALSE );

	for( int i=0; i<RANKING_NUM_TW; ++i )
	{
		if ( m_pSlot[i] )				m_pSlot[i]->SetVisibleSingle(FALSE);
		if ( m_pPlayerSlot[i] )			m_pPlayerSlot[i]->SetVisibleSingle(FALSE);
		if ( m_pResuScoreSlot[i] )		m_pResuScoreSlot[i]->SetVisibleSingle(FALSE);
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

			for( int i=0; i<RANKING_NUM_TW; ++i )
			{
				if ( m_pSlot[i] )			m_pSlot[i]->SetVisibleSingle(TRUE);
			}

			if ( m_pSlotOwn )
				m_pSlotOwn->SetVisibleSingle(TRUE);

		}break;

	case 1:
		{
			pVisibleButton = m_pButtonPlayer;

			for( int i=0; i<RANKING_NUM_TW; ++i )
			{
				if ( m_pPlayerSlot[i] )			m_pPlayerSlot[i]->SetVisibleSingle(TRUE);
			}

			if ( m_pPlayerSlotOwn )
				m_pPlayerSlotOwn->SetVisibleSingle(TRUE);

		}break;

	case 2:
		{
			pVisibleButton = m_pButtonResuScore;

			for( int i=0; i<RANKING_NUM_TW; ++i )
			{
				if ( m_pResuScoreSlot[i] )			m_pResuScoreSlot[i]->SetVisibleSingle(TRUE);
			}

			if ( m_pResuScoreSlotOwn )
				m_pResuScoreSlotOwn->SetVisibleSingle(TRUE);

		}break;
	};

	pVisibleButton->SetFlip ( TRUE );
}
void CCTyrannyWarRankingDisplay::RefreshTWRanking()
{
	for( int i=0; i<RANKING_NUM_TW; ++i )
	{
		if ( m_pSlot[i] )		m_pSlot[i]->ResetData();
		if ( m_pPlayerSlot[i] )			m_pPlayerSlot[i]->ResetData();
		if ( m_pResuScoreSlot[i] )			m_pResuScoreSlot[i]->ResetData();
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Top - Guild
	const TYRANNY_GUILD_RANK_EX& sMyRank = GLPVPTyrannyClient::GetInstance().m_sMyTWGuildRank;

	if ( m_pSlotOwn )
	{
		m_pSlotOwn->ResetData();
		m_pSlotOwn->SetData( sMyRank.wGuildRanking, sMyRank.szClubName, sMyRank.wKillNum, sMyRank.wDeathNum
			, sMyRank.wGuNum 
			, sMyRank.wGuMarkVer);
	}

	int nRankNum = GLPVPTyrannyClient::GetInstance().m_vecTWGuildRank.size();

	TYRANNY_GUILD_RANK_VEC& vecTWSchoolRank = GLPVPTyrannyClient::GetInstance().m_vecTWGuildRank;

	for ( int i=0; i<nRankNum; ++i )
	{
		if ( i >= RANKING_NUM_TW )	continue;;

		if ( m_pSlot[i] )	
			m_pSlot[i]->SetData( vecTWSchoolRank[i].wGuildRanking, vecTWSchoolRank[i].szClubName, vecTWSchoolRank[i].wKillNum, vecTWSchoolRank[i].wDeathNum
			, vecTWSchoolRank[i].wGuNum
			, vecTWSchoolRank[i].wGuMarkVer);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TOP - PLAYER
	const TYRANNY_PLAYER_RANK_EX& sMyPlayerRank = GLPVPTyrannyClient::GetInstance().m_sMyTWPlayerRank;

	if ( m_pPlayerSlotOwn )
	{
		m_pPlayerSlotOwn->ResetData();
		m_pPlayerSlotOwn->SetData( sMyPlayerRank.wPlayerRanking, sMyPlayerRank.szCharName, sMyPlayerRank.wKillNum, sMyPlayerRank.wDeathNum
			, sMyPlayerRank.wSchool
			, sMyPlayerRank.nChaClass
			, sMyPlayerRank.wGuNum
			, sMyPlayerRank.wGuMarkVer );
	}

	int nRankPlayerNum = GLPVPTyrannyClient::GetInstance().m_vecTWPlayerRank.size();

	TYRANNY_PLAYER_RANK_VEC& vecTWPlayerRank = GLPVPTyrannyClient::GetInstance().m_vecTWPlayerRank;

	for ( int i=0; i<nRankPlayerNum; ++i )
	{
		if ( i >= RANKING_NUM_TW )	continue;;

		if ( m_pPlayerSlot[i] )	
			m_pPlayerSlot[i]->SetData( vecTWPlayerRank[i].wPlayerRanking, vecTWPlayerRank[i].szCharName, vecTWPlayerRank[i].wKillNum, vecTWPlayerRank[i].wDeathNum
			, vecTWPlayerRank[i].wSchool
			, vecTWPlayerRank[i].nChaClass
			, vecTWPlayerRank[i].wGuNum
			, vecTWPlayerRank[i].wGuMarkVer );
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// top - resu
	const TYRANNY_PLAYER_RESU_SCORE_RANK_EX& sMyPlayerResuScoreRank = GLPVPTyrannyClient::GetInstance().m_sMyTWPlayerResuScoreRank;

	if (m_pResuScoreSlotOwn)
	{
		m_pResuScoreSlotOwn->ResetData();
		m_pResuScoreSlotOwn->SetData( sMyPlayerResuScoreRank.wPlayerResuScoreRanking, sMyPlayerResuScoreRank.szCharName, sMyPlayerResuScoreRank.wResuScoreNum
			, sMyPlayerResuScoreRank.wSchool
			, sMyPlayerResuScoreRank.nChaClass
			, sMyPlayerResuScoreRank.wGuNum
			, sMyPlayerResuScoreRank.wGuMarkVer);
	}

	int nRankPlayerResuScoreNum = GLPVPTyrannyClient::GetInstance().m_vecTWPlayerResuScoreRank.size();

	TYRANNY_PLAYER_RESU_SCORE_RANK_VEC& vecTWPlayerResuScoreRank = GLPVPTyrannyClient::GetInstance().m_vecTWPlayerResuScoreRank;

	for ( int i=0; i<nRankPlayerResuScoreNum; ++i )
	{
		if ( i >= RANKING_NUM_TW )	continue;;

		if ( m_pResuScoreSlot[i] )	
			m_pResuScoreSlot[i]->SetData( vecTWPlayerResuScoreRank[i].wPlayerResuScoreRanking, vecTWPlayerResuScoreRank[i].szCharName, vecTWPlayerResuScoreRank[i].wResuScoreNum
			, vecTWPlayerResuScoreRank[i].wSchool 
			, vecTWPlayerResuScoreRank[i].nChaClass
			, vecTWPlayerResuScoreRank[i].wGuNum
			, vecTWPlayerResuScoreRank[i].wGuMarkVer );
	}
}