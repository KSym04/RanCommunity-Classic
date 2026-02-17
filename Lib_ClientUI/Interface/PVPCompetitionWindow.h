#ifndef PVPCOMPETITIONWINDOW_H__INCLUDED
#define PVPCOMPETITIONWINDOW_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_ClientUI/Interface/UIWindowEx.h"

class CBasicTextButton;
class CMultiModeButton;
class CPVPTyrannyPage;			/*pvp tyranny, Juver, 2017/08/25 */
class CPVPSchoolWarsPage;		/*school wars, Juver, 2018/01/19 */
class CPVPCaptureTheFlagPage;	/*pvp capture the flag, Juver, 2018/01/25 */
class CPVPClubDeathMatchPage;	/* pvp club death match, Juver, 2020/11/26 */
class CPVPPartyBattlePassPage;
class CPVPWoePage;

class	CPVPCompetitionWindow : public CUIWindowEx
{
	enum
	{
		COMPETITION_WINDOW_BUTTON_CLOSE = ET_CONTROL_NEXT,

		COMPETITION_WINDOW_PAGE_TYRANNY,
		COMPETITION_WINDOW_PAGE_SCHOOLWARS,
		COMPETITION_WINDOW_PAGE_CAPTURE_THE_FLAG,
		COMPETITION_WINDOW_PAGE_CLUB_DEATH_MATCH,
		COMPETITION_WINDOW_PAGE_PBG,
		COMPETITION_WINDOW_PAGE_WOE,

		COMPETITION_WINDOW_BUTTON_0,
		COMPETITION_WINDOW_BUTTON_1,
		COMPETITION_WINDOW_BUTTON_2,
		COMPETITION_WINDOW_BUTTON_3,
		COMPETITION_WINDOW_BUTTON_4,
		COMPETITION_WINDOW_BUTTON_5,
	};

	enum
	{
		COMPETITION_MAX_PAGE = 6,
	};

public:
	CPVPCompetitionWindow ();
	virtual ~CPVPCompetitionWindow ();

public:
	void	CreateSubControl ();
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );

public:
	CBasicTextButton*		m_pButtonClose;
	CMultiModeButton*		m_pButtonEvent[COMPETITION_MAX_PAGE];
	CUIControl*				m_pPageEvent[COMPETITION_MAX_PAGE];

	CPVPTyrannyPage*		m_pPageTyranny;			/*pvp tyranny, Juver, 2017/08/25 */
	CPVPSchoolWarsPage*		m_pPageSchoolWars;		/*school wars, Juver, 2018/01/19 */
	CPVPCaptureTheFlagPage*	m_pPageCaptureTheFlag;	/*pvp capture the flag, Juver, 2018/01/25 */
	CPVPClubDeathMatchPage*	m_pPageClubDeathMatch;	/* pvp club death match, Juver, 2020/11/26 */
	CPVPPartyBattlePassPage* m_pPagePartyBattleGrounds;
	CPVPWoePage* m_pPageWoe;

public:
	int		m_nPage;

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

public:
	void	OpenPage( int nPage );
	void	SetCompetitionJoin( bool bEnable );
};

#endif // PVPCOMPETITIONWINDOW_H__INCLUDED


