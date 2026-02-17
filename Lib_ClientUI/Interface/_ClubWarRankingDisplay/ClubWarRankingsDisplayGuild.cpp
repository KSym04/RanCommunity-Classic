#include "StdAfx.h"
#include "ClubWarRankingsDisplayGuild.h"
#include "ClubWarRankingsDisplayGuildSlot.h"
#include "ClubWarRankingsDisplayGuildSlotSelf.h"
#include "../BasicTextBox.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../BasicLineBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/* 7-13-23 ClubWar War Ranking System - Montage */

CClubWarRankingsDisplayGuild::CClubWarRankingsDisplayGuild()
: m_pRankText(NULL)
, m_pClubText(NULL)
, m_pScoreText(NULL)
, m_pSlotOwn(NULL)
{
	for( int i=0; i<CLUBWAR_CLUB_RANKING_NUM; ++i )
		m_pSlot[i] = NULL;
}

CClubWarRankingsDisplayGuild::~CClubWarRankingsDisplayGuild()
{
}

void CClubWarRankingsDisplayGuild::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	std::string strSlots[CLUBWAR_CLUB_RANKING_NUM] = 
	{
		"PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_0",
		"PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_1",
		"PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_2",
		"PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_3",
		"PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_4",
		"PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_5",
		"PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_6",
		"PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_7",
		"PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_8",
		"PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_9",
	};

	for( int i=0; i<CLUBWAR_CLUB_RANKING_NUM; ++i )
	{
		m_pSlot[i] = new CClubWarRankingsDisplayGuildSlot;
		m_pSlot[i]->CreateSub( this, strSlots[i].c_str(), UI_FLAG_DEFAULT, TYRANNY_CLUB_RANKING_SLOT_0 + i );
		m_pSlot[i]->CreateSubControl ();
		RegisterControl ( m_pSlot[i] );
	}

	m_pSlotOwn = new CClubWarRankingsDisplayGuildSlotSelf;
	m_pSlotOwn->CreateSub( this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_OWN", UI_FLAG_DEFAULT, TYRANNY_CLUB_RANKING_SLOT_OWN );
	m_pSlotOwn->CreateSubControl ();
	RegisterControl ( m_pSlotOwn );
}

void CClubWarRankingsDisplayGuild::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CClubWarRankingsDisplayGuild::RefreshClubWarClubRanking()
{
	m_pSlotOwn->ResetData();
	m_pSlotOwn->SetVisibleSingle( FALSE );

	GLCLUB& sCLUB = GLGaeaClient::GetInstance().GetCharacter ()->m_sCLUB;
	CString strText;
	strText.Format( "%s", sCLUB.m_szName );		

	if ( m_pSlotOwn )
	{
		if ( sCLUB.m_dwID == NULL )
		{
			m_pSlotOwn->ResetData();
			m_pSlotOwn->SetVisibleSingle(FALSE);
		}
		else
		{
			const SCLUBWAR_CLUB_RANK_EX& sMyRank = GLGaeaClient::GetInstance().m_sMyClubWarClubRank;

			m_pSlotOwn->ResetData();
			m_pSlotOwn->SetVisibleSingle( FALSE );
			m_pSlotOwn->SetData( sMyRank.wClubRanking, strText.GetString(), GLGaeaClient::GetInstance().GetCharacter()->m_dwGuild,
				sMyRank.wKillNum, sMyRank.wDeathNum, sMyRank.wResuNum );

		}
	}

	ShowData();
}

void CClubWarRankingsDisplayGuild::ShowData()
{
	for( int i=0; i<CLUBWAR_CLUB_RANKING_NUM; ++i )
	{
		if ( m_pSlot[i] )	
		{
			m_pSlot[i]->ResetData();
			m_pSlot[i]->SetVisibleSingle(FALSE);
		}
	}
	CLUBWAR_CLUB_RANK_VEC& vecClubWarClubRank = GLGaeaClient::GetInstance().m_vecClubWarClubRank;

	int nEnd = m_nStart + CLUBWAR_CLUB_RANKING_NUM;
	int nSlot = 0;

	for ( int i = (int)m_nStart; i < nEnd; ++ i )
	{
		if ( i >= (int)vecClubWarClubRank.size() )	continue;
		if ( m_pSlot[nSlot] )
		{
			SCLUBWAR_CLUB_RANK sClubWarClubRank = vecClubWarClubRank[i];
			m_pSlot[nSlot]->SetVisibleSingle( TRUE );
			m_pSlot[nSlot]->SetData( sClubWarClubRank, sClubWarClubRank.wClubRanking, sClubWarClubRank.szClubName, sClubWarClubRank.dwClubID, sClubWarClubRank.wGuMarkVer,
				sClubWarClubRank.wKillNum, sClubWarClubRank.wDeathNum, sClubWarClubRank.wResuNum );
			nSlot ++ ;
		}
	}	
}