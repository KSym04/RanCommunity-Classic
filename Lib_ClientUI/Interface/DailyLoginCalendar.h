#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"

/*12-16-24, daily login - CNDev*/

class CSwapImage;
class CUIControl;

class	CDailyLoginCalendar : public CUIGroup
{
private:
	enum
	{
		ATTENDANCE_CALENEDAR = NO_ID,
	};

public:
	CDailyLoginCalendar ();
	virtual	~CDailyLoginCalendar ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CUIControl * CreateControl( const char * szControl );

public:
	void	SetDays( int nDay );

private:
	CSwapImage*	m_pDays[2];

private:
	UINT		m_nDays;
};