#include "StdAfx.h"
#include "./BattlePassReward.h"
#include "./BattlePassRewardSlot.h"

#include "./BasicLineBox.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicTextBox.h"
#include "./BasicProgressBar.h"

#include "./InnerInterface.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLBattlePassReward.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLCharacter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-9-14, Battle Pass - CNDev*/

CBattlePassReward::CBattlePassReward ()	
	: m_pScrollBar(NULL)
	, m_nStart(0)
	, m_nTotal(0)
	, m_fUpdateLevel(0.0f)
	, m_pTextLevel(NULL)
	, m_pTextEXP(NULL)
	, m_pBarEXP(NULL)
	, m_dwBackEXP ( USHRT_MAX )
	, m_pTextPremium(NULL)
{
	for( int i=0; i<BATTLEPASS_REWARD_MAX_SLOT; ++i )
		m_pSlot[i] = NULL;

	m_vecData.clear();
}

CBattlePassReward::~CBattlePassReward ()
{
}

void CBattlePassReward::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pScrollBar = new CBasicScrollBarEx;
	m_pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, BATTLEPASS_REWARD_SCROLLBAR );
	m_pScrollBar->CreateBaseScrollBar ( "RAN_BATTLEPASS_REWARD_SCROLLBAR" );
	m_pScrollBar->GetThumbFrame()->SetState ( 1, BATTLEPASS_REWARD_MAX_SLOT );
	RegisterControl ( m_pScrollBar );

	std::string strslot[BATTLEPASS_REWARD_MAX_SLOT] = 
	{
		"RAN_BATTLEPASS_REWARD_LIST_SLOT0",
		"RAN_BATTLEPASS_REWARD_LIST_SLOT1",
		"RAN_BATTLEPASS_REWARD_LIST_SLOT2",
		"RAN_BATTLEPASS_REWARD_LIST_SLOT3",
		"RAN_BATTLEPASS_REWARD_LIST_SLOT4",
		"RAN_BATTLEPASS_REWARD_LIST_SLOT5",
		"RAN_BATTLEPASS_REWARD_LIST_SLOT6",
		"RAN_BATTLEPASS_REWARD_LIST_SLOT7",
	};

	for( int i=0; i<BATTLEPASS_REWARD_MAX_SLOT; ++i )
	{
		m_pSlot[i] = new CBattlePassRewardSlot;
		m_pSlot[i]->CreateSub( this, strslot[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE, BATTLEPASS_REWARD_SLOT_0 + i );
		m_pSlot[i]->CreateSubControl ();
		m_pSlot[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pSlot[i] );
	}

	m_pBarEXP = new CBasicProgressBar;
	m_pBarEXP->CreateSub ( this, "RAN_BATTLEPASS_REWARD_EXP_BAR" );
	m_pBarEXP->CreateOverImage ( "RAN_BATTLEPASS_REWARD_EXP_BAR_OVERIMAGE" );
	RegisterControl ( m_pBarEXP );

	m_pTextLevel = new CBasicTextBox;
	m_pTextLevel->CreateSub ( this, "RAN_BATTLEPASS_REWARD_LEVEL_TEXT", UI_FLAG_DEFAULT );
	m_pTextLevel->SetFont ( pFont9 );
	m_pTextLevel->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pTextLevel );

	m_pTextEXP = new CBasicTextBox;
	m_pTextEXP->CreateSub ( this, "RAN_BATTLEPASS_REWARD_EXP_TEXT", UI_FLAG_DEFAULT );
	m_pTextEXP->SetFont ( pFont9 );
	m_pTextEXP->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pTextEXP );

	m_pTextPremium = new CBasicTextBox;
	m_pTextPremium->CreateSub ( this, "RAN_BATTLEPASS_REWARD_PREMIUM_TEXT", UI_FLAG_DEFAULT );
	m_pTextPremium->SetFont ( pFont9 );
	m_pTextPremium->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pTextPremium );
}

void CBattlePassReward::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CBattlePassReward::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fUpdateLevel += fElapsedTime;
	if ( m_fUpdateLevel >= 1.0f )
	{
		m_fUpdateLevel = 0.0f;
		UpdateBattlePassLevel();
	}

	m_nTotal = (int)m_vecData.size();

	if ( m_pScrollBar )
	{
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal();

			if( nTotal != m_nTotal )
				m_pScrollBar->GetThumbFrame()->SetState ( m_nTotal, BATTLEPASS_REWARD_MAX_SLOT );	
		}
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal ();
			if ( nTotal <= BATTLEPASS_REWARD_MAX_SLOT ) return ;
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
				ShowData();
			}
		}
	}
}

void CBattlePassReward::UpdateBattlePassLevel()
{
	if ( m_pTextLevel )		m_pTextLevel->ClearText();

	GLCharacter* pcharacter = GLGaeaClient::GetInstance().GetCharacter();
	if( !pcharacter )	return;

	CString strText;
	strText.Format( "%s %u", ID2GAMEWORD( "RAN_BATTLEPASS_REWARD_TEXT", 1 ), pcharacter->m_dwBattlePassLevel );
	if ( m_pTextLevel )	m_pTextLevel->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );

	DWORD dwNextEXP = (pcharacter->m_dwBattlePassLevel < BATTLEPASS_LEVEL_MAX) ? GLCONST_CHAR::dwBattlePassEXP[pcharacter->m_dwBattlePassLevel] : GLCONST_CHAR::dwBattlePassEXP[BATTLEPASS_LEVEL_MAX - 1];
	SetBattlePassEXP(pcharacter->m_dwBattlePassEXP, dwNextEXP);

	if ( m_pTextPremium )
	{
		if ( pcharacter->m_bBattlePassPremium )
		{
			if ( pcharacter->m_dwBattlePassLevel >= BATTLEPASS_LEVEL_MAX )
			{
				strText.Format("%s",ID2GAMEWORD("RAN_BATTLEPASS_PREMIUM_TEXT",2));
				m_pTextPremium->SetText(strText.GetString(), NS_UITEXTCOLOR::PRIVATE);
			}
			else
			{
				strText.Format("%s",ID2GAMEWORD("RAN_BATTLEPASS_PREMIUM_TEXT",1));
				m_pTextPremium->SetText(strText.GetString(), NS_UITEXTCOLOR::PRIVATE);
			}
		}
		else
		{
			strText.Format("%s",ID2GAMEWORD("RAN_BATTLEPASS_PREMIUM_TEXT",0));
			m_pTextPremium->SetText(strText.GetString(), NS_UITEXTCOLOR::PRIVATE);
		}
	}

	ShowData();
}

void CBattlePassReward::SetBattlePassEXP ( DWORD dwEXPCurrent, DWORD dwEXPRequire )
{
	GASSERT( m_pTextEXP && m_pBarEXP );

	if ( m_dwBackEXP != dwEXPCurrent )
	{
		m_dwBackEXP = dwEXPCurrent;

		CString strTemp;
		strTemp.Format ( "%d/%d", dwEXPCurrent, dwEXPRequire);
		m_pTextEXP->SetOneLineText ( strTemp, NS_UITEXTCOLOR::WHITE );

		const float fPercent = float(dwEXPCurrent) / float(dwEXPRequire);
		m_pBarEXP->SetPercent ( fPercent );
	}
}

void CBattlePassReward::ResetData()
{
	m_vecData.clear();

	for( int i=0; i<BATTLEPASS_REWARD_MAX_SLOT; ++i )
	{
		if ( m_pSlot[i] )
		{
			m_pSlot[i]->ItemReset();
			m_pSlot[i]->SetVisibleSingle( FALSE );
		}
	}

	m_nTotal = 0;
	m_nStart = 0;
}

void CBattlePassReward::LoadData()
{
	ResetData();

	BATTLEPASS_LEVEL_REWARD_MAP map_reward = GLBattlePassReward::GetInstance().m_mapBattlePassLevelRewardMap;
	for( BATTLEPASS_LEVEL_REWARD_MAP_ITER iter = map_reward.begin(); 
		iter != map_reward.end(); ++ iter )
	{
		SBATTLEPASS_LEVEL_REWARD &sreward_data = (*iter).second;

		m_vecData.push_back( sreward_data );
		continue;
	}

	std::sort( m_vecData.begin(), m_vecData.end() );

	ShowData();

	UpdateBattlePassLevel();
}

void CBattlePassReward::ShowData()
{
	for ( int i=0; i < BATTLEPASS_REWARD_MAX_SLOT; ++ i )
	{
		if ( m_pSlot[i] )
		{
			m_pSlot[i]->ItemReset();
			m_pSlot[i]->SetVisibleSingle( FALSE );
		}
	}

	int nEnd = m_nStart + BATTLEPASS_REWARD_MAX_SLOT;
	int nSlot = 0;

	for ( int i = m_nStart; i < nEnd; ++ i )
	{
		if ( i >= (int)m_vecData.size() )	continue;

		if ( m_pSlot[nSlot] )
		{
			m_pSlot[nSlot]->SetVisibleSingle( TRUE );
			m_pSlot[nSlot]->ItemSet( m_vecData[i].sNormalRewardID, m_vecData[i].sPremiumRewardID, m_vecData[i].dwLevel );
			nSlot ++;
		}
	}
}