/*!
 * \file ModernCharacterStat2.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharLogicClient.h"

class CBasicTextBox;
class CBasicLineBox;
class CModernCharacterStatMark;
class CModernCharacterStatOver;
class CBasicButton;

class	CModernCharacterStat2 : public CUIGroup
{
private:
	enum
	{
		MODERN_CHARACTER_WINDOW_STAT2_BUTTON_QUESTION = NO_ID + 1,
	};


public:
	CModernCharacterStat2 ();
	virtual	~CModernCharacterStat2 ();

public:
	CBasicLineBox*	CreateLineBox( char* szKeyword, const UIGUID& cID = NO_ID );
	CBasicTextBox*	CreateTextBox( char* szKeyword, const char* szText, DWORD dwColor, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );
	CModernCharacterStatMark*	CreateStatMark( char* szControl, const UIGUID& cID = NO_ID );
	CModernCharacterStatOver*	CreateStatOver( char* szControl, const char* szText, const UIGUID& cID = NO_ID );
	void	CreateSubControl ();


public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	SetData ( const GLCHARLOGIC_CLIENT& sCharData, const GLCHARLOGIC_CLIENT& sCharData2 );

public:
	CBasicButton*	m_pQuestionButton;

	CBasicTextBox*	m_pTextCriticalHitChance;
	CBasicTextBox*	m_pTextCriticalHitDamage;
	CBasicTextBox*	m_pTextCrushingBlowChance;
	CBasicTextBox*	m_pTextCrushingBlowDamage;
	CBasicTextBox*	m_pTextMovementSpeed;
	CBasicTextBox*	m_pTextAttackSpeed;
	CBasicTextBox*	m_pTextExpMultiple;
	CBasicTextBox*	m_pTextIncPerHP;
	CBasicTextBox*	m_pTextIncPerMP;
	CBasicTextBox*	m_pTextIncPerSP;
	CBasicTextBox*	m_pTextIncPerCP;
	CBasicTextBox*	m_pTextIncPotionHP;
	CBasicTextBox*	m_pTextIncPotionMP;
	CBasicTextBox*	m_pTextIncPotionSP;
	CBasicTextBox*	m_pTextIncPotionCP;

	CBasicTextBox*	m_pTextCriticalHitChanceMark;
	CBasicTextBox*	m_pTextCriticalHitDamageMark;
	CBasicTextBox*	m_pTextCrushingBlowChanceMark;
	CBasicTextBox*	m_pTextCrushingBlowDamageMark;
	CBasicTextBox*	m_pTextMovementSpeedMark;
	CBasicTextBox*	m_pTextAttackSpeedMark;
	CBasicTextBox*	m_pTextExpMultipleMark;
	CBasicTextBox*	m_pTextIncPerHPMark;
	CBasicTextBox*	m_pTextIncPerMPMark;
	CBasicTextBox*	m_pTextIncPerSPMark;
	CBasicTextBox*	m_pTextIncPerCPMark;
	CBasicTextBox*	m_pTextIncPotionHPMark;
	CBasicTextBox*	m_pTextIncPotionMPMark;
	CBasicTextBox*	m_pTextIncPotionSPMark;
	CBasicTextBox*	m_pTextIncPotionCPMark;

	CBasicTextBox*	m_pTextCriticalHitChanceUP;
	CBasicTextBox*	m_pTextCriticalHitDamageUP;
	CBasicTextBox*	m_pTextCrushingBlowChanceUP;
	CBasicTextBox*	m_pTextCrushingBlowDamageUP;
	CBasicTextBox*	m_pTextMovementSpeedUP;
	CBasicTextBox*	m_pTextAttackSpeedUP;
	CBasicTextBox*	m_pTextExpMultipleUP;
	CBasicTextBox*	m_pTextIncPerHPUP;
	CBasicTextBox*	m_pTextIncPerMPUP;
	CBasicTextBox*	m_pTextIncPerSPUP;
	CBasicTextBox*	m_pTextIncPerCPUP;
	CBasicTextBox*	m_pTextIncPotionHPUP;
	CBasicTextBox*	m_pTextIncPotionMPUP;
	CBasicTextBox*	m_pTextIncPotionSPUP;
	CBasicTextBox*	m_pTextIncPotionCPUP;

	CModernCharacterStatMark*	m_pTextCriticalHitChanceMarkImage;
	CModernCharacterStatMark*	m_pTextCriticalHitDamageMarkImage;
	CModernCharacterStatMark*	m_pTextCrushingBlowChanceMarkImage;
	CModernCharacterStatMark*	m_pTextCrushingBlowDamageMarkImage;
	CModernCharacterStatMark*	m_pTextMovementSpeedMarkImage;
	CModernCharacterStatMark*	m_pTextAttackSpeedMarkImage;
	CModernCharacterStatMark*	m_pTextExpMultipleMarkImage;
	CModernCharacterStatMark*	m_pTextIncPerHPMarkImage;
	CModernCharacterStatMark*	m_pTextIncPerMPMarkImage;
	CModernCharacterStatMark*	m_pTextIncPerSPMarkImage;
	CModernCharacterStatMark*	m_pTextIncPerCPMarkImage;
	CModernCharacterStatMark*	m_pTextIncPotionHPMarkImage;
	CModernCharacterStatMark*	m_pTextIncPotionMPMarkImage;
	CModernCharacterStatMark*	m_pTextIncPotionSPMarkImage;
	CModernCharacterStatMark*	m_pTextIncPotionCPMarkImage;

};