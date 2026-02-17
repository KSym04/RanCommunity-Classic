#include "StdAfx.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLCharacter.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLAutoPotion.h"

#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "./BasicLineBox.h"
#include "./BasicProgressBar.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"
#include "./BasicTextButton.h"
#include "./ItemMove.h"
#include "./AutoPotionWindow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAutoPotionWindow::CAutoPotionWindow(void)
	: m_pHPStatic(NULL)
	, m_pMPStatic(NULL)
	, m_pSPStatic(NULL)
	, m_pHPProgressBar(NULL)
	, m_pMPProgressBar(NULL)
	, m_pSPProgressBar(NULL)
	, m_pHPDummyControl(NULL)
	, m_pMPDummyControl(NULL)
	, m_pSPDummyControl(NULL)
	, m_pHPTextBox(NULL)
	, m_pMPTextBox(NULL)
	, m_pSPTextBox(NULL)
	, m_pApplyButton(NULL)
	, m_pCloseButton(NULL)
	, m_fHPThreshold(0.0f)
	, m_fMPThreshold(0.0f)
	, m_fSPThreshold(0.0f)
{
}

CAutoPotionWindow::~CAutoPotionWindow(void)
{
}

void CAutoPotionWindow::CreateSubControl()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite( "AUTO_POTION_WINDOW_REGION" );
	RegisterControl ( pBasicLineBox );

	CUIControl* pUIControl = new CUIControl;
	pUIControl->CreateSub ( this, "AUTO_POTION_WINDOW_REGION_TEXTURE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
	pUIControl->SetVisibleSingle ( TRUE );
	RegisterControl ( pUIControl );
	pUIControl->AlignToControl( pBasicLineBox, TRUE );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "AUTO_POTION_WINDOW_SETTING" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxOption ( "AUTO_POTION_HP_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxOption ( "AUTO_POTION_MP_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxOption ( "AUTO_POTION_SP_LINE" );
	RegisterControl ( pBasicLineBox );

	m_pHPStatic = new CBasicTextBox;
	m_pHPStatic->CreateSub ( this, "AUTO_POTION_HP_STATIC" );
	m_pHPStatic->SetFont ( pFont );
	m_pHPStatic->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pHPStatic->SetOneLineText ( ID2GAMEWORD ( "AUTO_POTION_WINDOW", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pHPStatic );

	m_pHPProgressBar = new CBasicProgressBar;
	m_pHPProgressBar->CreateSub ( this, "AUTO_POTION_HP_PROGRESS" );
	m_pHPProgressBar->CreateOverImage ( "AUTO_POTION_HP_PROGRESS_OVERIMAGE" );
	RegisterControl ( m_pHPProgressBar );

	m_pHPDummyControl = new CUIControl;
	m_pHPDummyControl->CreateSub ( this, "AUTO_POTION_HP_PROGRESS_DUMMY", UI_FLAG_DEFAULT, HP_THRESHOLD_PROGRESS );
	RegisterControl ( m_pHPDummyControl );

	m_pMPStatic = new CBasicTextBox;
	m_pMPStatic->CreateSub ( this, "AUTO_POTION_MP_STATIC" );
	m_pMPStatic->SetFont ( pFont );
	m_pMPStatic->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pMPStatic->SetOneLineText ( ID2GAMEWORD ( "AUTO_POTION_WINDOW", 2 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pMPStatic );
	
	m_pMPProgressBar = new CBasicProgressBar;
	m_pMPProgressBar->CreateSub ( this, "AUTO_POTION_MP_PROGRESS" );
	m_pMPProgressBar->CreateOverImage ( "AUTO_POTION_MP_PROGRESS_OVERIMAGE" );
	RegisterControl ( m_pMPProgressBar );

	m_pMPDummyControl = new CUIControl;
	m_pMPDummyControl->CreateSub ( this, "AUTO_POTION_MP_PROGRESS_DUMMY", UI_FLAG_DEFAULT, MP_THRESHOLD_PROGRESS );
	RegisterControl ( m_pMPDummyControl );

	m_pSPStatic = new CBasicTextBox;
	m_pSPStatic->CreateSub ( this, "AUTO_POTION_SP_STATIC" );
	m_pSPStatic->SetFont ( pFont );
	m_pSPStatic->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pSPStatic->SetOneLineText ( ID2GAMEWORD ( "AUTO_POTION_WINDOW", 3 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pSPStatic );
	
	m_pSPProgressBar = new CBasicProgressBar;
	m_pSPProgressBar->CreateSub ( this, "AUTO_POTION_SP_PROGRESS" );
	m_pSPProgressBar->CreateOverImage ( "AUTO_POTION_SP_PROGRESS_OVERIMAGE" );
	RegisterControl ( m_pSPProgressBar );	

	m_pSPDummyControl = new CUIControl;
	m_pSPDummyControl->CreateSub ( this, "AUTO_POTION_SP_PROGRESS_DUMMY", UI_FLAG_DEFAULT, SP_THRESHOLD_PROGRESS );
	RegisterControl ( m_pSPDummyControl );

	m_pHPTextBox = new CBasicTextBox;
	m_pHPTextBox->CreateSub ( this, "AUTO_POTION_HP_VALUE" );
	m_pHPTextBox->SetFont ( pFont );
	m_pHPTextBox->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pHPTextBox->SetOneLineText ( "xxx", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pHPTextBox );

	m_pMPTextBox = new CBasicTextBox;
	m_pMPTextBox->CreateSub ( this, "AUTO_POTION_MP_VALUE" );
	m_pMPTextBox->SetFont ( pFont );
	m_pMPTextBox->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pMPTextBox->SetOneLineText ( "xxx", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pMPTextBox );

	m_pSPTextBox = new CBasicTextBox;
	m_pSPTextBox->CreateSub ( this, "AUTO_POTION_SP_VALUE" );
	m_pSPTextBox->SetFont ( pFont );
	m_pSPTextBox->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pSPTextBox->SetOneLineText ( "xxx", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pSPTextBox );

	m_pApplyButton = new CBasicTextButton;
	m_pApplyButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, AUTO_POTION_APPLY_BUTTON );
	m_pApplyButton->CreateBaseButton ( "AUTO_POTION_BUTTON_APPLY", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "AUTO_POTION_WINDOW", 4 ) );
	RegisterControl ( m_pApplyButton );

	m_pCloseButton = new CBasicTextButton;
	m_pCloseButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, AUTO_POTION_CLOSE_BUTTON );
	m_pCloseButton->CreateBaseButton ( "AUTO_POTION_BUTTON_CLOSE", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "AUTO_POTION_WINDOW", 5 ) );
	RegisterControl ( m_pCloseButton );
}

void CAutoPotionWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( bFirstControl )
	{
		if ( m_pHPProgressBar && m_pHPProgressBar->IsExclusiveSelfControl() )
		{
			const float fSize = m_pHPProgressBar->GetGlobalPos ().sizeX;
			const float fPos = float(x) - m_pHPProgressBar->GetGlobalPos ().left;
			const float fPercent = fPos / fSize;
			m_pHPProgressBar->SetPercent ( fPercent );
		}

		if ( m_pMPProgressBar && m_pMPProgressBar->IsExclusiveSelfControl() )
		{
			const float fSize = m_pMPProgressBar->GetGlobalPos ().sizeX;
			const float fPos = float(x) - m_pMPProgressBar->GetGlobalPos ().left;
			const float fPercent = fPos / fSize;
			m_pMPProgressBar->SetPercent ( fPercent );
		}

		if ( m_pSPProgressBar && m_pSPProgressBar->IsExclusiveSelfControl() )
		{
			const float fSize = m_pSPProgressBar->GetGlobalPos ().sizeX;
			const float fPos = float(x) - m_pSPProgressBar->GetGlobalPos ().left;
			const float fPercent = fPos / fSize;
			m_pSPProgressBar->SetPercent ( fPercent );
		}
	}

	CString strText;
	if ( m_pHPTextBox )
	{
		m_pHPTextBox->ClearText();
		strText.Format( "%g %%", m_fHPThreshold );
		m_pHPTextBox->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pMPTextBox )
	{
		m_pMPTextBox->ClearText();
		strText.Format( "%g %%", m_fMPThreshold );
		m_pMPTextBox->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pSPTextBox )
	{
		m_pSPTextBox->ClearText();
		strText.Format( "%g %%", m_fSPThreshold );
		m_pSPTextBox->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
	}
}

void CAutoPotionWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case HP_THRESHOLD_PROGRESS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_DOWN_LIKE ( dwMsg ) )
				{			
					m_pHPProgressBar->SetExclusiveControl();
				}
				else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					m_pHPProgressBar->ResetExclusiveControl();
				}
			}
			else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
			{
				m_pHPProgressBar->ResetExclusiveControl();
			}

			m_fHPThreshold = floor( m_pHPProgressBar->GetPercent() * 100.0f);
		}
		break;

	case MP_THRESHOLD_PROGRESS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_DOWN_LIKE ( dwMsg ) )	
				{			
					m_pMPProgressBar->SetExclusiveControl();
				}
				else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					m_pMPProgressBar->ResetExclusiveControl();
				}
			}
			else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
			{
				m_pMPProgressBar->ResetExclusiveControl();
			}

			m_fMPThreshold = floor( m_pMPProgressBar->GetPercent() * 100.0f);
		}
		break;

	case SP_THRESHOLD_PROGRESS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_DOWN_LIKE ( dwMsg ) )
				{			
					m_pSPProgressBar->SetExclusiveControl();
				}
				else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					m_pSPProgressBar->ResetExclusiveControl();
				}
			}
			else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
			{
				m_pSPProgressBar->ResetExclusiveControl();
			}

			m_fSPThreshold = floor( m_pSPProgressBar->GetPercent() * 100.0f);
		}
		break;

	case AUTO_POTION_APPLY_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqSetAutoPotion( m_fHPThreshold, m_fMPThreshold, m_fSPThreshold );
			}
		}
		break;

	case AUTO_POTION_CLOSE_BUTTON:
	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup ( GetWndID () );
			}
		}
		break;

	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID () );
			}
		}
		break;
	}

	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
}

void CAutoPotionWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		LoadCurrentOption();
	}
}

void CAutoPotionWindow::LoadCurrentOption ()
{
	GLAutoPotion* pAutoPotion = GLGaeaClient::GetInstance().GetCharacter()->m_pAutoPotion;
	if ( pAutoPotion == NULL )
		return;

	m_fHPThreshold = pAutoPotion->m_fHPThreshold;
	m_fMPThreshold = pAutoPotion->m_fMPThreshold;
	m_fSPThreshold = pAutoPotion->m_fSPThreshold;

	m_pHPProgressBar->SetPercent ( m_fHPThreshold * 0.01f );
	m_pMPProgressBar->SetPercent ( m_fMPThreshold * 0.01f );
	m_pSPProgressBar->SetPercent ( m_fSPThreshold * 0.01f );
}
