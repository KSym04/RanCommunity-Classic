#pragma	once

#include "./UIWindowEx.h"

class	CD3DFontPar;
class	CBasicProgressBar;
class	CBasicTextBox;
class	CBasicButton;
class	CBasicComboBoxRollOver;
struct	GLCHARLOGIC_CLIENT;

class CCharacterWindow : public CUIWindowEx
{
protected:
	enum
	{
		CHARACTER_POW_BUTTON = ET_CONTROL_NEXT,
		CHARACTER_DEX_BUTTON,
		CHARACTER_SPI_BUTTON,
		CHARACTER_INT_BUTTON,
		CHARACTER_STR_BUTTON,
		CHARACTER_STA_BUTTON,
		CHARACTER_MOBILE_BUTTON,

		CHARACTER_PREMIUM_TEXT,

		CHARACTER_BIRGHTER,

		/*activity system, Juver, 2017/11/05 */
		CHARACTER_TITLE_BUTTON,
		CHARACTER_TITLE_COMBO_ROLLOVER,
	};

public:
    CCharacterWindow ();
	virtual	~CCharacterWindow ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual void SetGlobalPos(const D3DXVECTOR2& vPos);

public:
	void	CreateSubControl ();

private:
	CBasicTextBox*	CreateStaticControl ( char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );

private:
	void	SetPointControl ( BOOL bPointUsable );

private:	//	이름,클래스
	void	SetName ( const GLCHARLOGIC_CLIENT& sCharData );

private:	//	경험치
	void	SetEXP( const GLCHARLOGIC_CLIENT& sCharData );

private:	//	Stat
	void	SetLevel ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetPow ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetDex ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetSpi ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetInt ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetStr ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetSta ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetPoint ( const GLCHARLOGIC_CLIENT& sCharData );

private:	//	Status
	void	SetDamage ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetDefense ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetReqPa ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetReqSa ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetReqMa ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetHitRate ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetHP ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetMP ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetSP ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetCP ( const GLCHARLOGIC_CLIENT& sCharData ); /*combatpoint logic, Juver, 2017/05/29 */
	void	SetAvoidRate ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetPremium ();

private:	//	저항력
	void	SetFireResi ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetColdResi ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetElectricResi ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetPoisonResi ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetSpiritResi ( const GLCHARLOGIC_CLIENT& sCharData );

private:	//	사회 성향
//	void	SetPK ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetBright ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetPKState ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetGrade ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetActivityPoint ( const GLCHARLOGIC_CLIENT& sCharData );	//	생활점수
	void	SetAcademy ( const GLCHARLOGIC_CLIENT& sCharData );		//	학교
	void	SetDepartment ( const GLCHARLOGIC_CLIENT& sCharData );		//	부서

public:
	void	SetPhoneNumber( const CString & strPhoneNumber );

public:
	/*activity system, Juver, 2017/11/05 */
	void	LoadBadge( const GLCHARLOGIC_CLIENT& sCharData );

	/* reborn system, Juver, 2021/09/17 */
	void	SetReborn ( const GLCHARLOGIC_CLIENT& sCharData );

	void	SetVIP( const GLCHARLOGIC_CLIENT& sCharData );

private:
	CBasicTextBox*		m_pNameText;

private:
	CBasicTextBox*	m_pPremiumTextStatic;

private:	//	경험치
	CBasicProgressBar*	m_pEXP;
	CBasicTextBox*		m_pEXPText;

private:	//	Stats
	CBasicButton*	m_pPowButton;
	CBasicButton*	m_pDexButton;
	CBasicButton*	m_pSpiButton;
	CBasicButton*	m_pIntButton;
	CBasicButton*	m_pStrButton;
	CBasicButton*	m_pStaButton;

	CUIControl*	m_pPowDiaableButton;
	CUIControl*	m_pDexDiaableButton;
	CUIControl*	m_pSpiDiaableButton;
	CUIControl*	m_pIntDiaableButton;
	CUIControl*	m_pStrDiaableButton;
	CUIControl*	m_pStaDiaableButton;

	CBasicTextBox*	m_pPowText;
	CBasicTextBox*	m_pDexText;
	CBasicTextBox*	m_pSpiText;
	CBasicTextBox*	m_pIntText;
	CBasicTextBox*	m_pStrText;
	CBasicTextBox*	m_pStaText;

	CBasicTextBox*	m_pPowTextStatic;
	CBasicTextBox*	m_pDexTextStatic;
	CBasicTextBox*	m_pSpiTextStatic;
	CBasicTextBox*	m_pIntTextStatic;
	CBasicTextBox*	m_pStrTextStatic;
	CBasicTextBox*	m_pStaTextStatic;

	CUIControl*	m_pPointDisable;
	CUIControl*	m_pPointEnable;
	CBasicTextBox*	m_pPointText;	

private:	//	Status
	CBasicTextBox*	m_pLevelText;
	CBasicTextBox*	m_pDamageText;
	CBasicTextBox*	m_pDefenseText;
	CBasicTextBox*	m_pReqPaText;
	CBasicTextBox*	m_pReqSaText;	
	CBasicTextBox*	m_pReqMaText;	
	CBasicTextBox*	m_pHPText;
	CBasicTextBox*	m_pMPText;
	CBasicTextBox*	m_pSPText;
	CBasicTextBox*	m_pCPText; /*combatpoint logic, Juver, 2017/05/29 */
	CBasicTextBox*	m_pHitRateText;
	CBasicTextBox*	m_pAvoidRateText;
	CBasicTextBox*	m_pPremiumText;

private:	//	저항력
	CBasicTextBox*	m_pFireText;
	CBasicTextBox*	m_pColdText;
	CBasicTextBox*	m_pElectricText;
	CBasicTextBox*	m_pPoisonText;
	CBasicTextBox*	m_pSpiritText;

private:	//	사회 성향
	CBasicTextBox*	m_pAcademy;
	CBasicTextBox*	m_pActivityPoint;
	CBasicTextBox*	m_pPKStateText;

	CBasicTextBox*	m_pDepartment;
	CUIControl*		m_pBrightFrame;
	CUIControl*		m_pBrightSlider;
	CBasicTextBox*	m_pGradeText;

	CBasicTextBox*  m_pMobileText;

	/*activity system, Juver, 2017/11/05 */
	CBasicButton*			m_pButtonTitle;
	CBasicComboBoxRollOver*	m_pComboBoxTitleRollOver;

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

};