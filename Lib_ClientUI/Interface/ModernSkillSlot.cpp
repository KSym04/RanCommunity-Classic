#include "StdAfx.h"
#include "./ModernSkillSlot.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./SkillImage.h"

#include "./InnerInterface.h"
#include "./SkillWindowToTray.h"
#include "./ModalWindow.h"
#include "./ModalCallerID.h"

#include "./BasicButton.h"
#include "./BasicTextBox.h"
#include "./BasicLineBox.h"
#include "./BasicLineBoxEx.h"
#include "./MultiModeButton.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernSkillSlot::CModernSkillSlot () :
	m_pNotLearnImage ( NULL ),
	m_pImageHover_Y ( NULL ),
	m_pImageHover_G ( NULL ),
	m_pImageHover_B ( NULL ),
	m_pSkillImage ( NULL ),
	m_pSkillButton ( NULL ),
	m_pSkillButtonDisable( NULL ),
	m_pTextBox1 ( NULL ),
	m_pTextBox2 ( NULL ),
	m_sNativeID ( NATIVEID_NULL () ),
	m_bLearnSkill ( FALSE ),
	m_bLEVELUP_ENABLE ( FALSE ),
	m_bCANLEARN( FALSE ),
	m_bCANPURCHASE(FALSE),

	m_wLevel ( 0 ),
	m_bHover(false)
{
}

CModernSkillSlot::~CModernSkillSlot ()
{
}

void CModernSkillSlot::CreateSubControl ( SNATIVEID sNativeID )
{
	D3DCOLOR dwColor = NS_UITEXTCOLOR::DEFAULT;
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	int	nTextAlign = TEXT_ALIGN_LEFT;

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxSkill ( "RAN_CHARACTER_SKILL_SLOT_SIZE" );
	RegisterControl ( pLineBox );

	if ( sNativeID != NATIVEID_NULL () )
	{
		CSkillImage* pSkillImage = CreateSkillImage ();
		pSkillImage->SetSkill ( sNativeID );
		m_pSkillImage = pSkillImage;		

		m_pNotLearnImage = CreateControl ( "RAN_CHARACTER_SKILL_SLOT_IMAGE_NOTLEARN" );

		m_pImageHover_Y = new CBasicLineBoxEx;
		m_pImageHover_Y->CreateSub ( this, "RAN_BASIC_LINE_BOX_Y", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pImageHover_Y->CreateBaseBoxModernSkillYellow( "RAN_CHARACTER_SKILL_SLOT_HOVERIMAGE" );
		m_pImageHover_Y->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pImageHover_Y );

		m_pImageHover_G = new CBasicLineBoxEx;
		m_pImageHover_G->CreateSub ( this, "RAN_BASIC_LINE_BOX_G", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pImageHover_G->CreateBaseBoxModernSkillGreen( "RAN_CHARACTER_SKILL_SLOT_HOVERIMAGE" );
		m_pImageHover_G->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pImageHover_G );

		m_pImageHover_B = new CBasicLineBoxEx;
		m_pImageHover_B->CreateSub ( this, "RAN_BASIC_LINE_BOX_B", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pImageHover_B->CreateBaseBoxModernSkillBlue( "RAN_CHARACTER_SKILL_SLOT_HOVERIMAGE" );
		m_pImageHover_B->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pImageHover_B );

		m_pTextBox1 = CreateStaticControl ( "RAN_CHARACTER_SKILL_SLOT_TEXT0", pFont, dwColor, nTextAlign );			
		m_pTextBox1->SetLineInterval ( 4.0f );
		m_pTextBox2 = CreateStaticControl ( "RAN_CHARACTER_SKILL_SLOT_TEXT1", pFont, dwColor, nTextAlign );			
		m_pTextBox2->SetLineInterval ( 4.0f );

		m_pSkillButton = CreateButton( "RAN_CHARACTER_SKILL_SLOT_BUTTON", "+", SKILL_SLOT_LEVEL_UP );

		m_pSkillButtonDisable = CreateControl ( "RAN_CHARACTER_SKILL_SLOT_BUTTON_DISABLE" );
	}

	m_sNativeID = sNativeID;
}

CSkillImage* CModernSkillSlot::CreateSkillImage ()
{
	CSkillImage* pSkillImage = new CSkillImage;
	pSkillImage->CreateSub ( this, "RAN_CHARACTER_SKILL_SLOT_IMAGE", UI_FLAG_DEFAULT, SKILL_SLOT_IMAGE );
	pSkillImage->CreateSubControl ();
	pSkillImage->SetUseRender ( TRUE );
    RegisterControl ( pSkillImage );
	return pSkillImage;
}

CUIControl*	CModernSkillSlot::CreateControl ( char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

CBasicButton* CModernSkillSlot::CreateFlipButton ( char* szButton, char* szButtonFlip, char* szMouseOver, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->CreateMouseOver ( szMouseOver );
	RegisterControl ( pButton );
	return pButton;
}

CBasicTextBox* CModernSkillSlot::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );
	return pStaticText;
}

CMultiModeButton* CModernSkillSlot::CreateButton( char* szControl, const char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, "MODERN_BUTTON_LARGE_BLUE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, cID );
	pControl->CreateImageBaseMulti( "MODERN_BUTTON_LARGE_BLUE","MODERN_BUTTON_LARGE_BLUE_BASE_IMAGE_LEFT", "MODERN_BUTTON_LARGE_BLUE_BASE_IMAGE_CENTER", "MODERN_BUTTON_LARGE_BLUE_BASE_IMAGE_RIGHT" );
	pControl->CreateImageOverMulti( "MODERN_BUTTON_LARGE_BLUE","MODERN_BUTTON_LARGE_BLUE_OVER_IMAGE_LEFT", "MODERN_BUTTON_LARGE_BLUE_OVER_IMAGE_CENTER", "MODERN_BUTTON_LARGE_BLUE_OVER_IMAGE_RIGHT" );
	pControl->CreateImageHoldMulti( "MODERN_BUTTON_LARGE_BLUE","MODERN_BUTTON_LARGE_BLUE_CLICK_IMAGE_LEFT", "MODERN_BUTTON_LARGE_BLUE_CLICK_IMAGE_CENTER", "MODERN_BUTTON_LARGE_BLUE_CLICK_IMAGE_RIGHT" );
	pControl->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pControl->CreateTextBox( "MODERN_BUTTON_LARGE_BLUE_TEXT_POS", 8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pControl->AlignToControl( szControl );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pControl );

	return pControl;
}

void CModernSkillSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_sNativeID == NATIVEID_NULL () )
	{
		return ;
	}

	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sNativeID.wMainID, m_sNativeID.wSubID );
	if ( !pSkill )
	{
		GASSERT ( 0 && "찾을 수 없는 스킬입니다." );
		return ;
	}

	m_bLearnSkill = FALSE;
	m_bCANLEARN = FALSE;


	if ( m_pTextBox1 && m_pTextBox2 ) 
	{
		SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL ( m_sNativeID );
		BOOL bMASTER = GLGaeaClient::GetInstance().GetCharacter()->ISMASTER_SKILL ( m_sNativeID );
		if ( pCharSkill )
		{
			m_bLearnSkill = TRUE;

			WORD wLevel = pCharSkill->wLevel + 1;
			if ( m_wLevel != wLevel )
			{
				m_wLevel = wLevel;
				SKILL::SLEARN_LVL sLVL = pSkill->m_sLEARN.sLVL_STEP[wLevel];

				m_strLine1.Format ( "[%s]%s.%d", pSkill->GetName(), (char*)ID2GAMEWORD ( "SKILL_SLOT", 0 ), pSkill->m_sBASIC.dwGRADE );
				if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE ) m_strLine1 += (char*)ID2GAMEWORD ( "SKILL_SLOT", 1 );

				if ( !bMASTER )
				{
					CString strAA = (char*)ID2GAMEWORD ( "SKILL_SLOT", 2 );
					m_strLine2.Format ( "%s.%d %s.%d", (char*)ID2GAMEWORD ( "SKILL_SLOT", 3 ), wLevel, strAA, sLVL.dwSKP );
				}
				else
					m_strLine2.Format ( "%s", (char*)ID2GAMEWORD ( "SKILL_SLOT", 4 ) );			

				m_pTextBox1->ClearText ();
				m_pTextBox2->ClearText ();
				m_pTextBox1->AddTextNoSplit ( m_strLine1, NS_UITEXTUTIL::ENABLE );
				m_pTextBox2->AddTextNoSplit ( m_strLine2, NS_UITEXTUTIL::ENABLE );

				AddMessageEx ( MODERN_SKILL_SLOT_UIMSG_LEVEL_UP | UIMSG_TOTOPPARENT );
			}
		}		
		else
		{
			m_pTextBox1->ClearText ();
			m_pTextBox2->ClearText ();

			m_strLine1.Format ( "[%s]%s.%d", pSkill->GetName(), (char*)ID2GAMEWORD ( "SKILL_SLOT", 0 ), pSkill->m_sBASIC.dwGRADE );
			if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE ) m_strLine1 += (char*)ID2GAMEWORD ( "SKILL_SLOT", 1 );
			m_pTextBox1->AddTextNoSplit ( m_strLine1, NS_UITEXTUTIL::ENABLE );

			SKILL::SLEARN_LVL sLVL = pSkill->m_sLEARN.sLVL_STEP[0];
			m_strLine2.Format ( "%s.%d", (char*)ID2GAMEWORD ( "SKILL_SLOT", 2 ), sLVL.dwSKP );
			m_pTextBox2->AddTextNoSplit ( m_strLine2, NS_UITEXTUTIL::ENABLE );
		}
	}

	if ( m_pNotLearnImage ) m_pNotLearnImage->SetVisibleSingle ( !m_bLearnSkill );
	

	if ( m_bLearnSkill )
	{
		m_bLEVELUP_ENABLE = GLGaeaClient::GetInstance().GetCharacter()->CHECKLEARNABLE_SKILL ( m_sNativeID ) == EMSKILL_LEARN_OK;
		m_bCANPURCHASE = FALSE;

		if ( m_pSkillButton )	
		{
			m_pSkillButton->SetVisibleSingle( m_bLearnSkill );

			if ( !m_bLEVELUP_ENABLE )
			{
				m_pSkillButton->DoImageFlip ( !m_bLEVELUP_ENABLE );
				m_pSkillButton->DoImageOver( m_bLEVELUP_ENABLE );
			}

			if ( m_pSkillButtonDisable )	
				m_pSkillButtonDisable->SetVisibleSingle( !m_bLEVELUP_ENABLE );
		}
	}
	else
	{
		m_bLEVELUP_ENABLE = FALSE;

		m_bCANLEARN = GLGaeaClient::GetInstance().GetCharacter()->CHECKLEARNABLE_SKILL ( m_sNativeID ) == EMSKILL_LEARN_OK;

		m_bCANPURCHASE = GLGaeaClient::GetInstance().GetCharacter()->IsPurchaseSkill( m_sNativeID );
		
		if ( m_pSkillButton )	
			m_pSkillButton->SetVisibleSingle( m_bCANPURCHASE );

		if ( m_pSkillButtonDisable )	
			m_pSkillButtonDisable->SetVisibleSingle( FALSE );
	}


	if ( m_bLearnSkill )
	{
		if ( m_pSkillImage ) m_pSkillImage->SetSkillProcess ( m_sNativeID );
	}

	m_pImageHover_Y->SetVisibleSingle( FALSE );
	m_pImageHover_G->SetVisibleSingle( FALSE );
	m_pImageHover_B->SetVisibleSingle( FALSE );

	if ( m_bHover )
	{
		m_pImageHover_Y->SetVisibleSingle( !m_bCANLEARN && !m_bLearnSkill );
		m_pImageHover_G->SetVisibleSingle( m_bCANLEARN && !m_bLearnSkill );
		m_pImageHover_B->SetVisibleSingle( !m_bCANLEARN && m_bLearnSkill );
	}
}

void CModernSkillSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case SKILL_SLOT_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg )  )
			{
				AddMessageEx ( MODERN_SKILL_SLOT_UIMSG_MOUSEIN_ICON | UIMSG_TOTOPPARENT );

				if ( m_bLearnSkill )
				{
					if ( dwMsg & UIMSG_LB_DOWN )
					{
						PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sNativeID.wMainID, m_sNativeID.wSubID );
						if ( !pSkill )
						{
							GASSERT ( 0 && "찾을 수 없는 스킬입니다." );
							return ;
						}
		
						if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE ) return ;

						CSkillWindowToTray* pSkillWindowToTray = CInnerInterface::GetInstance().GetSkillWindowToTray ();
						if ( !pSkillWindowToTray ) return ;

						pSkillWindowToTray->SetSkill ( m_sNativeID );
						
						/*modern skill tray, Juver, 2018/08/21 */
						CInnerInterface::GetInstance().skill_tray_open( FALSE );
					}
					else if ( dwMsg & UIMSG_RB_DOWN )
					{
						PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sNativeID.wMainID, m_sNativeID.wSubID );
						if ( !pSkill )
						{
							GASSERT ( 0 && "찾을 수 없는 스킬입니다." );
							return ;
						}
		
						if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE ) return ;
						GLGaeaClient::GetInstance().GetCharacter()->ReqSkillQuickSet( m_sNativeID );
					}
				}
			}
		}
		break;

	case SKILL_SLOT_LEVEL_UP:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_bLearnSkill )
				{
					AddMessageEx ( MODERN_SKILL_SLOT_UIMSG_MOUSEIN_BUTTON | UIMSG_TOTOPPARENT );
				}
				

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( m_bLEVELUP_ENABLE )
					{
						SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL ( m_sNativeID );					
						BOOL bMASTER = GLGaeaClient::GetInstance().GetCharacter()->ISMASTER_SKILL ( m_sNativeID );
						if ( pCharSkill )
						{
							WORD wLevel = pCharSkill->wLevel + 1;
							if ( !bMASTER )
							{
								PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sNativeID.wMainID, m_sNativeID.wSubID );
								if ( !pSkill )
								{
									GASSERT ( 0 && "찾을 수 없는 스킬입니다." );
									return ;
								}

								CInnerInterface::GetInstance().SetSkillUpID ( m_sNativeID.dwID );

								CString strLevelUp = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("SKILL_LEVEL_UP_CONFIRM"), pSkill->GetName() );                            
								DoModal ( strLevelUp, MODAL_QUESTION, YESNO, MODAL_SKILL_LEVEL_UP_CONFIRM );//							
							}
						}
					}
					else if ( m_bCANPURCHASE )
					{
						BOOL bCanPurchase = GLGaeaClient::GetInstance().GetCharacter()->IsPurchaseSkill(m_sNativeID);
						if ( bCanPurchase )
						{
							PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sNativeID.wMainID, m_sNativeID.wSubID );
							if ( !pSkill )
							{
								GASSERT ( 0 && "찾을 수 없는 스킬입니다." );
								return ;
							}

							CInnerInterface::GetInstance().SetSkillUpID ( m_sNativeID.dwID );
							CString strLevelUp = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("SKILL_PURCHASE_CONFIRM"), pSkill->GetName() );                            
							DoModal ( strLevelUp, MODAL_QUESTION, YESNO, MODAL_SKILL_PURCHASE_CONFIRM );//			
						}
					}

				}
			}
		}
		break;
	}

	CUIGroup::TranslateUIMessage( ControlID, dwMsg );
}