/*!
 * \file SkillTrayTabExMini.h
 *
 * \author Juver
 * \date August 2018
 *
 * modern skill tray
 */

#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "./SkillTrayTabDefine.h"

class CBasicQuickSkillSlot2;
class CBasicButton;

class CSkillTrayTabExMini : public CUIGroup
{
public:
	enum
	{
		BASIC_QUICK_SKILL_BASE_SLOT = NO_ID + 1,
		QUICK_SKILL_TRAY_OPEN_BUTTON,
		QUICK_SKILL_TRAY_LOCK_BUTTON,
		QUICK_SKILL_TRAY_UNLOCK_BUTTON,
	};


public:
	void CreateSubControl();

public:
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle( BOOL bVisible );

public:
	CSkillTrayTabExMini(void);
	virtual ~CSkillTrayTabExMini(void);

protected:
	CBasicQuickSkillSlot2*	m_pBasicQuickSkillSlot;		

	CBasicButton*		m_pButtonOpen;
	CBasicButton*		m_pButtonLock;
	CBasicButton*		m_pButtonUnlock;

public:
	void	CheckMouseState ();

private:
	BOOL		m_bFirstGap;
	D3DXVECTOR2	m_vGap;
	int			m_PosX;
	int			m_PosY;
	bool		m_bCHECK_MOUSE_STATE;
};
