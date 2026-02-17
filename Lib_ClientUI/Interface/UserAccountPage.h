#pragma	once

#include "./UIOuterWindow.h"

class CBasicTextBox;
class CBasicTextButton;

class CUserAccountPage : public CUIOuterWindow
{
	enum
	{
		ACCOUNT_PAGE_BUTTON_BACK = ET_CONTROL_NEXT,
		ACCOUNT_PAGE_BUTTON_DEVICE_CHECK,				/* login device check, Juver, 2020/11/05 */
	};

public:

public:
	CUserAccountPage();
	virtual	~CUserAccountPage();

public:
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle( BOOL bVisible );

public:
	void CreateSubControl();
	void InfoShow();
	void InfoReset();
	void AddErrorString( std::string strError, DWORD dwColor );

private:
	CBasicTextButton*	m_pButtonBack;
	CBasicTextBox*		m_pTextStatus;

	/* login device check, Juver, 2020/11/05 */
	CBasicTextBox*		m_pStaticLoginDeviceCheck;
	CBasicTextBox*		m_pTextLoginDeviceCheck;
	CBasicTextButton*	m_pButtonLoginDeviceCheck;
};