#pragma once

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "../../Lib_Client/G-Logic/GLAutoPotionDef.h"

#include "./UIWindowEx.h"

class CBasicProgressBar;
class CBasicTextBox;
class CBasicButton;
class CBasicTextButton;
class CAutoPotionSlot;

class CAutoPotionWindow : public CUIWindowEx
{
	enum
	{
		HP_THRESHOLD_PROGRESS = ET_CONTROL_NEXT,
		MP_THRESHOLD_PROGRESS,
		SP_THRESHOLD_PROGRESS,
		AUTO_POTION_APPLY_BUTTON,
		AUTO_POTION_CLOSE_BUTTON,
	};

public:
	CAutoPotionWindow(void);
	virtual ~CAutoPotionWindow(void);

public:
	void CreateSubControl();

	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	LoadCurrentOption ();

private:
	CBasicTextBox*	m_pHPStatic;
	CBasicTextBox*	m_pMPStatic;
	CBasicTextBox*	m_pSPStatic;

	CBasicProgressBar*	m_pHPProgressBar;
	CBasicProgressBar*	m_pMPProgressBar;
	CBasicProgressBar*	m_pSPProgressBar;

	CUIControl*	m_pHPDummyControl;
	CUIControl*	m_pMPDummyControl;
	CUIControl*	m_pSPDummyControl;

	CBasicTextBox*	m_pHPTextBox;
	CBasicTextBox*	m_pMPTextBox;
	CBasicTextBox*	m_pSPTextBox;

	CBasicTextButton* m_pApplyButton;
	CBasicTextButton* m_pCloseButton;

	float m_fHPThreshold;
	float m_fMPThreshold;
	float m_fSPThreshold;
};