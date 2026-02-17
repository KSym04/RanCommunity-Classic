#pragma	once

#include "./UIWindow.h"

class CBasicButton;
class CBasicProgressBar;
class CBasicTextBox;
class CBasicTextButton;
class CUIEditBox;

class	CCaptchaWindow : public CUIWindow
{
	static	const int nLIMIT_CHAR;
	static	const char BLANK_SYMBOL;

protected:
	enum
	{
		BUTTON_ENTER = ET_CONTROL_NEXT,
		CAPTCHA_EDIT
	};

public:
	CCaptchaWindow ();
	virtual	~CCaptchaWindow ();

public:
	void	ShowCaptcha ( DWORD dwCaptcha );
	void	BeginEdit();
	void	EndEdit();

public:	
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

private:
	CUIEditBox*	m_pEditBox;
	CBasicTextBox* m_pCaptchaText;

	CBasicTextBox*	m_pTextDesc;
	CBasicProgressBar*	m_pTimer;
};
