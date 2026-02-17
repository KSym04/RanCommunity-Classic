#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

class CBasicTextBox;
class CBasicLineBox;
class CD3DFontPar;

class CPVPWoePageBattle;
class CPVPWoePageHistory;

const DWORD UIMSG_WOE_PAGE_BUTTON_CLOSE = UIMSG_USER1;

class CPVPWoePage : public CUIGroup
{
private:
	enum
	{
		WOE_PAGE_INFO_BATTLE = NO_ID + 1,
		WOE_PAGE_INFO_HISTORY,
	};

	
public:
	CPVPWoePage ();
	virtual	~CPVPWoePage ();

public:
	void	CreateSubControl ();
	CBasicTextBox*	CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign );
	CBasicLineBox*	CreateUILineBoxQuestList( char* szBaseControl );
	CBasicLineBox*	CreateUILineBoxWhiteNoBody( char* szBaseControl );
	CUIControl*		CreateUIControl( const char* szControl );

public:
	CBasicTextBox*		m_pTextMapName;

	CBasicTextBox*		m_pTextInfoMechanics;
	CBasicTextBox*		m_pTextInfoTitle;
	CBasicTextBox*		m_pTextInfoTitle1;
	CBasicTextBox*		m_pTextInfoMatches;
	CBasicTextBox*		m_pTextInfoPets;


	CBasicTextBox*		m_pTextReward;
	CBasicTextBox*		m_pTextRewardItem;
	CBasicTextBox*		m_pTextRewardContrib;

	CPVPWoePageBattle*	m_pInfoBattle;
	CPVPWoePageHistory*	m_pInfoHistory;

public:
	float	m_fUpdateTime;

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

	void	UpdateInfo();
	void	SetCompetitionJoin( bool bEnable );
};