#pragma	once

#include "./UIWindowEx.h"
#include "../../Lib_Client/G-Logic/GLSkill.h"

class CMultiModeButton;
class CBasicTextButton;
class CBasicButton;
class CModernSkillPage;
class CBasicTextBox;
struct SCHARSKILL;

class	CModernSkillWindow : public CUIWindowEx
{
protected:
	enum
	{
		SKILL_WINDOW_PAGE_BUTTON0 = ET_CONTROL_NEXT,
		SKILL_WINDOW_PAGE_BUTTON1,
		SKILL_WINDOW_PAGE_BUTTON2,
		SKILL_WINDOW_PAGE_BUTTON3,
		SKILL_WINDOW_PAGE0,
		SKILL_WINDOW_PAGE1,
		SKILL_WINDOW_PAGE2,
		SKILL_WINDOW_PAGE3,
		SKILL_WINDOW_HELP,
	};

	enum
	{
		nMAXPAGE = 4,
	};

public:
	CModernSkillWindow ();
	virtual	~CModernSkillWindow ();

public:
	void	CreateSubControl ();

private:
	void	SetVisiblePage ( int nPage );

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );
	CModernSkillPage*	CreateSkillPage ( UIGUID ControlID, SNATIVEID sNativeIDArray[GLSkillMan::MAX_CLASSSKILL], int nSkillCount );
	
private:
	CUIControl*		CreateControl ( char* szControl );
	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign );

private:
	void	GetSkillIndexRange ( EMCHARCLASS emCharClass, int* pnBeginIndex, int* pnEndIndex );
	void	SortSkill ( EMCHARCLASS emCharClass, SNATIVEID sNativeIDArrayAll[EMSKILL_PC_CLASSNUM][GLSkillMan::MAX_CLASSSKILL], int nSkillCountAll[EMSKILL_PC_CLASSNUM] );

private:
	CModernSkillPage*	m_pPage[nMAXPAGE];
	CMultiModeButton*	m_pPageButton[nMAXPAGE];
	CBasicButton*		m_pHelp;

private:
	DWORD				m_dwSkillPointBACK;
	CBasicTextBox*		m_pRestPoint;
};