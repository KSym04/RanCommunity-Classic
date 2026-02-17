#ifndef PVPCLUBDEATHMATCHPAGE_H__INCLUDED
#define PVPCLUBDEATHMATCHPAGE_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"

const DWORD UIMSG_CLUB_DEATH_MATCH_PAGE_BUTTON_CLOSE = UIMSG_USER1;

class CD3DFontPar;
class CBasicLineBox;
class CBasicTextBox;
class CBasicButton;
class CPVPClubDeathMatchPageBattle;

class CPVPClubDeathMatchPage : public CUIGroup
{
private:
	enum
	{
		PVP_CLUB_DEATH_MATCH_PAGE_BUTTON_REWARD = NO_ID + 1,
		PVP_CLUB_DEATH_MATCH_PAGE_BUTTON_RANKING,
		PVP_CLUB_DEATH_MATCH_PAGE_BATTLE_REGION,
	};


public:
	CPVPClubDeathMatchPage ();
	virtual	~CPVPClubDeathMatchPage ();

public:
	void	CreateSubControl ();
	CBasicLineBox*	CreateUILineBoxQuestList( char* szBaseControl );
	CBasicLineBox*	CreateUILineBoxWhiteNoBody( char* szBaseControl );
	CUIControl*		CreateUIControl( const char* szControl );
	CBasicTextBox*	CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign );

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	CBasicTextBox*	m_pTextMapTitle;
	CBasicTextBox*	m_pTextTargetTitle;
	CBasicTextBox*	m_pTextDetailTitle;
	CBasicTextBox*	m_pTextRewardTitle;
	CBasicTextBox*	m_pTextHistoryTitle;

	CBasicTextBox*	m_pTextTargetInfo;
	CBasicTextBox*	m_pTextDetailInfo;
	CBasicTextBox*	m_pTextRewardInfo;
	CBasicTextBox*	m_pTextHistoryInfoStatic1;
	CBasicTextBox*	m_pTextHistoryInfoStatic2;
	CBasicTextBox*	m_pTextHistoryInfo1;
	CBasicTextBox*	m_pTextHistoryInfo2;
	CUIControl*		m_pImageHistoryTop1;

	CBasicButton*	m_pButtonReward;
	CBasicButton*	m_pButtonRanking;

	CPVPClubDeathMatchPageBattle*	m_pInfoBattle;

public:
	float	m_fUpdateTime;

public:
	void UpdateDetailInfo();

	void	SetCompetitionJoin( bool bEnable );
};

#endif // PVPCLUBDEATHMATCHPAGE_H__INCLUDED


