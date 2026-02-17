#include "StdAfx.h"
#include "./BasicHwOptionWindow.h"
#include "../../Lib_Engine/Core/NSRProfile.h"
#include "../../Lib_Engine/Core/NSROption.h"
#include "./BasicTextButton.h"
#include "./GameTextControl.h"
#include "./BasicLineBox.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "./VideoOption.h"
#include "./AudioOption.h"
#include "./GamePlayOption.h"
#include "./CharViewOptionEP6.h"
#include "./CharViewOptionEP9.h"

#include "../../Lib_Client/DxParamSet.h"

#include "../../Lib_Client/DxGlobalStage.h"
#include "./InnerInterface.h"

#include "../../Lib_Engine/DxSound/DxSoundMan.h"
#include "../../Lib_Engine/DxSound/BgmSound.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./NameDisplayMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicHWOptionWindow::CBasicHWOptionWindow ()
{
}

CBasicHWOptionWindow::~CBasicHWOptionWindow ()
{
}

void CBasicHWOptionWindow::CreateSubControl ()
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE18;
	m_pVideoButton = new CBasicTextButton;
	m_pVideoButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, HWOPTION_VIDEO_BUTTON );
	m_pVideoButton->CreateBaseButton ( "HWOPTION_VIDEO_BUTTON", nBUTTONSIZE, CBasicButton::RADIO_FLIP, (char*)ID2GAMEWORD ( "HWOPTION_AV_NAME", 0 ) );
	m_pVideoButton->SetFlip ( TRUE );
	RegisterControl ( m_pVideoButton );

	m_pAudioButton = new CBasicTextButton;
	m_pAudioButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, HWOPTION_AUDIO_BUTTON );
	m_pAudioButton->CreateBaseButton ( "HWOPTION_AUDIO_BUTTON", nBUTTONSIZE, CBasicButton::RADIO_FLIP, (char*)ID2GAMEWORD ( "HWOPTION_AV_NAME", 1 ) );
	m_pAudioButton->SetFlip ( FALSE );
	RegisterControl ( m_pAudioButton );

	m_pGamePlayButton = new CBasicTextButton;
	m_pGamePlayButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, HWOPTION_GAMEPLAY_BUTTON );
	m_pGamePlayButton->CreateBaseButton ( "HWOPTION_GAMEPLAY_BUTTON", nBUTTONSIZE, CBasicButton::RADIO_FLIP, (char*)ID2GAMEWORD ( "HWOPTION_AV_NAME", 2 ) );
	m_pGamePlayButton->SetFlip ( FALSE );
	RegisterControl ( m_pGamePlayButton );

	//m_pCharViewButton = new CBasicTextButton;
	//m_pCharViewButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, HWOPTION_CHARVIEW_BUTTON );
	//m_pCharViewButton->CreateBaseButton ( "HWOPTION_CHARVIEW_BUTTON", nBUTTONSIZE, CBasicButton::RADIO_FLIP, (char*)ID2GAMEWORD ( "HWOPTION_AV_NAME_EX", 0 ) );
	//m_pCharViewButton->SetFlip ( FALSE );
	//RegisterControl ( m_pCharViewButton );


	//	배경판떼기
	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxOption ( "HWOPTION_BACK_BOX" );
	RegisterControl ( pBasicLineBox );

	CBasicTextButton* pApplyButton = new CBasicTextButton;
	pApplyButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, HWOPTION_APPLY );
	pApplyButton->CreateBaseButton ( "HWOPTION_APPLY_BUTTON", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "HWOPTION_APPLYOK", 0 ) );
	RegisterControl ( pApplyButton );

	CBasicTextButton* pOKButton = new CBasicTextButton;
	pOKButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, HWOPTION_OK );
	pOKButton->CreateBaseButton ( "HWOPTION_OK_BUTTON", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "HWOPTION_APPLYOK", 1 ) );
	RegisterControl ( pOKButton );

	m_pVideoOption = new CBasicVideoOption;
	m_pVideoOption->CreateSub ( this, "HWOPTION_SHEET", UI_FLAG_DEFAULT, HWOPTION_VIDEO_PAGE );
	m_pVideoOption->CreateSubControl ();
	m_pVideoOption->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pVideoOption );

	m_pAudioOption = new CBasicAudioOption;
	m_pAudioOption->CreateSub ( this, "HWOPTION_SHEET", UI_FLAG_DEFAULT, HWOPTION_AUDIO_PAGE );
	m_pAudioOption->CreateSubControl ();
	m_pAudioOption->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pAudioOption );

	m_pGamePlayOption = new CBasicGamePlayOption;
	m_pGamePlayOption->CreateSub ( this, "HWOPTION_SHEET", UI_FLAG_DEFAULT, HWOPTION_GAMEPLAY_PAGE );
	m_pGamePlayOption->CreateSubControl ();
	m_pGamePlayOption->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pGamePlayOption );

//#if defined( BUILD_EP7 ) || defined( BUILD_EP6 ) || defined( BUILD_EP4 )
//	m_pCharViewOption = new CBasicCharViewOptionEP6;
//	m_pCharViewOption->CreateSub ( this, "HWOPTION_SHEET", UI_FLAG_DEFAULT, HWOPTION_CHARVIEW_PAGE );
//	m_pCharViewOption->CreateSubControl ();
//	m_pCharViewOption->SetVisibleSingle ( FALSE );
//	RegisterControl ( m_pCharViewOption );
//#else
//	m_pCharViewOption = new CBasicCharViewOptionEP9;
//	m_pCharViewOption->CreateSub ( this, "HWOPTION_SHEET", UI_FLAG_DEFAULT, HWOPTION_CHARVIEW_PAGE );
//	m_pCharViewOption->CreateSubControl ();
//	m_pCharViewOption->SetVisibleSingle ( FALSE );
//	RegisterControl ( m_pCharViewOption );
//#endif 


	

	m_bFirstSetting = TRUE;
}

void CBasicHWOptionWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case HWOPTION_VIDEO_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					SetVisiblePage ( HWOPTION_VIDEO_PAGE );
				}
			}
		}break;

	case HWOPTION_AUDIO_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					SetVisiblePage ( HWOPTION_AUDIO_PAGE );
				}
			}
		}break;

	case HWOPTION_GAMEPLAY_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					SetVisiblePage ( HWOPTION_GAMEPLAY_PAGE );
				}
			}
		}break;

	/*case HWOPTION_CHARVIEW_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					SetVisiblePage ( HWOPTION_CHARVIEW_PAGE );
				}
			}
		}break;*/

	case HWOPTION_APPLY:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if( m_pVideoOption->IsVisible() )			VideoOption_OK();
				else if( m_pAudioOption->IsVisible() )		AudioOption_OK();
				else if( m_pGamePlayOption->IsVisible() )	GamePlayOption_OK();
				//else if( m_pCharViewOption->IsVisible() )	CharViewOption_OK();
			}
		}
		break;

	case HWOPTION_OK:
	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if( m_pVideoOption->IsVisible() )			VideoOption_OK();
				else if( m_pAudioOption->IsVisible() )		AudioOption_OK();
				else if( m_pGamePlayOption->IsVisible() )	GamePlayOption_OK();

				CInnerInterface::GetInstance().HideGroup ( GetWndID () );
			}
		}
		break;
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( OPTION_HW_WINDOW );
			}
		}
		break;
	}

	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
}


void CBasicHWOptionWindow::SetVisiblePage ( int nPage )
{
	if ( nPage < HWOPTION_VIDEO_PAGE || /*HWOPTION_CHARVIEW_PAGE*/ HWOPTION_GAMEPLAY_PAGE < nPage )
	{
		GASSERT ( 0 && "CBasicHWOptionWindow::SetVisiblePage invalid page" );
		return ;
	}

	CUIControl* pVisiblePage = NULL;
	CBasicTextButton* pVisibleButton = NULL;

	{	//	리셋
		m_pVideoOption->SetVisibleSingle ( FALSE );
		m_pAudioOption->SetVisibleSingle ( FALSE );
		m_pGamePlayOption->SetVisibleSingle ( FALSE );
		//m_pCharViewOption->SetVisibleSingle ( FALSE );
	}
	{	//	리셋
		m_pVideoButton->SetFlip ( FALSE );
		m_pAudioButton->SetFlip ( FALSE );
		m_pGamePlayButton->SetFlip ( FALSE );
		//m_pCharViewButton->SetFlip ( FALSE );
	}

	switch ( nPage )
	{
	case HWOPTION_VIDEO_PAGE:
		{
			pVisiblePage = m_pVideoOption;
			pVisibleButton = m_pVideoButton;
		}break;

	case HWOPTION_AUDIO_PAGE:
		{
			pVisiblePage = m_pAudioOption;
			pVisibleButton = m_pAudioButton;
		}break;

	case HWOPTION_GAMEPLAY_PAGE:
		{
			pVisiblePage = m_pGamePlayOption;
			pVisibleButton = m_pGamePlayButton;
		}break;

	/*case HWOPTION_CHARVIEW_PAGE:
		{
			pVisiblePage = m_pCharViewOption;
			pVisibleButton = m_pCharViewButton;
		}break;*/
	}

	pVisiblePage->SetVisibleSingle ( TRUE );
	pVisibleButton->SetFlip ( TRUE );
}

void CBasicHWOptionWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		m_pVideoOption->LoadCurrentOption();
		m_pAudioOption->LoadCurrentOption();
		m_pGamePlayOption->LoadCurrentOption();
		//m_pCharViewOption->LoadCurrentOption();
		
		if( m_bFirstSetting )
		{
			GLGaeaClient::GetInstance().GetCharacter()->ReqNonRebirth(RPROFILE::bNON_Rebirth );

			/*charinfoview , Juver, 2017/11/12 */
			GLGaeaClient::GetInstance().GetCharacter()->ReqPrivateStats(RPROFILE::bPrivateStats );
		}

		m_bFirstSetting = FALSE;
	}
}

void CBasicHWOptionWindow::GamePlayOption_OK()
{
	DXPARAMSET::GetInstance().m_bDIS_CONFT		= m_pGamePlayOption->m_bConft;
	DXPARAMSET::GetInstance().m_bDIS_TRADE		= m_pGamePlayOption->m_bTrade;
	DXPARAMSET::GetInstance().m_bDIS_PARTY		= m_pGamePlayOption->m_bParty;
	DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP	= m_pGamePlayOption->m_bSimpleHP;
	RPROFILE::bDIS_FRIEND		= m_pGamePlayOption->m_bFriend;
	RPROFILE::bMOVABLE_ON_CHAT	= m_pGamePlayOption->m_bMovableOnChat;
	RPROFILE::bSHOW_TIP			= m_pGamePlayOption->m_bShowTip;
	RPROFILE::bFORCED_ATTACK	= m_pGamePlayOption->m_bForcedAttack;
	RPROFILE::bNAME_DISPLAY		= m_pGamePlayOption->m_bNameDisplay;
	RPROFILE::bNON_Rebirth		= m_pGamePlayOption->m_bNon_Rebirth;

	ROPTION::bHideSkillEffect	= m_pGamePlayOption->m_bHideSkillEffect;	/*skill effect setting, Juver, 2017/10/01 */
	RPROFILE::bPrivateStats		= m_pGamePlayOption->m_bPrivateStats;		/*charinfoview , Juver, 2017/11/12 */
	
	
	if ( ROPTION::emHideDead != (ROPTION::EMHIDE_DEAD)m_pGamePlayOption->m_nHideDead )
	{
		GLLandManClient* pLandClient = GLGaeaClient::GetInstance().GetActiveMap();
		if ( pLandClient && pLandClient->IsAllowHideDead() == false )
		{
			//just warn player that hide dead has no effect for the current map
			//but allow saving to avoid repeated message when changing other settings
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("HIDE_DEAD_NOT_ALLOWED") );
		}

		ROPTION::emHideDead	= (ROPTION::EMHIDE_DEAD)m_pGamePlayOption->m_nHideDead;
	}

	/* character simplify, Juver, 2021/07/29 */
	if ( RPROFILE::emCharacterView != (EMCHARACTER_VIEW)m_pGamePlayOption->m_nCharacterSimplify )
	{
		RPROFILE::emCharacterView = (EMCHARACTER_VIEW)m_pGamePlayOption->m_nCharacterSimplify;
		GLGaeaClient::GetInstance().UpdateSuitClientActor();

	}

	if ( ROPTION::bShowNameBackground != m_pGamePlayOption->m_bShowNameBackGround )
	{
		ROPTION::bShowNameBackground = m_pGamePlayOption->m_bShowNameBackGround;

		//warn player that the option can only be applied after the game has restarted
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("SHOW_NAME_BACKGROUND_CHANGE") );
	}

	GLGaeaClient::GetInstance().GetCharacter()->ReqNonRebirth(RPROFILE::bNON_Rebirth );

	GLGaeaClient::GetInstance().GetCharacter()->ReqPrivateStats(RPROFILE::bPrivateStats );	/*charinfoview , Juver, 2017/11/12 */

	DXPARAMSET::GetInstance().GameOK();
}

void CBasicHWOptionWindow::AudioOption_OK()
{
	ROPTION::bSndMuteSfx	= !m_pAudioOption->m_bSndMuteSfx;
	ROPTION::bSndMuteMap	= !m_pAudioOption->m_bSndMuteMap;
	ROPTION::bSndMuteMusic = !m_pAudioOption->m_bSndMuteMusic;

	DxSoundMan::GetInstance().SetSfxMute(ROPTION::bSndMuteSfx );
	DxSoundMan::GetInstance().SetMapMute(ROPTION::bSndMuteMap );
	DxBgmSound::GetInstance().SetMute(ROPTION::bSndMuteMusic );

	DXPARAMSET::GetInstance().AudioOK();
}

void CBasicHWOptionWindow::VideoOption_OK()
{
	DXPARAMSET::GetInstance().m_dwVideoLevel	= m_pVideoOption->m_dwVideoLevel;
	DXPARAMSET::GetInstance().m_dwFogRange		= m_pVideoOption->m_dwSight;
	DXPARAMSET::GetInstance().m_dwSkinDetail	= m_pVideoOption->m_dwSkinDetail;
	DXPARAMSET::GetInstance().m_dwShadowChar	= m_pVideoOption->m_dwShadowChar;
	DXPARAMSET::GetInstance().m_dwScrWidth		= m_pVideoOption->m_dwScrWidth;
	DXPARAMSET::GetInstance().m_dwScrHeight		= m_pVideoOption->m_dwScrHeight;
	DXPARAMSET::GetInstance().m_emScrFormat		= m_pVideoOption->m_emScrFormat;

	DXPARAMSET::GetInstance().m_bBuff			= m_pVideoOption->m_bBuff;
	DXPARAMSET::GetInstance().m_bShadowLand		= m_pVideoOption->m_bShadowLand;
	DXPARAMSET::GetInstance().m_bRealReflect	= m_pVideoOption->m_bRealReflect;
	DXPARAMSET::GetInstance().m_bRefract		= m_pVideoOption->m_bRefract;
	DXPARAMSET::GetInstance().m_bGlow			= m_pVideoOption->m_bGlow;
	DXPARAMSET::GetInstance().m_bPost			= m_pVideoOption->m_bPost;
	DXPARAMSET::GetInstance().m_bFrameLimit		= m_pVideoOption->m_bFrameLimit;
	DXPARAMSET::GetInstance().m_bClickEffect	= m_pVideoOption->m_bClickEffect;
	DXPARAMSET::GetInstance().m_bTargetEffect	= m_pVideoOption->m_bTargetEffect;
	DXPARAMSET::GetInstance().m_bMineEffect		= m_pVideoOption->m_bMineEffect;
	DXPARAMSET::GetInstance().m_dwTextureFilter	= m_pVideoOption->m_dwTextureFilter;

	DXPARAMSET::GetInstance().VideoOK ( DxGlobalStage::GetInstance().GetD3DApp () );
}

//void CBasicHWOptionWindow::CharViewOption_OK()
//{
//	if ( !GLGaeaClient::GetInstance().IsChangeRenderReady() )
//	{
//		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("CHANGE_RENDER_SETTING_NOT_READY") );
//		return;
//	}
//
//#if defined( BUILD_EP7 ) || defined( BUILD_EP6 ) || defined( BUILD_EP4 )
//	RPROFILE::emRenderHead = m_pCharViewOption->m_emRenderHead;
//	RPROFILE::emRenderUpper = m_pCharViewOption->m_emRenderUpper;
//	RPROFILE::emRenderLower = m_pCharViewOption->m_emRenderLower;
//	RPROFILE::emRenderGloves = m_pCharViewOption->m_emRenderGloves;
//	RPROFILE::emRenderShoes = m_pCharViewOption->m_emRenderShoes;
//	RPROFILE::emRenderWeapon = m_pCharViewOption->m_emRenderWeapon;
//	RPROFILE::emRenderNeckLace = m_pCharViewOption->m_emRenderNeckLace;
//	RPROFILE::emRenderBracelet = m_pCharViewOption->m_emRenderBracelet;
//	RPROFILE::emRenderRing = m_pCharViewOption->m_emRenderRing;
//#else
//	RPROFILE::emRenderHead = m_pCharViewOption->m_emRenderHead;
//	RPROFILE::emRenderUpper = m_pCharViewOption->m_emRenderUpper;
//	RPROFILE::emRenderLower = m_pCharViewOption->m_emRenderLower;
//	RPROFILE::emRenderGloves = m_pCharViewOption->m_emRenderGloves;
//	RPROFILE::emRenderShoes = m_pCharViewOption->m_emRenderShoes;
//	RPROFILE::emRenderWeapon = m_pCharViewOption->m_emRenderWeapon;
//	RPROFILE::emRenderNeckLace = m_pCharViewOption->m_emRenderNeckLace;
//	RPROFILE::emRenderBracelet = m_pCharViewOption->m_emRenderBracelet;
//	RPROFILE::emRenderRing = m_pCharViewOption->m_emRenderRing;
//
//	RPROFILE::emRenderBelt = m_pCharViewOption->m_emRenderBelt;
//	RPROFILE::emRenderEarring = m_pCharViewOption->m_emRenderEarring;
//	RPROFILE::emRenderAccessory = m_pCharViewOption->m_emRenderAccessory;
//	RPROFILE::emRenderDecoration = m_pCharViewOption->m_emRenderDecoration;
//#endif
//	
//	RPROFILE::bVehicleSimple = m_pCharViewOption->m_bVehicleSimple;
//	RPROFILE::bApplySelf = m_pCharViewOption->m_bApplySelf;
//
//	RPROFILE::Save();
//
//	GLGaeaClient::GetInstance().ChangeRenderSettings();
//
//	CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("CHANGE_RENDER_SETTING_DONE") );
//}