#include "StdAfx.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#include "../../Lib_Client/G-Logic/GLReborn.h"
#include "../../Lib_Client/G-Logic/GLCharData.h"
#include "../../Lib_Client/G-Logic/GLCharLogicClient.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "./BasicLineBox.h"
#include "./BasicTextButton.h"
#include "./BasicTextBox.h"

#include "./CharacterRebornAWindow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharacterRebornAWindow::CCharacterRebornAWindow(void)
	: m_pBackGround(NULL)
	, m_pButtonReborn( NULL )
	, m_pButtonCancel( NULL )
	, m_pTextBefore(NULL)
	, m_pTextAfter(NULL)
	, m_nX(0)
	, m_nY(0)
{
}

CCharacterRebornAWindow::~CCharacterRebornAWindow(void)
{
}

void CCharacterRebornAWindow::CreateSubControl()
{
	CD3DFontPar * pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pBackGround = CreateControl( "CHARACTER_REBORN_A_WINDOW_BACKGROUND" );
	m_pBackGround->SetVisibleSingle ( TRUE );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "CHARACTER_REBORN_A_WINDOW_INFO_LINEBOX1" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "CHARACTER_REBORN_A_WINDOW_INFO_LINEBOX2" );
	RegisterControl ( pBasicLineBox );

	m_pButtonReborn = new CBasicTextButton;
	m_pButtonReborn->CreateSub ( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, REBORN_BUTTON_OK );
	m_pButtonReborn->CreateBaseButton ( "CHARACTER_REBORN_A_WINDOW_BUTTON_REBORN", 
		CBasicTextButton::SIZE19, CBasicButton::CLICK_FLIP, ID2GAMEWORD ( "CHARACTER_REBORN_A_WINDOW", 1 ), _DEFAULT_FONT_SHADOW_FLAG );
	m_pButtonReborn->SetFlip ( TRUE );
	RegisterControl ( m_pButtonReborn );

	m_pButtonCancel = new CBasicTextButton;
	m_pButtonCancel->CreateSub ( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, REBORN_BUTTON_CANCEL );
	m_pButtonCancel->CreateBaseButton ( "CHARACTER_REBORN_A_WINDOW_BUTTON_CANCEL", 
		CBasicTextButton::SIZE19, CBasicButton::CLICK_FLIP, ID2GAMEWORD ( "CHARACTER_REBORN_A_WINDOW", 2 ),_DEFAULT_FONT_SHADOW_FLAG );
	m_pButtonCancel->SetFlip ( TRUE );
	RegisterControl ( m_pButtonCancel );

	m_pTextBefore = new CBasicTextBox;
	m_pTextBefore->CreateSub ( this, "CHARACTER_REBORN_A_WINDOW_INFO_TEXT1" );
	m_pTextBefore->SetFont ( pFont );
	m_pTextBefore->SetTextAlign ( TEXT_ALIGN_LEFT );	
	RegisterControl ( m_pTextBefore );

	m_pTextAfter = new CBasicTextBox;
	m_pTextAfter->CreateSub ( this, "CHARACTER_REBORN_A_WINDOW_INFO_TEXT2" );
	m_pTextAfter->SetFont ( pFont );
	m_pTextAfter->SetTextAlign ( TEXT_ALIGN_LEFT );	
	RegisterControl ( m_pTextAfter );
}

void CCharacterRebornAWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	const GLCHARLOGIC_CLIENT& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();	
	SetData ( sCharData );
	ShowRebornData( sCharData );
	CheckCard();

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CCharacterRebornAWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case REBORN_BUTTON_OK:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP )
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqRebornA ( m_nX, m_nY );
				}
			}
		}
		break;

	case REBORN_BUTTON_CANCEL:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP )
				{
					CInnerInterface::GetInstance().HideGroup ( GetWndID() );
				}
			}
		}
		break;
	};
}

void CCharacterRebornAWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
	}
}

void CCharacterRebornAWindow::SetCard ( int nPosX, int nPosY )
{
	m_nX = nPosX;
	m_nY = nPosY;
}

void CCharacterRebornAWindow::SetData ( const GLCHARLOGIC_CLIENT& sCharData )
{
	m_pTextBefore->ClearText();

	m_pTextBefore->AddText (  ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 3 )  );

	CString strTemp;
	strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 5 ), sCharData.m_sStats.wPow );
	m_pTextBefore->AddText ( strTemp.GetString()  );

	strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 6 ), sCharData.m_sStats.wDex );
	m_pTextBefore->AddText ( strTemp.GetString()  );

	strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 7 ), sCharData.m_sStats.wSpi );
	m_pTextBefore->AddText ( strTemp.GetString()  );

	strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 8 ), sCharData.m_sStats.wStr );
	m_pTextBefore->AddText ( strTemp.GetString()  );

	strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 9 ), sCharData.m_sStats.wSta );
	m_pTextBefore->AddText ( strTemp.GetString()  );

	strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 10 ), sCharData.m_wStatsPoint );
	m_pTextBefore->AddText ( strTemp.GetString()  );

	strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 11 ), sCharData.m_wLevel );
	m_pTextBefore->AddText ( strTemp.GetString()  );

	strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 12 ), sCharData.m_dwReborn );
	m_pTextBefore->AddText ( strTemp.GetString()  );
}

void CCharacterRebornAWindow::ShowRebornData( const GLCHARLOGIC_CLIENT& sCharData )
{
	m_pTextAfter->ClearText();
	m_pTextAfter->AddText (  ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 4 )  );

	SREBORN* pReborn = GLReborn::GetInstance().GetRebornData ( sCharData.m_dwReborn );
	SREBORN* pRebornMax = GLReborn::GetInstance().GetRebornDataMax();

	if ( pReborn && sCharData.m_dwReborn < GLReborn::GetInstance().GetMaxReborn() )
	{
		CString strTemp;
		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 5 ), pReborn->sStatsReplace.wPow );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 6 ), pReborn->sStatsReplace.wDex );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 7 ), pReborn->sStatsReplace.wSpi );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 8 ), pReborn->sStatsReplace.wStr );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 9 ), pReborn->sStatsReplace.wSta );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 10 ), pReborn->wRemainStats );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 11 ), pReborn->wLevelAfter );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 12 ), sCharData.m_dwReborn + pReborn->wRebornIncrease );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		m_pTextAfter->AddText ( ""  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 13 ), pReborn->dwPrice );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 14 ), pReborn->wLevel );
		m_pTextAfter->AddText ( strTemp.GetString()  );

	}
	else if ( pRebornMax && sCharData.m_dwReborn >= GLReborn::GetInstance().GetMaxReborn() )
	{
		CString strTemp;
		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 5 ), pRebornMax->sStatsReplace.wPow );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 6 ), pRebornMax->sStatsReplace.wDex );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 7 ), pRebornMax->sStatsReplace.wSpi );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 8 ), pRebornMax->sStatsReplace.wStr );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 9 ), pRebornMax->sStatsReplace.wSta );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 10 ), pRebornMax->wRemainStats );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 11 ), pRebornMax->wLevelAfter );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 12 ), sCharData.m_dwReborn + pRebornMax->wRebornIncrease );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		m_pTextAfter->AddText ( ""  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 13 ), pRebornMax->dwPrice );
		m_pTextAfter->AddText ( strTemp.GetString()  );

		strTemp.Format ("%s %d", ID2GAMEWORD("CHARACTER_REBORN_A_WINDOW", 14 ), pRebornMax->wLevel );
		m_pTextAfter->AddText ( strTemp.GetString()  );
	}	
}

void CCharacterRebornAWindow::CheckCard()
{
	//check if player change card position
	if ( ! IsVisible() )	
		return;

	SINVENITEM* pInvenItem = GLGaeaClient::GetInstance().GetCharacterLogic().m_cInventory.FindPosItem ( m_nX, m_nY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REBORN_CARD_POS_ERROR") );	
		CInnerInterface::GetInstance().HideGroup ( GetWndID() );
		return;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REBORN_CARD_ITEM_ERROR") );	
		CInnerInterface::GetInstance().HideGroup ( GetWndID() );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_REBORD_CARD_A )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REBORN_CARD_ITEMTYPE_ERROR") );	
		CInnerInterface::GetInstance().HideGroup ( GetWndID() );
		return;
	}
}