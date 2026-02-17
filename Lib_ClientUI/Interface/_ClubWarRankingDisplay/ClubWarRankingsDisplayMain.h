#pragma	once

#include "../UIWindowEx.h"


/* 7-13-23 Tyranny War Ranking System - Montage */

class CBasicTextButton;
class CClubWarRankingsDisplay;
class CClubWarRankingsDisplayResu;
class CClubWarRankingsDisplayGuild;
class	CMultiModeButton;

class	CClubWarRankingsDisplayMain : public CUIWindowEx
{
	enum
	{
		BUTTON_RANKING_PLAYER = ET_CONTROL_NEXT,
		BUTTON_RANKING_PLAYER_RESU,
		BUTTON_RANKING_GUILD,

		PAGE_RANKING_PLAYER,
		PAGE_RANKING_PLAYER_RESU,
		PAGE_RANKING_GUILD,
	};

	enum
	{
		WINDOW_RANKING_PLAYER			= 0,
		WINDOW_RANKING_PLAYER_RESU		= 1,
		WINDOW_RANKING_GUILD			= 2,
	};

public:
	CClubWarRankingsDisplayMain ();
	virtual ~CClubWarRankingsDisplayMain ();

public:
	void	CreateSubControl ();
	CBasicTextButton*	CreateTextButton19 ( char* szButton, UIGUID ControlID, char* szText );
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );


private: 
	
	CUIControl*			m_pWhiteBG;
	
	CUIControl*			m_pWhiteBG1;

public:
	CMultiModeButton*		m_pButtonRankingPlayer;
	CMultiModeButton*		m_pButtonRankingPlayerResu;
	CMultiModeButton*		m_pButtonRankingGuild;

	CClubWarRankingsDisplay*				m_pPageRankingPlayer;
	CClubWarRankingsDisplayResu*			m_pPageRankingPlayerResu;
	CClubWarRankingsDisplayGuild*		m_pPageRankingGuild;

public:
	int		m_nPage;

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

public:
	void	OpenPage( int nPage );
	void	RefreshPage();
};