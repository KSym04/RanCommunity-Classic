/*
	AUTHOR: RSiLENT https://www.facebook.com/rsilent22/
*/
#pragma	once

//#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "UIWindowEx.h"

#include "../Lib_Client/G-Logic/GLPVPTyrannyDefine.h"
struct	GLCHARLOGIC;
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "../Lib_Client/G-Logic/GLPVPTyrannyData.h"

class CBasicTextBox;
class CCTyrannyWarRankingDisplayGuildSlot;
class CCTyrannyWarRankingDisplayPlayerSlot;
class CCTyrannyWarRankingDisplayPlayerResuScoreSlot;
class CBasicTextButton;
class CBasicButton;
class CBasicLineBox;

const DWORD UIMSG_TWRANKDISPLAY_GUILD_BUTTON_MOUSEIN = UIMSG_USER1;
const DWORD UIMSG_TWRANKDISPLAY_PLAYER_BUTTON_MOUSEIN = UIMSG_USER2;
const DWORD UIMSG_TWRANKDISPLAY_RESU_BUTTON_MOUSEIN = UIMSG_USER3;
const DWORD	UIMSG_TWRANKDISPLAY_BUTTON_SHOW_MOUSEIN = UIMSG_USER4;

class CCTyrannyWarRankingDisplay : public CUIWindowEx
{
	enum
	{
		TYRANNY_WAR_BUTTON_SHOW = ET_CONTROL_NEXT,
		TYRANNY_WAR_BUTTON_GUILD,
		TYRANNY_WAR_BUTTON_PLAYER,
		TYRANNY_WAR_BUTTON_RESU,

		TYRANNY_RANKING_GUILD_SLOT_0,
		TYRANNY_RANKING_GUILD_SLOT_1,
		TYRANNY_RANKING_GUILD_SLOT_2,
		TYRANNY_RANKING_GUILD_SLOT_3,
		TYRANNY_RANKING_GUILD_SLOT_4,
		TYRANNY_RANKING_GUILD_SLOT_5,
		TYRANNY_RANKING_GUILD_SLOT_6,
		TYRANNY_RANKING_GUILD_SLOT_7,
		TYRANNY_RANKING_GUILD_SLOT_8,
		TYRANNY_RANKING_GUILD_SLOT_9,
		TYRANNY_RANKING_GUILD_SLOT_OWN,

		TYRANNY_RANKING_PLAYER_SLOT_0,
		TYRANNY_RANKING_PLAYER_SLOT_1,
		TYRANNY_RANKING_PLAYER_SLOT_2,
		TYRANNY_RANKING_PLAYER_SLOT_3,
		TYRANNY_RANKING_PLAYER_SLOT_4,
		TYRANNY_RANKING_PLAYER_SLOT_5,
		TYRANNY_RANKING_PLAYER_SLOT_6,
		TYRANNY_RANKING_PLAYER_SLOT_7,
		TYRANNY_RANKING_PLAYER_SLOT_8,
		TYRANNY_RANKING_PLAYER_SLOT_9,
		TYRANNY_RANKING_PLAYER_SLOT_OWN,

		TYRANNY_RANKING_RESU_SCORE_SLOT_0,
		TYRANNY_RANKING_RESU_SCORE_SLOT_1,
		TYRANNY_RANKING_RESU_SCORE_SLOT_2,
		TYRANNY_RANKING_RESU_SCORE_SLOT_3,
		TYRANNY_RANKING_RESU_SCORE_SLOT_4,
		TYRANNY_RANKING_RESU_SCORE_SLOT_5,
		TYRANNY_RANKING_RESU_SCORE_SLOT_6,
		TYRANNY_RANKING_RESU_SCORE_SLOT_7,
		TYRANNY_RANKING_RESU_SCORE_SLOT_8,
		TYRANNY_RANKING_RESU_SCORE_SLOT_9,
		TYRANNY_RANKING_RESU_SCORE_SLOT_OWN
	};

private:
	CCTyrannyWarRankingDisplayGuildSlot*				m_pSlot[RANKING_NUM_TW];
	CCTyrannyWarRankingDisplayGuildSlot*				m_pSlotOwn;

	CCTyrannyWarRankingDisplayPlayerSlot*				m_pPlayerSlot[RANKING_NUM_TW];
	CCTyrannyWarRankingDisplayPlayerSlot*				m_pPlayerSlotOwn;

	CCTyrannyWarRankingDisplayPlayerResuScoreSlot*		m_pResuScoreSlot[RANKING_NUM_TW];
	CCTyrannyWarRankingDisplayPlayerResuScoreSlot*		m_pResuScoreSlotOwn;

public:
	CCTyrannyWarRankingDisplay();
	~CCTyrannyWarRankingDisplay();

	void	CreateSubControl ();
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

public:	
	void	OpenPage( int nPage );
	void	RefreshTWRanking();
	void	ShowAll();

	CBasicTextButton*		m_pButtonShow;
	CBasicTextButton*		m_pButtonGuild;
	CBasicTextButton*		m_pButtonPlayer;
	CBasicTextButton*		m_pButtonResuScore;
};