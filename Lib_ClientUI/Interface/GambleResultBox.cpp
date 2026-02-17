#include "StdAfx.h"

#include "./GambleResultBox.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Client/DxGlobalStage.h"
#include "./GameTextControl.h"
#include "../../Lib_Client/G-Logic/GLCharacter.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGambleResultBox::CGambleResultBox(void)
	: m_pControl(NULL)
	, m_TotElapsedTime(0.0f)
{
}

CGambleResultBox::~CGambleResultBox(void)
{
	m_TotElapsedTime = 0.0f;
}


void CGambleResultBox::CreateSubControl ()
{
	CUIControl * pControl = new CUIControl;
	pControl->CreateSub( this, "GAMBLE_RESULT" );
	RegisterControl( pControl );
	m_pControl = pControl;
}


void CGambleResultBox::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
	
	m_TotElapsedTime += fElapsedTime;

	if ( m_TotElapsedTime < 3.0f ) 
	{
		m_pControl->SetVisibleSingle( TRUE );
	}
	else 
	{
		CInnerInterface::GetInstance().HideGroup( GetWndID () );
		m_TotElapsedTime = 0.0f;
	}

}




