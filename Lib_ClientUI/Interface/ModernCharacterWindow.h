/*!
 * \file ModernCharacterWindow.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */

#pragma	once

#include "./UIWindowEx.h"
#include "../../Lib_Client/G-Logic/GLCharLogicClient.h"

class CModernCharacterStat;
class CModernCharacterStat2;
class CModernCharacterEquip;
class CModernCharacterEquipEx;

class CBasicTextBox;
class CBasicButton;
class CBasicComboBoxRollOver;
class CMultiModeButton;

struct	GLCHARLOGIC_CLIENT;

class CModernCharacterWindow : public CUIWindowEx
{
protected:
	enum
	{
		MODERN_CHARACTER_WINDOW_STAT = ET_CONTROL_NEXT,
		MODERN_CHARACTER_WINDOW_STAT2,
		MODERN_CHARACTER_WINDOW_EQUIP,
		MODERN_CHARACTER_WINDOW_EQUIPEX,
		MODERN_CHARACTER_WINDOW_BRIGHTBAR,
		MODERN_CHARACTER_WINDOW_BUTTON_HELP,
		MODERN_CHARACTER_PREMIUM_TEXT,

		/*activity system, Juver, 2017/11/05 */
		MODERN_CHARACTER_TITLE_BUTTON,
		MODERN_CHARACTER_TITLE_COMBO_ROLLOVER,

		MODERN_CHARACTER_WINDOW_BUTTON_STAT,
		MODERN_CHARACTER_WINDOW_BUTTON_STAT2,
	};

	enum
	{
		MODERN_CHARACTER_WINDOW_STATS_BASIC = 0,
		MODERN_CHARACTER_WINDOW_STATS_OTHER	= 1,
	};

public:
    CModernCharacterWindow ();
	virtual	~CModernCharacterWindow ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual void SetGlobalPos(const D3DXVECTOR2& vPos);

public:
	void	CreateSubControl ();

public:
	GLCHARLOGIC_CLIENT		m_sCharLogic;
	CModernCharacterStat*		m_pCharacterStat;
	CModernCharacterStat2*		m_pCharacterStat2;
	CModernCharacterEquip*		m_pCharacterEquip;
	CModernCharacterEquipEx*	m_pCharacterEquipEx;

	CBasicTextBox*		m_pTextName;
	CBasicTextBox*		m_pTextClub;
	CBasicTextBox*		m_pTextBadge;

	CBasicTextBox*		m_pBrightStatic;
	CBasicTextBox*		m_pActivityStatic;
	
	CBasicTextBox*		m_pPremiumStatic;
	
	CBasicTextBox*		m_pDepartmentText;
	CBasicTextBox*		m_pActivityText;
	CBasicTextBox*		m_pPremiumText;

	CUIControl*			m_pBrightFrame;
	CUIControl*			m_pBrightSlider;

	CBasicButton*		m_pButtonHelp;

	/*activity system, Juver, 2017/11/05 */
	CBasicButton*			m_pButtonTitle;
	CBasicComboBoxRollOver*	m_pComboBoxTitleRollOver;

	CMultiModeButton*	m_pStatButton;
	CMultiModeButton*	m_pStat2Button;

	/* reborn system, Juver, 2021/09/17 */
	CBasicTextBox*		m_pStaticReborn;
	CBasicTextBox*		m_pTextReborn;

	CBasicTextBox*		m_pStaticVIP;
	CBasicTextBox*		m_pTextVIP;

private:
	static const int nSCHOOL_MARK = 3;
	CUIControl*	m_pSchoolMark[nSCHOOL_MARK];

	UIGUID	m_RollOverID;
	BOOL	m_bFirstLBUP;

	float	m_fUpdateTime;
	int		m_nStatsType;

public:
	void	ResetData();
	void	UpdateData();
	void	UpdateStats();
	void	OpenStats( int nType );

	/*activity system, Juver, 2017/11/05 */
	void	LoadBadge( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetArmSwapTabButton( BOOL bArmSub );

};