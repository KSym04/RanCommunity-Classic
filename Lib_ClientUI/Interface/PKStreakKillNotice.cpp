#include "StdAfx.h"
#include "PKStreakKillNotice.h"
#include "./InnerInterface.h"
#include "./BasicTextBox.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EVENTNOTICE_RENDERTIME (float)20.0f

CPKStreakKillNotice::CPKStreakKillNotice ()	
	: m_pTextBox(NULL)
{
}

CPKStreakKillNotice::~CPKStreakKillNotice ()
{
}

void CPKStreakKillNotice::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 16, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, "PKSTREAK_KILL_NOTICE_TEXT" );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( pTextBox );
	m_pTextBox = pTextBox;
}

void CPKStreakKillNotice::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
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

int CPKStreakKillNotice::AddText(CString strMessage, D3DCOLOR dwMessageColor)
{
	if (m_pTextBox) {
		m_pTextBox->ClearText();
		int line = m_pTextBox->AddText(strMessage, dwMessageColor);
		m_fLifeTime = EVENTNOTICE_RENDERTIME;
		return line;
	}
	return 0;
}

int CPKStreakKillNotice::AddString(int Index, CString strMessage, D3DCOLOR dwMessageColor)
{
	if (m_pTextBox)
	{
		int line = m_pTextBox->AddString(Index, strMessage, dwMessageColor);
		//m_fLifeTime = EVENTNOTICE_RENDERTIME;

		return line;
	}
	return 0;
}
