#pragma once

#include "./UIWindowEx.h"

class	CBasicLineBox;
class	CBasicTextBox;
class	CD3DFontPar;
class	CBasicComboBox;
class	CBasicComboBoxRollOver;
class	CBasicButton;
class	CBasicTextButton;

#define PERSONAL_LOCK_PIN_RECOVER_MAX_SELECTION 5

class CPersonalLockPinRecover : public CUIWindowEx
{
	enum
	{
		PERSONAL_LOCK_PIN_RECOVER_SELECTION_COMBO_OPEN = ET_CONTROL_NEXT,
		PERSONAL_LOCK_PIN_RECOVER_SELECTION_COMBO_ROLLOVER,
		PERSONAL_LOCK_PIN_RECOVER_BUTTON_RECOVER,
		PERSONAL_LOCK_PIN_RECOVER_BUTTON_CANCEL,
	};

public:
	CPersonalLockPinRecover(void);
	virtual ~CPersonalLockPinRecover(void);

public:
	void CreateSubControl();

private:
	CD3DFontPar*				m_pFont;
	CBasicLineBox*				m_pLineBoxMain;
	CBasicTextBox*				m_pStaticSelect;
	CBasicComboBox*				m_pComboBoxSelectOpen;
	CBasicComboBoxRollOver*		m_pComboBoxSelectRollOver;
	CBasicTextButton*			m_pButtonClose;
	CBasicTextButton*			m_pButtonRecover;


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
	void DoRecover();
};