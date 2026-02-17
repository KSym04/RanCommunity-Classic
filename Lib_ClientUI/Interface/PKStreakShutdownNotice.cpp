/*
~ RRSECURTY DEVELOPMENTS 22/05/22 5:44 PM
*/

#include "StdAfx.h"
#include "PKStreakShutdownNotice.h"
#include "InnerInterface.h"
#include "./BasicTextBox.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EVENTNOTICE_RENDERTIME (float)20.0f

CPKStreakShutdownNotice::CPKStreakShutdownNotice ()	
	: m_pTextBox(NULL)
{
}

CPKStreakShutdownNotice::~CPKStreakShutdownNotice ()
{
}

void CPKStreakShutdownNotice::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 13, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, "PKSTREAK_SHUTDOWN_NOTICE_TEXT" );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( pTextBox );
	m_pTextBox = pTextBox;
}

void CPKStreakShutdownNotice::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ){
		return;
	}

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fLifeTime -= fElapsedTime;

	if ( m_fLifeTime < 0.0f ){
		m_pTextBox->ClearText();
		CInnerInterface::GetInstance().HideGroup ( GetWndID () );
		return;
	}
}

int CPKStreakShutdownNotice::AddText ( CString strMessage, D3DCOLOR dwMessageColor )
{
	if ( m_pTextBox ){
		m_pTextBox->ClearText();
		//m_pTextBox->AddText ( strMessage, dwMessageColor );
		int line = m_pTextBox->AddText(strMessage, dwMessageColor);
		m_fLifeTime = EVENTNOTICE_RENDERTIME;
	}
	return 0;
}

int CPKStreakShutdownNotice::AddString(int Index, CString strMessage, D3DCOLOR dwMessageColor)
{
	if (m_pTextBox)
	{
		int line = m_pTextBox->AddString(Index, strMessage, dwMessageColor);
		//m_fLifeTime = EVENTNOTICE_RENDERTIME;

		return line;
	}
	return 0;
}
