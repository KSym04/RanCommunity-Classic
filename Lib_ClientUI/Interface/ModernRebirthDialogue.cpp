#include "StdAfx.h"
#include "./ModernRebirthDialogue.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"
#include "./BasicButton.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/DxGlobalStage.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float CModernRebirthDialogue::fDEFAULT_TIME_LEFT = 5.0f;
const float CModernRebirthDialogue::fDEFAULT_SELECT_TIME_LEFT = 600;

CModernRebirthDialogue::CModernRebirthDialogue ()
	: m_pTextBoxQuestion(NULL)
	, m_pTextBoxInformation(NULL)
    , m_pCheckPointButton(NULL)
	, m_pCurrentPositionButton(NULL)
	, m_pRecoveryExpButton(NULL)
	, m_pAutoRebirthButton( NULL )
	, m_pAutoRebirthText( NULL )
	, m_pDummy_1BUTTON(NULL)
	, m_pDummy_2BUTTON_L(NULL)
	, m_pDummy_2BUTTON_R(NULL)
	, m_bDO_ACTION(false)
	, m_CallerID(NO_ID)
	, m_fTIME_LEFT(fDEFAULT_TIME_LEFT)
	, m_fSelTIME_LEFT( fDEFAULT_SELECT_TIME_LEFT )
	, m_nACTION(NO_ID)
	, m_bDONE_ACTION(false)
	, m_bAutoRebirth(false)
	, m_bForce_Revive(false)		/*force revive, Juver, 2018/07/09 */
{
}

CModernRebirthDialogue::~CModernRebirthDialogue ()
{
}

void CModernRebirthDialogue::CreateSubControl ()
{
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 12, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "MODERN_REBIRTH_WINDOW_NOTIFY_REBIRTH_BACK" );
	RegisterControl ( pControl );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "MODERN_REBIRTH_WINDOW_NOTIFY_REBIRTH_BACK_LINE" );
	RegisterControl ( pLineBox );	

	pControl = new CUIControl;
	pControl->CreateSub ( this, "MODERN_REBIRTH_WINDOW_NOTIFY_MAIN_IMAGE" );
	RegisterControl ( pControl );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "MODERN_REBIRTH_WINDOW_NOTIFY_REBIRTH_MAIN_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "MODERN_REBIRTH_WINDOW_NOTIFY_REBIRTH_MAIN_LINE" );
	RegisterControl ( pLineBox );	

	m_pTextBoxQuestion = new CBasicTextBox;
	m_pTextBoxQuestion->CreateSub ( this, "MODERN_REBIRTH_WINDOW_QUESTION_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextBoxQuestion->SetFont ( pFont10 );
	m_pTextBoxQuestion->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	m_pTextBoxQuestion->SetPartInterval ( 5.0f );
	RegisterControl ( m_pTextBoxQuestion );

	m_pTextBoxInformation = new CBasicTextBox;
	m_pTextBoxInformation->CreateSub ( this, "MODERN_REBIRTH_WINDOW_INFORMATION_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextBoxInformation->SetFont ( pFont9 );
	m_pTextBoxInformation->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	m_pTextBoxInformation->SetPartInterval ( 5.0f );
	RegisterControl ( m_pTextBoxInformation );


	m_pCheckPointButton = CreateTextButton ( "MODERN_REBIRTH_WINDOW_REBIRTH_BUTTON", REBIRTH_CHECK_POINT, (char*)ID2GAMEWORD ( "REBIRTH_BUTTON", 0 ) );
	m_pCurrentPositionButton = CreateTextButton ( "MODERN_REBIRTH_WINDOW_REBIRTH_BUTTON", REBIRTH_CURRENT_POSITION, (char*)ID2GAMEWORD ( "REBIRTH_BUTTON", 1 ) );
	m_pRecoveryExpButton = CreateTextButton ( "MODERN_REBIRTH_WINDOW_REBIRTH_BUTTON", REBIRTH_RECOVERY_EXP, (char*)ID2GAMEWORD ( "REBIRTH_BUTTON", 2 ) );

	m_pDummy_1BUTTON = CreateDummyControl ( "MODERN_REBIRTH_WINDOW_RIGHT_BUTTON" );
	m_pDummy_2BUTTON_L = CreateDummyControl ( "MODERN_REBIRTH_WINDOW_LEFT_BUTTON" );
	m_pDummy_2BUTTON_R = CreateDummyControl ( "MODERN_REBIRTH_WINDOW_RIGHT_BUTTON" );

	m_pAutoRebirthButton = CreateFlipButton ( "MODERN_REBIRTH_WINDOW_AUTO_REBIRTH_UNCHECK", "MODERN_REBIRTH_WINDOW_AUTO_REBIRTH_CHECK", RIBIRTH_AUTO_BUTTON );
	m_pAutoRebirthButton->SetVisibleSingle( FALSE );

	m_pAutoRebirthText = new CBasicTextBox;
	m_pAutoRebirthText->CreateSub ( this, "MODERN_REBIRTH_WINDOW_AUTO_REBIRTH_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pAutoRebirthText->SetFont ( pFont9 );
	m_pAutoRebirthText->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pAutoRebirthText->SetPartInterval ( 5.0f );
	RegisterControl ( m_pAutoRebirthText );
	m_pAutoRebirthText->SetVisibleSingle( FALSE );
}

CUIControl*	CModernRebirthDialogue::CreateDummyControl ( char* szKeyword )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szKeyword );
	pControl->SetVisibleSingle ( FALSE );
	RegisterControl ( pControl );
	return pControl;
}

CBasicTextButton* CModernRebirthDialogue::CreateTextButton ( char* szButton, UIGUID ControlID , char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE22;
	CBasicTextButton* pButton = new CBasicTextButton;
	pButton->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, ControlID );
	pButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText );
	RegisterControl ( pButton );
	return pButton;
}

CBasicButton* CModernRebirthDialogue::CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->SetControlNameEx ( szButton );
	RegisterControl ( pButton );

	return pButton;
}

void CModernRebirthDialogue::SET_ITEMREBIRTH ( const bool& bItemRebirth, bool bAutoRebirth, bool bCantUseRebirth )
{
	m_bDO_ACTION = false;
	m_bDONE_ACTION = false;
	m_fTIME_LEFT = fDEFAULT_TIME_LEFT;
	m_fSelTIME_LEFT = fDEFAULT_SELECT_TIME_LEFT;

	m_bAutoRebirth = false;
	m_bForce_Revive = false; /*force revive, Juver, 2018/07/09 */

	if( bCantUseRebirth )
	{
		/*recovery exp setting, Juver, 2017/11/18 */
		if ( RPARAM::bUseRecoveryEXP )
			m_pTextBoxQuestion->SetText ( ID2GAMEINTEXT("REBIRTH_DIALOGUE_TEXT3") );
		else
			m_pTextBoxQuestion->SetText ( ID2GAMEINTEXT("REBIRTH_DIALOGUE_TEXT2") );
	}else{
		m_pTextBoxQuestion->SetText ( ID2GAMEINTEXT("REBIRTH_DIALOGUE_TEXT") );
	}

	/*pvp tyranny, Juver, 2017/08/25 */
	bool bPVPMap(false);
	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if( pLand )
	{
		/*pvp tyranny, Juver, 2017/08/25 */
		/*school wars, Juver, 2018/01/19 */
		/*pvp capture the flag, Juver, 2018/01/31 */
		/* pvp club death match, Juver, 2020/11/26 */
		if ( pLand->m_bClubBattle || 
			pLand->m_bClubDeathMatch || 
			pLand->m_bPVPTyrannyMap || 
			pLand->m_bPVPSchoolWarsMap || 
			pLand->m_bPVPCaptureTheFlagMap ||
			pLand->m_bPVPClubDeathMatchLobbyMap || 
			pLand->m_bPVPClubDeathMatchBattleMap  ||
			pLand->m_bPVPPBGLobbyMap ||
			pLand->m_bPVPPBGBattleMap )
		{
			bPVPMap = true;
		}

		/*force revive, Juver, 2018/07/09 */
		m_bForce_Revive = pLand->IsForceRevive();
		if ( m_bForce_Revive )
			m_fSelTIME_LEFT = RPARAM::force_revive_time;
	}

	ShowButton ();

	/*pvp tyranny, Juver, 2017/08/25 */
	if ( bItemRebirth && !bPVPMap )	// ±ÍÈ¥ÁÖ, ¼±µµºÒ°¡Áö¿ªx, ±ÍÈ¥ºÒ°¡Áö¿ªx
	{
		{
			const UIRECT& rc2ButtonL_G = m_pDummy_2BUTTON_L->GetGlobalPos();
			const UIRECT& rc2ButtonL_L = m_pDummy_2BUTTON_L->GetLocalPos();
			
			m_pCheckPointButton->SetGlobalPos( rc2ButtonL_G );
			m_pCheckPointButton->SetLocalPos( rc2ButtonL_L );
			m_pCheckPointButton->SetFlip( TRUE );
		}
		{
			const UIRECT& rc2ButtonR_G = m_pDummy_2BUTTON_R->GetGlobalPos();
			const UIRECT& rc2ButtonR_L = m_pDummy_2BUTTON_R->GetLocalPos();

			m_pCurrentPositionButton->SetGlobalPos( rc2ButtonR_G );
			m_pCurrentPositionButton->SetLocalPos( rc2ButtonR_L );
			m_pCurrentPositionButton->SetFlip( TRUE );
		}

		m_pRecoveryExpButton->SetVisibleSingle ( FALSE );

		m_bAutoRebirth = bAutoRebirth;

		if ( m_pTextBoxInformation )
		{
			m_pTextBoxInformation->ClearText();

			CString strText;
			strText.Format( ID2GAMEINTEXT("REBIRTH_WINDOW_NOTIFY_CURRENT_POS_REBIRTH"), 0.0f, 0.0f );
			m_pTextBoxInformation->AddText( strText.GetString(), NS_UITEXTCOLOR::ORANGE );
		}
	}
	/*pvp tyranny, Juver, 2017/08/25 */
	/*recovery exp setting, Juver, 2017/11/18 */
	else if ( !bCantUseRebirth && !bPVPMap && RPARAM::bUseRecoveryEXP )	// ±ÍÈ¥ÁÖx, ±ÍÈ¥ºÒ°¡Áö¿ªx, ¼±µµÀüÁö¿ªx
	{
		{
			const UIRECT& rc2ButtonL_G = m_pDummy_2BUTTON_L->GetGlobalPos();
			const UIRECT& rc2ButtonL_L = m_pDummy_2BUTTON_L->GetLocalPos();
			
			m_pCheckPointButton->SetGlobalPos( rc2ButtonL_G );
			m_pCheckPointButton->SetLocalPos( rc2ButtonL_L );
			m_pCheckPointButton->SetFlip( TRUE );
		}
		
		{
			const UIRECT& rc2ButtonR_G = m_pDummy_2BUTTON_R->GetGlobalPos();
			const UIRECT& rc2ButtonR_L = m_pDummy_2BUTTON_R->GetLocalPos();

			m_pRecoveryExpButton->SetGlobalPos( rc2ButtonR_G );
			m_pRecoveryExpButton->SetLocalPos( rc2ButtonR_L );
			m_pRecoveryExpButton->SetFlip( TRUE );
		}

		{
			m_pCurrentPositionButton->SetVisibleSingle ( FALSE );
		}

		if ( m_pTextBoxInformation )
		{
			m_pTextBoxInformation->ClearText();

			CString strText;
			strText.Format( ID2GAMEINTEXT("REBIRTH_WINDOW_NOTIFY_RECOVERY_EXP"), 0.0f, 0.0f, 0.0f, 0.0f );
			m_pTextBoxInformation->AddText( strText.GetString(), NS_UITEXTCOLOR::ORANGE );
		}

	}
	else
	{
		{
			const UIRECT& rc1BUTTON = m_pDummy_1BUTTON->GetGlobalPos ();
			D3DXVECTOR2 vPos ( rc1BUTTON.left, rc1BUTTON.top );
			m_pCheckPointButton->SetGlobalPos ( vPos );
			m_pCheckPointButton->SetFlip ( TRUE );
		}

		{
			m_pCurrentPositionButton->SetVisibleSingle ( FALSE );
			m_pRecoveryExpButton->SetVisibleSingle ( FALSE );
		}

		if ( m_pTextBoxInformation )
		{
			m_pTextBoxInformation->ClearText();

			CString strText;
			strText.Format( ID2GAMEINTEXT("REBIRTH_WINDOW_NOTIFY_CURRENT_POS_REBIRTH"), 0.0f, 0.0f );
			m_pTextBoxInformation->AddText( strText.GetString(), NS_UITEXTCOLOR::ORANGE );

			if ( RPARAM::bUseRecoveryEXP )
			{
				strText.Format( ID2GAMEINTEXT("REBIRTH_WINDOW_NOTIFY_RECOVERY_EXP"), 0.0f, 0.0f, 0.0f, 0.0f );
				m_pTextBoxInformation->AddText( strText.GetString(), NS_UITEXTCOLOR::ORANGE );
			}
		}
	}


	if ( m_pAutoRebirthButton )
	{
		m_pAutoRebirthButton->SetVisibleSingle( m_bAutoRebirth && !m_bForce_Revive );
		m_pAutoRebirthButton->SetFlip( RPROFILE::bModernReviveWindowAutoRevive );
	}

	if ( m_pAutoRebirthText )
	{
		m_pAutoRebirthText->SetVisibleSingle( m_bAutoRebirth && !m_bForce_Revive );
		m_pAutoRebirthText->SetText( ID2GAMEINTEXT("REBIRTH_WINDOW_AUTO_REBIRTH") );
	}

	

}

void CModernRebirthDialogue::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case REBIRTH_CHECK_POINT:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
                m_nACTION = REBIRTH_CHECK_POINT;
				DoACTION ();
				HideButton ();
			}
		}
        break;

	case REBIRTH_CURRENT_POSITION:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_nACTION = REBIRTH_CURRENT_POSITION;
				DoACTION ();
                HideButton ();
			}
		}
		break;
	case REBIRTH_RECOVERY_EXP:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				/*recovery exp setting, Juver, 2017/11/18 */
				if( RPARAM::bUseRecoveryEXP )
					m_nACTION = REBIRTH_RECOVERY_EXP;
				else
					m_nACTION = REBIRTH_CHECK_POINT;

				DoACTION ();
                HideButton ();				
			}
		}
		break;

	case RIBIRTH_AUTO_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					if ( m_pAutoRebirthButton )
					{
						RPROFILE::bModernReviveWindowAutoRevive = !RPROFILE::bModernReviveWindowAutoRevive;
						m_pAutoRebirthButton->SetFlip( RPROFILE::bModernReviveWindowAutoRevive );
					}
				}
			}
		}break;
	}
}

void CModernRebirthDialogue::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

    if( m_bDO_ACTION )
	{
		m_fTIME_LEFT -= fElapsedTime;

		bool bTimeEvent = false;
		if ( m_fTIME_LEFT < -1.0f ) bTimeEvent = true;

		if ( bTimeEvent )
		{
			if ( m_bDONE_ACTION ) return ;

			switch ( m_nACTION )
			{
			case REBIRTH_CHECK_POINT:
				{
					AddMessageEx ( UIMSG_MODAL_OK );
					CInnerInterface::GetInstance().HideGroup ( GetWndID (), true );

					m_bDONE_ACTION = true;
				}
				break;

			case REBIRTH_CURRENT_POSITION:
				{
					AddMessageEx ( UIMSG_MODAL_CANCEL );
					CInnerInterface::GetInstance().HideGroup ( GetWndID (), true );

					m_bDONE_ACTION = true;
				}
				break;
			case REBIRTH_RECOVERY_EXP:
				{
					AddMessageEx ( UIMSG_MODAL_CANCEL );
					m_bDONE_ACTION = true;
				}
				break;
			}
		}
		else
		{
			CString strLeftTime;
			strLeftTime.Format ( "%1.0f%s", floor(m_fTIME_LEFT) + 1.0f, ID2GAMEWORD("WAITSERVER_TIMELEFT_UNIT"));

			m_pTextBoxQuestion->ClearText ();
			m_pTextBoxQuestion->AddText ( ID2GAMEINTEXT("WAITSERVER_MESSAGE") );
			m_pTextBoxQuestion->AddText ( strLeftTime );
		}	
	}
	else if ( m_bAutoRebirth )
	{
		m_fSelTIME_LEFT -= fElapsedTime;

		bool bTimeEvent = false;
		if ( m_fSelTIME_LEFT < -1.0f ) bTimeEvent = true;

		if ( bTimeEvent )
		{
			// Default ±ÍÈ¥ÁÖ »ç¿ë
			if ( RPROFILE::bModernReviveWindowAutoRevive )
				m_nACTION = REBIRTH_CHECK_POINT;
			else
				m_nACTION = REBIRTH_CURRENT_POSITION;
				
			DoACTION ();
			HideButton ();
		}
		else 
		{
			CString strLeftTime;
			strLeftTime.Format ( "%1.0f%s", floor(m_fSelTIME_LEFT) + 1.0f, ID2GAMEWORD("WAITSERVER_TIMELEFT_UNIT"));
		
			m_pTextBoxQuestion->SetText ( ID2GAMEINTEXT("REBIRTH_DIALOGUE_TEXT") );
			m_pTextBoxQuestion->AddText ( strLeftTime );
		}
	}
	/*force revive, Juver, 2018/07/09 */
	else if ( m_bForce_Revive && !m_bAutoRebirth )
	{
		m_fSelTIME_LEFT -= fElapsedTime;

		bool bTimeEvent = false;
		if ( m_fSelTIME_LEFT < -1.0f ) bTimeEvent = true;

		if ( bTimeEvent )
		{
			m_nACTION = REBIRTH_CHECK_POINT;

			DoACTION ();
			HideButton ();
		}
		else 
		{
			CString strLeftTime;
			strLeftTime.Format ( "%1.0f%s", floor(m_fSelTIME_LEFT) + 1.0f, ID2GAMEWORD("WAITSERVER_TIMELEFT_UNIT"));

			m_pTextBoxQuestion->SetText ( ID2GAMEINTEXT("WAITSERVER_MESSAGE") );
			m_pTextBoxQuestion->AddText ( strLeftTime );
		}
	}

}


void CModernRebirthDialogue::HideButton ()
{
	m_pCheckPointButton->SetVisibleSingle ( FALSE );
	m_pCurrentPositionButton->SetVisibleSingle ( FALSE );
	m_pRecoveryExpButton->SetVisibleSingle( FALSE );
}

void CModernRebirthDialogue::ShowButton ()
{
	m_pCheckPointButton->SetVisibleSingle ( TRUE );
	m_pCurrentPositionButton->SetVisibleSingle ( TRUE );
	m_pRecoveryExpButton->SetVisibleSingle( TRUE );
}