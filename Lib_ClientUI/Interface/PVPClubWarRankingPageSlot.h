#ifndef PVPCLUBWARRANKINGPAGESLOT_H__INCLUDED
#define PVPCLUBWARRANKINGPAGESLOT_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"
#include "../../Lib_Client/G-Logic/GLPVPClubWarData.h"

class CBasicTextBox;

class CPVPClubWarRankingPageSlot : public CUIGroup
{
private:
	enum
	{
	};

	enum 
	{
		PVP_CLUB_WAR_RANKING_PAGE_SLOT_SCHOOL_SIZE	= 3,
	};

public:
	CPVPClubWarRankingPageSlot ();
	virtual	~CPVPClubWarRankingPageSlot ();

public:
	void	CreateSubControl( BOOL bSelf );

public:
	BOOL			m_bSelf;

	CBasicTextBox*	m_pTextRank;
	CBasicTextBox*	m_pTextGuild;
	CBasicTextBox*	m_pTextName;
	
	CBasicTextBox*	m_pTextKill;
	CBasicTextBox*	m_pTextDeath;
	CBasicTextBox*	m_pTextResu;
	CBasicTextBox*	m_pTextScore;
	CBasicTextBox*	m_pTextReward;

	CUIControl*		m_pSchoolImage[PVP_CLUB_WAR_RANKING_PAGE_SLOT_SCHOOL_SIZE];
	CUIControl*		m_pClassImage[GLCI_NUM_8CLASS];
	CUIControl*		m_pGuildImage;
	CUIControl*		m_pImageWarChips;

	SPVP_CLUB_WAR_PLAYER_DATA*	m_pData;

public:
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );

	void	ResetData();
	void	SetData( SPVP_CLUB_WAR_PLAYER_DATA* pData );

};

#endif // PVPCLUBWARRANKINGPAGESLOT_H__INCLUDED
