#pragma once

#include "./UIWindowEx.h"

#include "../../Lib_Client/G-Logic/GLPersonalLock.h"

class CBasicTextBox;
class CUIEditBoxMan;
class CBasicTextButton;

class CPersonalLockPinInput : public CUIWindowEx
{
	enum
	{
		PERSONAL_LOCK_PIN_INPUT_EDIT_MAN = ET_CONTROL_NEXT,
		PERSONAL_LOCK_PIN_INPUT_EDIT_PIN,
		PERSONAL_LOCK_PIN_INPUT_BUTTON_ENTER,
		PERSONAL_LOCK_PIN_INPUT_BUTTON_CANCEL,
	};

public:
	static UINT nLIMIT_PIN;

public:
	CPersonalLockPinInput(void);
	virtual ~CPersonalLockPinInput(void);

public:
	void CreateSubControl();

public:
	EMPERSONAL_LOCK		m_emLock;
	CBasicTextBox*		m_pInfoDesc;
	CBasicTextBox*		m_pInfoPin;
	CUIEditBoxMan*		m_pEditBoxMan;
	CBasicTextButton*	m_pButtonEnter;
	CBasicTextButton*	m_pButtonClose;
	
public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID cID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

	void	CheckInfo();
	void	SetLockType( EMPERSONAL_LOCK emLock );
};