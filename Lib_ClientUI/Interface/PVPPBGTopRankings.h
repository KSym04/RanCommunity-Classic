/*!
 * \file PVPPBGTopRankings.h
 *
 * \author Juver
 * \date 2023/01/12
 *
 * 
 */

#ifndef PVPPBGTOPRANKINGS_H_INCLUDED__
#define PVPPBGTOPRANKINGS_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./UIWindowEx.h"

#include "../../Lib_Client/G-Logic/PVPPartyBattleGroundsData.h"

class CPVPPBGTopRankingsSlot;
class CBasicTextButton;
class CBasicTextBox;

class	CPVPPBGTopRankings : public CUIWindowEx
{
	enum
	{
		TOPRANK_BUTTON_CLOSE = ET_CONTROL_NEXT,
		TOPRANK_RANKING_SLOT_0,
		TOPRANK_RANKING_SLOT_1,
		TOPRANK_RANKING_SLOT_2,
		TOPRANK_RANKING_SLOT_3,
		TOPRANK_RANKING_SLOT_4,
		TOPRANK_RANKING_SLOT_5,
		TOPRANK_RANKING_SLOT_6,
		TOPRANK_RANKING_SLOT_7,
		TOPRANK_RANKING_SLOT_8,
		TOPRANK_RANKING_SLOT_9,
	};

public:
	CPVPPBGTopRankings ();
	virtual ~CPVPPBGTopRankings ();

public:
	void	CreateSubControl ();
	CBasicTextBox*	CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign );

public:
	CBasicTextBox*		m_pRankText;
	CBasicTextBox*		m_pSchoolText;
	CBasicTextBox*		m_pClassText;
	CBasicTextBox*		m_pNameText;

	CPVPPBGTopRankingsSlot*	m_pSlot[PVP_PBG_MINI_RANKING_NUM];

	CBasicTextButton*	m_pButtonClose;

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

public:
	void	SetVisiblePage( int nPage );
	void	UpdateRankings();
};

#endif // PVPPBGTOPRANKINGS_H_INCLUDED__