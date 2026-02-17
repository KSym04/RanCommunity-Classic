#ifndef PVPCLUBDEATHMATCHPAGEBATTLE_H__INCLUDED
#define PVPCLUBDEATHMATCHPAGEBATTLE_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class CBasicTextBox;
class CBasicLineBox;
class CD3DFontPar;
class CBasicTextButton;

class CPVPClubDeathMatchPageBattle : public CUIGroup
{
private:
	enum
	{
		BUTTON_REGISTER = NO_ID + 1,
	};

public:
	CPVPClubDeathMatchPageBattle ();
	virtual	~CPVPClubDeathMatchPageBattle ();

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
	CBasicTextBox*		m_pTextClubMembersStatic;

	CBasicTextBox*		m_pTextBattleTimeStartTimer;
	CBasicTextBox*		m_pTextBattleTimeEndTimer;
	CBasicTextBox*		m_pTextBattleProgress;
	CBasicTextBox*		m_pTextBattleStatus;
	CBasicTextBox*		m_pTextClubMembers;


public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

	float	m_fUpdateTime;

	void	ShowInfo();
	void	SetCompetitionJoin( bool bEnable );
};
#endif // PVPCLUBDEATHMATCHPAGEBATTLE_H__INCLUDED
