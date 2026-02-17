#include "StdAfx.h"
#include "PVPTyrannyRankingsDisplayGuild.h"
#include "PVPTyrannyRankingsDisplayGuildSlot.h"
#include "PVPTyrannyRankingsDisplayGuildSlotSelf.h"
#include "../BasicTextBox.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../BasicLineBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/* 7-13-23 Tyranny War Ranking System - Montage */

CPVPTyrannyRankingsDisplayGuild::CPVPTyrannyRankingsDisplayGuild()
: m_pRankText(NULL)
, m_pClubText(NULL)
, m_pScoreText(NULL)
, m_pSlotOwn(NULL)
{
	for( int i=0; i<TYRANNY_CLUB_RANKING_NUM; ++i )
		m_pSlot[i] = NULL;
}

CPVPTyrannyRankingsDisplayGuild::~CPVPTyrannyRankingsDisplayGuild()
{
}

void CPVPTyrannyRankingsDisplayGuild::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	std::string strSlots[TYRANNY_CLUB_RANKING_NUM] = 
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

	for( int i=0; i<TYRANNY_CLUB_RANKING_NUM; ++i )
	{
		m_pSlot[i] = new CPVPTyrannyRankingsDisplayGuildSlot;
		m_pSlot[i]->CreateSub( this, strSlots[i].c_str(), UI_FLAG_DEFAULT, TYRANNY_CLUB_RANKING_SLOT_0 + i );
		m_pSlot[i]->CreateSubControl ();
		RegisterControl ( m_pSlot[i] );
	}

	m_pSlotOwn = new CPVPTyrannyRankingsDisplayGuildSlotSelf;
	m_pSlotOwn->CreateSub( this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_OWN", UI_FLAG_DEFAULT, TYRANNY_CLUB_RANKING_SLOT_OWN );
	m_pSlotOwn->CreateSubControl ();
	RegisterControl ( m_pSlotOwn );
}

void CPVPTyrannyRankingsDisplayGuild::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CPVPTyrannyRankingsDisplayGuild::RefreshTyrannyClubRanking()
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
			const STYRANNY_CLUB_RANK_EX& sMyRank = GLGaeaClient::GetInstance().m_sMyTyrannyClubRank;

			m_pSlotOwn->ResetData();
			m_pSlotOwn->SetVisibleSingle( FALSE );
			m_pSlotOwn->SetData( sMyRank.wClubRanking, strText.GetString(), GLGaeaClient::GetInstance().GetCharacter()->m_dwGuild,
				sMyRank.wKillNum, sMyRank.wDeathNum, sMyRank.wResuNum );

		}
	}

	ShowData();
}

void CPVPTyrannyRankingsDisplayGuild::ShowData()
{
	for( int i=0; i<TYRANNY_CLUB_RANKING_NUM; ++i )
	{
		if ( m_pSlot[i] )	
		{
			m_pSlot[i]->ResetData();
			m_pSlot[i]->SetVisibleSingle(FALSE);
		}
	}
	TYRANNY_CLUB_RANK_VEC& vecTyrannyClubRank = GLGaeaClient::GetInstance().m_vecTyrannyClubRank;

	int nEnd = m_nStart + TYRANNY_CLUB_RANKING_NUM;
	int nSlot = 0;

	for ( int i = (int)m_nStart; i < nEnd; ++ i )
	{
		if ( i >= (int)vecTyrannyClubRank.size() )	continue;
		if ( m_pSlot[nSlot] )
		{
			STYRANNY_CLUB_RANK sTyrannyClubRank = vecTyrannyClubRank[i];
			m_pSlot[nSlot]->SetVisibleSingle( TRUE );
			m_pSlot[nSlot]->SetData( sTyrannyClubRank, sTyrannyClubRank.wClubRanking, sTyrannyClubRank.szClubName, sTyrannyClubRank.dwClubID, sTyrannyClubRank.wGuMarkVer,
				sTyrannyClubRank.wKillNum, sTyrannyClubRank.wDeathNum, sTyrannyClubRank.wResuNum );
			nSlot ++ ;
		}
	}	
}