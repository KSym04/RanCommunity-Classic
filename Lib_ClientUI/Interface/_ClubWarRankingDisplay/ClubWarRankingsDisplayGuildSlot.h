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

/* 7-13-23 ClubWar War Ranking System - Montage */

class CBasicTextBox;
class CBasicLineBox;

class CClubWarRankingsDisplayGuildSlot : public CUIGroup
{
private:
	enum
	{
	};

public:
	CClubWarRankingsDisplayGuildSlot ();
	virtual	~CClubWarRankingsDisplayGuildSlot ();

public:
	void	CreateSubControl ();
	virtual HRESULT Render(LPDIRECT3DDEVICEQ pd3dDevice);

public:
	CBasicTextBox*	m_pTextRank;
	CBasicTextBox*	m_pTextClub;
	CBasicTextBox*	m_pTextScoreKill;
	CBasicTextBox*	m_pTextScoreDeath;
	CBasicTextBox*	m_pTextScoreResu;


private:
	SCLUBWAR_CLUB_RANK	m_sClubWarClubRank;
	CUIControl*		m_pIconGuild;
	CUIControl*		m_pIconKill;
	CUIControl*		m_pIconDeath;
	CUIControl*		m_pIconResu;

	CBasicLineBox*	m_pLineBox;


public:
	void	ResetData();
	void	SetData( SCLUBWAR_CLUB_RANK sClubWarClubRank, int nRank, std::string strClub, DWORD dwClubID, WORD wGuMarkVer, WORD wKill, WORD wDeath, WORD wResuNum );

};