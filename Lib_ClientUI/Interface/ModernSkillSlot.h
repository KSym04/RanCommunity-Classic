#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"

const DWORD MODERN_SKILL_SLOT_UIMSG_MOUSEIN_BUTTON	= UIMSG_USER1;
const DWORD MODERN_SKILL_SLOT_UIMSG_LEVEL_UP		= UIMSG_USER2;
const DWORD MODERN_SKILL_SLOT_UIMSG_MOUSEIN_ICON	= UIMSG_USER3;

class CSkillImage;
class CBasicButton;
class CBasicTextBox;
class CBasicLineBox;
class CBasicLineBoxEx;
class CMultiModeButton;

class	CModernSkillSlot : public CUIGroup
{
protected:
	enum
	{
		SKILL_SLOT_LEVEL_UP = NO_ID + 1,
		SKILL_SLOT_LEVEL_DOWN,
		SKILL_SLOT_IMAGE,
	};

public:
	CModernSkillSlot ();
	virtual	~CModernSkillSlot ();

public:
	void	CreateSubControl ( SNATIVEID sNativeID );

public:
	void		SetNativeSkillID ( SNATIVEID sNativeID )			{ m_sNativeID = sNativeID; }
	SNATIVEID	GetNativeSkillID ()									{ return m_sNativeID; }

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

protected:
	CUIControl*		CreateControl ( char* szControl );
	CSkillImage*	CreateSkillImage ();
	CBasicButton*	CreateFlipButton ( char* szButton, char* szButtonFlip, char* szMouseOver, UIGUID ControlID );
	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign );
	CMultiModeButton* CreateButton( char* szControl, const char* szText, const UIGUID& cID = NO_ID );

private:
	CSkillImage*		m_pSkillImage;
	CMultiModeButton*	m_pSkillButton;
	CUIControl*			m_pSkillButtonDisable;
	CBasicTextBox*		m_pTextBox1;
	CBasicTextBox*		m_pTextBox2;
	CUIControl*			m_pNotLearnImage;
	CBasicLineBoxEx*	m_pImageHover_Y;
	CBasicLineBoxEx*	m_pImageHover_G;
	CBasicLineBoxEx*	m_pImageHover_B;

private:
	WORD		m_wLevel;
	bool		m_bHover;

	CString		m_strLine1;
	CString		m_strLine2;

private:
	SNATIVEID	m_sNativeID;

	BOOL	m_bLearnSkill;
	BOOL	m_bLEVELUP_ENABLE;
	BOOL	m_bCANLEARN;
	BOOL	m_bCANPURCHASE;

public:
	void	ShowHover( bool bShow )	{	m_bHover = bShow;	}
};