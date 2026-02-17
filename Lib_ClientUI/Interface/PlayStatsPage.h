#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"
#include "../../Lib_Client/G-Logic/GLPKComboDefine.h"
#include "../../Lib_Client/G-Logic/GLAwardTitle.h"

class CUIControl;
class CBasicTextBox;
class CBasicLineBox;
class CD3DFontPar;
class CBasicProgressBar;



class	CPlayStatsPage : public CUIGroup
{
private:

	enum 
	{
		INFO_SCHOOL_SIZE = 3,
		INFO_CLASS_SIZE = GLCI_NUM_8CLASS,
	};

public:
	CPlayStatsPage ();
	virtual	~CPlayStatsPage ();

public:
	CUIControl*		CreateCUIControl( const std::string& strControl, WORD wFlag );
	CBasicLineBox*	CreateLineBoxWhite( const std::string& strControl );
	CBasicLineBox*	CreateUILineBoxWhiteNoBody( const std::string& strControl );
	CBasicLineBox*	CreateLineBoxQuestList( const std::string& strControl );
	CBasicTextBox*	CreateTextBox( const std::string& strControl, CD3DFontPar* pFont, int nTextAlign, const std::string& strText, DWORD dwTextColor );

	CBasicProgressBar* m_pMMRProgressBar;
	CBasicTextBox* m_pMMRProgressText;


	void	CreateSubControl ();
	
	void	ResetMMRBadge();
	void	UpdateMMRProgressBar();

private:
	CBasicTextBox*	m_pTextBasicInformationName;
	CBasicTextBox*	m_pTextBasicInformationSchool;
	CBasicTextBox*	m_pTextBasicInformationClass;
	CBasicTextBox*	m_pTextBasicInformationGuild;
	CUIControl*		m_pImageBasicInfoSchool[INFO_SCHOOL_SIZE];
	CUIControl*		m_pImageBasicInfoClass[INFO_CLASS_SIZE];
	CUIControl*		m_pImageBasicInfoClub;

	CUIControl* m_pImageMMRBadge;
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	CUIControl* m_pImageMMRBadges[10];


	/*pk info, Juver, 2017/11/17 */
//	CBasicTextBox*	m_pTextPkInfoTotalKills;
//	CBasicTextBox*	m_pTextPkInfoTotalDeaths;

	/* leaderboard rankings, Juver, 2021/02/08 */
	CBasicTextBox*	m_pTextLeaderboardOverallRank;
	CBasicTextBox*	m_pTextLeaderboardClassRank;
	CBasicTextBox*	m_pTextLeaderboardClassGoldRank;

	/* pk combo, Juver, 2021/02/09 */
	CBasicTextBox*	m_pTextMultiKillStreak[EMPKCOMBO_COUNT_SIZE];

private:

	/*activity point, Juver, 2017/08/23 */	
	CBasicTextBox*	m_pTextPlayInfoActivityPoints;

	/*contribution point, Juver, 2017/08/23 */
	CBasicTextBox*	m_pTextPlayInfoContributionPoints;

	/* play time system, Juver, 2021/01/26 */	
	CBasicTextBox*	m_pTextPlayInfoPlayTime;		
	CBasicTextBox*	m_pTextPlayInfoPlayPoints;


	//combat record pvp
	CBasicTextBox*	m_pTextCombatRecordTotalKills;
	CBasicTextBox*	m_pTextCombatRecordTotalDeaths;
	CBasicTextBox*	m_pTextCombatRecordTotalResu;

	// MMR
	CBasicTextBox*	m_pTextMMRRankName;
	CBasicTextBox*	m_pTextMMRPoints;

	CBasicTextBox*	m_pTextCombatRecordTyrannyKills;
	CBasicTextBox*	m_pTextCombatRecordTyrannyDeaths;
	CBasicTextBox*	m_pTextCombatRecordTyrannyResu;

	CBasicTextBox*	m_pTextCombatRecordClubWarsKills;
	CBasicTextBox*	m_pTextCombatRecordClubWarsDeaths;
	CBasicTextBox*	m_pTextCombatRecordClubWarsResu;

	CBasicTextBox*	m_pTextCombatRecordCaptureTheFlagKills;
	CBasicTextBox*	m_pTextCombatRecordCaptureTheFlagDeaths;
	CBasicTextBox*	m_pTextCombatRecordCaptureTheFlagResu;

	CBasicTextBox*	m_pTextCombatRecordClubDeathMatchKills;
	CBasicTextBox*	m_pTextCombatRecordClubDeathMatchDeaths;
	CBasicTextBox*	m_pTextCombatRecordClubDeathMatchResu;

	CBasicTextBox*	m_pTextCombatRecordSchoolWarsKills;
	CBasicTextBox*	m_pTextCombatRecordSchoolWarsDeaths;
	CBasicTextBox*	m_pTextCombatRecordSchoolWarsResu;

	CBasicTextBox*	m_pTextCombatRecordPKMapKills;
	CBasicTextBox*	m_pTextCombatRecordPKMapDeaths;
	CBasicTextBox*	m_pTextCombatRecordPKMapResu;

	CBasicTextBox*	m_pTextCombatRecordPVEKills;
	CBasicTextBox*	m_pTextCombatRecordPVEDeaths;
	CBasicTextBox*	m_pTextCombatRecordPVEGoldLoot;

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	float	m_fUpdateTimer;
	BOOL	m_bFirstLBUP;

	DWORD m_dwMMR;

	int CalculateVal ( DWORD dwCalc );//MMR Rank


public:
	void UpdateInfo();

};