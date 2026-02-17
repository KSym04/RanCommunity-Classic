#include "StdAfx.h"

#include "./BasicVarTextBoxExItemLink.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"
#include "./BasicLineBoxEx.h"
#include "./BasicVarTextBoxExIcon.h"

#include "./UITextControl.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/GUInterface/UIDebugSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	float	CBasicVarTextBoxExItemLink::fMOUSEPOINT_GAP = 20.0f;

CBasicVarTextBoxExItemLink::CBasicVarTextBoxExItemLink () :
	m_pIconImage(NULL),
	m_pTextBox(NULL),
	m_pTextBoxTitle(NULL),
	m_pLineBox(NULL),
	m_pFont11(NULL),
	m_pFont(NULL),
	m_pSelfDummy(NULL),
	m_pCloseButton(NULL),
	m_vMousePointGap (fMOUSEPOINT_GAP,fMOUSEPOINT_GAP),
	m_bBLOCK_MOUSETRACKING ( false )
{
}

CBasicVarTextBoxExItemLink::~CBasicVarTextBoxExItemLink ()
{
}

void CBasicVarTextBoxExItemLink::CreateSubControl ()
{
	m_pFont11 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 11, _DEFAULT_FONT_SHADOW_FLAG );
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pLineBox = new CBasicLineBoxEx;
	m_pLineBox->CreateSub ( this, "BASIC_VAR_LINE_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBox->CreateBaseBoxVarTextBox ( "BASIC_VAR_LINE_BOX" );
	m_pLineBox->SetNoUpdate ( true );
	RegisterControl ( m_pLineBox );

	m_pSelfDummy = new CUIControl;
	m_pSelfDummy->CreateSub ( this, "BASIC_VAR_LINE_BOX" );
	m_pSelfDummy->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pSelfDummy );

	m_pIconImage = new CBasicVarTextBoxExIcon;
	m_pIconImage->CreateSub ( this, "INFO_DISPLAY_ICON_IMAGE", UI_FLAG_LEFT | UI_FLAG_TOP );
	m_pIconImage->CreateSubControl();
	m_pIconImage->SetVisibleSingle ( FALSE );
	RegisterControl (m_pIconImage );

	m_pTextBox = new CBasicTextBox;
	m_pTextBox->CreateSub ( this, "VAR_TEXT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextBox->SetFont ( m_pFont );
	m_pTextBox->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pTextBox->SetNoUpdate ( true );
	RegisterControl ( m_pTextBox );

	m_pTextBoxTitle = new CBasicTextBox;
	m_pTextBoxTitle->CreateSub ( this, "VAR_TEXT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextBoxTitle->SetFont ( m_pFont11 );
	m_pTextBoxTitle->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pTextBoxTitle->SetNoUpdate ( true );
	RegisterControl ( m_pTextBoxTitle );

	m_pCloseButton = new CBasicButton;
	m_pCloseButton->CreateSub ( this, "INFO_DISPLAY_LINK_CLOSE" , UI_FLAG_RIGHT | UI_FLAG_TOP, BUTTON_CLOSE );
	m_pCloseButton->CreateFlip ( "INFO_DISPLAY_LINK_CLOSE_F", CBasicButton::CLICK_FLIP );
	m_pCloseButton->CreateMouseOver( "INFO_DISPLAY_LINK_CLOSE_OVER" );
	m_pCloseButton->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pCloseButton );
}

void CBasicVarTextBoxExItemLink::SetTextAlign ( int nALIGN )
{
	m_pTextBox->SetTextAlign ( nALIGN );
	m_pTextBoxTitle->SetTextAlign ( nALIGN );
}

void CBasicVarTextBoxExItemLink::ClearText ()
{
	if ( m_pTextBox ) m_pTextBox->ClearText ();
	if ( m_pTextBoxTitle ) m_pTextBoxTitle->ClearText ();

	if ( m_pIconImage )	
	{
		m_pIconImage->ResetIcon();
		m_pIconImage->SetVisibleSingle( FALSE );
	}
}

int CBasicVarTextBoxExItemLink::AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddTextNoSplit ( strText, dwColor );

	return -1;
}

int	CBasicVarTextBoxExItemLink::SetTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->SetTextNoSplit ( strText, dwColor );

	return -1;
}

void CBasicVarTextBoxExItemLink::SetText( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) m_pTextBox->SetText( strText, dwColor );
}

void CBasicVarTextBoxExItemLink::AddString ( int nIndex, const CString& strText, const D3DCOLOR& dwColor )
{
	if ( m_pTextBox ) m_pTextBox->AddString ( nIndex, strText, dwColor );
}

int CBasicVarTextBoxExItemLink::AddText ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddText ( strText, dwColor );

	return -1;
}

void CBasicVarTextBoxExItemLink::SetUseOverColor ( bool bUseColor )
{
	m_pTextBox->SetUseOverColor ( (bUseColor)?TRUE:FALSE );
}

void CBasicVarTextBoxExItemLink::SetOverColor ( const D3DCOLOR& dwColor )
{
	int nIndex = m_pTextBox->GetCount () - 1;
	m_pTextBox->SetOverColor ( nIndex, dwColor );
}

void CBasicVarTextBoxExItemLink::RePosControl ( int x, int y )
{
	const UIRECT& rcLocalPosDummy = m_pSelfDummy->GetLocalPos ();
	const UIRECT& rcOriginPos = GetLocalPos ();

	AlignSubControl ( rcOriginPos, rcLocalPosDummy );

	float fTotalGapX = 0.0f;
	float fTotalGapY = 0.0f;

	if ( m_pTextBox )
	{
		const UIRECT& rcTextBoxLocalPos = m_pTextBox->GetLocalPos ();
		int nTotalLine = m_pTextBox->GetTotalLine ();
		const float fAllLine = m_pTextBox->CalcMaxHEIGHT ( 0, nTotalLine );
		fTotalGapX = (float)m_pTextBox->GetLongestLine () - rcTextBoxLocalPos.sizeX;
		fTotalGapY = fAllLine - rcTextBoxLocalPos.sizeY;
	}

	if ( m_pTextBoxTitle && m_pTextBoxTitle->GetCount() > 0 )
	{
		const UIRECT& rcTextBoxLocalPos = m_pTextBoxTitle->GetLocalPos ();
		int nTotalLine = m_pTextBoxTitle->GetTotalLine ();
		const float fAllLine = m_pTextBoxTitle->CalcMaxHEIGHT ( 0, nTotalLine );
		const float fTextBoxGapX = (float)m_pTextBoxTitle->GetLongestLine () - rcTextBoxLocalPos.sizeX;
		const float fTextBoxGapY = fAllLine - rcTextBoxLocalPos.sizeY;
		fTotalGapX = max( fTotalGapX, fTextBoxGapX );
		fTotalGapY = max( fTotalGapY, fTextBoxGapY );
	}

	if ( m_pCloseButton && m_pCloseButton->IsVisible() )
	{
		fTotalGapX += m_pCloseButton->GetLocalPos().sizeX;
	}

	if ( m_pIconImage && m_pIconImage->IsVisible() )
	{
		const UIRECT& rcIconLocalPos = m_pIconImage->GetLocalPos ();
		float fCenterY = (rcIconLocalPos.top*2.0f);
		if ( ( rcIconLocalPos.sizeY + fCenterY ) > (rcOriginPos.sizeY + fTotalGapY))
		{
			fTotalGapY += ( ( rcIconLocalPos.sizeY + fCenterY ) - (rcOriginPos.sizeY + fTotalGapY) );
		}
	}

	UIRECT rcLocalNewPos = UIRECT ( float(x) + m_vMousePointGap.x, float(y) + m_vMousePointGap.y,
			rcOriginPos.sizeX + fTotalGapX, rcOriginPos.sizeY + fTotalGapY );

	AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );
	SetGlobalPos ( rcLocalNewPos );		
}

void CBasicVarTextBoxExItemLink::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	SetUseOverColor ( false );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( !m_bBLOCK_MOUSETRACKING ) RePosControl ( x, y );	
}

void CBasicVarTextBoxExItemLink::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case BUTTON_CLOSE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_LB_UP & dwMsg )
				{
					AddMessageEx ( UIMSG_UIVARTEXT_EX_BUTTON_CLOSE_CLICK );
				}

				AddMessageEx ( UIMSG_UIVARTEXT_EX_BUTTON_CLOSE_MOUSEIN );
			}
		}break;
	};

	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CBasicVarTextBoxExItemLink::SetMousePointGap ( D3DXVECTOR2 vGap )
{
	m_vMousePointGap = vGap;
}

float CBasicVarTextBoxExItemLink::GetLongestLine ()
{
	float fLine1 = 0.0f;
	float fLine2 = 0.0f;

	if ( m_pTextBox )
		fLine1 = (float)m_pTextBox->GetLongestLine ();

	if ( m_pTextBoxTitle )
		fLine2 = (float)m_pTextBoxTitle->GetLongestLine ();

	return max(fLine1, fLine2);
}

void CBasicVarTextBoxExItemLink::AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
{
	float fLongestLine = GetLongestLine ();

	NS_UITEXTCONTROL::MULTILINETEXT& MLTextSet =
		NS_UITEXTCONTROL::GetMLTextWithoutCRLF ( strText, fLongestLine, m_pFont );

	for ( int i = 0; i < (int)MLTextSet.size(); ++i )
	{
		CString strTemp = MLTextSet[i].strLine;
		if ( strTemp.GetLength() )
		{
			AddTextNoSplit ( strTemp, /*NS_UITEXTCOLOR::DEFAULT*/ dwColor );		
		}
	}	
}

void CBasicVarTextBoxExItemLink::AddStringNoSplit ( int nIndex, const CString& strText, const D3DCOLOR& dwColor )
{
	if ( m_pTextBox ) m_pTextBox->AddStringNoSplit ( nIndex, strText, dwColor );
}

int	CBasicVarTextBoxExItemLink::GetCount ()
{
	return m_pTextBox->GetCount ();
}

void CBasicVarTextBoxExItemLink::SetLineInterval ( const float fLineInterval )
{
	m_pTextBox->SetLineInterval ( fLineInterval );
}

void CBasicVarTextBoxExItemLink::SetBlockMouseTracking ( bool bBlock )
{
	m_bBLOCK_MOUSETRACKING = bBlock;
}

void CBasicVarTextBoxExItemLink::ShowCloseButton( bool bSHOW )
{
	if ( m_pCloseButton )
		m_pCloseButton->SetVisibleSingle( bSHOW );
}

void CBasicVarTextBoxExItemLink::AddTitleText( CString strText, DWORD dwColor )
{
	if ( m_pTextBoxTitle ) 
		m_pTextBoxTitle->AddTextNoSplit ( strText, dwColor );
}

void CBasicVarTextBoxExItemLink::SetIcon( SNATIVEID sICONINDEX, const char* szTexture )
{
	if ( m_pIconImage )	
	{
		m_pIconImage->SetIcon( sICONINDEX, szTexture );
		m_pIconImage->SetVisibleSingle( TRUE );
	}
}