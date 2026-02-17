#include "StdAfx.h"
#include "./PVPclubDeathMatchBattleNotice.h"
#include "./InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubDeathMatchBattleNotice::CPVPClubDeathMatchBattleNotice ()
	: m_pImage(NULL)
	, m_pImageBG(NULL)
	, m_pImageIcon(NULL)
	, m_fTimer(0.0f)
{
}

CPVPClubDeathMatchBattleNotice::~CPVPClubDeathMatchBattleNotice ()
{
}

void CPVPClubDeathMatchBattleNotice::CreateSubControlStartImage ()
{
	m_pImageBG = new CUIControl;
	m_pImageBG->CreateSub ( this, "PROGRESS_DISPLAY_PVP_CLUB_DEATH_MATCH_BACK_0" );
	RegisterControl ( m_pImageBG );

	m_pImage = new CUIControl;
	m_pImage->CreateSub ( this, "PROGRESS_DISPLAY_PVP_CLUB_DEATH_MATCH_TEXT_START" );
	RegisterControl ( m_pImage );

	m_pImageIcon = new CUIControl;
	m_pImageIcon->CreateSub ( this, "PROGRESS_DISPLAY_PVP_CLUB_DEATH_MATCH_ICON" );
	RegisterControl ( m_pImageIcon );
}

void CPVPClubDeathMatchBattleNotice::CreateSubControlEndImage ()
{
	m_pImageBG = new CUIControl;
	m_pImageBG->CreateSub ( this, "PROGRESS_DISPLAY_PVP_CLUB_DEATH_MATCH_BACK_0" );
	RegisterControl ( m_pImageBG );

	m_pImage = new CUIControl;
	m_pImage->CreateSub ( this, "PROGRESS_DISPLAY_PVP_CLUB_DEATH_MATCH_TEXT_STOP" );
	RegisterControl ( m_pImage );

	m_pImageIcon = new CUIControl;
	m_pImageIcon->CreateSub ( this, "PROGRESS_DISPLAY_PVP_CLUB_DEATH_MATCH_ICON" );
	RegisterControl ( m_pImageIcon );
}

void CPVPClubDeathMatchBattleNotice::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fTimer += fElapsedTime;
	if ( m_fTimer >= 5.0f )
	{
		m_fTimer = 0.0f;
		CInnerInterface::GetInstance().HideGroup( GetWndID() );
	}
}
