/*!
 * \file PVPPartyBattleGroundsPageBattle.h
 *
 * \author Juver
 * \date 2023/01/07
 *
 * 
 */

#ifndef PVPPARTYBATTLEGROUNDSPAGEBATTLE_H_INCLUDED__
#define PVPPARTYBATTLEGROUNDSPAGEBATTLE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class CBasicTextBox;
class CBasicLineBox;
class CD3DFontPar;
class CBasicTextButton;

class CPVPPartyBattleGroundsPageBattle : public CUIGroup
{
private:
	enum
	{
		BUTTON_REGISTER = NO_ID + 1,
	};

public:
	CPVPPartyBattleGroundsPageBattle ();
	virtual	~CPVPPartyBattleGroundsPageBattle ();

public:
	void	CreateSubControl ();

	CBasicTextBox*	CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign );

public:
	CBasicTextButton*	m_pButtonRegister;

	CBasicTextBox*		m_pTextBattleTitleStatic;
	CBasicTextBox*		m_pTextBattleTimeStartStatic;
	CBasicTextBox*		m_pTextBattleTimeEndStatic;
	CBasicTextBox*		m_pTextBattleProgressStatic;
	CBasicTextBox*		m_pTextBattleStatusStatic;

	CBasicTextBox*		m_pTextBattleTimeStartTimer;
	CBasicTextBox*		m_pTextBattleTimeEndTimer;
	CBasicTextBox*		m_pTextBattleProgress;
	CBasicTextBox*		m_pTextBattleStatus;


public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

	float	m_fUpdateTime;

	void	ShowInfo();
	void	SetCompetitionJoin( bool bEnable );
};

#endif // PVPPARTYBATTLEGROUNDSPAGEBATTLE_H_INCLUDED__
