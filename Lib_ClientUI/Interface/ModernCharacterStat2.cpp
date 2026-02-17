#include "StdAfx.h"
#include "./ModernCharacterStat2.h"
#include "./ModernCharacterStatMark.h"
#include "./ModernCharacterStatOver.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"
#include "../../Lib_Helper/HLibDataConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernCharacterStat2::CModernCharacterStat2 ()
	: m_pQuestionButton(NULL)
	, m_pTextCriticalHitChance( NULL )
	, m_pTextCriticalHitDamage( NULL )
	, m_pTextCrushingBlowChance( NULL )
	, m_pTextCrushingBlowDamage( NULL )
	, m_pTextMovementSpeed( NULL )
	, m_pTextAttackSpeed( NULL )
	, m_pTextExpMultiple( NULL )
	, m_pTextIncPerHP( NULL )
	, m_pTextIncPerMP( NULL )
	, m_pTextIncPerSP( NULL )
	, m_pTextIncPerCP( NULL )
	, m_pTextIncPotionHP( NULL )
	, m_pTextIncPotionMP( NULL )
	, m_pTextIncPotionSP( NULL )
	, m_pTextIncPotionCP( NULL )

	, m_pTextCriticalHitChanceMark( NULL )
	, m_pTextCriticalHitDamageMark( NULL )
	, m_pTextCrushingBlowChanceMark( NULL )
	, m_pTextCrushingBlowDamageMark( NULL )
	, m_pTextMovementSpeedMark( NULL )
	, m_pTextAttackSpeedMark( NULL )
	, m_pTextExpMultipleMark( NULL )
	, m_pTextIncPerHPMark( NULL )
	, m_pTextIncPerMPMark( NULL )
	, m_pTextIncPerSPMark( NULL )
	, m_pTextIncPerCPMark( NULL )
	, m_pTextIncPotionHPMark( NULL )
	, m_pTextIncPotionMPMark( NULL )
	, m_pTextIncPotionSPMark( NULL )
	, m_pTextIncPotionCPMark( NULL )

	, m_pTextCriticalHitChanceUP( NULL )
	, m_pTextCriticalHitDamageUP( NULL )
	, m_pTextCrushingBlowChanceUP( NULL )
	, m_pTextCrushingBlowDamageUP( NULL )
	, m_pTextMovementSpeedUP( NULL )
	, m_pTextAttackSpeedUP( NULL )
	, m_pTextExpMultipleUP( NULL )
	, m_pTextIncPerHPUP( NULL )
	, m_pTextIncPerMPUP( NULL )
	, m_pTextIncPerSPUP( NULL )
	, m_pTextIncPerCPUP( NULL )
	, m_pTextIncPotionHPUP( NULL )
	, m_pTextIncPotionMPUP( NULL )
	, m_pTextIncPotionSPUP( NULL )
	, m_pTextIncPotionCPUP( NULL )

	, m_pTextCriticalHitChanceMarkImage( NULL )
	, m_pTextCriticalHitDamageMarkImage( NULL )
	, m_pTextCrushingBlowChanceMarkImage( NULL )
	, m_pTextCrushingBlowDamageMarkImage( NULL )
	, m_pTextMovementSpeedMarkImage( NULL )
	, m_pTextAttackSpeedMarkImage( NULL )
	, m_pTextExpMultipleMarkImage( NULL )
	, m_pTextIncPerHPMarkImage( NULL )
	, m_pTextIncPerMPMarkImage( NULL )
	, m_pTextIncPerSPMarkImage( NULL )
	, m_pTextIncPerCPMarkImage( NULL )
	, m_pTextIncPotionHPMarkImage( NULL )
	, m_pTextIncPotionMPMarkImage( NULL )
	, m_pTextIncPotionSPMarkImage( NULL )
	, m_pTextIncPotionCPMarkImage( NULL )
	
{
}

CModernCharacterStat2::~CModernCharacterStat2 ()
{
}

CBasicLineBox* CModernCharacterStat2::CreateLineBox ( char* szKeyword, const UIGUID& cID /*= NO_ID*/ )
{
	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE, cID );
	pBasicLineBox->CreateBaseBoxQuestList( szKeyword );
	RegisterControl ( pBasicLineBox );

	return pBasicLineBox;
}

CBasicTextBox* CModernCharacterStat2::CreateTextBox ( char* szKeyword, const char* szText, DWORD dwColor, CD3DFontPar* pFont, int nAlign, const UIGUID& cID /*= NO_ID*/ )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	pStaticText->AddText( szText, dwColor );
	RegisterControl ( pStaticText );

	return pStaticText;
}

CModernCharacterStatMark* CModernCharacterStat2::CreateStatMark ( char* szKeyword, const UIGUID& cID /*= NO_ID*/ )
{
	CModernCharacterStatMark* pControl = new CModernCharacterStatMark;
	pControl->CreateSub ( this, szKeyword, UI_FLAG_DEFAULT, cID );
	pControl->CreateSubControl();
	RegisterControl ( pControl );

	return pControl;
}

CModernCharacterStatOver* CModernCharacterStat2::CreateStatOver ( char* szKeyword, const char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CModernCharacterStatOver* pControl = new CModernCharacterStatOver;
	pControl->CreateSub ( this, szKeyword, UI_FLAG_DEFAULT );
	pControl->CreateSubControl( szText );
	RegisterControl ( pControl );

	return pControl;
}

void CModernCharacterStat2::CreateSubControl ()
{
	CreateLineBox( "MODERN_CHARACTOR_ADDITIONAL_ABILITY_POINT0_REGION" );
	CreateLineBox( "MODERN_CHARACTOR_ADDITIONAL_ABILITY_POINT1_REGION" );
	CreateLineBox( "MODERN_CHARACTOR_ADDITIONAL_ABILITY_POINT2_REGION" );

	m_pQuestionButton = new CBasicButton;
	m_pQuestionButton->CreateSub ( this, "MODERN_CHARACTER_ADDITION_WINDOW_STAT_QUESTION_BUTTON" , UI_FLAG_DEFAULT, MODERN_CHARACTER_WINDOW_STAT2_BUTTON_QUESTION );
	m_pQuestionButton->CreateFlip ( "MODERN_CHARACTER_ADDITION_WINDOW_STAT_QUESTION_BUTTON_OVER", CBasicButton::MOUSEIN_FLIP );
	m_pQuestionButton->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pQuestionButton );

	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_CRITICAL_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_DAMAGE_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_CRITICAL_DAMAGE_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_BLOW_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_CRUSHING_BLOW_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_DAMAGE_BLOW_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_CRUSHING_DAMAGE_BLOW_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_MOVE_SPEED_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_MOVE_SPEED_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_ATK_SPEED_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_ATK_SPEED_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_EXP_MULTIPLE_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_EXP_MULTIPLE_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_HP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_INC_PER_HP_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_MP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_INC_PER_MP_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_SP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_INC_PER_SP_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_CP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_INC_PER_CP_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_HP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_INC_INT_HP_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_MP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_INC_INT_MP_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_SP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_INC_INT_SP_TOOLTIP") );
	CreateStatOver( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_CP_MOUSEOVER_RECT", ID2GAMEINTEXT("MODERN_CHARACTER_WINDOW_ADDITION_INC_INT_CP_TOOLTIP") );

	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 0 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_DAMAGE_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT3_TEXT", 0 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_BLOW_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 1 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_DAMAGE_BLOW_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT3_TEXT", 1 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_MOVE_SPEED_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 2 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_ATK_SPEED_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 3 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_EXP_MULTIPLE_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 4 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_HP_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 5 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_MP_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 6 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_SP_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 7 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_CP_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 8 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_HP_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 9 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_MP_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 10 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_SP_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 11 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_CP_TEXT", ID2GAMEWORD("MODERN_CHARACTER_WINDOW_STAT2_TEXT", 12 ), NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );

	m_pTextCriticalHitChance = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextCriticalHitDamage = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_DAMAGE_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextCrushingBlowChance = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_BLOW_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextCrushingBlowDamage = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_DAMAGE_BLOW_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextMovementSpeed = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_MOVE_SPEED_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextAttackSpeed = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_ATK_SPEED_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextExpMultiple = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_EXP_MULTIPLE_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerHP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_HP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerMP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_MP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerSP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_SP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerCP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_CP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionHP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_HP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionMP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_MP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionSP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_SP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionCP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_CP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );

	m_pTextCriticalHitChanceMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextCriticalHitDamageMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_DAMAGE_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextCrushingBlowChanceMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_BLOW_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextCrushingBlowDamageMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_DAMAGE_BLOW_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextMovementSpeedMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_MOVE_SPEED_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextAttackSpeedMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_ATK_SPEED_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextExpMultipleMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_EXP_MULTIPLE_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerHPMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_HP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerMPMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_MP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerSPMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_SP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerCPMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_CP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionHPMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_HP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionMPMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_MP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionSPMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_SP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionCPMark = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_CP_MARK", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );

	m_pTextCriticalHitChanceUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextCriticalHitDamageUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_DAMAGE_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextCrushingBlowChanceUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_BLOW_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextCrushingBlowDamageUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_DAMAGE_BLOW_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextMovementSpeedUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_MOVE_SPEED_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextAttackSpeedUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_ATK_SPEED_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextExpMultipleUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_EXP_MULTIPLE_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerHPUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_HP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerMPUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_MP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerSPUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_SP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPerCPUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_CP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionHPUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_HP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionMPUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_MP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionSPUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_SP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextIncPotionCPUP = CreateTextBox ( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_CP_UP_VALUE", "--", NS_UITEXTCOLOR::WHITE, pFont8, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );

	m_pTextCriticalHitChanceMarkImage	= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_MARK_IMAGE" );
	m_pTextCriticalHitDamageMarkImage	= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRITICAL_DAMAGE_MARK_IMAGE" );
	m_pTextCrushingBlowChanceMarkImage	= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_BLOW_MARK_IMAGE" );
	m_pTextCrushingBlowDamageMarkImage	= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_CRUSHING_BLOW_DAMAGE_MARK_IMAGE" );
	m_pTextMovementSpeedMarkImage	= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_MOVE_SPEED_MARK_IMAGE" );
	m_pTextAttackSpeedMarkImage		= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_ATK_SPEED_MARK_IMAGE" );
	m_pTextExpMultipleMarkImage		= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_EXP_MULTIPLE_MARK_IMAGE" );
	m_pTextIncPerHPMarkImage		= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_HP_MARK_IMAGE" );
	m_pTextIncPerMPMarkImage		= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_MP_MARK_IMAGE" );
	m_pTextIncPerSPMarkImage		= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_SP_MARK_IMAGE" );
	m_pTextIncPerCPMarkImage		= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_PER_CP_MARK_IMAGE" );
	m_pTextIncPotionHPMarkImage		= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_HP_MARK_IMAGE" );
	m_pTextIncPotionMPMarkImage		= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_MP_MARK_IMAGE" );
	m_pTextIncPotionSPMarkImage		= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_SP_MARK_IMAGE" );
	m_pTextIncPotionCPMarkImage		= CreateStatMark( "MODERN_CHARACTOR_ADDITIONAL_STATE_INC_INT_CP_MARK_IMAGE" );
}

void CModernCharacterStat2::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernCharacterStat2::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case MODERN_CHARACTER_WINDOW_STAT2_BUTTON_QUESTION:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					CInnerInterface::GetInstance().InfoWindowOpen( GetWndID() );
					CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_ADDITION_WINDOW_POINT_QUESTION_BUTTON_SENTENCE_1" ), NS_UITEXTCOLOR::DARKLBUE );
				}
			}
		}break;
	}
}

void CModernCharacterStat2::SetData( const GLCHARLOGIC_CLIENT& sCharData, const GLCHARLOGIC_CLIENT& sCharData2 )
{

	m_pTextCriticalHitChance->ClearText();
	m_pTextCriticalHitDamage->ClearText();
	m_pTextCrushingBlowChance->ClearText();
	m_pTextCrushingBlowDamage->ClearText();
	m_pTextMovementSpeed->ClearText();
	m_pTextAttackSpeed->ClearText();
	m_pTextExpMultiple->ClearText();
	m_pTextIncPerHP->ClearText();
	m_pTextIncPerMP->ClearText();
	m_pTextIncPerSP->ClearText();
	m_pTextIncPerCP->ClearText();
	m_pTextIncPotionHP->ClearText();
	m_pTextIncPotionMP->ClearText();
	m_pTextIncPotionSP->ClearText();
	m_pTextIncPotionCP->ClearText();

	m_pTextCriticalHitChanceMark->ClearText();
	m_pTextCriticalHitDamageMark->ClearText();
	m_pTextCrushingBlowChanceMark->ClearText();
	m_pTextCrushingBlowDamageMark->ClearText();
	m_pTextMovementSpeedMark->ClearText();
	m_pTextAttackSpeedMark->ClearText();
	m_pTextExpMultipleMark->ClearText();
	m_pTextIncPerHPMark->ClearText();
	m_pTextIncPerMPMark->ClearText();
	m_pTextIncPerSPMark->ClearText();
	m_pTextIncPerCPMark->ClearText();
	m_pTextIncPotionHPMark->ClearText();
	m_pTextIncPotionMPMark->ClearText();
	m_pTextIncPotionSPMark->ClearText();
	m_pTextIncPotionCPMark->ClearText();

	m_pTextCriticalHitChanceUP->ClearText();
	m_pTextCriticalHitDamageUP->ClearText();
	m_pTextCrushingBlowChanceUP->ClearText();
	m_pTextCrushingBlowDamageUP->ClearText();
	m_pTextMovementSpeedUP->ClearText();
	m_pTextAttackSpeedUP->ClearText();
	m_pTextExpMultipleUP->ClearText();
	m_pTextIncPerHPUP->ClearText();
	m_pTextIncPerMPUP->ClearText();
	m_pTextIncPerSPUP->ClearText();
	m_pTextIncPerCPUP->ClearText();
	m_pTextIncPotionHPUP->ClearText();
	m_pTextIncPotionMPUP->ClearText();
	m_pTextIncPotionSPUP->ClearText();
	m_pTextIncPotionCPUP->ClearText();

	m_pTextCriticalHitChanceMarkImage->SetVisibleSingle ( FALSE );
	m_pTextCriticalHitDamageMarkImage->SetVisibleSingle ( FALSE );
	m_pTextCrushingBlowChanceMarkImage->SetVisibleSingle ( FALSE );
	m_pTextCrushingBlowDamageMarkImage->SetVisibleSingle ( FALSE );
	m_pTextMovementSpeedMarkImage->SetVisibleSingle ( FALSE );
	m_pTextAttackSpeedMarkImage->SetVisibleSingle ( FALSE );
	m_pTextExpMultipleMarkImage->SetVisibleSingle ( FALSE );
	m_pTextIncPerHPMarkImage->SetVisibleSingle ( FALSE );
	m_pTextIncPerMPMarkImage->SetVisibleSingle ( FALSE );
	m_pTextIncPerSPMarkImage->SetVisibleSingle ( FALSE );
	m_pTextIncPerCPMarkImage->SetVisibleSingle ( FALSE );
	m_pTextIncPotionHPMarkImage->SetVisibleSingle ( FALSE );
	m_pTextIncPotionMPMarkImage->SetVisibleSingle ( FALSE );
	m_pTextIncPotionSPMarkImage->SetVisibleSingle ( FALSE );
	m_pTextIncPotionCPMarkImage->SetVisibleSingle ( FALSE );

	m_pTextCriticalHitChance->AddText( _HLIB::cstringformat( "%2.f %%", sCharData.get_critical_hit_chance() ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextCriticalHitDamage->AddText( _HLIB::cstringformat( "%2.f %%", sCharData.get_critical_hit_damage() ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextCrushingBlowChance->AddText( _HLIB::cstringformat( "%2.f %%", sCharData.get_crushing_blow_chance() ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextCrushingBlowDamage->AddText( _HLIB::cstringformat( "%2.f %%", sCharData.get_crushing_blow_damage() ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextMovementSpeed->AddText( _HLIB::cstringformat( "%2.f %%", (sCharData.GET_MOVE_VELO() - 1.0f)*100.0f ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextAttackSpeed->AddText( _HLIB::cstringformat( "%2.f %%", (sCharData.GET_ATT_VELO() - 1.0f)*100.0f ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextExpMultiple->AddText( _HLIB::cstringformat( "%2.f %%", (sCharData.GET_EXP_TOTAL() - 1.0f)*100.0f ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextIncPerHP->AddText( _HLIB::cstringformat( "%2.f %%", sCharData.GET_HP_INCR()*100.0f ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextIncPerMP->AddText( _HLIB::cstringformat( "%2.f %%", sCharData.GET_MP_INCR()*100.0f ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextIncPerSP->AddText( _HLIB::cstringformat( "%2.f %%", sCharData.GET_SP_INCR()*100.0f ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextIncPerCP->AddText( _HLIB::cstringformat( "%d %%", 0 ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextIncPotionHP->AddText( _HLIB::cstringformat( "%d", sCharData.get_additional_gain_hp() ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextIncPotionMP->AddText( _HLIB::cstringformat( "%d", sCharData.get_additional_gain_mp() ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextIncPotionSP->AddText( _HLIB::cstringformat( "%d", sCharData.get_additional_gain_sp() ).GetString(), NS_UITEXTCOLOR::WHITE );
	m_pTextIncPotionCP->AddText( _HLIB::cstringformat( "%d", sCharData.get_additional_gain_cp() ).GetString(), NS_UITEXTCOLOR::WHITE );


	float fVal1 = 0.0f;
	float fVal2 = 0.0f;
	int nVal1 = 0;
	int nVal2 = 0;

	//crit
	fVal1 = sCharData2.get_critical_hit_chance();
	fVal2 = sCharData.get_critical_hit_chance();

	if ( fVal1 != fVal2 )		
	{
		m_pTextCriticalHitChanceMarkImage->SetVisibleSingle ( TRUE );
		m_pTextCriticalHitChanceMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextCriticalHitChanceMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextCriticalHitChanceUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );


	//crit damage
	fVal1 = sCharData2.get_critical_hit_damage();
	fVal2 = sCharData.get_critical_hit_damage();

	if ( fVal1 != fVal2 )		
	{
		m_pTextCriticalHitDamageMarkImage->SetVisibleSingle ( TRUE );
		m_pTextCriticalHitDamageMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextCriticalHitDamageMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextCriticalHitDamageUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );


	//strike
	fVal1 = sCharData2.get_crushing_blow_chance();
	fVal2 = sCharData.get_crushing_blow_chance();

	if ( fVal1 != fVal2 )		
	{
		m_pTextCrushingBlowChanceMarkImage->SetVisibleSingle ( TRUE );
		m_pTextCrushingBlowChanceMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextCrushingBlowChanceMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextCrushingBlowChanceUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );

	//strike damage
	fVal1 = sCharData2.get_crushing_blow_damage();
	fVal2 = sCharData.get_crushing_blow_damage();

	if ( fVal1 != fVal2 )		
	{
		m_pTextCrushingBlowDamageMarkImage->SetVisibleSingle ( TRUE );
		m_pTextCrushingBlowDamageMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextCrushingBlowDamageMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextCrushingBlowDamageUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );

	//move speed
	fVal1 = (sCharData2.GET_MOVE_VELO()-1.0f)*100.0f;
	fVal2 = (sCharData.GET_MOVE_VELO()-1.0f)*100.0f;

	if ( fVal1 != fVal2 )		
	{
		m_pTextMovementSpeedMarkImage->SetVisibleSingle ( TRUE );
		m_pTextMovementSpeedMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextMovementSpeedMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextMovementSpeedUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );


	//att speed
	fVal1 = (sCharData2.GET_ATT_VELO()-1.0f)*100.0f;
	fVal2 = (sCharData.GET_ATT_VELO()-1.0f)*100.0f;

	if ( fVal1 != fVal2 )		
	{
		m_pTextAttackSpeedMarkImage->SetVisibleSingle ( TRUE );
		m_pTextAttackSpeedMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextAttackSpeedMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextAttackSpeedUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );

	//exp
	fVal1 = (sCharData2.GET_EXP_TOTAL()-1.0f)*100.0f;
	fVal2 = (sCharData.GET_EXP_TOTAL()-1.0f)*100.0f;

	if ( fVal1 != fVal2 )		
	{
		m_pTextExpMultipleMarkImage->SetVisibleSingle ( TRUE );
		m_pTextExpMultipleMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextExpMultipleMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextExpMultipleUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );

	//hp rate
	fVal1 = sCharData2.GET_HP_INCR()*100.0f;
	fVal2 = sCharData.GET_HP_INCR()*100.0f;

	if ( fVal1 != fVal2 )		
	{
		m_pTextIncPerHPMarkImage->SetVisibleSingle ( TRUE );
		m_pTextIncPerHPMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextIncPerHPMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextIncPerHPUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );

	//mp rate
	fVal1 = sCharData2.GET_MP_INCR()*100.0f;
	fVal2 = sCharData.GET_MP_INCR()*100.0f;

	if ( fVal1 != fVal2 )		
	{
		m_pTextIncPerMPMarkImage->SetVisibleSingle ( TRUE );
		m_pTextIncPerMPMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextIncPerMPMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextIncPerMPUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );

	//sp rate
	fVal1 = sCharData2.GET_SP_INCR()*100.0f;
	fVal2 = sCharData.GET_SP_INCR()*100.0f;

	if ( fVal1 != fVal2 )		
	{
		m_pTextIncPerSPMarkImage->SetVisibleSingle ( TRUE );
		m_pTextIncPerSPMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextIncPerSPMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextIncPerSPUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );


	//cp rate
	fVal1 = 0.0f;
	fVal2 = 0.0f;

	if ( fVal1 != fVal2 )		
	{
		m_pTextIncPerCPMarkImage->SetVisibleSingle ( TRUE );
		m_pTextIncPerCPMarkImage->CheckValue( fVal2, fVal1 );
	}else{
		m_pTextIncPerCPMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextIncPerCPUP->SetOneLineText ( _HLIB::cstringformat( "%2.f %%", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );


	//hp Potion
	nVal1 = sCharData2.get_additional_gain_hp();
	nVal2 = sCharData.get_additional_gain_hp();

	if ( nVal1 != nVal2 )		
	{
		m_pTextIncPotionHPMarkImage->SetVisibleSingle ( TRUE );
		m_pTextIncPotionHPMarkImage->CheckValue( nVal2, nVal1 );
	}else{
		m_pTextIncPotionHPMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextIncPotionHPUP->SetOneLineText ( _HLIB::cstringformat( "%2.f", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );

	//mp Potion
	nVal1 = sCharData2.get_additional_gain_mp();
	nVal2 = sCharData.get_additional_gain_mp();

	if ( nVal1 != nVal2 )		
	{
		m_pTextIncPotionMPMarkImage->SetVisibleSingle ( TRUE );
		m_pTextIncPotionMPMarkImage->CheckValue( nVal2, nVal1 );
	}else{
		m_pTextIncPotionMPMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextIncPotionMPUP->SetOneLineText ( _HLIB::cstringformat( "%2.f", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );

	//sp Potion
	nVal1 = sCharData2.get_additional_gain_sp();
	nVal2 = sCharData.get_additional_gain_sp();

	if ( nVal1 != nVal2 )		
	{
		m_pTextIncPotionSPMarkImage->SetVisibleSingle ( TRUE );
		m_pTextIncPotionSPMarkImage->CheckValue( nVal2, nVal1 );
	}else{
		m_pTextIncPotionSPMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextIncPotionSPUP->SetOneLineText ( _HLIB::cstringformat( "%2.f", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );

	//cp Potion
	nVal1 = sCharData2.get_additional_gain_cp();
	nVal2 = sCharData.get_additional_gain_cp();

	if ( nVal1 != nVal2 )		
	{
		m_pTextIncPotionCPMarkImage->SetVisibleSingle ( TRUE );
		m_pTextIncPotionCPMarkImage->CheckValue( nVal2, nVal1 );
	}else{
		m_pTextIncPotionCPMark->AddText ( "-", NS_UITEXTCOLOR::WHITE );
	}

	m_pTextIncPotionCPUP->SetOneLineText ( _HLIB::cstringformat( "%2.f", fVal1 - fVal2 ).GetString(), NS_UITEXTCONTROL::GetTextColor_Compare( fVal2, fVal1 ) );

	

}
