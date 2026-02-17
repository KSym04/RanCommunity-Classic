/*!
 * \file PVPPBGRankingPageSlot.h
 *
 * \author Juver
 * \date 2023/01/12
 *
 * 
 */

#ifndef PVPPBGRANKINGPAGESLOT_H_INCLUDED__
#define PVPPBGRANKINGPAGESLOT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/PVPPartyBattleGroundsData.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"

class CBasicTextBox;

class CPVPPBGRankingPageSlot : public CUIGroup
{
private:
	enum
	{
	};

public:
	CPVPPBGRankingPageSlot ();
	virtual	~CPVPPBGRankingPageSlot ();

public:
	void	CreateSubControl( BOOL bSelf );

public:
	CBasicTextBox*	m_pTextRank;
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextKill;
	CBasicTextBox*	m_pTextDeath;
	CBasicTextBox*	m_pTextResu;
	CBasicTextBox*	m_pTextHeal;
	CBasicTextBox*	m_pTextScore;
	CBasicTextBox*	m_pTextPoint;

	CUIControl*		m_pSchoolImage[PVPPBG::SCHOOL_SIZE];
	CUIControl*		m_pClassImage[GLCI_NUM_8CLASS];

public:
	BOOL	m_bSelf;
	void	ResetData();
	void	SetData( PVPPBG::PLAYER_DATA* pData );

};

#endif // PVPPBGRANKINGPAGESLOT_H_INCLUDED__