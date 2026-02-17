/*!
 * \file PVPTyrannyRankingsDisplayGuildSlot.h
 *
 * \author Montage
 * \date July 2023
 *
 * 
 */

#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

#include "../Lib_Client/G-Logic/GLPVPTyrannyDefine.h"

/* 7-13-23 Tyranny War Ranking System - Montage */

class CBasicTextBox;
class CBasicLineBox;

class CPVPTyrannyRankingsDisplayGuildSlotSelf : public CUIGroup
{
private:
	enum
	{
	};

public:
	CPVPTyrannyRankingsDisplayGuildSlotSelf ();
	virtual	~CPVPTyrannyRankingsDisplayGuildSlotSelf ();

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
	CUIControl*		m_pIconGuild;
	CUIControl*		m_pIconKill;
	CUIControl*		m_pIconDeath;
	CUIControl*		m_pIconResu;

	CBasicLineBox*	m_pLineBox;


public:
	void	ResetData();
	void	SetData( int nRank, std::string strClub, DWORD dwClubID, WORD wKill, WORD wDeath, WORD wResuNum );

};