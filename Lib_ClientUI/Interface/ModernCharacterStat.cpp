#include "StdAfx.h"
#include "./ModernCharacterStat.h"
#include "./ModernCharacterStatMark.h"
#include "./ModernCharacterStatOver.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"
#include "./MultiModeButton.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Helper/HLibDataConvert.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "./InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernCharacterStat::CModernCharacterStat ()
	: m_wPowAdd(0)
	, m_wDexAdd(0)
	, m_wSpiAdd(0)
	, m_wStrAdd(0)
	, m_wStaAdd(0)
	, m_wStatsDeduct(0)
	, m_bActionBlock(FALSE)

	, m_pQuestionButton(NULL)

	, m_pPowTextStatic(NULL)
	, m_pDexTextStatic(NULL)
	, m_pSpiTextStatic(NULL)
	, m_pStrTextStatic(NULL)
	, m_pStaTextStatic(NULL)

	, m_pPointValue(NULL)
	, m_pPowTextValue(NULL)
	, m_pDexTextValue(NULL)
	, m_pSpiTextValue(NULL)
	, m_pStrTextValue(NULL)
	, m_pStaTextValue(NULL)

	, m_pPowTextMark(NULL)
	, m_pDexTextMark(NULL)
	, m_pSpiTextMark(NULL)
	, m_pStrTextMark(NULL)
	, m_pStaTextMark(NULL)

	, m_pPowTextUp(NULL)
	, m_pDexTextUp(NULL)
	, m_pSpiTextUp(NULL)
	, m_pStrTextUp(NULL)
	, m_pStaTextUp(NULL)

	, m_pAttackMinTextValue(NULL)
	, m_pAttackMaxTextValue(NULL)
	, m_pDefenseTextValue(NULL)
	, m_pFightTextValue(NULL)
	, m_pShootTextValue(NULL)
	, m_pEnergyTextValue(NULL)
	, m_pHitTextValue(NULL)
	, m_pAvoidTextValue(NULL)
	, m_pHPTextValue(NULL)
	, m_pMPTextValue(NULL)
	, m_pSPTextValue(NULL)

	, m_pAttackMinTextMark(NULL)
	, m_pAttackMaxTextMark(NULL)
	, m_pDefenseTextMark(NULL)
	, m_pFightTextMark(NULL)
	, m_pShootTextMark(NULL)
	, m_pEnergyTextMark(NULL)
	, m_pHitTextMark(NULL)
	, m_pAvoidTextMark(NULL)
	, m_pHPTextMark(NULL)
	, m_pMPTextMark(NULL)
	, m_pSPTextMark(NULL)

	, m_pAttackMinTextUp(NULL)
	, m_pAttackMaxTextUp(NULL)
	, m_pDefenseTextUp(NULL)
	, m_pFightTextUp(NULL)
	, m_pShootTextUp(NULL)
	, m_pEnergyTextUp(NULL)
	, m_pHitTextUp(NULL)
	, m_pAvoidTextUp(NULL)
	, m_pHPTextUp(NULL)
	, m_pMPTextUp(NULL)
	, m_pSPTextUp(NULL)

	, m_pFireTextValue(NULL)
	, m_pColdTextValue(NULL)
	, m_pElectricTextValue(NULL)
	, m_pPoisonTextValue(NULL)
	, m_pSpiritTextValue(NULL)

	, m_pFireTextMark(NULL)
	, m_pColdTextMark(NULL)
	, m_pElectricTextMark(NULL)
	, m_pPoisonTextMark(NULL)
	, m_pSpiritTextMark(NULL)

	, m_pFireTextUp(NULL)
	, m_pColdTextUp(NULL)
	, m_pElectricTextUp(NULL)
	, m_pPoisonTextUp(NULL)
	, m_pSpiritTextUp(NULL)

	, m_pAttackMinMarkImage(NULL)
	, m_pAttackMaxMarkImage(NULL)
	, m_pDefenseMarkImage(NULL)
	, m_pFightMarkImage(NULL)
	, m_pShootMarkImage(NULL)
	, m_pEnergyMarkImage(NULL)
	, m_pHitMarkImage(NULL)
	, m_pAvoidMarkImage(NULL)
	, m_pHPMarkImage(NULL)
	, m_pMPMarkImage(NULL)
	, m_pSPMarkImage(NULL)

	, m_pFireMarkImage(NULL)
	, m_pColdMarkImage(NULL)
	, m_pElectricMarkImage(NULL)
	, m_pPoisonMarkImage(NULL)
	, m_pSpiritMarkImage(NULL)

	, m_pPowButton_Plus(NULL)
	, m_pDexButton_Plus(NULL)
	, m_pSpiButton_Plus(NULL)
	, m_pStrButton_Plus(NULL)
	, m_pStaButton_Plus(NULL)
	, m_pPowButton_Minus(NULL)
	, m_pDexButton_Minus(NULL)
	, m_pSpiButton_Minus(NULL)
	, m_pStrButton_Minus(NULL)
	, m_pStaButton_Minus(NULL)

	, m_pButtonStatReset(NULL)
	, m_pButtonStatApply(NULL)

{
}

CModernCharacterStat::~CModernCharacterStat ()
{
}

CUIControl*	CModernCharacterStat::CreateControl ( const char* szControl, const UIGUID& cID /*= NO_ID*/ )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	pControl->SetVisibleSingle( TRUE );
	RegisterControl ( pControl );
	return pControl;
}

CBasicLineBox* CModernCharacterStat::CreateLineBox ( char* szKeyword, const UIGUID& cID /*= NO_ID*/ )
{
	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE, cID );
	pBasicLineBox->CreateBaseBoxQuestList( szKeyword );
	RegisterControl ( pBasicLineBox );

	return pBasicLineBox;
}

CBasicTextBox* CModernCharacterStat::CreateTextBox ( char* szKeyword, const char* szText, DWORD dwColor, CD3DFontPar* pFont, int nAlign, const UIGUID& cID /*= NO_ID*/ )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	pStaticText->AddText( szText, dwColor );
	RegisterControl ( pStaticText );

	return pStaticText;
}

CModernCharacterStatMark* CModernCharacterStat::CreateStatMark ( char* szKeyword, const UIGUID& cID /*= NO_ID*/ )
{
	CModernCharacterStatMark* pControl = new CModernCharacterStatMark;
	pControl->CreateSub ( this, szKeyword, UI_FLAG_DEFAULT, cID );
	pControl->CreateSubControl();
	RegisterControl ( pControl );

	return pControl;
}

CModernCharacterStatOver* CModernCharacterStat::CreateStatOver ( char* szKeyword, const char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CModernCharacterStatOver* pControl = new CModernCharacterStatOver;
	pControl->CreateSub ( this, szKeyword, UI_FLAG_DEFAULT );
	pControl->CreateSubControl( szText );
	RegisterControl ( pControl );

	return pControl;
}

CMultiModeButton* CModernCharacterStat::CreateStatButton( char* szControl, const char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, "MODERN_BUTTON_BLUE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, cID );
	pControl->CreateImageBaseMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_BASE_IMAGE_LEFT", "MODERN_BUTTON_BLUE_BASE_IMAGE_CENTER", "MODERN_BUTTON_BLUE_BASE_IMAGE_RIGHT" );
	pControl->CreateImageOverMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_OVER_IMAGE_LEFT", "MODERN_BUTTON_BLUE_OVER_IMAGE_CENTER", "MODERN_BUTTON_BLUE_OVER_IMAGE_RIGHT" );
	pControl->CreateImageHoldMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_CLICK_IMAGE_LEFT", "MODERN_BUTTON_BLUE_CLICK_IMAGE_CENTER", "MODERN_BUTTON_BLUE_CLICK_IMAGE_RIGHT" );
	pControl->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pControl->CreateTextBox( "MODERN_BUTTON_BLUE_TEXT_POS", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pControl->AlignToControl( szControl );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pControl );

	return pControl;
}

CMultiModeButton* CModernCharacterStat::CreateStat2Button( char* szControl, const char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, "MODERN_BUTTON_RED", UI_FLAG_XSIZE | UI_FLAG_YSIZE, cID );
	pControl->CreateImageBaseMulti( "MODERN_BUTTON_RED","MODERN_BUTTON_RED_BASE_IMAGE_LEFT", "MODERN_BUTTON_RED_BASE_IMAGE_CENTER", "MODERN_BUTTON_RED_BASE_IMAGE_RIGHT" );
	pControl->CreateImageOverMulti( "MODERN_BUTTON_RED","MODERN_BUTTON_RED_OVER_IMAGE_LEFT", "MODERN_BUTTON_RED_OVER_IMAGE_CENTER", "MODERN_BUTTON_RED_OVER_IMAGE_RIGHT" );
	pControl->CreateImageHoldMulti( "MODERN_BUTTON_RED","MODERN_BUTTON_RED_CLICK_IMAGE_LEFT", "MODERN_BUTTON_RED_CLICK_IMAGE_CENTER", "MODERN_BUTTON_RED_CLICK_IMAGE_RIGHT" );
	pControl->CreateTextBox( "MODERN_BUTTON_RED_TEXT_POS", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pControl->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pControl->AlignToControl( szControl );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::WHITE );
	
	RegisterControl ( pControl );

	return pControl;
}

void CModernCharacterStat::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CreateLineBox( "MODERN_CHARACTER_WINDOW_STAT_POINT1_REGION" );
	CreateLineBox( "MODERN_CHARACTER_WINDOW_STAT_POINT2_REGION" );
	CreateLineBox( "MODERN_CHARACTER_WINDOW_STAT_POINT3_REGION" );
	CreateLineBox( "MODERN_CHARACTER_WINDOW_STAT_POINT4_REGION" );
	CreateLineBox( "MODERN_CHARACTER_WINDOW_STAT_POINT5_REGION" );
	CreateLineBox( "MODERN_CHARACTER_WINDOW_STAT_POINT6_REGION" );
	CreateLineBox( "MODERN_CHARACTER_WINDOW_STAT_POINT7_REGION" );
	CreateLineBox( "MODERN_CHARACTER_WINDOW_STAT_POINT8_REGION" );
	CreateLineBox( "MODERN_CHARACTER_WINDOW_STAT_POINT9_REGION" );

	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_1_POW_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_POW_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_1_DEX_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_DEX_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_1_SPI_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_SPI_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_1_STR_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_STR_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_1_STA_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_STA_TOOLTIP") );

	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MIN_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ATTACK_MIN_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MAX_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ATTACK_MAX_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_DEFENSE_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_DEFENSE_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_FIGHT_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_FIGHT_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_SHOOT_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_SHOOT_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_STAMINA_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_STAMINA_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_HIT_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_HIT_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_AVOID_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_AVOID_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_HP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_HP_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_MP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_MP_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_2_SP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_SP_TOOLTIP") );

	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_3_FIRE_RESIST_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_FIRE_RESIST_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_3_COLD_RESIST_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_COLD_RESIST_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_3_ELECTRIC_RESIST_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ELECTRIC_RESIST_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_3_POISON_RESIST_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_POISON_RESIST_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTER_WINDOW_STAT_3_SPIRIT_RESIST_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_SPIRIT_RESIST_TOOLTIP") );

	m_pQuestionButton = new CBasicButton;
	m_pQuestionButton->CreateSub ( this, "MODERN_CHARACTER_WINDOW_STAT_QUESTION_BUTTON" , UI_FLAG_DEFAULT, MODERN_CHARACTER_WINDOW_STAT_BUTTON_QUESTION );
	m_pQuestionButton->CreateFlip ( "MODERN_CHARACTER_WINDOW_STAT_QUESTION_BUTTON_OVER", CBasicButton::MOUSEIN_FLIP );
	m_pQuestionButton->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pQuestionButton );


	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_POINT_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 0 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pPowTextStatic = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_POW_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 1 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pDexTextStatic = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_DEX_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 2 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pSpiTextStatic = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_SPI_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 3 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pStrTextStatic = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_STR_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 4 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pStaTextStatic = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_STA_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 5 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MIN_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 6 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MAX_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 7 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_DEFENSE_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 8 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_FIGHT_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 9 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_SHOOT_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 10 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_STAMINA_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 11 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_HIT_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 12 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_AVOID_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 13 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_HP_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 14 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_MP_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 15 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_SP_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 16 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_FIRE_RESIST_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 17 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_COLD_RESIST_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 18 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_ELECTRIC_RESIST_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 19 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_POISON_RESIST_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 20 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_SPIRIT_RESIST_TEXT", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_TEXT", 21 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );


	m_pPointValue = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_POINT_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );

	m_pPowTextValue = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_POW_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pDexTextValue = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_DEX_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pSpiTextValue = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_SPI_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pStrTextValue = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_STR_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pStaTextValue = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_STA_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	
	m_pPowTextMark = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_POW_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pDexTextMark = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_DEX_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pSpiTextMark = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_SPI_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pStrTextMark = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_STR_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pStaTextMark = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_STA_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );

	m_pPowTextUp = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_POW_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pDexTextUp = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_DEX_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pSpiTextUp = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_SPI_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pStrTextUp = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_STR_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pStaTextUp = CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_1_STA_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );


	
	m_pAttackMinTextValue	= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MIN_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pAttackMaxTextValue	= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MAX_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pDefenseTextValue		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_DEFENSE_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pFightTextValue		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_FIGHT_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pShootTextValue		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_SHOOT_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pEnergyTextValue		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_STAMINA_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pHitTextValue			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_HIT_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pAvoidTextValue		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_AVOID_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pHPTextValue			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_HP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pMPTextValue			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_MP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pSPTextValue			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_SP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );

	m_pAttackMinTextMark	= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MIN_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pAttackMaxTextMark	= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MAX_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pDefenseTextMark		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_DEFENSE_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pFightTextMark		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_FIGHT_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pShootTextMark		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_SHOOT_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pEnergyTextMark		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_STAMINA_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pHitTextMark			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_HIT_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pAvoidTextMark		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_AVOID_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pHPTextMark			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_HP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pMPTextMark			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_MP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pSPTextMark			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_SP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );

	m_pAttackMinTextUp		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MIN_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pAttackMaxTextUp		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MAX_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pDefenseTextUp		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_DEFENSE_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pFightTextUp			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_FIGHT_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pShootTextUp			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_SHOOT_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pEnergyTextUp			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_STAMINA_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pHitTextUp			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_HIT_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pAvoidTextUp			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_AVOID_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pHPTextUp				= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_HP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pMPTextUp				= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_MP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pSPTextUp				= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_2_SP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );


	
	m_pFireTextValue		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_FIRE_RESIST_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pColdTextValue		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_COLD_RESIST_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pElectricTextValue	= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_ELECTRIC_RESIST_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pPoisonTextValue		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_POISON_RESIST_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pSpiritTextValue		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_SPIRIT_RESIST_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );

	m_pFireTextMark			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_FIRE_RESIST_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pColdTextMark			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_COLD_RESIST_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pElectricTextMark		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_ELECTRIC_RESIST_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pPoisonTextMark		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_POISON_RESIST_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pSpiritTextMark		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_SPIRIT_RESIST_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	
	m_pFireTextUp			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_FIRE_RESIST_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pColdTextUp			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_COLD_RESIST_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pElectricTextUp		= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_ELECTRIC_RESIST_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pPoisonTextUp			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_POISON_RESIST_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pSpiritTextUp			= CreateTextBox( "MODERN_CHARACTER_WINDOW_STAT_3_SPIRIT_RESIST_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );

	m_pPowMarkImage			= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_1_POW_MARK_IMAGE" );
	m_pDexMarkImage			= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_1_DEX_MARK_IMAGE" );
	m_pSpiMarkImage			= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_1_SPI_MARK_IMAGE" );
	m_pStrMarkImage			= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_1_STR_MARK_IMAGE" );
	m_pStaMarkImage			= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_1_STA_MARK_IMAGE" );
	
	m_pAttackMinMarkImage	= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MIN_MARK_IMAGE" );
	m_pAttackMaxMarkImage	= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_ATTACK_MAX_MARK_IMAGE" );
	m_pDefenseMarkImage		= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_DEFENSE_MARK_IMAGE" );
	m_pFightMarkImage		= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_FIGHT_MARK_IMAGE" );
	m_pShootMarkImage		= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_SHOOT_MARK_IMAGE" );
	m_pEnergyMarkImage		= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_STAMINA_MARK_IMAGE" );
	m_pHitMarkImage			= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_HIT_MARK_IMAGE" );
	m_pAvoidMarkImage		= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_AVOID_MARK_IMAGE" );
	m_pHPMarkImage			= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_HP_MARK_IMAGE" );
	m_pMPMarkImage			= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_MP_MARK_IMAGE" );
	m_pSPMarkImage			= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_2_SP_MARK_IMAGE" );

	m_pFireMarkImage		= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_3_FIRE_RESIST_MARK_IMAGE" );
	m_pColdMarkImage		= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_3_COLD_RESIST_MARK_IMAGE" );
	m_pElectricMarkImage	= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_3_ELECTRIC_RESIST_MARK_IMAGE" );
	m_pPoisonMarkImage		= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_3_POISON_RESIST_MARK_IMAGE" );
	m_pSpiritMarkImage		= CreateStatMark( "MODERN_CHARACTER_WINDOW_STAT_3_SPIRIT_RESIST_MARK_IMAGE" );

	m_pPowButton_Plus = CreateStatButton( "MODERN_CHARACTER_WINDOW_STAT_POW_PLUS_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 0 ), MODERN_CHARACTER_WINDOW_POW_BUTTON_PLUS );
	m_pDexButton_Plus = CreateStatButton( "MODERN_CHARACTER_WINDOW_STAT_DEX_PLUS_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 0 ), MODERN_CHARACTER_WINDOW_DEX_BUTTON_PLUS );
	m_pSpiButton_Plus = CreateStatButton( "MODERN_CHARACTER_WINDOW_STAT_SPI_PLUS_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 0 ), MODERN_CHARACTER_WINDOW_SPI_BUTTON_PLUS );
	m_pStrButton_Plus = CreateStatButton( "MODERN_CHARACTER_WINDOW_STAT_STR_PLUS_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 0 ), MODERN_CHARACTER_WINDOW_STR_BUTTON_PLUS );
	m_pStaButton_Plus = CreateStatButton( "MODERN_CHARACTER_WINDOW_STAT_STA_PLUS_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 0 ), MODERN_CHARACTER_WINDOW_STA_BUTTON_PLUS );

	m_pPowButton_Minus = CreateStatButton( "MODERN_CHARACTER_WINDOW_STAT_POW_MINUS_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 1 ), MODERN_CHARACTER_WINDOW_POW_BUTTON_MINUS );
	m_pDexButton_Minus = CreateStatButton( "MODERN_CHARACTER_WINDOW_STAT_DEX_MINUS_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 1 ), MODERN_CHARACTER_WINDOW_DEX_BUTTON_MINUS );
	m_pSpiButton_Minus = CreateStatButton( "MODERN_CHARACTER_WINDOW_STAT_SPI_MINUS_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 1 ), MODERN_CHARACTER_WINDOW_SPI_BUTTON_MINUS );
	m_pStrButton_Minus = CreateStatButton( "MODERN_CHARACTER_WINDOW_STAT_STR_MINUS_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 1 ), MODERN_CHARACTER_WINDOW_STR_BUTTON_MINUS );
	m_pStaButton_Minus = CreateStatButton( "MODERN_CHARACTER_WINDOW_STAT_STA_MINUS_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 1 ), MODERN_CHARACTER_WINDOW_STA_BUTTON_MINUS );

	m_pButtonStatReset = CreateStat2Button( "MODERN_CHARACTER_WINDOW_STAT_INIT_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 2 ), MODERN_CHARACTER_WINDOW_STAT_RESET );
	m_pButtonStatApply = CreateStat2Button( "MODERN_CHARACTER_WINDOW_STAT_APPLY_BUTTON", ID2GAMEWORD( "MODERN_CHARACTER_WINDOW_STAT_BUTTON_TEXT", 3 ), MODERN_CHARACTER_WINDOW_STAT_APPLY );
}

void CModernCharacterStat::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if( m_pPowTextValue && !m_pPowTextValue->empty() )		m_pPowTextValue->SetUseTextColor ( 0, FALSE );
	if( m_pPowTextStatic && !m_pPowTextStatic->empty() )	m_pPowTextStatic->SetUseTextColor ( 0, FALSE );

	if( m_pDexTextValue && !m_pDexTextValue->empty() )		m_pDexTextValue->SetUseTextColor ( 0, FALSE );
	if( m_pDexTextStatic && !m_pDexTextStatic->empty() )	m_pDexTextStatic->SetUseTextColor ( 0, FALSE );

	if( m_pSpiTextValue && !m_pSpiTextValue->empty() )		m_pSpiTextValue->SetUseTextColor ( 0, FALSE );
	if( m_pSpiTextStatic && !m_pSpiTextStatic->empty() )	m_pSpiTextStatic->SetUseTextColor ( 0, FALSE );

	if( m_pStrTextValue && !m_pStrTextValue->empty() )		m_pStrTextValue->SetUseTextColor ( 0, FALSE );
	if( m_pStrTextStatic && !m_pStrTextStatic->empty() )	m_pStrTextStatic->SetUseTextColor ( 0, FALSE );

	if( m_pStaTextValue && !m_pStaTextValue->empty() )		m_pStaTextValue->SetUseTextColor ( 0, FALSE );
	if( m_pStaTextStatic && !m_pStaTextStatic->empty() )	m_pStaTextStatic->SetUseTextColor ( 0, FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernCharacterStat::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case MODERN_CHARACTER_WINDOW_POW_BUTTON_PLUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pPowTextValue && !m_pPowTextValue->empty() )
				{
					m_pPowTextValue->SetUseTextColor ( 0, TRUE );
					m_pPowTextValue->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD );
				}

				if ( m_pPowTextStatic && !m_pPowTextStatic->empty() )
				{
					m_pPowTextStatic->SetUseTextColor ( 0, TRUE );
					m_pPowTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) || CHECK_RB_UP_LIKE ( dwMsg ))
				{
					if ( m_bActionBlock )	return;

					m_wPowAdd++;
					m_wStatsDeduct++;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_DEX_BUTTON_PLUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pDexTextValue && !m_pDexTextValue->empty() )
				{
					m_pDexTextValue->SetUseTextColor ( 0, TRUE );
					m_pDexTextValue->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pDexTextStatic && !m_pDexTextStatic->empty() )
				{
					m_pDexTextStatic->SetUseTextColor ( 0, TRUE );
					m_pDexTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) || CHECK_RB_UP_LIKE ( dwMsg ))
				{
					if ( m_bActionBlock )	return;

					m_wDexAdd++;
					m_wStatsDeduct++;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_SPI_BUTTON_PLUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pSpiTextValue && !m_pSpiTextValue->empty() )
				{
					m_pSpiTextValue->SetUseTextColor ( 0, TRUE );
					m_pSpiTextValue->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pSpiTextStatic && !m_pSpiTextStatic->empty() )
				{
					m_pSpiTextStatic->SetUseTextColor ( 0, TRUE );
					m_pSpiTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) || CHECK_RB_UP_LIKE ( dwMsg ))
				{
					if ( m_bActionBlock )	return;

					m_wSpiAdd++;
					m_wStatsDeduct++;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_STR_BUTTON_PLUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pStrTextValue && !m_pStrTextValue->empty() )
				{
					m_pStrTextValue->SetUseTextColor ( 0, TRUE );
					m_pStrTextValue->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStrTextStatic && !m_pStrTextStatic->empty() )
				{
					m_pStrTextStatic->SetUseTextColor ( 0, TRUE );
					m_pStrTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) || CHECK_RB_UP_LIKE ( dwMsg ))
				{
					if ( m_bActionBlock )	return;

					m_wStrAdd++;
					m_wStatsDeduct++;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_STA_BUTTON_PLUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pStaTextValue && !m_pStaTextValue->empty() )
				{
					m_pStaTextValue->SetUseTextColor ( 0, TRUE );
					m_pStaTextValue->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStaTextStatic && !m_pStaTextStatic->empty() )
				{
					m_pStaTextStatic->SetUseTextColor ( 0, TRUE );
					m_pStaTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) || CHECK_RB_UP_LIKE ( dwMsg ))
				{
					if ( m_bActionBlock )	return;

					m_wStaAdd++;
					m_wStatsDeduct++;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_POW_BUTTON_MINUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pPowTextValue && !m_pPowTextValue->empty() )
				{
					m_pPowTextValue->SetUseTextColor ( 0, TRUE );
					m_pPowTextValue->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD );
				}

				if ( m_pPowTextStatic && !m_pPowTextStatic->empty() )
				{
					m_pPowTextStatic->SetUseTextColor ( 0, TRUE );
					m_pPowTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) || CHECK_RB_UP_LIKE ( dwMsg ))
				{
					if ( m_bActionBlock )	return;

					m_wPowAdd--;
					m_wStatsDeduct--;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_DEX_BUTTON_MINUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pDexTextValue && !m_pDexTextValue->empty() )
				{
					m_pDexTextValue->SetUseTextColor ( 0, TRUE );
					m_pDexTextValue->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pDexTextStatic && !m_pDexTextStatic->empty() )
				{
					m_pDexTextStatic->SetUseTextColor ( 0, TRUE );
					m_pDexTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) || CHECK_RB_UP_LIKE ( dwMsg ))
				{
					if ( m_bActionBlock )	return;

					m_wDexAdd--;
					m_wStatsDeduct--;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_SPI_BUTTON_MINUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pSpiTextValue && !m_pSpiTextValue->empty() )
				{
					m_pSpiTextValue->SetUseTextColor ( 0, TRUE );
					m_pSpiTextValue->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pSpiTextStatic && !m_pSpiTextStatic->empty() )
				{
					m_pSpiTextStatic->SetUseTextColor ( 0, TRUE );
					m_pSpiTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) || CHECK_RB_UP_LIKE ( dwMsg ))
				{
					if ( m_bActionBlock )	return;

					m_wSpiAdd--;
					m_wStatsDeduct--;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_STR_BUTTON_MINUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pStrTextValue && !m_pStrTextValue->empty() )
				{
					m_pStrTextValue->SetUseTextColor ( 0, TRUE );
					m_pStrTextValue->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStrTextStatic && !m_pStrTextStatic->empty() )
				{
					m_pStrTextStatic->SetUseTextColor ( 0, TRUE );
					m_pStrTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) || CHECK_RB_UP_LIKE ( dwMsg ))
				{
					if ( m_bActionBlock )	return;

					m_wStrAdd--;
					m_wStatsDeduct--;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_STA_BUTTON_MINUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pStaTextValue && !m_pStaTextValue->empty() )
				{
					m_pStaTextValue->SetUseTextColor ( 0, TRUE );
					m_pStaTextValue->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStaTextStatic && !m_pStaTextStatic->empty() )
				{
					m_pStaTextStatic->SetUseTextColor ( 0, TRUE );
					m_pStaTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) || CHECK_RB_UP_LIKE ( dwMsg ))
				{
					if ( m_bActionBlock )	return;

					m_wStaAdd--;
					m_wStatsDeduct--;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_STAT_RESET:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )				
				{
					if ( m_bActionBlock )	return;

					m_wPowAdd = 0;
					m_wDexAdd = 0;
					m_wSpiAdd = 0;
					m_wStrAdd = 0;
					m_wStaAdd = 0;
					m_wStatsDeduct = 0;

					m_bActionBlock = TRUE;
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_STAT_APPLY:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )				
				{
					if ( m_bActionBlock )	return;

					SCHARSTATS sStats;
					sStats.wPow = m_wPowAdd;
					sStats.wDex = m_wDexAdd;
					sStats.wSpi = m_wSpiAdd;
					sStats.wStr = m_wStrAdd;
					sStats.wSta = m_wStaAdd;
					GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUpModern( sStats );

					m_wPowAdd = 0;
					m_wDexAdd = 0;
					m_wSpiAdd = 0;
					m_wStrAdd = 0;
					m_wStaAdd = 0;
					m_wStatsDeduct = 0;

					m_bActionBlock = TRUE;
				}
			}
		}break;


	case MODERN_CHARACTER_WINDOW_STAT_BUTTON_QUESTION:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					CInnerInterface::GetInstance().InfoWindowOpen( GetWndID() );
					CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_POINT_QUESTION_BUTTON_SENTENCE_1" ), NS_UITEXTCOLOR::DARKLBUE );
					CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_POINT_QUESTION_BUTTON_SENTENCE_2" ), NS_UITEXTCOLOR::DARKLBUE );
				}
			}
		}break;


	
	}
}

void CModernCharacterStat::SetPointControl ( const GLCHARLOGIC_CLIENT& sCharData )
{
	BOOL bPointUsable = sCharData.m_wStatsPoint > 0;
	BOOL bResetOrApply = (m_wPowAdd>0) || (m_wDexAdd>0) || (m_wSpiAdd>0) || (m_wStrAdd>0) || (m_wStaAdd>0);

	/*BOOL bPow = m_sCharLogic.GETCHARDATA().m_sStats.wPow < GLCONST_CHAR::dwMaxPow;
	BOOL bStr = m_sCharLogic.GETCHARDATA().m_sStats.wStr < GLCONST_CHAR::dwMaxStr;
	BOOL bSpi = m_sCharLogic.GETCHARDATA().m_sStats.wSpi < GLCONST_CHAR::dwMaxSpi;
	BOOL bDex = m_sCharLogic.GETCHARDATA().m_sStats.wDex < GLCONST_CHAR::dwMaxDex;
	BOOL bSta = m_sCharLogic.GETCHARDATA().m_sStats.wSta < GLCONST_CHAR::dwMaxSta;*/

	if ( m_pPowButton_Plus ) m_pPowButton_Plus->SetVisibleSingle ( bPointUsable /*&& bPow*/ );
	if ( m_pDexButton_Plus ) m_pDexButton_Plus->SetVisibleSingle ( bPointUsable /*&& bDex*/ );
	if ( m_pSpiButton_Plus ) m_pSpiButton_Plus->SetVisibleSingle ( bPointUsable /*&& bSpi*/ );
	if ( m_pStrButton_Plus ) m_pStrButton_Plus->SetVisibleSingle ( bPointUsable /*&& bStr*/ );
	if ( m_pStaButton_Plus ) m_pStaButton_Plus->SetVisibleSingle ( bPointUsable /*&& bSta*/ );

	if ( m_pPowButton_Minus ) m_pPowButton_Minus->SetVisibleSingle ( m_wPowAdd !=0 );
	if ( m_pDexButton_Minus ) m_pDexButton_Minus->SetVisibleSingle ( m_wDexAdd !=0 );
	if ( m_pSpiButton_Minus ) m_pSpiButton_Minus->SetVisibleSingle ( m_wSpiAdd !=0 );
	if ( m_pStrButton_Minus ) m_pStrButton_Minus->SetVisibleSingle ( m_wStrAdd !=0 );
	if ( m_pStaButton_Minus ) m_pStaButton_Minus->SetVisibleSingle ( m_wStaAdd !=0 );

	if ( m_pButtonStatReset ) m_pButtonStatReset->SetVisibleSingle ( bResetOrApply );
	if ( m_pButtonStatApply ) m_pButtonStatApply->SetVisibleSingle ( bResetOrApply );
}

void CModernCharacterStat::SetData( const GLCHARLOGIC_CLIENT& sCharData, const GLCHARLOGIC_CLIENT& sCharData2 )
{
	m_pPointValue->ClearText();

	m_pPowTextValue->ClearText();
	m_pDexTextValue->ClearText();
	m_pSpiTextValue->ClearText();
	m_pStrTextValue->ClearText();
	m_pStaTextValue->ClearText();

	m_pAttackMinTextValue->ClearText();
	m_pAttackMaxTextValue->ClearText();
	m_pDefenseTextValue->ClearText();
	m_pFightTextValue->ClearText();
	m_pShootTextValue->ClearText();
	m_pEnergyTextValue->ClearText();
	m_pHitTextValue->ClearText();
	m_pAvoidTextValue->ClearText();
	m_pHPTextValue->ClearText();
	m_pMPTextValue->ClearText();
	m_pSPTextValue->ClearText();

	m_pFireTextValue->ClearText();
	m_pColdTextValue->ClearText();
	m_pElectricTextValue->ClearText();
	m_pPoisonTextValue->ClearText();
	m_pSpiritTextValue->ClearText();

	m_pAttackMinTextMark->ClearText();
	m_pAttackMaxTextMark->ClearText();
	m_pDefenseTextMark->ClearText();
	m_pFightTextMark->ClearText();
	m_pShootTextMark->ClearText();
	m_pEnergyTextMark->ClearText();
	m_pHitTextMark->ClearText();
	m_pAvoidTextMark->ClearText();
	m_pHPTextMark->ClearText();
	m_pMPTextMark->ClearText();
	m_pSPTextMark->ClearText();

	m_pAttackMinTextUp->ClearText();
	m_pAttackMaxTextUp->ClearText();
	m_pDefenseTextUp->ClearText();
	m_pFightTextUp->ClearText();
	m_pShootTextUp->ClearText();
	m_pEnergyTextUp->ClearText();
	m_pHitTextUp->ClearText();
	m_pAvoidTextUp->ClearText();
	m_pHPTextUp->ClearText();
	m_pMPTextUp->ClearText();
	m_pSPTextUp->ClearText();

	m_pAttackMinMarkImage->SetVisibleSingle ( FALSE );
	m_pAttackMaxMarkImage->SetVisibleSingle ( FALSE );
	m_pDefenseMarkImage->SetVisibleSingle ( FALSE );
	m_pFightMarkImage->SetVisibleSingle ( FALSE );
	m_pShootMarkImage->SetVisibleSingle ( FALSE );
	m_pEnergyMarkImage->SetVisibleSingle ( FALSE );
	m_pHitMarkImage->SetVisibleSingle ( FALSE );
	m_pAvoidMarkImage->SetVisibleSingle ( FALSE );
	m_pHPMarkImage->SetVisibleSingle ( FALSE );
	m_pMPMarkImage->SetVisibleSingle ( FALSE );
	m_pSPMarkImage->SetVisibleSingle ( FALSE );

	m_pPowTextMark->ClearText();
	m_pDexTextMark->ClearText();
	m_pSpiTextMark->ClearText();
	m_pStrTextMark->ClearText();
	m_pStaTextMark->ClearText();

	m_pPowMarkImage->SetVisibleSingle ( FALSE );
	m_pDexMarkImage->SetVisibleSingle ( FALSE );
	m_pSpiMarkImage->SetVisibleSingle ( FALSE );
	m_pStrMarkImage->SetVisibleSingle ( FALSE );
	m_pStaMarkImage->SetVisibleSingle ( FALSE );

	m_pPowTextUp->ClearText();
	m_pDexTextUp->ClearText();
	m_pSpiTextUp->ClearText();
	m_pStrTextUp->ClearText();
	m_pStaTextUp->ClearText();

	m_pFireTextMark->ClearText();
	m_pColdTextMark->ClearText();
	m_pElectricTextMark->ClearText();
	m_pPoisonTextMark->ClearText();
	m_pSpiritTextMark->ClearText();

	m_pFireTextUp->ClearText();
	m_pColdTextUp->ClearText();
	m_pElectricTextUp->ClearText();
	m_pPoisonTextUp->ClearText();
	m_pSpiritTextUp->ClearText();

	m_pFireMarkImage->SetVisibleSingle ( FALSE );
	m_pColdMarkImage->SetVisibleSingle ( FALSE );
	m_pElectricMarkImage->SetVisibleSingle ( FALSE );
	m_pPoisonMarkImage->SetVisibleSingle ( FALSE );
	m_pSpiritMarkImage->SetVisibleSingle ( FALSE );

	m_pPointValue->AddText( _HLIB::cstring_dword(sCharData2.m_wStatsPoint).GetString(), NS_UITEXTCOLOR::GOLD );

	m_pPowTextValue->AddText( _HLIB::cstring_dword(sCharData.m_sSUMSTATS.wPow).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pDexTextValue->AddText( _HLIB::cstring_dword(sCharData.m_sSUMSTATS.wDex).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pSpiTextValue->AddText( _HLIB::cstring_dword(sCharData.m_sSUMSTATS.wSpi).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pStrTextValue->AddText( _HLIB::cstring_dword(sCharData.m_sSUMSTATS.wStr).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pStaTextValue->AddText( _HLIB::cstring_dword(sCharData.m_sSUMSTATS.wSta).GetString(), NS_UITEXTCOLOR::WHITE );

	m_pAttackMinTextValue->AddText( _HLIB::cstring_int(sCharData.GETFORCE_LOW()).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pAttackMaxTextValue->AddText( _HLIB::cstring_int(sCharData.GETFORCE_HIGH()).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pDefenseTextValue->AddText( _HLIB::cstring_int(sCharData.GETDEFENSE()).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pFightTextValue->AddText( _HLIB::cstring_dword(sCharData.m_wSUM_PA).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pShootTextValue->AddText( _HLIB::cstring_dword(sCharData.m_wSUM_SA).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pEnergyTextValue->AddText( _HLIB::cstring_dword(sCharData.m_wSUM_MA).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pHitTextValue->AddText( _HLIB::cstring_int(sCharData.GETHIT()).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pAvoidTextValue->AddText( _HLIB::cstring_int(sCharData.GETAVOID()).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pHPTextValue->AddText( _HLIB::cstring_dword(sCharData.GETMAXHP()).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pMPTextValue->AddText( _HLIB::cstring_dword(sCharData.m_sMP.wMax).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pSPTextValue->AddText( _HLIB::cstring_dword(sCharData.m_sSP.wMax).GetString(), NS_UITEXTCOLOR::WHITE );

	m_pFireTextValue->AddText( _HLIB::cstring_int(sCharData.m_sSUMRESIST_SKILL.nFire).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pColdTextValue->AddText( _HLIB::cstring_int(sCharData.m_sSUMRESIST_SKILL.nIce).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pElectricTextValue->AddText( _HLIB::cstring_int(sCharData.m_sSUMRESIST_SKILL.nElectric).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pPoisonTextValue->AddText( _HLIB::cstring_int(sCharData.m_sSUMRESIST_SKILL.nPoison).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pSpiritTextValue->AddText( _HLIB::cstring_int(sCharData.m_sSUMRESIST_SKILL.nSpirit).GetString(), NS_UITEXTCOLOR::WHITE );

	int nPow = sCharData2.m_sSUMSTATS.wPow - sCharData.m_sSUMSTATS.wPow;
	int nDex = sCharData2.m_sSUMSTATS.wDex - sCharData.m_sSUMSTATS.wDex;
	int nInt = sCharData2.m_sSUMSTATS.wSpi - sCharData.m_sSUMSTATS.wSpi;
	int nStr = sCharData2.m_sSUMSTATS.wStr - sCharData.m_sSUMSTATS.wStr;
	int nSta = sCharData2.m_sSUMSTATS.wSta - sCharData.m_sSUMSTATS.wSta;

	int nAttackMin = sCharData2.GETFORCE_LOW() - sCharData.GETFORCE_LOW ();
	int nAttackMax = sCharData2.GETFORCE_HIGH() - sCharData.GETFORCE_HIGH();
	int nDefense = sCharData2.GETDEFENSE() - sCharData.GETDEFENSE();
	int nFight = sCharData2.m_wSUM_PA - sCharData.m_wSUM_PA;
	int nShoot = sCharData2.m_wSUM_SA - sCharData.m_wSUM_SA;
	int nEnergy = sCharData2.m_wSUM_MA - sCharData.m_wSUM_MA;
	int nHit = sCharData2.GETHIT() - sCharData.GETHIT();
	int nAvoid = sCharData2.GETAVOID() - sCharData.GETAVOID();
	int nHP = sCharData2.m_sHP.wMax - sCharData.m_sHP.wMax;
	int nMP = sCharData2.m_sMP.wMax - sCharData.m_sMP.wMax;
	int nSP = sCharData2.m_sSP.wMax - sCharData.m_sSP.wMax;

	int nResistFire = sCharData2.m_sSUMRESIST_SKILL.nFire - sCharData.m_sSUMRESIST_SKILL.nFire;
	int nResistCold = sCharData2.m_sSUMRESIST_SKILL.nIce - sCharData.m_sSUMRESIST_SKILL.nIce;
	int nResistElect = sCharData2.m_sSUMRESIST_SKILL.nElectric - sCharData.m_sSUMRESIST_SKILL.nElectric;
	int nResistPoison = sCharData2.m_sSUMRESIST_SKILL.nPoison - sCharData.m_sSUMRESIST_SKILL.nPoison;
	int nResistSpirit = sCharData2.m_sSUMRESIST_SKILL.nSpirit - sCharData.m_sSUMRESIST_SKILL.nSpirit;

	if ( nPow == 0 )	
	{
		m_pPowTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pPowMarkImage->SetVisibleSingle ( TRUE );
		m_pPowMarkImage->CheckValue( nPow );
	}

	if ( nDex == 0 )	
	{
		m_pDexTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pDexMarkImage->SetVisibleSingle ( TRUE );
		m_pDexMarkImage->CheckValue( nDex );
	}

	if ( nInt == 0 )	
	{
		m_pSpiTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pSpiMarkImage->SetVisibleSingle ( TRUE );
		m_pSpiMarkImage->CheckValue( nInt );
	}

	if ( nStr == 0 )	
	{
		m_pStrTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pStrMarkImage->SetVisibleSingle ( TRUE );
		m_pStrMarkImage->CheckValue( nStr );
	}

	if ( nSta == 0 )	
	{
		m_pStaTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pStaMarkImage->SetVisibleSingle ( TRUE );
		m_pStaMarkImage->CheckValue( nSta );
	}

	
	if ( nAttackMin == 0 )	
	{
		m_pAttackMinTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pAttackMinMarkImage->SetVisibleSingle ( TRUE );
		m_pAttackMinMarkImage->CheckValue( nAttackMin );
	}

	if ( nAttackMax == 0 ) 
	{
		m_pAttackMaxTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pAttackMaxMarkImage->SetVisibleSingle ( TRUE );
		m_pAttackMaxMarkImage->CheckValue( nAttackMax );
	}

	if ( nDefense == 0 )	
	{
		m_pDefenseTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pDefenseMarkImage->SetVisibleSingle ( TRUE );
		m_pDefenseMarkImage->CheckValue( nDefense );
	}

	if ( nFight == 0 )		
	{
		m_pFightTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pFightMarkImage->SetVisibleSingle ( TRUE );
		m_pFightMarkImage->CheckValue( nFight );
	}

	if ( nShoot == 0 )		
	{
		m_pShootTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pShootMarkImage->SetVisibleSingle ( TRUE );
		m_pShootMarkImage->CheckValue( nShoot );
	}

	if ( nEnergy == 0 )	
	{
		m_pEnergyTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pEnergyMarkImage->SetVisibleSingle ( TRUE );
		m_pEnergyMarkImage->CheckValue( nEnergy );
	}

	if ( nHit == 0 )		
	{
		m_pHitTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pHitMarkImage->SetVisibleSingle ( TRUE );
		m_pHitMarkImage->CheckValue( nHit );
	}

	if ( nAvoid == 0 )		
	{
		m_pAvoidTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pAvoidMarkImage->SetVisibleSingle ( TRUE );
		m_pAvoidMarkImage->CheckValue( nAvoid );
	}

	if ( nHP == 0 )		
	{
		m_pHPTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pHPMarkImage->SetVisibleSingle ( TRUE );
		m_pHPMarkImage->CheckValue( nHP );
	}

	if ( nMP == 0 )		
	{
		m_pMPTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pMPMarkImage->SetVisibleSingle ( TRUE );
		m_pMPMarkImage->CheckValue( nMP );
	}

	if ( nSP == 0 )		
	{
		m_pSPTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pSPMarkImage->SetVisibleSingle ( TRUE );
		m_pSPMarkImage->CheckValue( nSP );
	}

	if ( nResistFire == 0 )		
	{
		m_pFireTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pFireMarkImage->SetVisibleSingle ( TRUE );
		m_pFireMarkImage->CheckValue( nResistFire );
	}

	if ( nResistCold == 0 )		
	{
		m_pColdTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pColdMarkImage->SetVisibleSingle ( TRUE );
		m_pColdMarkImage->CheckValue( nResistCold );
	}

	if ( nResistElect == 0 )		
	{
		m_pElectricTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pElectricMarkImage->SetVisibleSingle ( TRUE );
		m_pElectricMarkImage->CheckValue( nResistElect );
	}

	if ( nResistPoison == 0 )		
	{
		m_pPoisonTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pPoisonMarkImage->SetVisibleSingle ( TRUE );
		m_pPoisonMarkImage->CheckValue( nResistPoison );
	}

	if ( nResistSpirit == 0 )		
	{
		m_pSpiritTextMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}else{
		m_pSpiritMarkImage->SetVisibleSingle ( TRUE );
		m_pSpiritMarkImage->CheckValue( nResistSpirit );
	}

	m_pPowTextUp->AddText( _HLIB::cstring_int(nPow).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nPow) );
	m_pDexTextUp->AddText( _HLIB::cstring_int(nDex).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nDex) );
	m_pSpiTextUp->AddText( _HLIB::cstring_int(nInt).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nInt) );
	m_pStrTextUp->AddText( _HLIB::cstring_int(nStr).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nStr) );
	m_pStaTextUp->AddText( _HLIB::cstring_int(nSta).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nSta) );

	m_pAttackMinTextUp->AddText( _HLIB::cstring_int(nAttackMin).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nAttackMin) );
	m_pAttackMaxTextUp->AddText( _HLIB::cstring_int(nAttackMax).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nAttackMax) );
	m_pDefenseTextUp->AddText( _HLIB::cstring_int(nDefense).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nDefense) );
	m_pFightTextUp->AddText( _HLIB::cstring_int(nFight).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nFight) );
	m_pShootTextUp->AddText( _HLIB::cstring_int(nShoot).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nShoot) );
	m_pEnergyTextUp->AddText( _HLIB::cstring_int(nEnergy).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nEnergy) );
	m_pHitTextUp->AddText( _HLIB::cstring_int(nHit).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nHit) );
	m_pAvoidTextUp->AddText( _HLIB::cstring_int(nAvoid).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nAvoid) );
	m_pHPTextUp->AddText( _HLIB::cstring_int(nHP).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nHP) );
	m_pMPTextUp->AddText( _HLIB::cstring_int(nMP).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nMP) );
	m_pSPTextUp->AddText( _HLIB::cstring_int(nSP).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nSP) );

	m_pFireTextUp->AddText( _HLIB::cstring_int(nResistFire).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nResistFire) );
	m_pColdTextUp->AddText( _HLIB::cstring_int(nResistCold).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nResistCold) );
	m_pElectricTextUp->AddText( _HLIB::cstring_int(nResistElect).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nResistElect) );
	m_pPoisonTextUp->AddText( _HLIB::cstring_int(nResistPoison).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nResistPoison) );
	m_pSpiritTextUp->AddText( _HLIB::cstring_int(nResistSpirit).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare(nResistSpirit) );
}
