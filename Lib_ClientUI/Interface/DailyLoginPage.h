#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLDailyLogin.h"

/*12-16-24, daily login - CNDev*/

class CBasicTextBoxEx;
class CBasicScrollBarEx;
class CBasicTextBox;
class CBasicButton;
class CDailyLoginList;
class CDailyLoginCalendar;

class	CDailyLoginPage : public CUIGroup
{
private:
	enum
	{
		DAILYLOGIN_LIST = NO_ID + 1,
		DAILYLOGIN_CALENDAR,
	};

public:
	CDailyLoginPage ();
	virtual	~CDailyLoginPage ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void SetVisibleSingle ( BOOL bVisible );

private:
	CBasicTextBox*		m_pTextTitle;
	CBasicTextBox*		m_pTextDescription;
	CBasicTextBox*		m_pTextTimer;

	CDailyLoginList*	m_pDailyLoginList;
	CDailyLoginCalendar*	m_pDailyLoginCalendar;

	float				m_fUpdateTime;

private:
	void	LoadData();

};