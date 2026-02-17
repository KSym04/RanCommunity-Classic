#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

/*12-9-14, Battle Pass - CNDev*/

class CBasicTextBox;

class	CBattlePassListSlot : public CUIGroup
{
private:
	enum
	{
	};

public:
	CBattlePassListSlot ();
	virtual	~CBattlePassListSlot ();

public:
	void	CreateSubControl ();

public:
	CUIControl*		m_pImagePoint;

	CBasicTextBox*	m_pTextComplete;
	CBasicTextBox*	m_pTextTitle;
	CBasicTextBox*	m_pTextDesc;
	CBasicTextBox*	m_pTextProgress;

	CBasicTextBox*	m_pTextBadge;
	CBasicTextBox*	m_pTextPoint;

	float			m_fUpdateTime;
	DWORD			m_dwBattlePassID;

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

	void	ResetData();
	void	SetData( DWORD dwID );
	void	UpdateInfo();
};