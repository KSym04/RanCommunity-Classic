#include "StdAfx.h"
#include "PVPTyrannyRankingsDisplayResu.h"
#include "PVPTyrannyRankingsDisplayResuSlot.h"
#include "PVPTyrannyRankingsDisplayResuSlotSelf.h"
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

CPVPTyrannyRankingsDisplayResu::CPVPTyrannyRankingsDisplayResu()
: m_pRankText(NULL)
, m_pNameText(NULL)
, m_pScoreText(NULL)
, m_pSlotOwn(NULL)
{
	for( int i=0; i<TYRANNY_RANKING_NUM; ++i )
		m_pSlot[i] = NULL;
}

CPVPTyrannyRankingsDisplayResu::~CPVPTyrannyRankingsDisplayResu()
{
}

void CPVPTyrannyRankingsDisplayResu::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	std::string strSlots[TYRANNY_RANKING_NUM] = 
	{
		"PVP_TYRANNY_RANKING_DISPLAY_SLOT_0",
			"PVP_TYRANNY_RANKING_DISPLAY_SLOT_1",
			"PVP_TYRANNY_RANKING_DISPLAY_SLOT_2",
			"PVP_TYRANNY_RANKING_DISPLAY_SLOT_3",
			"PVP_TYRANNY_RANKING_DISPLAY_SLOT_4",
			"PVP_TYRANNY_RANKING_DISPLAY_SLOT_5",
			"PVP_TYRANNY_RANKING_DISPLAY_SLOT_6",
			"PVP_TYRANNY_RANKING_DISPLAY_SLOT_7",
			"PVP_TYRANNY_RANKING_DISPLAY_SLOT_8",
			"PVP_TYRANNY_RANKING_DISPLAY_SLOT_9",
	};

	for( int i=0; i<TYRANNY_RANKING_NUM; ++i )
	{
		m_pSlot[i] = new CPVPTyrannyRankingsDisplayResuSlot;
		m_pSlot[i]->CreateSub( this, strSlots[i].c_str(), UI_FLAG_DEFAULT, TYRANNY_RANKING_DISPLAY_SLOT_0 + i );
		m_pSlot[i]->CreateSubControl ();
		RegisterControl ( m_pSlot[i] );
	}

	m_pSlotOwn = new CPVPTyrannyRankingsDisplayResuSlotSelf;
	m_pSlotOwn->CreateSub( this, "PVP_TYRANNY_RANKING_DISPLAY_SLOT_OWN", UI_FLAG_DEFAULT, TYRANNY_RANKING_DISPLAY_SLOT_OWN );
	m_pSlotOwn->CreateSubControl ();
	RegisterControl ( m_pSlotOwn );
}

void CPVPTyrannyRankingsDisplayResu::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CPVPTyrannyRankingsDisplayResu::RefreshTyrannyRankingResu()
{
	m_pSlotOwn->ResetData();
	m_pSlotOwn->SetVisibleSingle( FALSE );

	if ( m_pSlotOwn )
	{
		const STYRANNY_RANK_RESU_EX& sMyRank = GLGaeaClient::GetInstance().m_sMyTyrannyRankResu;
		CString strText = GLGaeaClient::GetInstance().GetCharacter()->m_szName;

		int nChaClass = (BYTE)CharClassToIndex(GLGaeaClient::GetInstance().GetCharacter()->m_emClass);

		m_pSlotOwn->ResetData();
		m_pSlotOwn->SetVisibleSingle( TRUE );
		m_pSlotOwn->SetData( sMyRank.wCharRanking, strText.GetString(), GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID, nChaClass,
			sMyRank.wKillNum, sMyRank.wDeathNum, sMyRank.wResuNum, sMyRank.wSchoolNum );
	}

	ShowData();
}

void CPVPTyrannyRankingsDisplayResu::ShowData()
{
	for( int i=0; i<TYRANNY_RANKING_NUM; ++i )
	{
		if ( m_pSlot[i] )	
		{
			m_pSlot[i]->ResetData();
			m_pSlot[i]->SetVisibleSingle(FALSE);
		}
	}
	TYRANNY_RANK_RESU_VEC& vecTyrannyRank = GLGaeaClient::GetInstance().m_vecTyrannyRankResu;

	int nEnd = m_nStart + TYRANNY_RANKING_NUM;
	int nSlot = 0;

	for ( int i = (int)m_nStart; i < nEnd; ++ i )
	{
		if ( i >= (int)vecTyrannyRank.size() )	continue;
		if ( m_pSlot[nSlot] )
		{
			STYRANNY_RANK_RESU sTyrannyRank = vecTyrannyRank[i];
			m_pSlot[nSlot]->SetVisibleSingle( TRUE );
			m_pSlot[nSlot]->SetData( sTyrannyRank, 
				sTyrannyRank.wCharRanking, sTyrannyRank.szName, 
				sTyrannyRank.dwCharID, sTyrannyRank.nCharClass,
				sTyrannyRank.wGuNum, sTyrannyRank.wGuMarkVer,
				sTyrannyRank.wKillNum, sTyrannyRank.wDeathNum, 
				sTyrannyRank.wResuNum, sTyrannyRank.wSchoolNum );
			nSlot ++ ;
		}
	}	
}