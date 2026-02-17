/*!
 * \file ClubWarRankingsDisplayGuildSlot.h
 *
 * \author Montage
 * \date July 2023
 *
 * 
 */

#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

#include "../Lib_Client/G-Logic/GLClubWarDefine.h"

/* 7-13-23 Tyranny War Ranking System - Montage */

class CBasicTextBox;
class CBasicLineBox;

class CClubWarRankingsDisplayResuSlot : public CUIGroup
{
private:
	enum
	{
		PVP_TYRANNY_RANKING_DISPLAY_ICON_CLASS = GLCI_NUM_8CLASS,
	};

public:
	CClubWarRankingsDisplayResuSlot ();
	virtual	~CClubWarRankingsDisplayResuSlot ();

public:
	void	CreateSubControl ();
	virtual HRESULT Render(LPDIRECT3DDEVICEQ pd3dDevice);

public:
	CBasicTextBox*	m_pTextRank;
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextScoreKill;
	CBasicTextBox*	m_pTextScoreDeath;
	CBasicTextBox*	m_pTextScoreResu;

	CBasicLineBox*	m_pLineBox;


private:
	SCLUBWAR_RANK_RESU	m_sClubWarRank;
	CUIControl*		m_pIconGuild;
	CUIControl*		m_pIconKill[PVP_TYRANNY_RANKING_DISPLAY_ICON_CLASS];
	CUIControl*		m_pIconDeath;
	CUIControl*		m_pIconResu;
	CUIControl*		m_pIconClass[PVP_TYRANNY_RANKING_DISPLAY_ICON_CLASS];

	static const int nSCHOOL_MARK = 3;
	CUIControl*		m_pSchoolMark[nSCHOOL_MARK];

public:
	void	ResetData();
	void	SetData( SCLUBWAR_RANK_RESU sClubWarRank, int nRank, std::string strName, DWORD dwCharID, int nChaClass,
		WORD wGuNum, WORD wGuMarkVer, WORD wKill, WORD wDeath, WORD wResuNum, WORD wSchool );

};