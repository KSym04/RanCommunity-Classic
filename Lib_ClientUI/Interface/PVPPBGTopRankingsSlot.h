/*!
 * \file PVPPBGTopRankingsSlot.h
 *
 * \author Juver
 * \date 2023/01/12
 *
 * 
 */

#ifndef PVPPBGTOPRANKINGSSLOT_H_INCLUDED__
#define PVPPBGTOPRANKINGSSLOT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/PVPPartyBattleGroundsData.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"

class CBasicTextBox;

class CPVPPBGTopRankingsSlot : public CUIGroup
{
private:
	enum
	{
	};

public:
	CPVPPBGTopRankingsSlot ();
	virtual	~CPVPPBGTopRankingsSlot ();

public:
	void	CreateSubControl ();

public:
	CBasicTextBox*	m_pTextRank;
	CBasicTextBox*	m_pTextName;

	CUIControl*		m_pSchoolImage[PVPPBG::SCHOOL_SIZE];
	CUIControl*		m_pClassImage[GLCI_NUM_8CLASS];

public:
	void	ResetData();
	void	SetData( PVPPBG::PLAYER_DATA* pData );

};

#endif // PVPPBGTOPRANKINGSSLOT_H_INCLUDED__