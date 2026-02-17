/*
	AUTHOR: RSiLENT https://www.facebook.com/rsilent22/
*/
#pragma	once

//#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "./UIWindowEx.h"

#include "../Lib_Client/G-Logic/GLCWDefine.h"
struct	GLCHARLOGIC;
#include "../Lib_Client/G-Logic/GLCharDefine.h"

class CBasicTextBox;
class CClubWarRankingDisplaySlot;
class CClubWarRankingDisplayPlayerSlot;
class CClubWarRankingDisplayResuScoreSlot;

class CBasicTextButton;
class CBasicButton;
class CBasicLineBox;

const DWORD UIMSG_CWRANKDISPLAY_BUTTON_GUILD_MOUSEIN = UIMSG_USER1;
const DWORD UIMSG_CWRANKDISPLAY_BUTTON_PLAYER_MOUSEIN = UIMSG_USER2;
const DWORD UIMSG_CWRANKDISPLAY_BUTTON_RESU_SCORE_MOUSEIN = UIMSG_USER3;
const DWORD UIMSG_CWRANKDISPLAY_BUTTON_SHOW_MOUSEIN = UIMSG_USER4;

class CClubWarRankingDisplay : public CUIWindowEx
{
	enum
	{
		CLUBWAR_BUTTON_SHOW = ET_CONTROL_NEXT,
		CLUBWAR_BUTTON_GUILD,
		CLUBWAR_BUTTON_PLAYER,
		CLUBWAR_BUTTON_RESU,

		CLUBWAR_RANKING_GUILD_SLOT_0,
		CLUBWAR_RANKING_GUILD_SLOT_1,
		CLUBWAR_RANKING_GUILD_SLOT_2,
		CLUBWAR_RANKING_GUILD_SLOT_3,
		CLUBWAR_RANKING_GUILD_SLOT_4,
		CLUBWAR_RANKING_GUILD_SLOT_5,
		CLUBWAR_RANKING_GUILD_SLOT_6,
		CLUBWAR_RANKING_GUILD_SLOT_7,
		CLUBWAR_RANKING_GUILD_SLOT_8,
		CLUBWAR_RANKING_GUILD_SLOT_9,
		CLUBWAR_RANKING_GUILD_SLOT_OWN,

		CLUBWAR_RANKING_PLAYER_SLOT_0,
		CLUBWAR_RANKING_PLAYER_SLOT_1,
		CLUBWAR_RANKING_PLAYER_SLOT_2,
		CLUBWAR_RANKING_PLAYER_SLOT_3,
		CLUBWAR_RANKING_PLAYER_SLOT_4,
		CLUBWAR_RANKING_PLAYER_SLOT_5,
		CLUBWAR_RANKING_PLAYER_SLOT_6,
		CLUBWAR_RANKING_PLAYER_SLOT_7,
		CLUBWAR_RANKING_PLAYER_SLOT_8,
		CLUBWAR_RANKING_PLAYER_SLOT_9,
		CLUBWAR_RANKING_PLAYER_SLOT_OWN,

		CLUBWAR_RANKING_RESU_SCORE_SLOT_0,
		CLUBWAR_RANKING_RESU_SCORE_SLOT_1,
		CLUBWAR_RANKING_RESU_SCORE_SLOT_2,
		CLUBWAR_RANKING_RESU_SCORE_SLOT_3,
		CLUBWAR_RANKING_RESU_SCORE_SLOT_4,
		CLUBWAR_RANKING_RESU_SCORE_SLOT_5,
		CLUBWAR_RANKING_RESU_SCORE_SLOT_6,
		CLUBWAR_RANKING_RESU_SCORE_SLOT_7,
		CLUBWAR_RANKING_RESU_SCORE_SLOT_8,
		CLUBWAR_RANKING_RESU_SCORE_SLOT_9,
		CLUBWAR_RANKING_RESU_SCORE_SLOT_OWN
	};

private:
	CClubWarRankingDisplaySlot*				m_pSlot[RANKING_NUM_CW];
	CClubWarRankingDisplaySlot*				m_pSlotOwn;

	CClubWarRankingDisplayPlayerSlot*		m_pPlayerSlot[RANKING_NUM_CW];
	CClubWarRankingDisplayPlayerSlot*		m_pPlayerSlotOwn;

	CClubWarRankingDisplayResuScoreSlot*	m_pResuScoreSlot[RANKING_NUM_CW];
	CClubWarRankingDisplayResuScoreSlot*	m_pResuScoreSlotOwn;

public:
	CClubWarRankingDisplay();
	~CClubWarRankingDisplay();

	void	CreateSubControl ();
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

public:	
	void	OpenPage( int nPage );
	void	RefreshCWRanking();
	void	ShowAll();

	CBasicTextButton*		m_pButtonShow;
	CBasicTextButton*		m_pButtonGuild;
	CBasicTextButton*		m_pButtonPlayer;
	CBasicTextButton*		m_pButtonResuScore;
};