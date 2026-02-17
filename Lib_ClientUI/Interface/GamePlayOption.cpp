#include "StdAfx.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRProfile.h"
#include "../../Lib_Engine/Core/NSROption.h"

#include "./GamePlayOption.h"
#include "./BasicComboBox.h"
#include "./BasicComboBoxRollOver.h"
#include "./BasicButton.h"
#include "./BasicTextBox.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "../../Lib_Client/DxParamSet.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicGamePlayOption::CBasicGamePlayOption () :
	m_pFont ( NULL ),
	m_RollOverID(NO_ID),
	m_bFirstLBUP(FALSE),
	m_pConftButton ( NULL ),
	m_pTradeButton ( NULL ),
	m_pPartyButton ( NULL ),
	m_pSimpleHPButton ( NULL ),
	m_pFriendButton ( NULL ),
	m_pMovableOnChatButton ( NULL ),
	m_pShowTipButton ( NULL ),

	/*skill effect setting, Juver, 2017/10/01 */
	m_pHideSkillEffectButton(NULL),  
	m_bHideSkillEffect(FALSE),

	/*charinfoview , Juver, 2017/11/12 */
	m_pPrivateStats(NULL),
	m_bPrivateStats(TRUE),

	/* character simplify, Juver, 2021/07/29 */
	m_pComboBoxCharacterSimplifyOpen(NULL),
	m_pComboBoxCharacterSimplifyRollOver(NULL),

	m_nHideDead(FALSE),
	m_pComboBoxHideDeadOpen(NULL),
	m_pComboBoxHideDeadRollOver(NULL),

	m_pShowNameBackGroundButton(NULL),
	m_bShowNameBackGround(TRUE)
{
}

CBasicGamePlayOption::~CBasicGamePlayOption ()
{
}

void CBasicGamePlayOption::CreateSubControl ()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicTextBox* pTextBox = NULL;

	{	//	기본 텍스트		
		DWORD dwFontColor = NS_UITEXTCOLOR::DEFAULT;
		int nAlign = TEXT_ALIGN_LEFT;

		//	자동 대련 거부
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_CONFT_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 0 ) );

		//	자동 거래 거부
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_TRADE_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 1 ) );

		//	자동 파티 거부
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_PARTY_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 2 ) );

		//	미니 HP 표시
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_SIMPLEHP_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 3 ) );

		//	자동 친구 거부
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_FRIEND_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 4 ) );

		//	게임창 기본 위치로
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_DEFAULTPOS_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 5 ) );

		//  채팅창 뚫기
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_MOVABLE_ON_CHAT_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 6 ) );

		//  팁 보기
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_SHOW_TIP_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 7 ) );

		//  강제 공격
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_FORCED_ATTACK_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 8 ) );

		//  이름 항상 표시
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_NAME_DISPLAY_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 9 ) );

		//  부활 스킬 금지
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_REVIVAL_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 10 ) );

		/*skill effect setting, Juver, 2017/10/01 */
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_HIDE_SKILL_EFFECT_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_EX", 1 ) );

		/*charinfoview , Juver, 2017/11/12 */
		if ( RPARAM::bViewCharInfo && RPARAM::bViewCharAllowPrivate )
		{
			pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_PRIVATE_STATS_STATIC", m_pFont, dwFontColor, nAlign );
			pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_EX", 2 ) );
		}

		/* character simplify, Juver, 2021/07/29 */
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_CHARACTER_SIMPLIFY_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_EX", 3 ) );

		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_HIDE_DEAD_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_EX", 4 ) );
		

		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_SHOW_NAME_BACKGROUND_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_EX", 5 ) );
	}

	{	//	기능
		//	버튼
		m_pConftButton = CreateFlipButton( "HWOPTION_GAMEPLAY_CONFT_BUTTON", "HWOPTION_GAMEPLAY_CONFT_BUTTON_F", HWOPTION_GAMEPLAY_CONFT_BUTTON );
		m_pTradeButton = CreateFlipButton( "HWOPTION_GAMEPLAY_TRADE_BUTTON", "HWOPTION_GAMEPLAY_TRADE_BUTTON_F", HWOPTION_GAMEPLAY_TRADE_BUTTON );
		m_pPartyButton = CreateFlipButton( "HWOPTION_GAMEPLAY_PARTY_BUTTON", "HWOPTION_GAMEPLAY_PARTY_BUTTON_F", HWOPTION_GAMEPLAY_PARTY_BUTTON );
		m_pSimpleHPButton = CreateFlipButton( "HWOPTION_GAMEPLAY_SIMPLEHP_BUTTON", "HWOPTION_GAMEPLAY_SIMPLEHP_BUTTON_F", HWOPTION_GAMEPLAY_SIMPLEHP_BUTTON );
		m_pFriendButton = CreateFlipButton( "HWOPTION_GAMEPLAY_FRIEND_BUTTON", "HWOPTION_GAMEPLAY_FRIEND_BUTTON_F", HWOPTION_GAMEPLAY_FRIEND_BUTTON );
		m_pMovableOnChatButton = CreateFlipButton( "HWOPTION_GAMEPLAY_MOVABLE_ON_CHAT_BUTTON", "HWOPTION_GAMEPLAY_MOVABLE_ON_CHAT_BUTTON_F", HWOPTION_GAMEPLAY_MOVABLE_ON_CHAT_BUTTON );
		m_pShowTipButton = CreateFlipButton( "HWOPTION_GAMEPLAY_SHOW_TIP_BUTTON", "HWOPTION_GAMEPLAY_SHOW_TIP_BUTTON_F", HWOPTION_GAMEPLAY_SHOW_TIP_BUTTON );
		m_pForcedAttackButton = CreateFlipButton( "HWOPTION_GAMEPLAY_FORCED_ATTACK_BUTTON", "HWOPTION_GAMEPLAY_FORCED_ATTACK_BUTTON_F", HWOPTION_GAMEPLAY_FORCED_ATTACK_BUTTON );
		m_pNameDisplayButton = CreateFlipButton( "HWOPTION_GAMEPLAY_NAME_DISPLAY_BUTTON", "HWOPTION_GAMEPLAY_NAME_DISPLAY_BUTTON_F", HWOPTION_GAMEPLAY_NAME_DISPLAY_BUTTON );
		m_pRevivalButton = CreateFlipButton( "HWOPTION_GAMEPLAY_REVIVAL_BUTTON", "HWOPTION_GAMEPLAY_REVIVAL_BUTTON_F", HWOPTION_GAMEPLAY_REVIVAL_BUTTON );

		
		/*skill effect setting, Juver, 2017/10/01 */
		m_pHideSkillEffectButton = CreateFlipButton( "HWOPTION_GAMEPLAY_HIDE_SKILL_EFFECT_BUTTON", "HWOPTION_GAMEPLAY_HIDE_SKILL_EFFECT_BUTTON_F", HWOPTION_GAMEPLAY_HIDE_SKILL_EFFECT_BUTTON );


		m_pShowNameBackGroundButton = CreateFlipButton( "HWOPTION_GAMEPLAY_SHOW_NAME_BACKGROUND_BUTTON", "HWOPTION_GAMEPLAY_SHOW_NAME_BACKGROUND_BUTTON_F", HWOPTION_GAMEPLAY_SHOW_NAME_BACKGROUND );


		/*charinfoview , Juver, 2017/11/12 */
		if ( RPARAM::bViewCharInfo && RPARAM::bViewCharAllowPrivate )
		{
			m_pPrivateStats = CreateFlipButton( "HWOPTION_GAMEPLAY_PRIVATE_STATS_BUTTON", "HWOPTION_GAMEPLAY_PRIVATE_STATS_BUTTON_F", HWOPTION_GAMEPLAY_PRIVATE_STATS_BUTTON );
		}
		

		{
			CBasicButton* pButton = new CBasicButton;
			pButton->CreateSub ( this, "HWOPTION_GAMEPLAY_DEFAULTPOS_BUTTON", UI_FLAG_DEFAULT, HWOPTION_GAMEPLAY_DEFAULTPOS_BUTTON );
			pButton->CreateFlip ( "HWOPTION_GAMEPLAY_DEFAULTPOS_BUTTON_F", CBasicButton::CLICK_FLIP );				
			RegisterControl ( pButton );
		}

		/* character simplify, Juver, 2021/07/29 */
		m_pComboBoxCharacterSimplifyOpen = new CBasicComboBox;
		m_pComboBoxCharacterSimplifyOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, HWOPTION_GAMEPLAY_CHARACTER_SIMPLIFY_COMBO_OPEN );
		m_pComboBoxCharacterSimplifyOpen->CreateBaseComboBox ( "HWOPTION_GAMEPLAY_CHARACTER_SIMPLIFY_COMBO_OPEN" );			
		RegisterControl ( m_pComboBoxCharacterSimplifyOpen );

		m_pComboBoxHideDeadOpen = new CBasicComboBox;
		m_pComboBoxHideDeadOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, HWOPTION_GAMEPLAY_HIDE_DEAD_COMBO_OPEN );
		m_pComboBoxHideDeadOpen->CreateBaseComboBox ( "HWOPTION_GAMEPLAY_HIDE_DEAD_COMBO_OPEN" );			
		RegisterControl ( m_pComboBoxHideDeadOpen );


		/* character simplify, Juver, 2021/07/29 */
		m_pComboBoxCharacterSimplifyRollOver = new CBasicComboBoxRollOver;
		m_pComboBoxCharacterSimplifyRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, HWOPTION_GAMEPLAY_CHARACTER_SIMPLIFY_COMBO_ROLLOVER );
		m_pComboBoxCharacterSimplifyRollOver->CreateBaseComboBoxRollOver ( "HWOPTION_GAMEPLAY_CHARACTER_SIMPLIFY_COMBO_ROLLOVER" );			
		m_pComboBoxCharacterSimplifyRollOver->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pComboBoxCharacterSimplifyRollOver );

		m_pComboBoxHideDeadRollOver = new CBasicComboBoxRollOver;
		m_pComboBoxHideDeadRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, HWOPTION_GAMEPLAY_HIDE_DEAD_COMBO_ROLLOVER );
		m_pComboBoxHideDeadRollOver->CreateBaseComboBoxRollOver ( "HWOPTION_GAMEPLAY_HIDE_DEAD_COMBO_ROLLOVER" );			
		m_pComboBoxHideDeadRollOver->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pComboBoxHideDeadRollOver );

	}
}

CBasicButton* CBasicGamePlayOption::CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->SetControlNameEx ( szButton );
	RegisterControl ( pButton );
	return pButton;
}

CBasicTextBox* CBasicGamePlayOption::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );
	return pStaticText;
}

void CBasicGamePlayOption::LoadConft ()
{
	m_bConft = DXPARAMSET::GetInstance().m_bDIS_CONFT;
	m_pConftButton->SetFlip ( m_bConft );
}

void CBasicGamePlayOption::LoadTrade ()
{
	m_bTrade = DXPARAMSET::GetInstance().m_bDIS_TRADE;
	m_pTradeButton->SetFlip ( m_bTrade );
}

void CBasicGamePlayOption::LoadParty ()
{
	m_bParty = DXPARAMSET::GetInstance().m_bDIS_PARTY;
	m_pPartyButton->SetFlip ( m_bParty );
}

void CBasicGamePlayOption::LoadSimpleHP ()
{
	m_bSimpleHP = DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP;
	m_pSimpleHPButton->SetFlip ( m_bSimpleHP );
}

void CBasicGamePlayOption::LoadFriend ()
{
	m_bFriend = RPROFILE::bDIS_FRIEND;
	m_pFriendButton->SetFlip ( m_bFriend );
}

void CBasicGamePlayOption::LoadMovableOnChat ()
{
	m_bMovableOnChat = RPROFILE::bMOVABLE_ON_CHAT;
	m_pMovableOnChatButton->SetFlip ( m_bMovableOnChat );
}

void CBasicGamePlayOption::LoadShowTip ()
{
	m_bShowTip = RPROFILE::bSHOW_TIP;
	m_pShowTipButton->SetFlip ( m_bShowTip );
}

void CBasicGamePlayOption::LoadForcedAttack ()
{
	m_bForcedAttack = RPROFILE::bFORCED_ATTACK;
	m_pForcedAttackButton->SetFlip ( m_bForcedAttack );
}

void CBasicGamePlayOption::LoadNameDisplay ()
{
	m_bNameDisplay = RPROFILE::bNAME_DISPLAY;
	m_pNameDisplayButton->SetFlip ( m_bNameDisplay );
}

void CBasicGamePlayOption::LoadRevival()
{
	m_bNon_Rebirth = RPROFILE::bNON_Rebirth;
	m_pRevivalButton->SetFlip ( m_bNon_Rebirth );
}

/*charinfoview , Juver, 2017/11/12 */
void CBasicGamePlayOption::LoadPrivateStats()
{
	m_bPrivateStats = RPROFILE::bPrivateStats;

	if ( m_pPrivateStats )
		m_pPrivateStats->SetFlip ( m_bPrivateStats );
}

/*skill effect setting, Juver, 2017/10/01 */
void CBasicGamePlayOption::LoadHideSkillEffect()
{
	m_bHideSkillEffect = ROPTION::bHideSkillEffect;
	m_pHideSkillEffectButton->SetFlip ( m_bHideSkillEffect );
}

/* character simplify, Juver, 2021/07/29 */
void CBasicGamePlayOption::LoadCharacterSimplify()
{
	m_nCharacterSimplify = RPROFILE::emCharacterView;

	if ( 0 <= m_nCharacterSimplify && m_nCharacterSimplify < EMCHARACTER_VIEW_SIZE )
	{
		CString strTemp( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_CHARACTER_SIMPLIFY", m_nCharacterSimplify ) );

		if ( m_pComboBoxCharacterSimplifyOpen )
		{
			m_pComboBoxCharacterSimplifyOpen->SetText ( strTemp );
		}
	}
}

void CBasicGamePlayOption::LoadHideDead()
{
	m_nHideDead = ROPTION::emHideDead;

	if ( 0 <= m_nHideDead && m_nHideDead < ROPTION::EMHIDE_DEAD_SIZE )
	{
		CString strTemp( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_HIDE_DEAD", m_nHideDead ) );

		if ( m_pComboBoxHideDeadOpen )
		{
			m_pComboBoxHideDeadOpen->SetText ( strTemp );
		}
	}
}

void CBasicGamePlayOption::LoadData()
{
	if ( m_pComboBoxCharacterSimplifyRollOver )
	{
		m_pComboBoxCharacterSimplifyRollOver->ClearText ();
		for ( int i = 0; i < EMCHARACTER_VIEW_SIZE; i++ )
		{
			m_pComboBoxCharacterSimplifyRollOver->AddText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_CHARACTER_SIMPLIFY", i ) );
		}
	}

	if ( m_pComboBoxHideDeadRollOver )
	{
		m_pComboBoxHideDeadRollOver->ClearText ();
		for ( int i = 0; i < ROPTION::EMHIDE_DEAD_SIZE; i++ )
		{
			m_pComboBoxHideDeadRollOver->AddText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_HIDE_DEAD", i ) );
		}
	}
}

void CBasicGamePlayOption::LoadShowNameBackGround()
{
	m_bShowNameBackGround = ROPTION::bShowNameBackground;

	if ( m_pShowNameBackGroundButton )
		m_pShowNameBackGroundButton->SetFlip ( m_bShowNameBackGround );
}

void CBasicGamePlayOption::LoadCurrentOption()
{
	LoadConft();
	LoadTrade();
	LoadParty();
	LoadSimpleHP();
	LoadFriend();
	LoadMovableOnChat();
	LoadShowTip();
	LoadForcedAttack();
	LoadNameDisplay();
	LoadRevival();

	LoadHideSkillEffect();	/*skill effect setting, Juver, 2017/10/01 */
	LoadPrivateStats();		/*charinfoview , Juver, 2017/11/12 */

	LoadCharacterSimplify();		/* character simplify, Juver, 2021/07/29 */

	LoadHideDead();

	LoadShowNameBackGround();
}

void CBasicGamePlayOption::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( m_RollOverID == NO_ID )
	{
		CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
	}
	else
	{
		ResetMessageEx ();

		CUIControl::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		CUIControl* pControl = m_ControlContainer.FindControl ( m_RollOverID );
		if ( !pControl )
		{
			GASSERT ( 0 && "Invalid RollOver ID." );
			return;
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

void CBasicGamePlayOption::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		LoadData();
		LoadCurrentOption();
	}
}