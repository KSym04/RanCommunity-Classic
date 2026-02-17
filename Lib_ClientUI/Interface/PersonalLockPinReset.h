#pragma once

#include "./UIWindowEx.h"

class	CBasicLineBox;
class	CBasicTextBox;
class	CD3DFontPar;
class	CBasicComboBox;
class	CBasicComboBoxRollOver;
class	CBasicButton;
class	CBasicTextButton;

#define PERSONAL_LOCK_PIN_RESET_MAX_SELECTION 5

class CPersonalLockPinReset : public CUIWindowEx
{
	enum
	{
		PERSONAL_LOCK_PIN_RESET_SELECTION_COMBO_OPEN = ET_CONTROL_NEXT,
		PERSONAL_LOCK_PIN_RESET_SELECTION_COMBO_ROLLOVER,
		PERSONAL_LOCK_PIN_RESET_BUTTON_RESET,
		PERSONAL_LOCK_PIN_RESET_BUTTON_CANCEL,
	};

public:
	CPersonalLockPinReset(void);
	virtual ~CPersonalLockPinReset(void);

public:
	void CreateSubControl();

private:
	CD3DFontPar*				m_pFont;
	CBasicLineBox*				m_pLineBoxMain;
	CBasicTextBox*				m_pStaticSelect;
	CBasicComboBox*				m_pComboBoxSelectOpen;
	CBasicComboBoxRollOver*		m_pComboBoxSelectRollOver;
	CBasicTextButton*			m_pButtonClose;
	CBasicTextButton*			m_pButtonReset;


private:
	UIGUID	m_RollOverID;
	BOOL	m_bFirstLBUP;
	int		m_nSelected;

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID cID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void LoadData();
	void SetTextComboPart( DWORD dwIndex, CBasicComboBox* pComboBox );
	void DoReset();
};