#include "StdAfx.h"
#include "ChangeColorNameWindow.h"
#include "InnerInterface.h"

#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "BasicTextButton.h"
#include "BasicLineBox.h"
#include "BasicTextBox.h"

#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "UITextControl.h"
#include "GameTextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* 5-15-23 Change Color Name Card - CNDev */

CChangeColorNameWindow::CChangeColorNameWindow(void)
: m_pButtonClose(NULL)
, m_pButtonSave(NULL)
, m_pInfoTitle(NULL)
, m_pInfoDesc(NULL)
, m_nSelectedColor( -1 )
, m_nSelectedLastColor( -1 )
{
	for( int i=0; i<CHANGE_COLOR_NAME_WINDOW_SIZE; ++i )
	{
		m_pTypeColor_Off[i] = NULL;
		m_pTypeColor_On[i] = NULL;
		m_pTypeColor_Box[i] = NULL;
		m_pTypeColor_Focus[i] = NULL;
	}
}

CChangeColorNameWindow::~CChangeColorNameWindow(void)
{
}

void CChangeColorNameWindow::CreateSubControl()
{
	CD3DFontPar* pFont12 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 12, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont15 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 15, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont08 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "RAN_CHANGE_COLOR_NAME_WINDOW_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "RAN_CHANGE_COLOR_NAME_PREVIEW_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "RAN_CHANGE_COLOR_NAME_INFORMATION_LINE" );
	RegisterControl ( pBasicLineBox );


	std::string strSchoolOff[CHANGE_COLOR_NAME_WINDOW_SIZE] = 
	{
		"RAN_CHANGE_COLOR_NAME_OFF_ONE",
		"RAN_CHANGE_COLOR_NAME_OFF_TWO",
		"RAN_CHANGE_COLOR_NAME_OFF_THREE",
		"RAN_CHANGE_COLOR_NAME_OFF_FOUR",
		"RAN_CHANGE_COLOR_NAME_OFF_FIVE",
		"RAN_CHANGE_COLOR_NAME_OFF_SIX",
	};

	std::string strSchoolOn[CHANGE_COLOR_NAME_WINDOW_SIZE] = 
	{
		"RAN_CHANGE_COLOR_NAME_ON_ONE",
		"RAN_CHANGE_COLOR_NAME_ON_TWO",
		"RAN_CHANGE_COLOR_NAME_ON_THREE",
		"RAN_CHANGE_COLOR_NAME_ON_FOUR",
		"RAN_CHANGE_COLOR_NAME_ON_FIVE",
		"RAN_CHANGE_COLOR_NAME_ON_SIX",
	};

	std::string strSchoolBox[CHANGE_COLOR_NAME_WINDOW_SIZE] = 
	{
		"RAN_CHANGE_COLOR_NAME_BOX_ONE",
		"RAN_CHANGE_COLOR_NAME_BOX_TWO",
		"RAN_CHANGE_COLOR_NAME_BOX_THREE",
		"RAN_CHANGE_COLOR_NAME_BOX_FOUR",
		"RAN_CHANGE_COLOR_NAME_BOX_FIVE",
		"RAN_CHANGE_COLOR_NAME_BOX_SIX",
	};

	std::string strSchoolFocus[CHANGE_COLOR_NAME_WINDOW_SIZE] = 
	{
		"RAN_CHANGE_COLOR_NAME_FOCUS_ONE",
		"RAN_CHANGE_COLOR_NAME_FOCUS_TWO",
		"RAN_CHANGE_COLOR_NAME_FOCUS_THREE",
		"RAN_CHANGE_COLOR_NAME_FOCUS_FOUR",
		"RAN_CHANGE_COLOR_NAME_FOCUS_FIVE",
		"RAN_CHANGE_COLOR_NAME_FOCUS_SIX",
	};

	for( int i=0; i<CHANGE_COLOR_NAME_WINDOW_SIZE; ++i )
	{

		m_pTypeColor_Off[i] = new CUIControl;
		m_pTypeColor_Off[i]->CreateSub ( this, strSchoolOff[i].c_str() );	
		m_pTypeColor_Off[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pTypeColor_Off[i] );

		m_pTypeColor_On[i] = new CUIControl;
		m_pTypeColor_On[i]->CreateSub ( this, strSchoolOn[i].c_str() );	
		m_pTypeColor_On[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pTypeColor_On[i] );

		m_pTypeColor_Box[i] = new CUIControl;
		m_pTypeColor_Box[i]->CreateSub ( this, strSchoolBox[i].c_str() );	
		m_pTypeColor_Box[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pTypeColor_Box[i] );

		m_pTypeColor_Focus[i] = new CUIControl;
		m_pTypeColor_Focus[i]->CreateSub ( this, strSchoolFocus[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHANGE_COLOR_NAME_WINDOW_SELECT_ONE + i );	
		m_pTypeColor_Focus[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pTypeColor_Focus[i] );
	}

	CBasicTextBox* pTextBox = NULL;
	pTextBox = CreateStaticControl ( "RAN_CHANGE_COLOR_NAME_STATIC_ONE", pFont15, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	pTextBox->SetOneLineText ( ID2GAMEWORD("RAN_CHANGE_COLOR_NAME_WINDOW_TEXT",0 ) );

	pTextBox = CreateStaticControl ( "RAN_CHANGE_COLOR_NAME_STATIC_TWO", pFont15, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	pTextBox->SetOneLineText ( ID2GAMEWORD("RAN_CHANGE_COLOR_NAME_WINDOW_TEXT",1 ) );

	pTextBox = CreateStaticControl ( "RAN_CHANGE_COLOR_NAME_STATIC_THREE", pFont15, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	pTextBox->SetOneLineText ( ID2GAMEWORD("RAN_CHANGE_COLOR_NAME_WINDOW_TEXT",2 ) );

	pTextBox = CreateStaticControl ( "RAN_CHANGE_COLOR_NAME_STATIC_FOUR", pFont15, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	pTextBox->SetOneLineText ( ID2GAMEWORD("RAN_CHANGE_COLOR_NAME_WINDOW_TEXT",3 ) );

	pTextBox = CreateStaticControl ( "RAN_CHANGE_COLOR_NAME_STATIC_FIVE", pFont15, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	pTextBox->SetOneLineText ( ID2GAMEWORD("RAN_CHANGE_COLOR_NAME_WINDOW_TEXT",4 ) );

	pTextBox = CreateStaticControl ( "RAN_CHANGE_COLOR_NAME_STATIC_SIX", pFont15, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	pTextBox->SetOneLineText ( ID2GAMEWORD("RAN_CHANGE_COLOR_NAME_WINDOW_TEXT",5 ) );


	m_pInfoTitle = new CBasicTextBox;
	m_pInfoTitle->CreateSub ( this, "RAN_CHANGE_COLOR_NAME_PREVIEW_TEXT", UI_FLAG_DEFAULT );
	m_pInfoTitle->SetFont ( pFont12 );
	m_pInfoTitle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );		
	RegisterControl ( m_pInfoTitle );

	m_pInfoDesc = new CBasicTextBox;
	m_pInfoDesc->CreateSub ( this, "RAN_CHANGE_COLOR_NAME_INFORMATION_TEXT", UI_FLAG_DEFAULT );
	m_pInfoDesc->SetFont ( pFont08 );
	m_pInfoDesc->SetTextAlign ( TEXT_ALIGN_LEFT );		
	RegisterControl ( m_pInfoDesc );	


	m_pButtonSave = new CBasicTextButton;
	m_pButtonSave->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CHANGE_COLOR_NAME_WINDOW_BUTTON_SAVE );
	m_pButtonSave->CreateBaseButton ( "RAN_CHANGE_COLOR_NAME_OK", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("RAN_CHANGE_COLOR_NAME_WINDOW", 2 ) );
	RegisterControl ( m_pButtonSave );

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CHANGE_COLOR_NAME_WINDOW_BUTTON_CANCEL );
	m_pButtonClose->CreateBaseButton ( "RAN_CHANGE_COLOR_NAME_CANCEL", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("RAN_CHANGE_COLOR_NAME_WINDOW", 1 ) );
	RegisterControl ( m_pButtonClose );

	ResetAll();
}

void CChangeColorNameWindow::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage( cID, dwMsg );

	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;

	case ET_CONTROL_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;


	case CHANGE_COLOR_NAME_WINDOW_SELECT_ONE:
	case CHANGE_COLOR_NAME_WINDOW_SELECT_TWO:
	case CHANGE_COLOR_NAME_WINDOW_SELECT_THREE:
	case CHANGE_COLOR_NAME_WINDOW_SELECT_FOUR:
	case CHANGE_COLOR_NAME_WINDOW_SELECT_FIVE:
	case CHANGE_COLOR_NAME_WINDOW_SELECT_SIX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) ) {
				int nSelect = cID - CHANGE_COLOR_NAME_WINDOW_SELECT_ONE;
				ColorMouseIn( nSelect );

				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) ){
					ColorSelect( nSelect );
				}
			}

		}break;

	case CHANGE_COLOR_NAME_WINDOW_BUTTON_SAVE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( m_nSelectedColor >= 0 && m_nSelectedColor < CHANGE_COLOR_NAME_WINDOW_SIZE )
				{
					if ( GLGaeaClient::GetInstance().GetCharacter()->m_wColorName == (WORD)m_nSelectedColor+1 ){
						CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_COLOR_NAME_SAME_COLOR") );
					}else{
						GLGaeaClient::GetInstance().GetCharacter()->ReqChangeColorNameMsg( (WORD)m_nSelectedColor+1 );
						CInnerInterface::GetInstance().HideGroup( GetWndID() );
					}
				}else{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_COLOR_NAME_NOT_SELECTED") );
				}
			}
		}break;

	case CHANGE_COLOR_NAME_WINDOW_BUTTON_CANCEL:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) ){
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;

	};

}

void CChangeColorNameWindow::Update ( int x ,int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	for( int i=0; i<CHANGE_COLOR_NAME_WINDOW_SIZE; ++i )
	{
		m_pTypeColor_Box[i]->SetVisibleSingle( FALSE );
	}

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_nSelectedColor != m_nSelectedLastColor )
	{
		UpdateUI();
		m_nSelectedLastColor = m_nSelectedColor;
	}
}

void CChangeColorNameWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		ResetAll();
	}
	else
	{

	}
}

void CChangeColorNameWindow::ResetAll()
{
	m_nSelectedColor = -1;

	for( int i=0; i<CHANGE_COLOR_NAME_WINDOW_SIZE; ++i )
	{
		m_pTypeColor_Off[i]->SetVisibleSingle( TRUE );
		m_pTypeColor_On[i]->SetVisibleSingle( FALSE );
		m_pTypeColor_Box[i]->SetVisibleSingle( FALSE );
		m_pTypeColor_Focus[i]->SetVisibleSingle( TRUE );
	}

	m_pInfoTitle->ClearText();
	m_pInfoDesc->ClearText();
}

void CChangeColorNameWindow::UpdateUI()
{
	const GLCHARLOGIC_CLIENT& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();

	for( int i=0; i<CHANGE_COLOR_NAME_WINDOW_SIZE; ++i )
	{
		m_pTypeColor_Off[i]->SetVisibleSingle( TRUE );
		m_pTypeColor_On[i]->SetVisibleSingle( FALSE );
	}

	m_pInfoTitle->ClearText();
	m_pInfoDesc->ClearText();

	if ( m_nSelectedColor >= 0 && m_nSelectedColor < CHANGE_COLOR_NAME_WINDOW_SIZE )
	{
		m_pTypeColor_On[m_nSelectedColor]->SetVisibleSingle( TRUE );

		//CString strText;
		//strText.Format ( "NS_UITEXTCOLOR::wColorName%d", m_nSelectedColor );
		//m_pInfoTitle->SetText( ID2GAMEWORD("RAN_CHANGE_COLOR_NAME_WINDOW_TEXT", m_nSelectedColor ), strText );	

		m_pInfoDesc->SetText ( ID2GAMEINTEXT("CHANGE_COLOR_NAME_INFORMATION"), NS_UITEXTCOLOR::WHITE );
	}

	CString strText;
	strText.Format("%s",sCharData.m_szName);

	if (m_nSelectedColor == 0)	m_pInfoTitle->SetText( strText, NS_UITEXTCOLOR::wColorName1 );	
	else if (m_nSelectedColor == 1)	m_pInfoTitle->SetText( strText , NS_UITEXTCOLOR::wColorName2 );
	else if (m_nSelectedColor == 2)	m_pInfoTitle->SetText( strText , NS_UITEXTCOLOR::wColorName3 );
	else if (m_nSelectedColor == 3)	m_pInfoTitle->SetText( strText , NS_UITEXTCOLOR::wColorName4 );
	else if (m_nSelectedColor == 4)	m_pInfoTitle->SetText( strText , NS_UITEXTCOLOR::wColorName5 );
	else if (m_nSelectedColor == 5)	m_pInfoTitle->SetText( strText , NS_UITEXTCOLOR::wColorName6 );

	/*cndev disable - much lesser code if else
	switch ( m_nSelectedColor )
	{
	case 0:
		{

		}
		break;
	}
	*/
}

void CChangeColorNameWindow::ColorSelect( int nSelect )
{
	if ( nSelect < 0 )	return;
	if ( nSelect >= CHANGE_COLOR_NAME_WINDOW_SIZE )	return;
	if ( nSelect == m_nSelectedColor )	return;

	m_nSelectedColor = nSelect;
}

void CChangeColorNameWindow::ColorMouseIn( int nSelect )
{
	if ( nSelect < 0 )	return;
	if ( nSelect >= CHANGE_COLOR_NAME_WINDOW_SIZE )	return;
	m_pTypeColor_Box[nSelect]->SetVisibleSingle( TRUE );
}