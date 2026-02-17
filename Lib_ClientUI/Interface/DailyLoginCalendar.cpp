#include "StdAfx.h"
#include "./DailyLoginCalendar.h"

#include "./BasicLineBoxSmart.h"
#include "./SwapImage.h"

#include "../../Lib_Engine/GUInterface/UIControl.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/GLDailyLogin.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-16-24, daily login - CNDev*/

CDailyLoginCalendar::CDailyLoginCalendar ()		
	: m_nDays(0)
{
}

CDailyLoginCalendar::~CDailyLoginCalendar ()
{
}

void CDailyLoginCalendar::CreateSubControl ()
{
	{
		CBasicLineBoxSmart* pLineBox = new CBasicLineBoxSmart();
		pLineBox->CreateSub( this, "RAN_DAILYLOGIN_CALENDAR_LINEBOX" );
		pLineBox->CreateSubControl( "RAN_DAILYLOGIN_CALENDAR_LINEBOX_TEXINFO" );
		RegisterControl( pLineBox );
	}

	{
		CUIControl* pControl = new CUIControl();
		pControl->CreateSub( this, "RAN_DAILYLOGIN_CALENDAR_LINEBOX_PANEL" );
		RegisterControl( pControl );
	}

	{
		CSwapImage* pImage;

		pImage = new CSwapImage();
		pImage->CreateSub( this, "RAN_DAILYLOGIN_CALENDAR_DAYS_10" );
		RegisterControl( pImage );
		m_pDays[0] = pImage;

		pImage = new CSwapImage();
		pImage->CreateSub( this, "RAN_DAILYLOGIN_CALENDAR_DAYS_1" );
		RegisterControl( pImage );
		m_pDays[1] = pImage;
	}
}

CUIControl* CDailyLoginCalendar::CreateControl( const char * szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );

	return pControl;
}

void CDailyLoginCalendar::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
    CUIGroup::TranslateUIMessage(ControlID, dwMsg); 
}
void CDailyLoginCalendar::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

}

void CDailyLoginCalendar::SetDays( int nDay )
{
	m_nDays = nDay;

	CUIControl BaseControl; 
	BaseControl.Create ( 1, "RAN_DAILYLOGIN_CALENDAR_NUMBER_BASE" );
	const UIRECT& rcBaseLocal = BaseControl.GetLocalPos();

	{
		UINT nNumber = m_nDays/10;
		m_pDays[0]->SetImageEx( "RAN_DAILYLOGIN_CALENDAR_NUMBER", (UINT)(rcBaseLocal.sizeX)*nNumber, 0, (UINT)rcBaseLocal.sizeX, (UINT)rcBaseLocal.sizeY );
	}

	{
		UINT nNumber = m_nDays%10;
		m_pDays[1]->SetImageEx( "RAN_DAILYLOGIN_CALENDAR_NUMBER", (UINT)(rcBaseLocal.sizeX)*nNumber, 0, (UINT)rcBaseLocal.sizeX, (UINT)rcBaseLocal.sizeY );
	}
}