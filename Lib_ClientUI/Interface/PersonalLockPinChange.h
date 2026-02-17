#pragma once

#include "./UIWindowEx.h"

class	CBasicLineBox;
class	CBasicTextBox;
class	CD3DFontPar;
class	CBasicComboBox;
class	CBasicComboBoxRollOver;
class	CBasicButton;
class	CBasicTextButton;
class	CUIEditBoxMan;

#define PERSONAL_LOCK_PIN_CHANGE_MAX_SELECTION 4

class CPersonalLockPinChange : public CUIWindowEx
{
	enum
	{
		PERSONAL_LOCK_PIN_CHANGE_SELECTION_COMBO_OPEN = ET_CONTROL_NEXT,
		PERSONAL_LOCK_PIN_CHANGE_SELECTION_COMBO_ROLLOVER,
		PERSONAL_LOCK_PIN_CHANGE_BUTTON_RESET,
		PERSONAL_LOCK_PIN_CHANGE_BUTTON_CANCEL,

		PERSONAL_LOCK_PIN_CHANGE_EDIT_MAN,
		PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_OLD,
		PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_NEW,
		PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_NEW2,
	};

public:
	static UINT nLIMIT_PIN;

public:
	CPersonalLockPinChange(void);
	virtual ~CPersonalLockPinChange(void);

public:
	void CreateSubControl();

private:
	CD3DFontPar*				m_pFont;
	CBasicTextBox*				m_pStaticSelect;
	CBasicTextBox*				m_pStaticPinOld;
	CBasicTextBox*				m_pStaticPinNew;
	CBasicTextBox*				m_pStaticPinNew2;

	CBasicComboBox*				m_pComboBoxSelectOpen;
	CBasicComboBoxRollOver*		m_pComboBoxSelectRollOver;
	CBasicTextButton*			m_pButtonClose;
	CBasicTextButton*			m_pButtonChange;

	

	CUIEditBoxMan*				m_pEditBoxMan;

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
	void DoChange();
};