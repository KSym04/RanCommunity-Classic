#include "StdAfx.h"
#include "PlatinumBuffWindow.h"

#include "../BasicLineBox.h"
#include "../BasicTextButton.h"

#include "../BasicTextBox.h"

#include "../UITextControl.h"
#include "../GameTextControl.h"

#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../InnerInterface.h"

#include "../BasicComboBox.h"
#include "../BasicComboBoxRollOver.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_Engine/Common/StringFormat.h"

#include "../Lib_ClientUI/Interface/Util/CheckBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CPlatinumBuffWindow::nSTARTLINE = 0;

CPlatinumBuffWindow::CPlatinumBuffWindow ()
	: m_pApplyButton( NULL )
	, m_pCancelButton( NULL )
	, m_pRemainText( NULL )
	, m_pMoneyText( NULL )
	, m_pPointText( NULL )
	, m_pAccountCurrency( NULL )
	, m_pBuffInfo( NULL )
	, m_bGold( false )
{
}

CPlatinumBuffWindow::~CPlatinumBuffWindow ()
{
}

void CPlatinumBuffWindow::CreateSubControl ()
{

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CreateUILineBoxQuestList("PLATINUM_WINDOW_LINE");
	CreateUILineBoxQuestList("PLATINUM_LEFT_SIDE_LINE");
	CreateUILineBoxQuestList("PLATINUM_INFO_LINE");
	CreateUILineBoxQuestList("PLATINUM_BUFF_LINE");
	CreateUILineBoxQuestList("PLATINUM_RIGHT_LINE");
	CreateUILineBoxQuestList("PLATINUM_STAT_LINE");
	CreateUILineBoxQuestList("PLATINUM_MONEY_REQ_BACK");
	CreateUILineBoxQuestList("PLATINUM_POINT_REQ_BACK");
	
	CreateUILineBoxWhiteNoBody("PLATINUM_LEFT_SIDE_LINE");
	CreateUILineBoxWhiteNoBody("PLATINUM_INFO_LINE");
	CreateUILineBoxWhiteNoBody("PLATINUM_BUFF_LINE");
	CreateUILineBoxWhiteNoBody("PLATINUM_RIGHT_LINE");
	CreateUILineBoxWhiteNoBody("PLATINUM_STAT_LINE");
	CreateUILineBoxWhiteNoBody("PLATINUM_MONEY_REQ_BACK");
	CreateUILineBoxWhiteNoBody("PLATINUM_POINT_REQ_BACK");
	
	
	CreateStaticControl("PLATINUM_WINDOW_INFO_STATIC", pFont9, ID2GAMEWORD("PLATINUM_WINDOW_STATIC",0), NS_UITEXTCOLOR::PALEGOLDENROD, TEXT_ALIGN_BOTH_X);
	CreateStaticControl("PLATINUM_WINDOW_REMAIN_STATIC", pFont9, ID2GAMEWORD("PLATINUM_WINDOW_STATIC",1), NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_X);
	CreateStaticControl("PLATINUM_WINDOW_BUFF_STATIC", pFont9, ID2GAMEWORD("PLATINUM_WINDOW_STATIC",2), NS_UITEXTCOLOR::PALEGOLDENROD, TEXT_ALIGN_BOTH_X);
	CreateStaticControl("PLATINUM_WINDOW_STATS_STATIC", pFont9, ID2GAMEWORD("PLATINUM_WINDOW_STATIC",5), NS_UITEXTCOLOR::PALEGOLDENROD, TEXT_ALIGN_BOTH_X);
	CreateStaticControl("PLATINUM_WINDOW_PERIOD_STATIC", pFont9, ID2GAMEWORD("PLATINUM_WINDOW_STATIC",3), NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_X);

	m_pRemainText = CreateText("PLATINUM_REMAIN_TEXT", pFont9, TEXT_ALIGN_BOTH_X);
	m_pMoneyText = CreateText("PLATINUM_MONEY_TEXT", pFont9, TEXT_ALIGN_LEFT);
	m_pPointText = CreateText("PLATINUM_POINT_TEXT", pFont9, TEXT_ALIGN_LEFT);
	m_pAccountCurrency = CreateText("PLATINUM_ACCOUNT_POINT_TEXT", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_X);
	m_pBuffInfo = CreateText("PLATINUM_BUFF_INFO", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_X);

	m_pApplyButton = new CBasicTextButton;
	m_pApplyButton->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, PLATINUM_BUFF_BUTTON_APPLY );
	m_pApplyButton->CreateBaseButton ( "PLATINUM_APPLY_BUTTON", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, "Apply" );
	m_pApplyButton->SetFlip ( TRUE );
	RegisterControl ( m_pApplyButton );

	m_pCancelButton = new CBasicTextButton;
	m_pCancelButton->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, PLATINUM_BUFF_BUTTON_CLOSE );
	m_pCancelButton->CreateBaseButton ( "PLATINUM_CANCEL_BUTTON", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, "Cancel" );
	m_pCancelButton->SetFlip ( TRUE );
	RegisterControl ( m_pCancelButton );
	
	CBasicComboBox* pComboBox = NULL;
	CBasicComboBoxRollOver*	pComboBoxRollOver = NULL;

	pComboBox = new CBasicComboBox;
	pComboBox->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, PLATINUM_TYPE_COMBO_BUTTON );
	pComboBox->CreateBaseComboBox ( "PLATINUM_TYPE_COMBO_BUTTON" );			
	RegisterControl ( pComboBox );
	m_pComboBoxTypeOpen = pComboBox;

	pComboBoxRollOver = new CBasicComboBoxRollOver;
	pComboBoxRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PLATINUM_TYPE_COMBO_ROLLOVER );
	pComboBoxRollOver->CreateBaseComboBoxRollOver ( "PLATINUM_TYPE_COMBO_ROLLOVER" );			
	pComboBoxRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( pComboBoxRollOver );
	m_pComboBoxTypeRollOver = pComboBoxRollOver;

	m_pComboBoxTypeRollOver->ClearText ();

	for ( int i = 0; i < 6; i++ )
	{
		m_pComboBoxTypeRollOver->AddText( ID2GAMEWORD("PLATINUM_BUFF_TYPES",i), NS_UITEXTCOLOR::WHITE );
	}

	m_pComboBoxTypeOpen->SetText ( ID2GAMEWORD("PLATINUM_BUFF_TYPES",0) );
	
	CBasicComboBox* pDurationComboBox = NULL;
	CBasicComboBoxRollOver*	pComboBoxDurationRollOver = NULL;

	pDurationComboBox = new CBasicComboBox;
	pDurationComboBox->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, PLATINUM_COMBO_BUTTON );
	pDurationComboBox->CreateBaseComboBox ( "PLATINUM_COMBO_BUTTON" );			
	RegisterControl ( pDurationComboBox );
	m_pComboBoxDurationTypeOpen = pDurationComboBox;

	pComboBoxDurationRollOver = new CBasicComboBoxRollOver;
	pComboBoxDurationRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PLATINUM_COMBO_ROLLOVER );
	pComboBoxDurationRollOver->CreateBaseComboBoxRollOver ( "PLATINUM_COMBO_ROLLOVER" );			
	pComboBoxDurationRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( pComboBoxDurationRollOver );
	m_pComboBoxDurationTypeRollOver = pComboBoxDurationRollOver;

	m_pComboBoxDurationTypeRollOver->ClearText ();

	for ( int i = 0; i < 3; i++ )
	{
		m_pComboBoxDurationTypeRollOver->AddText( ID2GAMEWORD("PLATINUM_ROLL_OPTION",i), NS_UITEXTCOLOR::WHITE );
	}

	m_pComboBoxDurationTypeOpen->SetText ( ID2GAMEWORD("PLATINUM_ROLL_OPTION",0) );

	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, "PLATINUM_BUTTON_RES_INVENTORY_MONEY_BUTTON", UI_FLAG_DEFAULT );
	pButton->CreateFlip ( "PLATINUM_BUTTON_RES_INVENTORY_MONEY_BUTTON_F", CBasicButton::CLICK_FLIP );
	pButton->SetControlNameEx ( "PLATINUM_BUTTON_RES_INVENTORY_MONEY_BUTTON" );
	RegisterControl ( pButton );

	pButton = new CBasicButton;
	pButton->CreateSub ( this, "PLATINUM_BUTTON_RES_INVENTORY_POINT_BUTTON", UI_FLAG_DEFAULT );
	pButton->CreateFlip ( "PLATINUM_BUTTON_RES_INVENTORY_POINT_BUTTON_F", CBasicButton::CLICK_FLIP );
	pButton->SetControlNameEx ( "PLATINUM_BUTTON_RES_INVENTORY_POINT_BUTTON" );
	RegisterControl ( pButton );

	m_pPlatinumMarkNone = new CUIControl;
	m_pPlatinumMarkNone->CreateSub ( this, "PLATINUM_WINDOW_MARK_NONE", UI_FLAG_DEFAULT, PLATINUM_WINDOW_MARK_NONE );
	m_pPlatinumMarkNone->SetTransparentOption( TRUE );
	RegisterControl ( m_pPlatinumMarkNone );

	m_pPlatinumMarkBlue = new CUIControl;
	m_pPlatinumMarkBlue->CreateSub ( this, "PLATINUM_WINDOW_MARK_BLUE", UI_FLAG_DEFAULT, PLATINUM_WINDOW_MARK_BLUE );
	m_pPlatinumMarkBlue->SetTransparentOption( TRUE );
	RegisterControl ( m_pPlatinumMarkBlue );
	
	m_pOpenType[0] = new CCheckBox;
	m_pOpenType[0]->CreateSub ( this, "PLATINUM_CHECKBOX_MONEY", UI_FLAG_DEFAULT, PLATINUM_CHECKBOX_MONEY );
	m_pOpenType[0]->CreateSubControl ( "PLATINUM_CHECKBOX_OFF", "PLATINUM_CHECKBOX_ON" );
	m_pOpenType[0]->SetCheck( TRUE );
	RegisterControl ( m_pOpenType[0] );

	m_pOpenType[1] = new CCheckBox;
	m_pOpenType[1]->CreateSub ( this, "PLATINUM_CHECKBOX_POINT", UI_FLAG_DEFAULT, PLATINUM_CHECKBOX_POINT );
	m_pOpenType[1]->CreateSubControl ( "PLATINUM_CHECKBOX_OFF", "PLATINUM_CHECKBOX_ON" );
	m_pOpenType[1]->SetCheck( FALSE );
	RegisterControl ( m_pOpenType[1] );
	
	SelectType(0);
	SelectDurationType(-1);

}

CBasicLineBox* CPlatinumBuffWindow::CreateUILineBoxWhiteNoBody( char* szBaseControl )
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( szBaseControl );
	RegisterControl ( pLineBox );
	return pLineBox;
}

CBasicLineBox* CPlatinumBuffWindow::CreateUILineBoxQuestList( char* szBaseControl )
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( szBaseControl );
	RegisterControl ( pLineBox );
	return pLineBox;
}

CBasicLineBox* CPlatinumBuffWindow::CreateUILineBoxWhite( char* szBaseControl )
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhite ( szBaseControl );
	RegisterControl ( pLineBox );
	return pLineBox;
}

CBasicTextBox* CPlatinumBuffWindow::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, CString strText, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	pStaticText->AddText( strText.GetString(), dwColor );
	RegisterControl ( pStaticText );
	return pStaticText;
}

CBasicTextBox* CPlatinumBuffWindow::CreateText ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );
	return pStaticText;
}

CUIControl*	CPlatinumBuffWindow::CreateControl( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT );
	RegisterControl ( pControl );
	return pControl;
}

void CPlatinumBuffWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	GLCHARLOGIC_CLIENT& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter ();

	if( pCharacter->m_bPlatinumUsing )
	{
		if( m_pRemainText )
		{			
			CTime tPlatinumExpiry;
			tPlatinumExpiry = pCharacter->m_tPlatinumExpiryTime;
			CString strTime = tPlatinumExpiry.Format(_T("%Y-%m-%d %H:%M:%S"));
			CString szTemp;
			szTemp.Format("%s",strTime.GetString());

			m_pRemainText->SetText( szTemp, NS_UITEXTCOLOR::WHITE );
		}
	}
	else
	{
		CTime cCurTime = CTime::GetCurrentTime();
		CTime cTimeEnd(pCharacter->m_tPlatinumExpiryTime);
		
		if (cCurTime > cTimeEnd)
		{
			m_pRemainText->SetText("Not Applicable", NS_UITEXTCOLOR::RED);
		}
	}

	if( m_pAccountCurrency )
	{
		std::string strText = "";

		strText = sc::string::format( "%s: ", ID2GAMEWORD("PLATINUM_WINDOW_CURRENCY", 0) );
		
		int nIndex = m_pAccountCurrency->AddText( strText.c_str(), NS_UITEXTCOLOR::WHITE );
		strText = sc::string::format( "%d", ref_CharData.m_lnMoney );
		m_pAccountCurrency->AddString(nIndex, strText.c_str(), NS_UITEXTCOLOR::ORNAGERED );
		m_pAccountCurrency->AddString(nIndex, "|", NS_UITEXTCOLOR::WHITE );
		strText = sc::string::format( "%s: ", ID2GAMEWORD("PLATINUM_WINDOW_CURRENCY", 1) );
		m_pAccountCurrency->AddString(nIndex, strText.c_str(), NS_UITEXTCOLOR::WHITE );
		strText = sc::string::format( "%d", ref_CharData.m_dwPPoints );
		m_pAccountCurrency->AddString(nIndex, strText.c_str(), NS_UITEXTCOLOR::DODGERBLUE );
	}
}

void CPlatinumBuffWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;

	case PLATINUM_BUFF_BUTTON_APPLY:
		{
			int nIndexType = m_pComboBoxTypeRollOver->GetSelectIndex ();
			int nIndexDuration = m_pComboBoxDurationTypeRollOver->GetSelectIndex ();
			if ( nIndexType < 0 || nIndexDuration < 0 ) return ;
			
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if( m_pOpenType[0]->IsChecked() )		DoPlatinumBuff( nIndexType, nIndexDuration, true );
				else if( m_pOpenType[1]->IsChecked() )	DoPlatinumBuff( nIndexType, nIndexDuration, false );
			}
		}
		break;
		
	case PLATINUM_BUFF_BUTTON_CLOSE:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) ){
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;
		
	case PLATINUM_TYPE_COMBO_BUTTON:
	{
		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{
			if ( dwMsg & UIMSG_LB_DOWN )
			{
				if( !m_pComboBoxTypeRollOver->IsVisible() )	m_pComboBoxTypeRollOver->SetVisibleSingle ( TRUE );
				else										m_pComboBoxTypeRollOver->SetVisibleSingle ( FALSE );
				m_pComboBoxTypeRollOver->SetScrollPercent ( 0.0f );
			}
		}
	}
	break;

	case PLATINUM_TYPE_COMBO_ROLLOVER:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{				
				int nIndex = m_pComboBoxTypeRollOver->GetSelectIndex ();
				if ( nIndex < 0 ) return ;

				m_pComboBoxTypeRollOver->SetVisibleSingle ( FALSE );
				m_pComboBoxTypeOpen->SetText ( (char*)ID2GAMEWORD("PLATINUM_BUFF_TYPES", nIndex) );

				SelectType( nIndex );
			}
		}
		break;

	case PLATINUM_COMBO_BUTTON:
	{
		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{
			if ( dwMsg & UIMSG_LB_DOWN )
			{
				if( !m_pComboBoxDurationTypeRollOver->IsVisible() )	m_pComboBoxDurationTypeRollOver->SetVisibleSingle ( TRUE );
				else												m_pComboBoxDurationTypeRollOver->SetVisibleSingle ( FALSE );
				m_pComboBoxDurationTypeRollOver->SetScrollPercent ( 0.0f );
			}
		}
	}
	break;

	case PLATINUM_COMBO_ROLLOVER:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{				
				int nIndex = m_pComboBoxDurationTypeRollOver->GetSelectIndex ();
				if ( nIndex < 0 ) return ;

				m_pComboBoxDurationTypeRollOver->SetVisibleSingle ( FALSE );
				m_pComboBoxDurationTypeOpen->SetText ( (char*)ID2GAMEWORD("PLATINUM_ROLL_OPTION", nIndex) );

				SelectDurationType( nIndex );
			}
		}
		break;
	
	case PLATINUM_CHECKBOX_MONEY:
		{
			if( dwMsg & UIMSG_CHECKBOX_PUSH )
			{
				if ( m_pOpenType[1] ) 
					m_pOpenType[1]->SetCheck( !m_pOpenType[0]->IsChecked() );
				
				
			}
		}
		break;
	case PLATINUM_CHECKBOX_POINT:
		{
			if( dwMsg & UIMSG_CHECKBOX_PUSH )
			{
				if ( m_pOpenType[0] ) 
					m_pOpenType[0]->SetCheck( !m_pOpenType[1]->IsChecked() );
			}
		}
		break;
	};
}

void CPlatinumBuffWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible ){}
	else
	{
		Reset();
	}
}

void CPlatinumBuffWindow::SelectType( int nSelect )
{
	Reset();
	
	switch( nSelect )
	{
	case 0:
		m_pBuffInfo->ClearText();
		break;
	case 1:
		{
			m_pBuffInfo->ClearText();
			m_pBuffInfo->AddText( ID2GAMEWORD("PLATINUM_WINDOW_BUFFS",0), NS_UITEXTCOLOR::DODGERBLUE );
			m_pBuffInfo->AddText( ID2GAMEWORD("PLATINUM_WINDOW_BUFFS",1), NS_UITEXTCOLOR::DODGERBLUE );
		}break;
	case 2:
		{
			m_pBuffInfo->ClearText();
			m_pBuffInfo->AddText( ID2GAMEWORD("PLATINUM_WINDOW_BUFFS_1",0), NS_UITEXTCOLOR::DODGERBLUE );
			m_pBuffInfo->AddText( ID2GAMEWORD("PLATINUM_WINDOW_BUFFS_1",1), NS_UITEXTCOLOR::DODGERBLUE );
		}break;
	case 3:
		{
			m_pBuffInfo->ClearText();
			m_pBuffInfo->AddText( ID2GAMEWORD("PLATINUM_WINDOW_BUFFS_2",0), NS_UITEXTCOLOR::DODGERBLUE );
			m_pBuffInfo->AddText( ID2GAMEWORD("PLATINUM_WINDOW_BUFFS_2",1), NS_UITEXTCOLOR::DODGERBLUE );
		}break;
	case 4:
		{
			m_pBuffInfo->ClearText();
			m_pBuffInfo->AddText( ID2GAMEWORD("PLATINUM_WINDOW_BUFFS_3",0), NS_UITEXTCOLOR::DODGERBLUE );
			m_pBuffInfo->AddText( ID2GAMEWORD("PLATINUM_WINDOW_BUFFS_3",1), NS_UITEXTCOLOR::DODGERBLUE );
		}break;
	case 5:
		{
			m_pBuffInfo->ClearText();
			m_pBuffInfo->AddText( ID2GAMEWORD("PLATINUM_WINDOW_BUFFS_4",0), NS_UITEXTCOLOR::DODGERBLUE );
			m_pBuffInfo->AddText( ID2GAMEWORD("PLATINUM_WINDOW_BUFFS_4",1), NS_UITEXTCOLOR::DODGERBLUE );
		}break;
	}
}

void CPlatinumBuffWindow::SelectDurationType( int nSelect )
{
	Reset();

	switch( nSelect )
	{
	case 0:
		{
			if( m_pMoneyText )
			{
				CString strMoney, strReqMoney;
				
				m_pMoneyText->ClearText();
				LONGLONG llGoldDurationCost = GLCONST_CHAR::llGoldDurationCost[0];
				strReqMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( llGoldDurationCost, 3, "," );
				
				D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
				if( GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney >= llGoldDurationCost )
				{
					dwColor = NS_UITEXTCOLOR::WHITE;
				}

				strMoney.Format( "%s", strReqMoney );
				m_pMoneyText->SetText( strMoney.GetString(), dwColor );
			}

			if( m_pPointText )
			{
				CString strPoint, strReqPoint;
				
				m_pPointText->ClearText();
				DWORD dwPointDurationCost = GLCONST_CHAR::dwPointDurationCost[0];
				strReqPoint = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT(dwPointDurationCost, 3, "," );
				
				D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
				if( GLGaeaClient::GetInstance().GetCharacterLogic().m_dwPPoints >= dwPointDurationCost)
				{
					dwColor = NS_UITEXTCOLOR::WHITE;
				}

				strPoint.Format( "%s", strReqPoint );
				m_pPointText->SetText( strPoint.GetString(), dwColor );
			}
		}
		break;
	case 1:
		{
			if( m_pMoneyText )
			{
				CString strMoney, strReqMoney;
				
				m_pMoneyText->ClearText();
				LONGLONG llGoldDurationCost = GLCONST_CHAR::llGoldDurationCost[1];
				strReqMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( llGoldDurationCost, 3, "," );
				
				D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
				if( GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney >= llGoldDurationCost )
				{
					dwColor = NS_UITEXTCOLOR::WHITE;
				}

				strMoney.Format( "%s", strReqMoney );
				m_pMoneyText->SetText( strMoney.GetString(), dwColor );
			}

			if( m_pPointText )
			{
				CString strPoint, strReqPoint;
				
				m_pPointText->ClearText();
				DWORD dwPointDurationCost = GLCONST_CHAR::dwPointDurationCost[1];
				strReqPoint = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( dwPointDurationCost, 3, "," );
				
				D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
				if( GLGaeaClient::GetInstance().GetCharacterLogic().m_dwPPoints >= dwPointDurationCost )
				{
					dwColor = NS_UITEXTCOLOR::WHITE;
				}

				strPoint.Format( "%s", strReqPoint );
				m_pPointText->SetText( strPoint.GetString(), dwColor );
			}
		}
		break;
	case 2:
		{
			if( m_pMoneyText )
			{
				CString strMoney, strReqMoney;
				
				m_pMoneyText->ClearText();
				LONGLONG llGoldDurationCost = GLCONST_CHAR::llGoldDurationCost[2];
				strReqMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( llGoldDurationCost, 3, "," );
				
				D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
				if( GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney >= llGoldDurationCost )
				{
					dwColor = NS_UITEXTCOLOR::WHITE;
				}

				strMoney.Format( "%s", strReqMoney );
				m_pMoneyText->SetText( strMoney.GetString(), dwColor );
			}

			if( m_pPointText )
			{
				CString strPoint, strReqPoint;
				
				m_pPointText->ClearText();
				LONGLONG dwPointDurationCost = GLCONST_CHAR::dwPointDurationCost[2];
				strReqPoint = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT(dwPointDurationCost, 3, "," );
				
				D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
				if( GLGaeaClient::GetInstance().GetCharacterLogic().m_dwPPoints >= dwPointDurationCost)
				{
					dwColor = NS_UITEXTCOLOR::WHITE;
				}

				strPoint.Format( "%s", strReqPoint );
				m_pPointText->SetText( strPoint.GetString(), dwColor );
			}
		}
		break;
	}
}

void CPlatinumBuffWindow::Reset() 
{
	if( m_pRemainText )			m_pRemainText->ClearText();
	if( m_pAccountCurrency )	m_pAccountCurrency->ClearText();
}

void CPlatinumBuffWindow::DoPlatinumBuff( int nIndexType, DWORD dwDuration, bool bGold )
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if (!pCharacter)	return;

	pCharacter->ReqPlatinumBuff( nIndexType, dwDuration, bGold );
}