/*!
 * \file ModernCharacterStat.h
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
class CBasicButton;
class CMultiModeButton;
class CBasicLineBox;
class CModernCharacterStatMark;
class CModernCharacterStatOver;

class	CModernCharacterStat : public CUIGroup
{
private:
	enum
	{
		MODERN_CHARACTER_WINDOW_STAT_BUTTON_QUESTION = NO_ID + 1,

		MODERN_CHARACTER_WINDOW_POW_BUTTON_PLUS,
		MODERN_CHARACTER_WINDOW_DEX_BUTTON_PLUS,
		MODERN_CHARACTER_WINDOW_SPI_BUTTON_PLUS,
		MODERN_CHARACTER_WINDOW_STR_BUTTON_PLUS,
		MODERN_CHARACTER_WINDOW_STA_BUTTON_PLUS,

		MODERN_CHARACTER_WINDOW_POW_BUTTON_MINUS,
		MODERN_CHARACTER_WINDOW_DEX_BUTTON_MINUS,
		MODERN_CHARACTER_WINDOW_SPI_BUTTON_MINUS,
		MODERN_CHARACTER_WINDOW_STR_BUTTON_MINUS,
		MODERN_CHARACTER_WINDOW_STA_BUTTON_MINUS,

		MODERN_CHARACTER_WINDOW_STAT_RESET,
		MODERN_CHARACTER_WINDOW_STAT_APPLY,
	};


public:
	CModernCharacterStat ();
	virtual	~CModernCharacterStat ();

public:
	CUIControl*		CreateControl ( const char* szControl, const UIGUID& cID = NO_ID );
	CBasicLineBox*	CreateLineBox( char* szKeyword, const UIGUID& cID = NO_ID );
	CBasicTextBox*	CreateTextBox( char* szKeyword, const char* szText, DWORD dwColor, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );
	CModernCharacterStatMark*	CreateStatMark( char* szControl, const UIGUID& cID = NO_ID );
	CModernCharacterStatOver*	CreateStatOver( char* szControl, const char* szText, const UIGUID& cID = NO_ID );
	CMultiModeButton* CreateStatButton( char* szControl, const char* szText, const UIGUID& cID = NO_ID );
	CMultiModeButton* CreateStat2Button( char* szControl, const char* szText, const UIGUID& cID = NO_ID );

	void	CreateSubControl ();


public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	SetPointControl ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetData ( const GLCHARLOGIC_CLIENT& sCharData, const GLCHARLOGIC_CLIENT& sCharData2 );
	void	SetActionBlock( BOOL bBlock )	{ m_bActionBlock = bBlock; }

public:
	WORD			m_wPowAdd;
	WORD			m_wDexAdd;
	WORD			m_wSpiAdd;
	WORD			m_wStrAdd;
	WORD			m_wStaAdd;
	WORD			m_wStatsDeduct;
	BOOL			m_bActionBlock;

public:
	CBasicButton*	m_pQuestionButton;
	
	CBasicTextBox*	m_pPointValue;

	CBasicTextBox*	m_pPowTextStatic;
	CBasicTextBox*	m_pDexTextStatic;
	CBasicTextBox*	m_pSpiTextStatic;
	CBasicTextBox*	m_pStrTextStatic;
	CBasicTextBox*	m_pStaTextStatic;

	CBasicTextBox*	m_pPowTextValue;
	CBasicTextBox*	m_pDexTextValue;
	CBasicTextBox*	m_pSpiTextValue;
	CBasicTextBox*	m_pStrTextValue;
	CBasicTextBox*	m_pStaTextValue;

	CBasicTextBox*	m_pPowTextMark;
	CBasicTextBox*	m_pDexTextMark;
	CBasicTextBox*	m_pSpiTextMark;
	CBasicTextBox*	m_pStrTextMark;
	CBasicTextBox*	m_pStaTextMark;

	CBasicTextBox*	m_pPowTextUp;
	CBasicTextBox*	m_pDexTextUp;
	CBasicTextBox*	m_pSpiTextUp;
	CBasicTextBox*	m_pStrTextUp;
	CBasicTextBox*	m_pStaTextUp;


	CBasicTextBox*	m_pAttackMinTextValue;
	CBasicTextBox*	m_pAttackMaxTextValue;
	CBasicTextBox*	m_pDefenseTextValue;
	CBasicTextBox*	m_pFightTextValue;
	CBasicTextBox*	m_pShootTextValue;
	CBasicTextBox*	m_pEnergyTextValue;
	CBasicTextBox*	m_pHitTextValue;
	CBasicTextBox*	m_pAvoidTextValue;
	CBasicTextBox*	m_pHPTextValue;
	CBasicTextBox*	m_pMPTextValue;
	CBasicTextBox*	m_pSPTextValue;

	CBasicTextBox*	m_pAttackMinTextMark;
	CBasicTextBox*	m_pAttackMaxTextMark;
	CBasicTextBox*	m_pDefenseTextMark;
	CBasicTextBox*	m_pFightTextMark;
	CBasicTextBox*	m_pShootTextMark;
	CBasicTextBox*	m_pEnergyTextMark;
	CBasicTextBox*	m_pHitTextMark;
	CBasicTextBox*	m_pAvoidTextMark;
	CBasicTextBox*	m_pHPTextMark;
	CBasicTextBox*	m_pMPTextMark;
	CBasicTextBox*	m_pSPTextMark;

	CBasicTextBox*	m_pAttackMinTextUp;
	CBasicTextBox*	m_pAttackMaxTextUp;
	CBasicTextBox*	m_pDefenseTextUp;
	CBasicTextBox*	m_pFightTextUp;
	CBasicTextBox*	m_pShootTextUp;
	CBasicTextBox*	m_pEnergyTextUp;
	CBasicTextBox*	m_pHitTextUp;
	CBasicTextBox*	m_pAvoidTextUp;
	CBasicTextBox*	m_pHPTextUp;
	CBasicTextBox*	m_pMPTextUp;
	CBasicTextBox*	m_pSPTextUp;


	CBasicTextBox*	m_pFireTextValue;
	CBasicTextBox*	m_pColdTextValue;
	CBasicTextBox*	m_pElectricTextValue;
	CBasicTextBox*	m_pPoisonTextValue;
	CBasicTextBox*	m_pSpiritTextValue;

	CBasicTextBox*	m_pFireTextMark;
	CBasicTextBox*	m_pColdTextMark;
	CBasicTextBox*	m_pElectricTextMark;
	CBasicTextBox*	m_pPoisonTextMark;
	CBasicTextBox*	m_pSpiritTextMark;

	CBasicTextBox*	m_pFireTextUp;
	CBasicTextBox*	m_pColdTextUp;
	CBasicTextBox*	m_pElectricTextUp;
	CBasicTextBox*	m_pPoisonTextUp;
	CBasicTextBox*	m_pSpiritTextUp;


	CModernCharacterStatMark*		m_pPowMarkImage;
	CModernCharacterStatMark*		m_pDexMarkImage;
	CModernCharacterStatMark*		m_pSpiMarkImage;
	CModernCharacterStatMark*		m_pStrMarkImage;
	CModernCharacterStatMark*		m_pStaMarkImage;

	CModernCharacterStatMark*		m_pAttackMinMarkImage;
	CModernCharacterStatMark*		m_pAttackMaxMarkImage;
	CModernCharacterStatMark*		m_pDefenseMarkImage;
	CModernCharacterStatMark*		m_pFightMarkImage;
	CModernCharacterStatMark*		m_pShootMarkImage;
	CModernCharacterStatMark*		m_pEnergyMarkImage;
	CModernCharacterStatMark*		m_pHitMarkImage;
	CModernCharacterStatMark*		m_pAvoidMarkImage;
	CModernCharacterStatMark*		m_pHPMarkImage;
	CModernCharacterStatMark*		m_pMPMarkImage;
	CModernCharacterStatMark*		m_pSPMarkImage;

	CModernCharacterStatMark*		m_pFireMarkImage;
	CModernCharacterStatMark*		m_pColdMarkImage;
	CModernCharacterStatMark*		m_pElectricMarkImage;
	CModernCharacterStatMark*		m_pPoisonMarkImage;
	CModernCharacterStatMark*		m_pSpiritMarkImage;


	CMultiModeButton*	m_pPowButton_Plus;
	CMultiModeButton*	m_pDexButton_Plus;
	CMultiModeButton*	m_pSpiButton_Plus;
	CMultiModeButton*	m_pStrButton_Plus;
	CMultiModeButton*	m_pStaButton_Plus;
	CMultiModeButton*	m_pPowButton_Minus;
	CMultiModeButton*	m_pDexButton_Minus;
	CMultiModeButton*	m_pSpiButton_Minus;
	CMultiModeButton*	m_pStrButton_Minus;
	CMultiModeButton*	m_pStaButton_Minus;

	CMultiModeButton*	m_pButtonStatReset;
	CMultiModeButton*	m_pButtonStatApply;

};