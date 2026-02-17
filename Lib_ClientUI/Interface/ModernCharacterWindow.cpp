#include "StdAfx.h"
#include "./ModernCharacterWindow.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"
#include "./BasicComboBoxRollOver.h"
#include "./MultiModeButton.h"

#include "./ModernCharacterStat.h"
#include "./ModernCharacterStat2.h"
#include "./ModernCharacterEquip.h"
#include "./ModernCharacterEquipEx.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/DxGlobalStage.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"
#include "../../Lib_Client/G-Logic/GLActivity.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernCharacterWindow::CModernCharacterWindow () 
	: m_pCharacterStat(NULL)
	, m_pCharacterStat2(NULL)
	, m_pCharacterEquip(NULL)
	, m_pCharacterEquipEx(NULL)
	, m_pTextName(NULL)
	, m_pTextClub(NULL)
	, m_pTextBadge(NULL)
	, m_pBrightStatic(NULL)
	, m_pActivityStatic(NULL)
	, m_pPremiumStatic(NULL)
	, m_pDepartmentText(NULL)
	, m_pActivityText(NULL)
	, m_pPremiumText(NULL)
	, m_pBrightFrame(NULL)
	, m_pBrightSlider(NULL)
	, m_pButtonHelp(NULL)
	, m_pStatButton(NULL)
	, m_pStat2Button(NULL)
	, m_pStaticReborn(NULL)		/* reborn system, Juver, 2021/09/17 */
	, m_pTextReborn(NULL)		/* reborn system, Juver, 2021/09/17 */
	, m_pStaticVIP(NULL)
	, m_pTextVIP(NULL)
	, m_fUpdateTime(0.0f)
	, m_RollOverID(NO_ID)
	, m_nStatsType(-1)
	, m_pComboBoxTitleRollOver(NULL)
{
}

CModernCharacterWindow::~CModernCharacterWindow ()
{
}

void CModernCharacterWindow::CreateSubControl ()
{
	const EMCHARCLASS emCharClass = DxGlobalStage::GetInstance().GetGameStage()->GetCharJoinData().m_CharData2.m_emClass;
	const EMCHARINDEX emCharIndex = CharClassToIndex ( emCharClass );

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite( "MODERN_CHARACTER_WINDOW_REGION" );
	RegisterControl ( pBasicLineBox );

	CUIControl* pUIControl = new CUIControl;
	pUIControl->CreateSub ( this, "MODERN_CHARACTER_WINDOW_REGION_TEXTURE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
	pUIControl->SetVisibleSingle ( TRUE );
	RegisterControl ( pUIControl );
	pUIControl->AlignToControl( pBasicLineBox, TRUE );
	
	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_CHARACTER_WINDOW_EQUIP_REGION" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_CHARACTER_WINDOW_TITLE_NAME_REGION" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_CHARACTER_WINDOW_SCHOOL_REGION" );
	RegisterControl ( pBasicLineBox );

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "MODERN_CHARACTER_WINDOW_CHARACTER_NAME_TEXT", UI_FLAG_DEFAULT );
	m_pTextName->SetFont ( pFont9 );
	m_pTextName->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );	
	m_pTextName->AddText( "--" );
	RegisterControl ( m_pTextName );

	m_pTextClub = new CBasicTextBox;
	m_pTextClub->CreateSub ( this, "MODERN_CHARACTER_WINDOW_CLUB_NAME_TEXT", UI_FLAG_DEFAULT );
	m_pTextClub->SetFont ( pFont9 );
	m_pTextClub->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );	
	m_pTextClub->AddText( "--" );
	RegisterControl ( m_pTextClub );

	m_pTextBadge = new CBasicTextBox;
	m_pTextBadge->CreateSub ( this, "MODERN_CHARACTER_WINDOW_TITLE_NAME_TEXT", UI_FLAG_DEFAULT );
	m_pTextBadge->SetFont ( pFont9 );
	m_pTextBadge->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );	
	m_pTextBadge->AddText( "--" );
	RegisterControl ( m_pTextBadge );


	m_pBrightStatic = new CBasicTextBox;
	m_pBrightStatic->CreateSub ( this, "MODERN_CHARACTER_WINDOW_BRIGHT_TEXT", UI_FLAG_DEFAULT );
	m_pBrightStatic->SetFont ( pFont9 );
	m_pBrightStatic->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
	m_pBrightStatic->AddText( ID2GAMEWORD( "MODERN_CHARACTER_WINDOW", 3), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pBrightStatic );

	m_pActivityStatic = new CBasicTextBox;
	m_pActivityStatic->CreateSub ( this, "MODERN_CHARACTER_WINDOW_ACTIVITY_POINT_TEXT", UI_FLAG_DEFAULT );
	m_pActivityStatic->SetFont ( pFont9 );
	m_pActivityStatic->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
	m_pActivityStatic->AddText( ID2GAMEWORD( "MODERN_CHARACTER_WINDOW", 4), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pActivityStatic );

	m_pPremiumStatic = new CBasicTextBox;
	m_pPremiumStatic->CreateSub ( this, "MODERN_CHARACTER_WINDOW_CHECK_TEXT", UI_FLAG_DEFAULT );
	m_pPremiumStatic->SetFont ( pFont9 );
	m_pPremiumStatic->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
	m_pPremiumStatic->AddText( ID2GAMEWORD( "MODERN_CHARACTER_WINDOW", 7), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pPremiumStatic );

	m_pDepartmentText = new CBasicTextBox;
	m_pDepartmentText->CreateSub ( this, "MODERN_CHARACTER_WINDOW_ACADEMY_DEPARTMENT_TEXT", UI_FLAG_DEFAULT );
	m_pDepartmentText->SetFont ( pFont9 );
	m_pDepartmentText->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
	m_pDepartmentText->AddText( "-----", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pDepartmentText );

	m_pActivityText = new CBasicTextBox;
	m_pActivityText->CreateSub ( this, "MODERN_CHARACTER_WINDOW_ACTIVITY_POINT_VALUE", UI_FLAG_DEFAULT );
	m_pActivityText->SetFont ( pFont9 );
	m_pActivityText->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
	m_pActivityText->AddText( "-----", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pActivityText );

	m_pPremiumText = new CBasicTextBox;
	m_pPremiumText->CreateSub ( this, "MODERN_CHARACTER_WINDOW_CHECK_VALUE", UI_FLAG_DEFAULT, MODERN_CHARACTER_PREMIUM_TEXT );
	m_pPremiumText->SetFont ( pFont9 );
	m_pPremiumText->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
	m_pPremiumText->AddText( "-----", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pPremiumText );

	m_pBrightFrame = new CUIControl;
	m_pBrightFrame->CreateSub ( this, "MODERN_CHARACTER_WINDOW_BRIGHTBAR" );
	RegisterControl ( m_pBrightFrame );

	m_pBrightSlider = new CUIControl;
	m_pBrightSlider->CreateSub ( this, "MODERN_CHARACTER_WINDOW_BRIGHTBAR_THUMB", UI_FLAG_DEFAULT, MODERN_CHARACTER_WINDOW_BRIGHTBAR );
	RegisterControl ( m_pBrightSlider );

	m_pButtonHelp = new CBasicButton;
	m_pButtonHelp->CreateSub ( this, "MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_BUTTON" , UI_FLAG_DEFAULT, MODERN_CHARACTER_WINDOW_BUTTON_HELP );
	m_pButtonHelp->CreateFlip ( "MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_BUTTON_OVER", CBasicButton::MOUSEIN_FLIP );
	m_pButtonHelp->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pButtonHelp );

	CUIControl	TempControl;
	TempControl.Create ( 1, "MODERN_CHARACTER_WINDOW_SCHOOL_MARK" );
	const UIRECT& rcLocalPos = TempControl.GetLocalPos ();

	CString strSchoolMark[] = { "NAME_DISPLAY_SCHOOL1", "NAME_DISPLAY_SCHOOL2", "NAME_DISPLAY_SCHOOL3" };
	for ( int i=0; i<nSCHOOL_MARK; ++i )
	{
		m_pSchoolMark[i] = CreateControl ( strSchoolMark[i].GetString() );
		m_pSchoolMark[i]->SetLocalPos ( rcLocalPos );
		m_pSchoolMark[i]->SetVisibleSingle ( FALSE );
	}

	m_pCharacterStat = new CModernCharacterStat;
	m_pCharacterStat->CreateSub ( this, "MODERN_CHARACTER_WINDOW_STAT_REGION", UI_FLAG_YSIZE, MODERN_CHARACTER_WINDOW_STAT );
	m_pCharacterStat->CreateSubControl ();
	RegisterControl ( m_pCharacterStat );

	m_pCharacterStat2 = new CModernCharacterStat2;
	m_pCharacterStat2->CreateSub ( this, "MODERN_CHARACTER_WINDOW_STAT_REGION", UI_FLAG_YSIZE, MODERN_CHARACTER_WINDOW_STAT2 );
	m_pCharacterStat2->CreateSubControl ();
	RegisterControl ( m_pCharacterStat2 );

	if( emCharIndex == GLCI_EXTREME_M || emCharIndex == GLCI_EXTREME_W || emCharIndex == GLCI_TRICKER_M || emCharIndex == GLCI_TRICKER_W )
	{
		m_pCharacterEquipEx = new CModernCharacterEquipEx;
		m_pCharacterEquipEx->CreateSub ( this, "MODERN_CHARACTER_WINDOW_WEAR_WINDOW", UI_FLAG_YSIZE, MODERN_CHARACTER_WINDOW_EQUIPEX );
		m_pCharacterEquipEx->CreateSubControl ();
		RegisterControl ( m_pCharacterEquipEx );
	}else{
		m_pCharacterEquip = new CModernCharacterEquip;
		m_pCharacterEquip->CreateSub ( this, "MODERN_CHARACTER_WINDOW_WEAR_WINDOW", UI_FLAG_YSIZE, MODERN_CHARACTER_WINDOW_EQUIP );
		m_pCharacterEquip->CreateSubControl ();
		RegisterControl ( m_pCharacterEquip );
	}

	/*activity system, Juver, 2017/11/05 */
	if ( RPARAM::bActivity )
	{
		m_pButtonTitle = new CBasicButton;
		m_pButtonTitle->CreateSub ( this, "MODERN_CHARACTER_WINDOW_TITLE_BUTTON", UI_FLAG_DEFAULT, MODERN_CHARACTER_TITLE_BUTTON );
		m_pButtonTitle->CreateFlip ( "MODERN_CHARACTER_WINDOW_TITLE_BUTTON_OVER", CBasicButton::MOUSEIN_FLIP );
		m_pButtonTitle->SetControlNameEx ( "MODERN_CHARACTER_WINDOW_TITLE_BUTTON" );
		RegisterControl ( m_pButtonTitle );

		m_pComboBoxTitleRollOver = new CBasicComboBoxRollOver;
		m_pComboBoxTitleRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, MODERN_CHARACTER_TITLE_COMBO_ROLLOVER );
		m_pComboBoxTitleRollOver->CreateBaseComboBoxRollOver ( "MODERN_CHARACTER_WINDOW_TITLE_COMBO_ROLLOVER" );			
		m_pComboBoxTitleRollOver->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pComboBoxTitleRollOver );
	}

	m_pStatButton = new CMultiModeButton;
	m_pStatButton->CreateSub ( this, "MODERN_CHARACTER_WINDOW_STAT_RADIO_BUTTON", UI_FLAG_DEFAULT, MODERN_CHARACTER_WINDOW_BUTTON_STAT );
	m_pStatButton->CreateImageBase( "MODERN_CHARACTER_WINDOW_RADIO_OFF_TEXTUREINFO" );
	m_pStatButton->CreateImageFlip( "MODERN_CHARACTER_WINDOW_RADIO_ON_TEXTUREINFO" );
	m_pStatButton->CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_RADIO_BUTTON_TEXT", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
	m_pStatButton->SetOneLineText( ID2GAMEWORD( "MODERN_CHARACTER_WINDOW", 10 ), NS_UITEXTCOLOR::BLACK );
	m_pStatButton->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::BLACK );
	m_pStatButton->DoImageFlip( TRUE );
	RegisterControl ( m_pStatButton );

	m_pStat2Button = new CMultiModeButton;
	m_pStat2Button->CreateSub ( this, "MODERN_CHARACTER_WINDOW_HISTORY_RADIO_BUTTON", UI_FLAG_DEFAULT, MODERN_CHARACTER_WINDOW_BUTTON_STAT2 );
	m_pStat2Button->CreateImageBase( "MODERN_CHARACTER_WINDOW_RADIO_OFF_TEXTUREINFO" );
	m_pStat2Button->CreateImageFlip( "MODERN_CHARACTER_WINDOW_RADIO_ON_TEXTUREINFO" );
	m_pStat2Button->CreateTextBox( "MODERN_CHARACTER_WINDOW_HISTORY_RADIO_BUTTON_TEXT", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
	m_pStat2Button->SetOneLineText( ID2GAMEWORD( "MODERN_CHARACTER_WINDOW", 11 ), NS_UITEXTCOLOR::BLACK );
	m_pStat2Button->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::BLACK );
	m_pStat2Button->DoImageFlip( FALSE );
	RegisterControl ( m_pStat2Button );

	/* reborn system, Juver, 2021/09/17 */
	m_pStaticReborn = new CBasicTextBox;
	m_pStaticReborn->CreateSub ( this, "MODERN_CHARACTER_WINDOW_REBORN_TEXT", UI_FLAG_DEFAULT );
	m_pStaticReborn->SetFont ( pFont9 );
	m_pStaticReborn->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
	m_pStaticReborn->AddText( ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_EX", 0), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticReborn );

	m_pTextReborn = new CBasicTextBox;
	m_pTextReborn->CreateSub ( this, "MODERN_CHARACTER_WINDOW_REBORN_VALUE", UI_FLAG_DEFAULT );
	m_pTextReborn->SetFont ( pFont9 );
	m_pTextReborn->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
	m_pTextReborn->AddText( "--" );
	RegisterControl ( m_pTextReborn );

	if ( RPARAM::bRBSystem == FALSE )
	{
		m_pStaticReborn->SetVisibleSingle( FALSE );
		m_pTextReborn->SetVisibleSingle( FALSE );
	}


	m_pStaticVIP = new CBasicTextBox;
	m_pStaticVIP->CreateSub ( this, "MODERN_CHARACTER_WINDOW_VIP_TEXT", UI_FLAG_DEFAULT );
	m_pStaticVIP->SetFont ( pFont9 );
	m_pStaticVIP->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
	m_pStaticVIP->AddText( ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_EX", 1), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticVIP );

	m_pTextVIP = new CBasicTextBox;
	m_pTextVIP->CreateSub ( this, "MODERN_CHARACTER_WINDOW_VIP_VALUE", UI_FLAG_DEFAULT );
	m_pTextVIP->SetFont ( pFont9 );
	m_pTextVIP->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
	m_pTextVIP->AddText( "--" );
	RegisterControl ( m_pTextVIP );

	if ( RPARAM::bVIPSystem == FALSE )
	{
		m_pStaticVIP->SetVisibleSingle( FALSE );
		m_pTextVIP->SetVisibleSingle( FALSE );
	}
}

void CModernCharacterWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	if( m_RollOverID == NO_ID )
	{
		m_fUpdateTime += fElapsedTime;
		if ( m_fUpdateTime > 0.033f ) //30 fps
		{
			m_fUpdateTime = 0.0f;
			UpdateData();
			UpdateStats();
		}

		CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
	}
	else
	{
		/*activity system, Juver, 2017/11/05 */
		ResetMessageEx ();

		CUIControl::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		CUIControl* pControl = m_ControlContainer.FindControl ( m_RollOverID );
		if ( !pControl )
		{
			return ;
		}

		pControl->Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		DWORD dwControlMsg = pControl->GetMessageEx ();
		NS_UIDEBUGSET::BlockBegin ();
		if ( dwControlMsg ) TranslateUIMessage ( m_RollOverID, dwControlMsg );
		NS_UIDEBUGSET::BlockEnd ();

		if ( !(dwControlMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL) )
		{
			DWORD dwMsg = GetMessageEx ();
			if ( dwMsg & UIMSG_LB_UP )
			{
				if ( !m_bFirstLBUP )
				{
					m_RollOverID = NO_ID;
					pControl->SetVisibleSingle ( FALSE );					
				}
				m_bFirstLBUP = FALSE;
			}
		}
	}
	
}

void CModernCharacterWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	const float fBUTTON_CLICK = CBasicButton::fBUTTON_CLICK;

	switch ( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;
	
	case MODERN_CHARACTER_PREMIUM_TEXT:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				bool bPREMIUM = GLGaeaClient::GetInstance().GetCharacter()->m_bPREMIUM;
				if ( bPREMIUM )
				{
					if ( CInnerInterface::GetInstance().InfoDisplayShowCommonMultiLineBegin  () )
					{
						CInnerInterface::GetInstance().InfoDisplayShowCommonMultiLineInsert ( ID2GAMEWORD("CHARACTER_PREMIUM_EXPIRE_DATE_STATIC"), NS_UITEXTCOLOR::DARKORANGE );

						__time64_t tPREMIUM = GLGaeaClient::GetInstance().GetCharacter()->m_tPREMIUM;
						CTime cTime ( tPREMIUM );
						CString strExpireDate = CInnerInterface::GetInstance().MakeString ( ID2GAMEWORD("CHARACTER_PREMIUM_EXPIRE_DATE"),
							cTime.GetYear(), cTime.GetMonth(), cTime.GetDay (), cTime.GetHour (), cTime.GetMinute () );

						CInnerInterface::GetInstance().InfoDisplayShowCommonMultiLineInsert ( strExpireDate, NS_UITEXTCOLOR::WHITE );
						CInnerInterface::GetInstance().InfoDisplayShowCommonMultiLineEnd ();
					}					
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_BRIGHTBAR:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter ();

				CString strTemp;				
				strTemp.Format ( "%d", pCharacter->m_nBright );

				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine ( strTemp, NS_UITEXTCOLOR::WHITE );
			}
		}break;


		/*activity system, Juver, 2017/11/05 */
	case MODERN_CHARACTER_TITLE_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxTitleRollOver->SetVisibleSingle( TRUE );
					m_pComboBoxTitleRollOver->SetScrollPercent ( 0.0f );

					m_RollOverID = MODERN_CHARACTER_TITLE_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

		/*activity system, Juver, 2017/11/05 */
	case MODERN_CHARACTER_TITLE_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );

				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{				
				int nIndex = m_pComboBoxTitleRollOver->GetSelectIndex ();
				if ( nIndex < 0 ) return ;

				DWORD dwSelected = m_pComboBoxTitleRollOver->GetTextData( nIndex );
				GLGaeaClient::GetInstance().GetCharacter()->ReqChangeBadge( dwSelected );
			}
		}break;


	case MODERN_CHARACTER_WINDOW_BUTTON_HELP:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoWindowOpen( GetWndID() );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_SENTENCE_1" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_SENTENCE_2" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_SENTENCE_3" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_SENTENCE_4" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_SENTENCE_5" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( "  ", NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_SENTENCE_6" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_SENTENCE_7" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_SENTENCE_8" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( "  " , NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_SENTENCE_9" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ACADEMY_QUESTION_SENTENCE_10" ), NS_UITEXTCOLOR::DARKLBUE );
			}
		}break;

	case MODERN_CHARACTER_WINDOW_EQUIP:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = m_pCharacterEquip->GetItemIndex ();
				if ( nIndex < 0 ) return ;		

				SITEMCUSTOM sItemCustom = m_pCharacterEquip->GetItem ( nIndex );
				if ( sItemCustom.sNativeID != NATIVEID_NULL () )
				{
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sItemCustom, FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );

					/*item preview, Juver, 2017/07/27 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().PreviewItem( sItemCustom );
							return;
						}
					}

					/*item link, Juver, 2017/07/31 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_RB_UP )
						{
							CInnerInterface::GetInstance().LinkItem( &sItemCustom );
							return;
						}
					}

					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().ReqMaxRVShow( sItemCustom );
							return;
						}
					}
				}

				if ( dwMsg & UIMSG_LB_UP )
				{
					EMSLOT emSlot = m_pCharacterEquip->GetItemSlot ();
					GLGaeaClient::GetInstance().GetCharacter()->ReqSlotTo ( emSlot );
				}

				/*rightclick wear/unwear, Juver, 2017/06/24 */
				if ( dwMsg & UIMSG_RB_UP )
				{
					EMSLOT emSlot = m_pCharacterEquip->GetItemSlot ();

					SNATIVEID sid_item_hold = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM().sNativeID;
					if ( sid_item_hold != NATIVEID_NULL () )
					{
						BOOL bUse = GLGaeaClient::GetInstance().GetCharacter()->InvenUseToPutOn ( emSlot );
						if ( bUse )	return;
					}

					GLGaeaClient::GetInstance().GetCharacter()->ReqWearToInven ( emSlot );
					return;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_EQUIPEX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = m_pCharacterEquipEx->GetItemIndex ();
				if ( nIndex < 0 ) return ;	

				SITEMCUSTOM sItemCustom = m_pCharacterEquipEx->GetItem ( nIndex );
				if ( sItemCustom.sNativeID != NATIVEID_NULL () )
				{
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sItemCustom, FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );

					/*item preview, Juver, 2017/07/27 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().PreviewItem( sItemCustom );
							return;
						}
					}

					/*item link, Juver, 2017/07/31 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_RB_UP )
						{
							CInnerInterface::GetInstance().LinkItem( &sItemCustom );
							return;
						}
					}

					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().ReqMaxRVShow( sItemCustom );
							return;
						}
					}
				}

				if ( dwMsg & UIMSG_LB_UP )
				{
					EMSLOT emSlot = m_pCharacterEquipEx->GetItemSlot ();
					GLGaeaClient::GetInstance().GetCharacter()->ReqSlotTo ( emSlot );
				}

				/*rightclick wear/unwear, Juver, 2017/06/24 */
				if ( dwMsg & UIMSG_RB_UP )
				{
					EMSLOT emSlot = m_pCharacterEquipEx->GetItemSlot ();

					SNATIVEID sid_item_hold = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM().sNativeID;
					if ( sid_item_hold != NATIVEID_NULL () )
					{
						BOOL bUse = GLGaeaClient::GetInstance().GetCharacter()->InvenUseToPutOn ( emSlot );
						if ( bUse )	return;
					}

					GLGaeaClient::GetInstance().GetCharacter()->ReqWearToInven ( emSlot );
					return;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_BUTTON_STAT:
	case MODERN_CHARACTER_WINDOW_BUTTON_STAT2:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				int nSelected = ControlID - MODERN_CHARACTER_WINDOW_BUTTON_STAT;
				OpenStats( nSelected );
			}
		}break;
	}
}

void CModernCharacterWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindow::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		const GLCHARLOGIC_CLIENT& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
		
		/*activity system, Juver, 2017/11/05 */
		LoadBadge( sCharData );

		ResetData();

		if( GLGaeaClient::GetInstance().GetCharacter()->IsUseArmSub() )
		{
			SetArmSwapTabButton( FALSE );
		}else{
			SetArmSwapTabButton( TRUE );
		}

		if ( m_pCharacterEquip )
			m_pCharacterEquip->StartRender();

		if ( m_pCharacterEquipEx )
			m_pCharacterEquipEx->StartRender();

		OpenStats( MODERN_CHARACTER_WINDOW_STATS_BASIC );
	}
	else
	{
		/*activity system, Juver, 2017/11/05 */
		if ( m_pComboBoxTitleRollOver )
			m_pComboBoxTitleRollOver->SetVisibleSingle( FALSE );

		m_RollOverID = NO_ID;
		m_bFirstLBUP = FALSE;

		if ( m_pCharacterEquip )
			m_pCharacterEquip->ClearRender();

		if ( m_pCharacterEquipEx )
			m_pCharacterEquipEx->ClearRender();
	}
}

void CModernCharacterWindow::SetGlobalPos(const D3DXVECTOR2& vPos)
{
	CUIGroup::SetGlobalPos( vPos );

	const GLCHARLOGIC_CLIENT& schar_data = GLGaeaClient::GetInstance().GetCharacterLogic ();	

	if ( m_pBrightFrame && m_pBrightSlider )
	{
		const int nMIN = -100;
		const int nMAX = 100;
		const int nRANGE = nMAX - nMIN;

		int nPercent = schar_data.GETBRIGHTPER ();
		nPercent = -(nPercent);
		if ( nPercent < nMIN ) nPercent = nMIN;
		if ( nPercent > nMAX ) nPercent = nMAX;

		nPercent += nMAX;

		float fPercent = static_cast<float>(nPercent) / static_cast<float>(nRANGE);

		const UIRECT& rcFramePos = m_pBrightFrame->GetGlobalPos ();
		const UIRECT& rcThumbPos = m_pBrightSlider->GetGlobalPos ();

		float fMOVE = (rcFramePos.sizeX - rcThumbPos.sizeX) * fPercent;
		float fLeft = rcFramePos.left + fMOVE;

		m_pBrightSlider->SetGlobalPos ( D3DXVECTOR2 ( fLeft, rcThumbPos.top ) );
	}
}

void CModernCharacterWindow::ResetData()
{
	if ( m_pTextName )	m_pTextName->ClearText();
	if ( m_pTextClub )	m_pTextClub->ClearText();
	if ( m_pTextBadge )	m_pTextBadge->ClearText();

	if ( m_pDepartmentText )	m_pDepartmentText->ClearText();
	if ( m_pActivityText )		m_pActivityText->ClearText();
	if ( m_pPremiumText )		m_pPremiumText->ClearText();
	
	for ( int i=0; i<nSCHOOL_MARK; ++i )
	{
		if( m_pSchoolMark[i] )
			m_pSchoolMark[i]->SetVisibleSingle( FALSE );
	}

	/* reborn system, Juver, 2021/09/17 */
	if ( RPARAM::bRBSystem )
	{
		if ( m_pTextReborn )	
			m_pTextReborn->ClearText();
	}

	if ( RPARAM::bVIPSystem )
	{
		if ( m_pTextVIP )
			m_pTextVIP->ClearText();
	}
}

void CModernCharacterWindow::UpdateData()
{
	ResetData();

	const GLCHARLOGIC_CLIENT& schar_data = GLGaeaClient::GetInstance().GetCharacterLogic();

	CString strtemp;

	if ( m_pTextName )
	{
		strtemp.Format("Lv. %u ", schar_data.m_wLevel );
		int index = m_pTextName->AddText( strtemp, NS_UITEXTCOLOR::ORANGE );
		strtemp.Format("%s", schar_data.m_szName );
		m_pTextName->AddString( index, strtemp, NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextClub )
	{
		GLCLUB& sclub_data = GLGaeaClient::GetInstance().GetCharacter ()->m_sCLUB;
		
		if ( sclub_data.IsMember( schar_data.m_dwCharID ) )
			strtemp.Format("[%s]", sclub_data.m_szName );
		else
			strtemp.Format("[%s]", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_EMPTY_CLUB") );

		m_pTextClub->SetOneLineText( strtemp, NS_UITEXTCOLOR::YELLOW );
	}

	if ( m_pTextBadge )
	{
		/*activity system, Juver, 2017/11/05 */
		if ( RPARAM::bActivity && strlen( schar_data.m_szBadge ) )
			m_pTextBadge->SetOneLineText( schar_data.m_szBadge, NS_UITEXTCOLOR::WHITE );
		else
			m_pTextBadge->SetOneLineText( ID2GAMEWORD("CHARWINDOW_TITLE",0), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pDepartmentText )
	{
		strtemp.Format ( "%s, %s", GLCONST_CHAR::strSCHOOLNAME[schar_data.m_wSchool].c_str() , COMMENT::CHARCLASS[CharClassToIndex( schar_data.m_emClass )].c_str() );
		m_pDepartmentText->SetOneLineText ( strtemp, NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pActivityText )
	{
		strtemp.Format ( "%d", schar_data.m_nLiving );
		m_pActivityText->SetOneLineText ( strtemp, NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pPremiumText && m_pPremiumStatic )
	{
		m_pPremiumStatic->SetVisibleSingle ( FALSE );

		bool bPREMIUM = GLGaeaClient::GetInstance().GetCharacter()->m_bPREMIUM;
		if ( bPREMIUM )
		{
			__time64_t tPREMIUM = GLGaeaClient::GetInstance().GetCharacter()->m_tPREMIUM;
			CTime cTime ( tPREMIUM );;
			strtemp.Format ( "%02d/%02d/%02d", cTime.GetYear()%2000, cTime.GetMonth(), cTime.GetDay () );
			m_pPremiumText->SetOneLineText ( strtemp, NS_UITEXTCOLOR::DARKORANGE );
			m_pPremiumStatic->SetVisibleSingle ( TRUE );
		}
	}

	if ( m_pBrightFrame && m_pBrightSlider )
	{
		const int nMIN = -100;
		const int nMAX = 100;
		const int nRANGE = nMAX - nMIN;

		int nPercent = schar_data.GETBRIGHTPER ();
		nPercent = -(nPercent);
		if ( nPercent < nMIN ) nPercent = nMIN;
		if ( nPercent > nMAX ) nPercent = nMAX;

		nPercent += nMAX;

		float fPercent = static_cast<float>(nPercent) / static_cast<float>(nRANGE);

		const UIRECT& rcFramePos = m_pBrightFrame->GetGlobalPos ();
		const UIRECT& rcThumbPos = m_pBrightSlider->GetGlobalPos ();

		float fMOVE = (rcFramePos.sizeX - rcThumbPos.sizeX) * fPercent;
		float fLeft = rcFramePos.left + fMOVE;

		m_pBrightSlider->SetGlobalPos ( D3DXVECTOR2 ( fLeft, rcThumbPos.top ) );
	}

	if ( schar_data.m_wSchool < nSCHOOL_MARK )
	{
		if ( m_pSchoolMark[schar_data.m_wSchool] )
			m_pSchoolMark[schar_data.m_wSchool]->SetVisibleSingle( TRUE );
	}
	

	CString strWindowTitle;
	strWindowTitle.Format ( ID2GAMEINTEXT( "MODERN_CHARACTER_CHARACTER_WINDOW_MAIN_TITLE" ), schar_data.m_szName );
	SetTitleName( strWindowTitle.GetString() );


	/* reborn system, Juver, 2021/09/17 */
	if ( RPARAM::bRBSystem )
	{
		if ( m_pTextReborn )
		{
			strtemp.Format ( "%d", schar_data.m_dwReborn );
			m_pTextReborn->SetOneLineText ( strtemp, NS_UITEXTCOLOR::ORANGE );
		}
	}

	if ( RPARAM::bVIPSystem )
	{
		if ( m_pTextVIP )
		{
			m_pTextVIP->SetOneLineText (
				COMMENT::CHAR_VIP[schar_data.m_emVIPLevel].c_str(),
				NS_UITEXTCOLOR::BRIGHTGREEN );
		}
	}
}

void CModernCharacterWindow::UpdateStats()
{
	GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();
	if ( pChar )
	{
		m_sCharLogic.INIT_NEW_CHAR( 
			CharClassToIndex( pChar->m_emClass ),
			pChar->GetUserID(),
			pChar->m_dwServerID,
			pChar->m_szName,
			pChar->m_wSchool,
			pChar->m_wHair,
			pChar->m_wFace,
			pChar->m_wHairColor,
			pChar->m_wSex );

		m_sCharLogic.Assign( pChar->GetCharData() );

		if ( m_pCharacterStat )
		{
			if ( m_pCharacterStat->m_wStatsDeduct > m_sCharLogic.GETCHARDATA().m_wStatsPoint )
			{
				//something wrong stats added more than remaining stats
				m_pCharacterStat->m_wPowAdd = 0;
				m_pCharacterStat->m_wDexAdd = 0;
				m_pCharacterStat->m_wSpiAdd = 0;
				m_pCharacterStat->m_wStaAdd = 0;
				m_pCharacterStat->m_wStrAdd = 0;
				m_pCharacterStat->m_wStatsDeduct = 0;
			}
			else
			{
				m_sCharLogic.GETCHARDATA().m_sStats.wPow += m_pCharacterStat->m_wPowAdd;
				m_sCharLogic.GETCHARDATA().m_sStats.wDex += m_pCharacterStat->m_wDexAdd;
				m_sCharLogic.GETCHARDATA().m_sStats.wSpi += m_pCharacterStat->m_wSpiAdd;
				m_sCharLogic.GETCHARDATA().m_sStats.wSta += m_pCharacterStat->m_wStaAdd;
				m_sCharLogic.GETCHARDATA().m_sStats.wStr += m_pCharacterStat->m_wStrAdd;
				m_sCharLogic.GETCHARDATA().m_wStatsPoint -= m_pCharacterStat->m_wStatsDeduct;
			}
		}
		
		m_sCharLogic.SetUseArmSub( pChar->IsUseArmSub() );

		m_sCharLogic.m_bVehicle = pChar->m_bVehicle;

		SITEMCUSTOM sItemCheck = CInnerInterface::GetInstance().GetItemInfoTemp();

		SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCheck.sNativeID );
		if ( pItem && pItem->sBasicOp.emItemType == ITEM_SUIT )
		{
			EMSLOT emSlot;
			switch ( pItem->sSuitOp.emSuit )
			{
			case SUIT_HEADGEAR:		emSlot = SLOT_HEADGEAR;		break;
			case SUIT_UPPER:		emSlot = SLOT_UPPER;		break;
			case SUIT_LOWER:		emSlot = SLOT_LOWER;		break;
			case SUIT_HAND:			emSlot = SLOT_HAND;			break;
			case SUIT_FOOT:			emSlot = SLOT_FOOT;			break;

			case SUIT_HANDHELD:		
				{
					if( pChar->IsUseArmSub() )	emSlot = SLOT_RHAND_S;
					else	emSlot = SLOT_RHAND;
				}break;

			case SUIT_NECK:			emSlot = SLOT_NECK;			break;
			case SUIT_WRIST:		emSlot = SLOT_WRIST;		break;

			case SUIT_FINGER:		
				{
					if ( pChar->GetCharLogic().VALID_SLOT_ITEM( SLOT_RFINGER ) )	
						emSlot = SLOT_LFINGER;
					else	
						emSlot = SLOT_RFINGER;
				}break;

			case SUIT_VEHICLE:		emSlot = SLOT_VEHICLE;		break;

				/*ep8 puton slots, Juver, 2018/03/30 */
			case SUIT_BELT:			emSlot = SLOT_BELT;			break;
			case SUIT_EARRING:		emSlot = SLOT_EARRING;		break;
			case SUIT_DECORATION:	emSlot = SLOT_DECORATION;	break;

			case SUIT_ACCESSORY:		
				{
					if ( pChar->GetCharLogic().VALID_SLOT_ITEM( SLOT_ACCESSORY_R ) )	
						emSlot = SLOT_ACCESSORY_L;
					else	
						emSlot = SLOT_ACCESSORY_R;
				}break;

			default:				emSlot = SLOT_NSIZE_S_2;	break;
			};

			if ( pItem->sBasicOp.emItemType == ITEM_ARROW || pItem->sBasicOp.emItemType == ITEM_CHARM || pItem->sBasicOp.emItemType == ITEM_BULLET )	
			{
				if( pChar->IsUseArmSub() )	
					emSlot = SLOT_LHAND_S;
				else	
					emSlot = SLOT_LHAND;
			}

			if ( emSlot != SLOT_NSIZE_S_2 )
			{
				m_sCharLogic.m_PutOnItems[emSlot] = sItemCheck;
			}
		}

		for ( int i=0; i<SKILLFACT_SIZE; ++i )
		{
			m_sCharLogic.m_sSKILLFACT[i] = pChar->m_sSKILLFACT[i];
		}

		m_sCharLogic.m_sPETSKILLFACT_A = pChar->m_sPETSKILLFACT_A;
		m_sCharLogic.m_sPETSKILLFACT_B = pChar->m_sPETSKILLFACT_B;

		for ( int i=0; i<FITEMFACT_SIZE; ++i )
		{
			m_sCharLogic.m_sFITEMFACT[i] = pChar->m_sFITEMFACT[i];
		}

		for ( int i=0; i<SYSTEM_BUFF_SIZE; ++i )
		{
			m_sCharLogic.m_sSYSTEM_BUFF[i] = pChar->m_sSYSTEM_BUFF[i];
		}

		for ( int i=0; i<EMLANDEFFECT_MULTI; ++i )
		{
			m_sCharLogic.m_sLandEffect[i] = pChar->m_sLandEffect[i];
		}

		m_sCharLogic.m_sQITEMFACT = pChar->m_sQITEMFACT;
		m_sCharLogic.m_sEVENTFACT = pChar->m_sEVENTFACT;

		m_sCharLogic.INIT_DATA( TRUE, FALSE );
		m_sCharLogic.UPDATE_DATA( 0.0f, 0.0f );

		if ( m_pCharacterStat && m_pCharacterStat->IsVisible() )
		{
			m_pCharacterStat->SetPointControl( m_sCharLogic );
			m_pCharacterStat->SetData( pChar->GetCharLogic(), m_sCharLogic );
			m_pCharacterStat->SetActionBlock( FALSE );
		}

		if ( m_pCharacterStat2 && m_pCharacterStat2->IsVisible() )
		{
			m_pCharacterStat2->SetData( pChar->GetCharLogic(), m_sCharLogic );
		}
	}

	

}

void CModernCharacterWindow::LoadBadge( const GLCHARLOGIC_CLIENT& sCharData )
{
	if ( !RPARAM::bActivity )	return;

	if ( !m_pComboBoxTitleRollOver )	return;

	m_pComboBoxTitleRollOver->ClearText();

	int nIndex = m_pComboBoxTitleRollOver->AddText( ID2GAMEWORD("CHARWINDOW_TITLE",0) );
	m_pComboBoxTitleRollOver->SetTextData( nIndex, UINT_MAX );

	SACTIVITY_CHAR_DATA_MAP mapCharActivity = sCharData.m_mapActivityDone;

	SACTIVITY_CHAR_DATA_MAP_ITER pos = mapCharActivity.begin();
	SACTIVITY_CHAR_DATA_MAP_ITER end = mapCharActivity.end();
	for ( ; pos!=end; ++pos )
	{
		SACTIVITY_CHAR_DATA sactivity_data( (*pos).second );

		SACTIVITY_FILE_DATA* pactivity = GLActivity::GetInstance().GetActivity( sactivity_data.dwActivityID );
		if( !pactivity ) continue;
		if( !pactivity->bRewardBadge ) continue;
		if( !pactivity->strBadgeString.size() ) continue;

		int nIndex = m_pComboBoxTitleRollOver->AddText( pactivity->strBadgeString.c_str() );
		m_pComboBoxTitleRollOver->SetTextData( nIndex, pactivity->dwActivityID );
	}
}

void CModernCharacterWindow::SetArmSwapTabButton( BOOL bArmSub )
{
	if( m_pCharacterEquip )
		m_pCharacterEquip->SetTabButton( bArmSub );

	if( m_pCharacterEquipEx )
		m_pCharacterEquipEx->SetTabButton( bArmSub );
}

void CModernCharacterWindow::OpenStats( int nType )
{
	if ( m_nStatsType == nType )	return;

	m_nStatsType = nType;

	m_pStatButton->DoImageFlip( FALSE );
	m_pStat2Button->DoImageFlip( FALSE );
	m_pCharacterStat->SetVisibleSingle( FALSE );
	m_pCharacterStat2->SetVisibleSingle( FALSE );

	switch( m_nStatsType )
	{
	case MODERN_CHARACTER_WINDOW_STATS_BASIC:
		{
			m_pStatButton->DoImageFlip( TRUE );
			m_pCharacterStat->SetVisibleSingle( TRUE );
		}break;

	case MODERN_CHARACTER_WINDOW_STATS_OTHER:
		{
			m_pStat2Button->DoImageFlip( TRUE );
			m_pCharacterStat2->SetVisibleSingle( TRUE );
		}break;
	}
}