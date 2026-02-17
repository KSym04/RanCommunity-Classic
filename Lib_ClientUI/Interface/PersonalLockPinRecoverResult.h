#pragma once

#include "./UIWindowEx.h"
#include "../../Lib_Client/G-Logic/GLPersonalLock.h"

class CBasicTextButton;
class CBasicTextBox;

class CPersonalLockPinRecoverResult : public CUIWindowEx
{
	enum
	{
		PERSONAL_LOCK_PIN_RECOVER_RESULT_BUTTON_CLOSE = ET_CONTROL_NEXT,
	};

public:
	CPersonalLockPinRecoverResult(void);
	virtual ~CPersonalLockPinRecoverResult(void);

public:
	void CreateSubControl();

private:
	CBasicTextButton*	m_pButtonClose;
	CBasicTextBox*		m_pTextResult;

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID cID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

	void InfoClear();
	void InfoAdd( SPERSONAL_LOCK_PIN* pPin, EMPERSONAL_LOCK emLock );
};