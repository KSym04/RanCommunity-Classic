#include "StdAfx.h"
#include "./PVPCompetitionNotify.h"

#include "../../Lib_ClientUI/Interface/BasicButton.h"
#include "../../Lib_ClientUI/Interface/BasicTextBox.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"
#include "../../Lib_ClientUI/Interface/BasicLineBoxEx.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPCompetitionNotify::CPVPCompetitionNotify ()
	: m_pFont(NULL)
	, m_pLineBox(NULL)
	, m_pTextBox(NULL)
	, m_pTail(NULL)
	, m_pDummy(NULL)
	, m_pControlPos(NULL)
{
}

CPVPCompetitionNotify::~CPVPCompetitionNotify ()
{
}

void CPVPCompetitionNotify::CreateSubControl ()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	m_pLineBox = new CBasicLineBoxEx;
	m_pLineBox->CreateSub ( this, "BASIC_LINE_BOX_COMPETITION_NOTIFY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBox->CreateBaseBoxCompetitionNotify ( "BASIC_LINE_BOX_COMPETITION_NOTIFY" );
	m_pLineBox->SetNoUpdate ( true );
	RegisterControl ( m_pLineBox );

	m_pTextBox = new CBasicTextBox;
	m_pTextBox->CreateSub ( this, "PVP_COMPETITION_NOTIFY_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextBox->SetFont ( m_pFont );
	m_pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	m_pTextBox->SetNoUpdate ( true );
	RegisterControl ( m_pTextBox );

	m_pDummy = new CUIControl;
	m_pDummy->CreateSub ( this, "BASIC_LINE_BOX_COMPETITION_NOTIFY" );
	m_pDummy->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pDummy );

	m_pTail = new CUIControl;
	m_pTail->CreateSub ( this, "PVP_COMPETITION_NOTIFY_TAIL", UI_FLAG_CENTER_X, UI_FLAG_BOTTOM );
	RegisterControl ( m_pTail );
}

void CPVPCompetitionNotify::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pControlPos )
	{
		if ( m_pTextBox )
		{
			UIRECT rectControlPos = m_pControlPos->GetGlobalPos();
			const UIRECT& rcTextBoxLocalPos = m_pTextBox->GetLocalPos ();
			const UIRECT& rcLocalPosDummy = m_pDummy->GetLocalPos ();
			const UIRECT& rcOriginPos = GetLocalPos ();

			AlignSubControl ( rcOriginPos, rcLocalPosDummy );

			int nTotalLine = m_pTextBox->GetTotalLine ();
			const float fAllLine = m_pTextBox->CalcMaxHEIGHT ( 0, nTotalLine );
			const float fTextBoxGapX = (float)m_pTextBox->GetLongestLine () - rcTextBoxLocalPos.sizeX;
			const float fTextBoxGapY = fAllLine - rcTextBoxLocalPos.sizeY;

			float fFollowPosX = rectControlPos.left + ( rectControlPos.sizeX / 2 ) - ( fTextBoxGapX / 2 );
			float fFollowPosY = rectControlPos.top - ( rcOriginPos.sizeY + fTextBoxGapY ) - 8;
			UIRECT rcLocalNewPos = UIRECT ( fFollowPosX, fFollowPosY, rcOriginPos.sizeX + fTextBoxGapX, rcOriginPos.sizeY + fTextBoxGapY );

			AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );

			SetGlobalPos ( rcLocalNewPos );		

			if ( m_pTail )
			{
				UIRECT rcTailSize = m_pTail->GetLocalPos();
				float fX = rectControlPos.left + ( rectControlPos.sizeX / 2 ) - ( rcTailSize.sizeX / 2 );
				float fY = rectControlPos.top - ( rcTailSize.sizeY );
				m_pTail->SetGlobalPos( D3DXVECTOR2( fX, fY ) );
			}
		}
	}
}

void CPVPCompetitionNotify::SetControlPos( CUIControl* pControl )
{
	m_pControlPos = pControl;
}

void CPVPCompetitionNotify::SetNotify( std::string strText, DWORD dwColor )
{
	if ( m_pTextBox )
	{
		m_pTextBox->ClearText();
		m_pTextBox->SetTextNoSplit( strText.c_str(), dwColor );
	}
}