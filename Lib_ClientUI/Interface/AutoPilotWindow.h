#pragma once

#include "../../Lib_Engine/G-Logic/GLDefine.h"

#include "./UIWindowEx.h"

class CBasicTextBox;
class CBasicProgressBar;
class CBasicTextButton;

class CAutoPilotWindow : public CUIWindowEx
{
	enum
	{
		AUTO_PILOT_SCAN_RANGE_PROGRESS = ET_CONTROL_NEXT,
		AUTO_PILOT_APPLY_BUTTON,
		AUTO_PILOT_CLOSE_BUTTON,
		AUTO_PILOT_TOGGLE_BUTTON,
	};

public:
	CAutoPilotWindow(void);
	virtual ~CAutoPilotWindow(void);

public:
	void CreateSubControl();

	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	LoadCurrentOption ();

public:
	CBasicTextBox*	m_pScanRangeStatic;

	CBasicProgressBar*	m_pScanRangeProgressBar;

	CUIControl*	m_pScanRangeDummyControl;

	CBasicTextBox*	m_pScanRangeTextBox;

	CBasicTextButton* m_pApplyButton;
	CBasicTextButton* m_pCloseButton;
	CBasicTextButton* m_pToggleButton;

	float m_fRange;
	BOOL m_bActive;
};