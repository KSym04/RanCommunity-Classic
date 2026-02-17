/*!
 * \file SkillTrayTabEx.h
 *
 * \author Juver
 * \date August 2018
 *
 * modern skill tray
 */

#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "./SkillTrayTabDefine.h"

class CBasicSkillTrayEx;
class CMultiModeButton;
class CBasicQuickSkillSlot2;
class CBasicButton;

class CSkillTrayTabEx : public CUIGroup
{
	static INT NEXT_INDEX[MAX_TAB_INDEX];

public:
	enum
	{
		BASIC_QUICK_SKILL_TRAY_F1 = NO_ID + 1,
		BASIC_QUICK_SKILL_TRAY_F2,
		BASIC_QUICK_SKILL_TRAY_F3,
		BASIC_QUICK_SKILL_TRAY_F4,

		BASIC_TEXT_BUTTON_F1,
		BASIC_TEXT_BUTTON_F2,
		BASIC_TEXT_BUTTON_F3,
		BASIC_TEXT_BUTTON_F4,

		BASIC_QUICK_SKILL_BASE_SLOT,

		QUICK_SKILL_TRAY_CLOSE_BUTTON,
		QUICK_SKILL_TRAY_ROTATE_BUTTON,
		QUICK_SKILL_TRAY_LOCK_BUTTON,
		QUICK_SKILL_TRAY_UNLOCK_BUTTON,
		QUICK_SKILL_TRAY_SETTING_BUTTON,
		
	};

	enum
	{
		TABBUTTON_FOLDGROUP_ID = 1
	};

public:
	void CreateSubControl();

	void SetTabIndex( INT nIndex )			{ GASSERT( 0 <= nIndex && nIndex <= MAX_TAB_INDEX ); m_nTabIndex = nIndex; }
	INT GetTabIndex()						{ return m_nTabIndex; }

public:
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle( BOOL bVisible );
	void	SetShotcutText ( DWORD nID, CString& strTemp );

public:
	CSkillTrayTabEx(void);
	virtual ~CSkillTrayTabEx(void);

protected:
	CBasicSkillTrayEx*		m_pSkillTray[MAX_TAB_INDEX];
	CMultiModeButton*		m_pTextButton[MAX_TAB_INDEX];
	CBasicQuickSkillSlot2*	m_pBasicQuickSkillSlot;		

	CBasicButton*		m_pButtonClose;
	CBasicButton*		m_pButtonRotate;
	CBasicButton*		m_pButtonLock;
	CBasicButton*		m_pButtonUnlock;
	CBasicButton*		m_pButtonSetting;

	INT m_nTabIndex;
	INT m_nCount;

public:
	void	CheckMouseState ();

private:
	BOOL		m_bFirstGap;
	D3DXVECTOR2	m_vGap;
	int			m_PosX;
	int			m_PosY;
	bool		m_bCHECK_MOUSE_STATE;
};
