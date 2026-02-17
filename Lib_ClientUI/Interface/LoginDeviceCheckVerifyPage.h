#pragma	once

#include "./UIOuterWindow.h"

class CUIEditBoxMan;
class CBasicTextBox;
class CBasicTextButton;

class CLoginDeviceCheckVerifyPage : public CUIOuterWindow
{
	enum
	{
		USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_OK = ET_CONTROL_NEXT,
		USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_CANCEL,

		USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_PASS_1,
		USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_PASS_2,
		USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_CAPTCHA,

		USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_MAN,
		USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_1,
		USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_2,
		USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_CAPTCHA,
	};

public:
	static int nLIMIT_PW;
	static int nLIMIT_RP;

public:
	CLoginDeviceCheckVerifyPage();
	virtual	~CLoginDeviceCheckVerifyPage();

public:
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );

	virtual	void SetVisibleSingle( BOOL bVisible );

public:
	void CreateSubControl();
	
	void ResetAll();
	void SetUser( std::string strUser );
	void CheckHideStatusPass1();
	void CheckHideStatusPass2();
	void CheckHideStatusCaptcha();

	void VerifyStart();
	void VerifyEnd();

	void AddErrorString( std::string strError, DWORD dwColor );

private:
	BOOL CheckString( CString strTemp );

private:
	CBasicTextBox*		m_pTextBoxDetail;
	CBasicTextBox*		m_pTextBoxUser;
	CBasicTextBox*		m_pTextBoxPass1;
	CBasicTextBox*		m_pTextBoxPass2;
	CBasicTextBox*		m_pTextBoxCaptcha;
	CBasicTextBox*		m_pTextBoxAccount;
	CUIEditBoxMan*		m_pEditBoxMan;
	CBasicTextBox*		m_pTextBoxStatus;
	
	CBasicTextButton*	m_pButtonPass1;
	CBasicTextButton*	m_pButtonPass2;
	CBasicTextButton*	m_pButtonCaptcha;
	CBasicTextButton*	m_pButtonOK;
	CBasicTextButton*	m_pButtonCancel;

private:
	std::string			m_strCaptchaText;
	std::string			m_strAccountText;
	BOOL				m_bShowPass1;
	BOOL				m_bShowPass2;
	BOOL				m_bShowCaptcha;


};