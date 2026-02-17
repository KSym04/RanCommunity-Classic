#include "StdAfx.h"
#include "./PlayStatsPage.h"

#include "./InnerInterface.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicLineBoxSmart.h"
#include "./BasicProgressBar.h"

#include "../../Lib_Engine/Core/NSRProfile.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"
#include "../../Lib_Engine/GUInterface/UITextUtil.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLAwardTitleClient.h"
#include "../Lib_Client/G-Logic/GLogicData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPlayStatsPage::CPlayStatsPage ()
	: m_fUpdateTimer(0.0f)
	, m_bFirstLBUP(FALSE)

	, m_pTextBasicInformationName(NULL)
	, m_pTextBasicInformationSchool(NULL)
	, m_pTextBasicInformationClass(NULL)
	, m_pTextBasicInformationGuild(NULL)
	, m_pImageBasicInfoClub(NULL)

	/*pk info, Juver, 2017/11/17 */
//	, m_pTextPkInfoTotalKills(NULL)
//	, m_pTextPkInfoTotalDeaths(NULL)

	/* leaderboard rankings, Juver, 2021/02/08 */
//	, m_pTextLeaderboardOverallRank(NULL)
//	, m_pTextLeaderboardClassRank(NULL)
//	, m_pTextLeaderboardClassGoldRank(NULL)

	/*activity point, Juver, 2017/08/23 */
	, m_pTextPlayInfoActivityPoints(NULL)

	/*contribution point, Juver, 2017/08/23 */
	, m_pTextPlayInfoContributionPoints(NULL)

	/* play time system, Juver, 2021/01/26 */	
	, m_pTextPlayInfoPlayTime(NULL)	
	, m_pTextPlayInfoPlayPoints(NULL)

	, m_pTextCombatRecordTotalKills(NULL)
	, m_pTextCombatRecordTotalDeaths(NULL)
	, m_pTextCombatRecordTotalResu(NULL)

	, m_pTextCombatRecordTyrannyKills(NULL)
	, m_pTextCombatRecordTyrannyDeaths(NULL)
	, m_pTextCombatRecordTyrannyResu(NULL)

	, m_pTextCombatRecordClubWarsKills(NULL)
	, m_pTextCombatRecordClubWarsDeaths(NULL)
	, m_pTextCombatRecordClubWarsResu(NULL)

	, m_pTextCombatRecordCaptureTheFlagKills(NULL)
	, m_pTextCombatRecordCaptureTheFlagDeaths(NULL)
	, m_pTextCombatRecordCaptureTheFlagResu(NULL)

	, m_pTextCombatRecordClubDeathMatchKills(NULL)
	, m_pTextCombatRecordClubDeathMatchDeaths(NULL)
	, m_pTextCombatRecordClubDeathMatchResu(NULL)

	, m_pTextCombatRecordSchoolWarsKills(NULL)
	, m_pTextCombatRecordSchoolWarsDeaths(NULL)
	, m_pTextCombatRecordSchoolWarsResu(NULL)

	, m_pTextCombatRecordPKMapKills(NULL)
	, m_pTextCombatRecordPKMapDeaths(NULL)
	, m_pTextCombatRecordPKMapResu(NULL)

	, m_pTextCombatRecordPVEKills(NULL)
	, m_pTextCombatRecordPVEDeaths(NULL)
	, m_pTextCombatRecordPVEGoldLoot(NULL)

	// MMR - JX
	, m_pTextMMRRankName(NULL)
	, m_pTextMMRPoints(NULL)

{
	for ( int i=0; i<INFO_SCHOOL_SIZE; ++i )
		m_pImageBasicInfoSchool[i] = NULL;

	for ( int i=0; i<INFO_CLASS_SIZE; ++i )
		m_pImageBasicInfoClass[i] = NULL;

	/* pk combo, Juver, 2021/02/09 */
	for ( int i=0; i<EMPKCOMBO_COUNT_SIZE; ++i )
		m_pTextMultiKillStreak[i] = NULL;

}

CPlayStatsPage::~CPlayStatsPage ()
{
}

CUIControl*	CPlayStatsPage::CreateCUIControl( const std::string& strControl, WORD wFlag )
{
	if ( strControl.empty() )	return NULL;

	CUIControl* pUIControl = new CUIControl;
	pUIControl->CreateSub ( this, strControl.c_str(), wFlag );	
	pUIControl->SetVisibleSingle ( TRUE );
	RegisterControl ( pUIControl );

	return pUIControl;
}

CBasicLineBox* CPlayStatsPage::CreateUILineBoxWhiteNoBody( const std::string& strControl )
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( (char*)strControl.c_str() );
	RegisterControl ( pLineBox );

	return pLineBox;
}
CBasicLineBox*	CPlayStatsPage::CreateLineBoxWhite( const std::string& strControl )
{
	if ( strControl.empty() )	return NULL;

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhite ( (char*)strControl.c_str() );
	RegisterControl ( pLineBox );

	return pLineBox;

}

CBasicLineBox*	CPlayStatsPage::CreateLineBoxQuestList( const std::string& strControl )
{
	if ( strControl.empty() )	return NULL;

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( (char*)strControl.c_str() );
	RegisterControl ( pLineBox );

	return pLineBox;
}

CBasicTextBox*	CPlayStatsPage::CreateTextBox( const std::string& strControl, CD3DFontPar* pFont, int nTextAlign, const std::string& strText, DWORD dwTextColor )
{
	if ( strControl.empty() )	return NULL;
	if ( !pFont )		return NULL;

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, strControl.c_str() );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( nTextAlign );
	pTextBox->SetText( strText.c_str(), dwTextColor );

	RegisterControl ( pTextBox );

	return pTextBox;
}


void CPlayStatsPage::CreateSubControl ()
{
	CD3DFontPar* pFont11 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 11, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont09 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = CreateLineBoxWhite( "RAN_STUDENTRECORD_OBJECT_LINE" );
	CUIControl* pControl = CreateCUIControl( "PLAY_STATS_LINEBOX_REGION_TEXTURE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	if ( pLineBox && pControl )
	{
		pControl->AlignToControl( pLineBox, TRUE );
	}


	{	//basic information group
		CreateLineBoxQuestList( "PLAY_STATS_BASIC_INFORMATION_REGION_LINEBOX" );
		CreateLineBoxQuestList( "PLAY_STATS_BASIC_INFORMATION_TITLE_LINEBOX" );
		CreateTextBox( "PLAY_STATS_BASIC_INFORMATION_TITLE_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_BASIC_INFORMATION_STATIC", 0 ), NS_UITEXTCOLOR::ORANGE );
		CreateTextBox( "PLAY_STATS_BASIC_INFORMATION_CHANAME_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_BASIC_INFORMATION_STATIC", 1 ), NS_UITEXTCOLOR::WHITE );
		CreateTextBox( "PLAY_STATS_BASIC_INFORMATION_CHASCHOOL_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_BASIC_INFORMATION_STATIC", 2 ), NS_UITEXTCOLOR::WHITE );
		CreateTextBox( "PLAY_STATS_BASIC_INFORMATION_CHACLASS_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_BASIC_INFORMATION_STATIC", 3 ), NS_UITEXTCOLOR::WHITE );
		CreateTextBox( "PLAY_STATS_BASIC_INFORMATION_CHAGUILD_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_BASIC_INFORMATION_STATIC", 4 ), NS_UITEXTCOLOR::WHITE );

		m_pTextBasicInformationName		= CreateTextBox( "PLAY_STATS_BASIC_INFORMATION_CHANAME_TEXTBOX", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextBasicInformationSchool	= CreateTextBox( "PLAY_STATS_BASIC_INFORMATION_CHASCHOOL_TEXTBOX", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextBasicInformationClass	= CreateTextBox( "PLAY_STATS_BASIC_INFORMATION_CHACLASS_TEXTBOX", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextBasicInformationGuild	= CreateTextBox( "PLAY_STATS_BASIC_INFORMATION_CHAGUILD_TEXTBOX", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );


		std::string strSchoolImage[INFO_SCHOOL_SIZE] = 
		{
			"PLAY_STATS_BASIC_INFORMATION_SCHOOL_WINDOW_SM",
			"PLAY_STATS_BASIC_INFORMATION_SCHOOL_WINDOW_HA",
			"PLAY_STATS_BASIC_INFORMATION_SCHOOL_WINDOW_BH",
		};

		std::string strClassImage[INFO_CLASS_SIZE] = 
		{
			"PLAY_STATS_BASIC_INFORMATION_CLASS_BRAWLER_MALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_SWORDMAN_MALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_ARCHER_FEMALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_SHAMAN_FEMALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_EXTREME_MALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_EXTREME_FEMALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_BRAWLER_FEMALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_SWORDMAN_FEMALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_ARCHER_MALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_SHAMAN_MALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_SCIENTIST_MALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_SCIENTIST_FEMALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_ASSASSIN_MALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_ASSASSIN_FEMALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_TRICKER_MALE",
			"PLAY_STATS_BASIC_INFORMATION_CLASS_TRICKER_FEMALE",
		};

		for ( int i=0; i<INFO_SCHOOL_SIZE; ++i )
		{
			m_pImageBasicInfoSchool[i] = CreateCUIControl( strSchoolImage[i], UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pImageBasicInfoSchool[i]->SetVisibleSingle( FALSE );
		}

		for ( int i=0; i<INFO_CLASS_SIZE; ++i )
		{
			m_pImageBasicInfoClass[i] = CreateCUIControl( strClassImage[i], UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pImageBasicInfoClass[i]->SetVisibleSingle( FALSE );
		}

		m_pImageBasicInfoClub = CreateCUIControl( "PLAY_STATS_BASIC_INFORMATION_IMAGE_CLUB", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pImageBasicInfoClub->SetUseRender( TRUE );
	}
	
	
// 	{	//pk score group
// 		CreateLineBoxQuestList( "PLAY_STATS_PK_INFO_REGION_LINEBOX" );
// 		CreateLineBoxQuestList( "PLAY_STATS_PK_INFO_TITLE_LINEBOX" );
// 		CreateTextBox( "PLAY_STATS_PK_INFO_TITLE_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_PK_INFO_STATIC", 0 ), NS_UITEXTCOLOR::ORANGE );
// 		CreateTextBox( "PLAY_STATS_PK_INFO_TOTAL_KILLS_STATIC", pFont09, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_PK_INFO_STATIC", 1 ), NS_UITEXTCOLOR::WHITE );
// 		CreateTextBox( "PLAY_STATS_PK_INFO_TOTAL_DEATHS_STATIC", pFont09, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_PK_INFO_STATIC", 2 ), NS_UITEXTCOLOR::WHITE );
// 		CreateCUIControl( "PLAY_STATS_PK_INFO_TOTAL_KILLS_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
// 		CreateCUIControl( "PLAY_STATS_PK_INFO_TOTAL_DEATHS_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
// 
// 		m_pTextPkInfoTotalKills		= CreateTextBox( "PLAY_STATS_PK_INFO_TOTAL_KILLS_TEXTBOX", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
// 		m_pTextPkInfoTotalDeaths	= CreateTextBox( "PLAY_STATS_PK_INFO_TOTAL_DEATHS_TEXTBOX", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::ORNAGERED );
// 	}

	
	//{	//leaderboard rankings group
	//	CreateLineBoxQuestList( "PLAY_STATS_LEADERBOARD_RANKINGS_REGION_LINEBOX" );
	//	CreateLineBoxQuestList( "PLAY_STATS_LEADERBOARD_RANKINGS_TITLE_LINEBOX" );
	//	CreateCUIControl( "PLAY_STATS_LEADERBOARD_RANKINGS_TITLE_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	//	CreateTextBox( "PLAY_STATS_LEADERBOARD_RANKINGS_TITLE_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_LEADERBOARDS_STATIC", 0 ), NS_UITEXTCOLOR::ORANGE );
	//	CreateTextBox( "PLAY_STATS_LEADERBOARD_RANKINGS_OVERALL_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_LEADERBOARDS_STATIC", 1 ), NS_UITEXTCOLOR::WHITE );
	//	CreateTextBox( "PLAY_STATS_LEADERBOARD_RANKINGS_CLASS_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_LEADERBOARDS_STATIC", 2 ), NS_UITEXTCOLOR::WHITE );
	//	CreateTextBox( "PLAY_STATS_LEADERBOARD_RANKINGS_GOLD_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_LEADERBOARDS_STATIC", 3 ), NS_UITEXTCOLOR::WHITE );
	//	CreateCUIControl( "PLAY_STATS_LEADERBOARD_RANKINGS_TOTAL_RANKINGS_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	//	CreateCUIControl( "PLAY_STATS_LEADERBOARD_RANKINGS_CLASS_RANKINGS_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	//	CreateCUIControl( "PLAY_STATS_LEADERBOARD_RANKINGS_GOLD_RANKINGS_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );

	//	m_pTextLeaderboardOverallRank		= CreateTextBox( "PLAY_STATS_LEADERBOARD_RANKINGS_OVERALL_TEXT", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
	//	m_pTextLeaderboardClassRank			= CreateTextBox( "PLAY_STATS_LEADERBOARD_RANKINGS_CLASS_TEXT", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
	//	m_pTextLeaderboardClassGoldRank		= CreateTextBox( "PLAY_STATS_LEADERBOARD_RANKINGS_GOLD_TEXT", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
	//}

		

	{	//multikill streak group
		CreateLineBoxQuestList( "PLAY_STATS_MULTIKILL_STREAK_REGION_LINEBOX" );
		CreateLineBoxQuestList( "PLAY_STATS_MULTIKILL_STREAK_TITLE_LINEBOX" );

		// Mega Kill
		CBasicLineBoxSmart* pLineBoxSmart = new CBasicLineBoxSmart();
		pLineBoxSmart->CreateSub( this, "MULTIKILL_MEGAKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart->CreateSubControl( "MULTIKILL_BLUELINE" );
		RegisterControl( pLineBoxSmart );

		// Godlike
		CBasicLineBoxSmart* pLineBoxSmart0 = new CBasicLineBoxSmart();
		pLineBoxSmart0->CreateSub( this, "MULTIKILL_GODLIKE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart0->CreateSubControl( "MULTIKILL_BLUELINE0" );
		RegisterControl( pLineBoxSmart0 );

		// Unstoppable
		CBasicLineBoxSmart* pLineBoxSmart1 = new CBasicLineBoxSmart();
		pLineBoxSmart1->CreateSub( this, "MULTIKILL_UNSTOPPABLE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart1->CreateSubControl( "MULTIKILL_BLUELINE1" );
		RegisterControl( pLineBoxSmart1 );

		// Wicked Sick
		CBasicLineBoxSmart* pLineBoxSmart2 = new CBasicLineBoxSmart();
		pLineBoxSmart2->CreateSub( this, "MULTIKILL_WICKEDSICK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart2->CreateSubControl( "MULTIKILL_BLUELINE2" );
		RegisterControl( pLineBoxSmart2 );

		// Monster Kill
		CBasicLineBoxSmart* pLineBoxSmart3 = new CBasicLineBoxSmart();
		pLineBoxSmart3->CreateSub( this, "MULTIKILL_MONSTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart3->CreateSubControl( "MULTIKILL_BLUELINE3" );
		RegisterControl( pLineBoxSmart3 );

		// Holy Shit
		CBasicLineBoxSmart* pLineBoxSmart4 = new CBasicLineBoxSmart();
		pLineBoxSmart4->CreateSub( this, "MULTIKILL_HOLYSHIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart4->CreateSubControl( "MULTIKILL_BLUELINE4" );
		RegisterControl( pLineBoxSmart4 );

		// Ownage
		CBasicLineBoxSmart* pLineBoxSmart5 = new CBasicLineBoxSmart();
		pLineBoxSmart5->CreateSub( this, "MULTIKILL_OWNAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart5->CreateSubControl( "MULTIKILL_BLUELINE5" );
		RegisterControl( pLineBoxSmart5 );

		// Combo Whore
		CBasicLineBoxSmart* pLineBoxSmart6 = new CBasicLineBoxSmart();
		pLineBoxSmart6->CreateSub( this, "MULTIKILL_CBWHORE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart6->CreateSubControl( "MULTIKILL_BLUELINE6" );
		RegisterControl( pLineBoxSmart6 );

		CreateCUIControl( "PLAY_STATS_MULTIKILL_STREAK_TITLE_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		CreateTextBox( "PLAY_STATS_MULTIKILL_STREAK_TITLE_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_MULTI_KILL_STREAK_STATIC", 0 ), NS_UITEXTCOLOR::ORANGE );

		std::string strStatic[EMPKCOMBO_COUNT_SIZE] = 
		{
			"PLAY_STATS_MULTIKILL_STREAK_STATIC_00",
			"PLAY_STATS_MULTIKILL_STREAK_STATIC_01",
			"PLAY_STATS_MULTIKILL_STREAK_STATIC_02",
			"PLAY_STATS_MULTIKILL_STREAK_STATIC_03",
			"PLAY_STATS_MULTIKILL_STREAK_STATIC_04",
			"PLAY_STATS_MULTIKILL_STREAK_STATIC_05",
			"PLAY_STATS_MULTIKILL_STREAK_STATIC_06",
			"PLAY_STATS_MULTIKILL_STREAK_STATIC_07",
		};

		std::string strText[EMPKCOMBO_COUNT_SIZE] = 
		{
			"PLAY_STATS_MULTIKILL_STREAK_TEXT_00",
			"PLAY_STATS_MULTIKILL_STREAK_TEXT_01",
			"PLAY_STATS_MULTIKILL_STREAK_TEXT_02",
			"PLAY_STATS_MULTIKILL_STREAK_TEXT_03",
			"PLAY_STATS_MULTIKILL_STREAK_TEXT_04",
			"PLAY_STATS_MULTIKILL_STREAK_TEXT_05",
			"PLAY_STATS_MULTIKILL_STREAK_TEXT_06",
			"PLAY_STATS_MULTIKILL_STREAK_TEXT_07",
		};

		for ( int i=0; i<EMPKCOMBO_COUNT_SIZE; ++i )
		{
			CreateTextBox( strStatic[i], pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_MULTI_KILL_STREAK_STATIC", i + 1 ), NS_UITEXTCOLOR::WHITE );

			m_pTextMultiKillStreak[i] = CreateTextBox( strText[i], pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		}
	}

	{	//achievements group

		CreateLineBoxQuestList( "PLAY_STATS_ACHIEVEMENTS_INFO_REGION_LINEBOX" );
		CreateLineBoxQuestList( "PLAY_STATS_ACHIEVEMENTS_INFO_TITLE_LINEBOX" );
		CreateTextBox( "PLAY_STATS_ACHIEVEMENTS_INFO_TITLE_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_ACHIEVEMENTS_INFO_STATIC", 0 ), NS_UITEXTCOLOR::ORANGE );

		CreateCUIControl( "PLAY_STATS_ACHIEVEMENTS_INFO_ACTIVITY_POINT_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		CreateCUIControl( "PLAY_STATS_ACHIEVEMENTS_INFO_CONTRIBUTION_POINT_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		CreateCUIControl( "PLAY_STATS_ACHIEVEMENTS_INFO_PLAY_TIME_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		CreateCUIControl( "PLAY_STATS_ACHIEVEMENTS_INFO_PLAY_POINTS_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		CreateTextBox( "PLAY_STATS_ACHIEVEMENTS_INFO_ACTIVITY_POINT_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_ACHIEVEMENTS_INFO_STATIC", 1 ), NS_UITEXTCOLOR::WHITE );
		CreateTextBox( "PLAY_STATS_ACHIEVEMENTS_INFO_CONTRIBUTION_POINT_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_ACHIEVEMENTS_INFO_STATIC", 2 ), NS_UITEXTCOLOR::WHITE );
		CreateTextBox( "PLAY_STATS_ACHIEVEMENTS_INFO_PLAY_TIME_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_ACHIEVEMENTS_INFO_STATIC", 3 ), NS_UITEXTCOLOR::WHITE );
		CreateTextBox( "PLAY_STATS_ACHIEVEMENTS_INFO_PLAY_POINTS_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_ACHIEVEMENTS_INFO_STATIC", 4 ), NS_UITEXTCOLOR::WHITE );

		m_pTextPlayInfoActivityPoints		= CreateTextBox( "PLAY_STATS_ACHIEVEMENTS_INFO_ACTIVITY_POINT_TEXTBOX", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextPlayInfoContributionPoints	= CreateTextBox( "PLAY_STATS_ACHIEVEMENTS_INFO_CONTRIBUTION_POINT_TEXTBOX", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextPlayInfoPlayTime				= CreateTextBox( "PLAY_STATS_ACHIEVEMENTS_INFO_PLAY_TIME_TEXTBOX", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextPlayInfoPlayPoints			= CreateTextBox( "PLAY_STATS_ACHIEVEMENTS_INFO_PLAY_POINTS_TEXTBOX", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );


		/* award title, Juver, 2021/02/12 */
		/*m_pComboBoxAwardTitle00Open = CreateComboBox( "PLAY_STATS_ACHIEVEMENTS_INFO_AWARD_TITLE_00_OPEN", UI_FLAG_XSIZE | UI_FLAG_YSIZE, AWARD_TITLE_00_COMBO_OPEN );
		m_pComboBoxAwardTitle01Open = CreateComboBox( "PLAY_STATS_ACHIEVEMENTS_INFO_AWARD_TITLE_01_OPEN", UI_FLAG_XSIZE | UI_FLAG_YSIZE, AWARD_TITLE_01_COMBO_OPEN );
		m_pComboBoxAwardTitle02Open = CreateComboBox( "PLAY_STATS_ACHIEVEMENTS_INFO_AWARD_TITLE_02_OPEN", UI_FLAG_XSIZE | UI_FLAG_YSIZE, AWARD_TITLE_02_COMBO_OPEN );

		m_pComboBoxAwardTitle00RollOver = CreateComboBoxRollOver( "PLAY_STATS_ACHIEVEMENTS_INFO_AWARD_TITLE_00_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, AWARD_TITLE_00_COMBO_ROLLOVER );
		m_pComboBoxAwardTitle01RollOver = CreateComboBoxRollOver( "PLAY_STATS_ACHIEVEMENTS_INFO_AWARD_TITLE_01_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, AWARD_TITLE_01_COMBO_ROLLOVER );
		m_pComboBoxAwardTitle02RollOver = CreateComboBoxRollOver( "PLAY_STATS_ACHIEVEMENTS_INFO_AWARD_TITLE_02_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, AWARD_TITLE_02_COMBO_ROLLOVER );*/
	}

	{
		CreateLineBoxQuestList( "PLAY_STATS_COMBAT_RECORD_PVP_REGION_LINEBOX" );
		CreateLineBoxQuestList( "PLAY_STATS_COMBAT_RECORD_PVP_TITLE_LINEBOX" );

		// Over All
		CBasicLineBoxSmart* pLineBoxSmart7 = new CBasicLineBoxSmart();
		pLineBoxSmart7->CreateSub( this, "COMBATRECORD_OVERALL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart7->CreateSubControl( "COMBATRECORD_BLUELINE0" );
		RegisterControl( pLineBoxSmart7 );

		// Tyranny
		CBasicLineBoxSmart* pLineBoxSmart8 = new CBasicLineBoxSmart();
		pLineBoxSmart8->CreateSub( this, "COMBATRECORD_TYRANNY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart8->CreateSubControl( "COMBATRECORD_BLUELINE1" );
		RegisterControl( pLineBoxSmart8 );

		// Club Wars
		CBasicLineBoxSmart* pLineBoxSmart9 = new CBasicLineBoxSmart();
		pLineBoxSmart9->CreateSub( this, "COMBATRECORD_CLUBWARS", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart9->CreateSubControl( "COMBATRECORD_BLUELINE2" );
		RegisterControl( pLineBoxSmart9 );

		// Capture the flag
		CBasicLineBoxSmart* pLineBoxSmart10 = new CBasicLineBoxSmart();
		pLineBoxSmart10->CreateSub( this, "COMBATRECORD_CAPTURETF", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart10->CreateSubControl( "COMBATRECORD_BLUELINE3" );
		RegisterControl( pLineBoxSmart10 );

		// Club Death Match
		CBasicLineBoxSmart* pLineBoxSmart11 = new CBasicLineBoxSmart();
		pLineBoxSmart11->CreateSub( this, "COMBATRECORD_CLUBDM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart11->CreateSubControl( "COMBATRECORD_BLUELINE4" );
		RegisterControl( pLineBoxSmart11 );

		// School Wars
		CBasicLineBoxSmart* pLineBoxSmart12 = new CBasicLineBoxSmart();
		pLineBoxSmart12->CreateSub( this, "COMBATRECORD_SCHOOLWARSS", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart12->CreateSubControl( "COMBATRECORD_BLUELINE5" );
		RegisterControl( pLineBoxSmart12 );

		// PK Map
		CBasicLineBoxSmart* pLineBoxSmart13 = new CBasicLineBoxSmart();
		pLineBoxSmart13->CreateSub( this, "COMBATRECORD_PK_MAPP", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart13->CreateSubControl( "COMBATRECORD_BLUELINE6" );
		RegisterControl( pLineBoxSmart13 );


		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_TITLE_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVP_STATIC", 0 ), NS_UITEXTCOLOR::ORANGE );

		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_OVERALL_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVP_STATIC", 1 ), NS_UITEXTCOLOR::WHITE );
		m_pTextCombatRecordTotalKills		= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_OVERALL_KILLS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextCombatRecordTotalDeaths		= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_OVERALL_DEATHS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::RED );
		m_pTextCombatRecordTotalResu		= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_OVERALL_RESU", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GOLDENROD );

		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_TYRANNY_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVP_STATIC", 2 ), NS_UITEXTCOLOR::WHITE );
		m_pTextCombatRecordTyrannyKills		= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_TYRANNY_KILLS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextCombatRecordTyrannyDeaths	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_TYRANNY_DEATHS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::RED );
		m_pTextCombatRecordTyrannyResu		= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_TYRANNY_RESU", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GOLDENROD );

		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CLUB_WARS_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVP_STATIC", 3 ), NS_UITEXTCOLOR::WHITE );
		m_pTextCombatRecordClubWarsKills	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CLUB_WARS_KILLS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextCombatRecordClubWarsDeaths	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CLUB_WARS_DEATHS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::RED );
		m_pTextCombatRecordClubWarsResu		= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CLUB_WARS_RESU", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GOLDENROD );

		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CAPTURE_THE_FLAG_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVP_STATIC", 4 ), NS_UITEXTCOLOR::WHITE );
		m_pTextCombatRecordCaptureTheFlagKills	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CAPTURE_THE_FLAG_KILLS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextCombatRecordCaptureTheFlagDeaths	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CAPTURE_THE_FLAG_DEATHS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::RED );
		m_pTextCombatRecordCaptureTheFlagResu	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CAPTURE_THE_FLAG_RESU", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GOLDENROD );

		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CLUB_DEATH_MATCH_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVP_STATIC", 5 ), NS_UITEXTCOLOR::WHITE );
		m_pTextCombatRecordClubDeathMatchKills	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CLUB_DEATH_MATCH_KILLS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextCombatRecordClubDeathMatchDeaths	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CLUB_DEATH_MATCH_DEATHS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::RED );
		m_pTextCombatRecordClubDeathMatchResu	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_CLUB_DEATH_MATCH_RESU", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GOLDENROD );

		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_SCHOOL_WARS_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVP_STATIC", 6 ), NS_UITEXTCOLOR::WHITE );
		m_pTextCombatRecordSchoolWarsKills	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_SCHOOL_WARS_KILLS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextCombatRecordSchoolWarsDeaths	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_SCHOOL_WARS_DEATHS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::RED );
		m_pTextCombatRecordSchoolWarsResu	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_SCHOOL_WARS_RESU", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GOLDENROD );

		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_PK_MAP_STATIC", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVP_STATIC", 7 ), NS_UITEXTCOLOR::WHITE );
		m_pTextCombatRecordPKMapKills	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_PK_MAP_KILLS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextCombatRecordPKMapDeaths	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_PK_MAP_DEATHS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::RED );
		m_pTextCombatRecordPKMapResu	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVP_PK_MAP_RESU", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GOLDENROD );

	}

	{
		CreateLineBoxQuestList( "PLAY_STATS_COMBAT_RECORD_PVE_REGION_LINEBOX" );
		CreateLineBoxQuestList( "PLAY_STATS_COMBAT_RECORD_PVE_TITLE_LINEBOX" );

		// Monsters
		CBasicLineBoxSmart* pLineBoxSmart14 = new CBasicLineBoxSmart();
		pLineBoxSmart14->CreateSub( this, "PVECOMBATRECORD_MONSTERS", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart14->CreateSubControl( "PVECOMBATRECORD_BLUELINE0" );
		RegisterControl( pLineBoxSmart14 );

		// Gold Loots
		CBasicLineBoxSmart* pLineBoxSmart15 = new CBasicLineBoxSmart();
		pLineBoxSmart15->CreateSub( this, "PVECOMBATRECORD_GOLDLOOTS", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxSmart15->CreateSubControl( "PVECOMBATRECORD_BLUELINE1" );
		RegisterControl( pLineBoxSmart15 );


		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVE_TITLE_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVE_STATIC", 0 ), NS_UITEXTCOLOR::ORANGE );

		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVE_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVE_STATIC", 1 ), NS_UITEXTCOLOR::WHITE );
		m_pTextCombatRecordPVEKills		= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVE_KILLS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextCombatRecordPVEDeaths	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVE_DEATHS", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::RED );
		CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVE_GOLD_LOOT_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_PAGE_COMBAT_RECORD_PVE_STATIC", 2 ), NS_UITEXTCOLOR::WHITE );
		m_pTextCombatRecordPVEGoldLoot	= CreateTextBox( "PLAY_STATS_COMBAT_RECORD_PVE_GOLD_LOOT", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GOLDENROD );

	}

	{	// MMR - JX

		CreateLineBoxQuestList( "PLAY_STATS_MMR_INFO_REGION_LINEBOX" );	
		CreateUILineBoxWhiteNoBody( "PLAY_STATS_MMR_INFO_REGION_LINEBOX_WHITE" );	
		CreateUILineBoxWhiteNoBody( "PLAY_STATS_MMR_INFO_BADGE_LINEBOX_WHITE" );

		CreateTextBox( "PLAY_STATS_MMR_RANK_TEXT_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_MMR_RANKING_TEXTS", 0 ), NS_UITEXTCOLOR::WHITE );
		CreateTextBox( "PLAY_STATS_MMR_RANK_POINTS_STATIC", pFont10, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, ID2GAMEWORD( "PLAY_STATS_MMR_RANKING_TEXTS", 1 ), NS_UITEXTCOLOR::WHITE );


		m_pTextMMRRankName	= CreateTextBox( "PLAY_STATS_MMR_RANK_NAME_TEXT", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );
		m_pTextMMRPoints	= CreateTextBox( "PLAY_STATS_MMR_RANK_POINT_TEXT", pFont09, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW );

		//m_pImageMMRBadge = CreateCUIControl("PLAY_STATS_MMR_BADGE_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		//m_pImageMMRBadge->SetVisibleSingle(FALSE); // Initially hidden


		}

	for (int i = 0; i < 10; ++i)
	{
		CString mmrControlID;
		mmrControlID.Format("MMR_DISPLAY_%d", i + 1);

		m_pImageMMRBadges[i] = CreateCUIControl(std::string(CT2CA(mmrControlID)), UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		if (m_pImageMMRBadges[i])
		{
			m_pImageMMRBadges[i]->SetVisibleSingle(FALSE);
			RegisterControl(m_pImageMMRBadges[i]);
		}
	}


	m_pImageMMRBadge = CreateCUIControl("MMR_DISPLAY_1", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pImageMMRBadge->SetVisibleSingle(FALSE);  // Initially hidden
	RegisterControl(m_pImageMMRBadge);

	m_pMMRProgressBar = new CBasicProgressBar;
	m_pMMRProgressBar->CreateSub(this, "PLAY_STATS_MMR_PROGRESS_BAR");
	m_pMMRProgressBar->CreateOverImage("PLAY_STATS_MMR_PROGRESS_BAR_OVERIMAGE");
	RegisterControl(m_pMMRProgressBar);

	m_pMMRProgressText = CreateTextBox("PLAY_STATS_MMR_PROGRESS_TEXT", pFont09, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, "--", NS_UITEXTCOLOR::GREENYELLOW);
	

}

void CPlayStatsPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{

	default:
		break;
	}
}

void CPlayStatsPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

    m_fUpdateTimer += fElapsedTime;
    if (m_fUpdateTimer >= 1.0f)
    {
        m_fUpdateTimer = 0.0f;

        // Track previous MMR value for accurate badge refresh
        DWORD previousMMR = m_dwMMR;
        
        // Fetch the updated MMR value
        GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
        if (pCharacter) 
            m_dwMMR = pCharacter->m_dwMMR;

        // Only refresh the badge if MMR has changed
        if (m_dwMMR != previousMMR)
        {
            ResetMMRBadge();
            UpdateInfo();
        }
    }
}


void CPlayStatsPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		//LoadComboBox();
		UpdateInfo();
	}
}

HRESULT CPlayStatsPage::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	if ( !IsVisible () ) return S_OK;

	HRESULT hr = S_OK;

	if ( m_pImageBasicInfoClub )	m_pImageBasicInfoClub->SetVisibleSingle ( FALSE );
	{
		hr = CUIGroup::Render ( pd3dDevice );
		if ( FAILED ( hr ) ) return hr;
	}
	if ( m_pImageBasicInfoClub )	m_pImageBasicInfoClub->SetVisibleSingle ( TRUE );

	GLCLUB& sCLUB = GLGaeaClient::GetInstance().GetCharacter ()->m_sCLUB;

	if ( sCLUB.m_dwID != CLUB_NULL )
	{
		int nServer = static_cast<int>(GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID);
		int nID = static_cast<int>(sCLUB.m_dwID);
		DWORD dwVer = sCLUB.m_dwMarkVER;

		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( pd3dDevice, nServer, nID, dwVer );

		if ( m_pImageBasicInfoClub )
		{
			m_pImageBasicInfoClub->SetTexturePos ( 0, sMarkData.vTex_1_LU );
			m_pImageBasicInfoClub->SetTexturePos ( 1, sMarkData.vTex_2_RU );
			m_pImageBasicInfoClub->SetTexturePos ( 3, sMarkData.vTex_3_LD );
			m_pImageBasicInfoClub->SetTexturePos ( 2, sMarkData.vTex_4_RD );

			const UIRECT& rcGlobalPos = m_pImageBasicInfoClub->GetGlobalPos ();
			m_pImageBasicInfoClub->SetGlobalPos( UIRECT( ceil(rcGlobalPos.left), ceil(rcGlobalPos.top), ceil(rcGlobalPos.sizeX), ceil(rcGlobalPos.sizeY) ) );

			m_pImageBasicInfoClub->SetTexture ( sMarkData.pddsTexture );
			hr = m_pImageBasicInfoClub->Render ( pd3dDevice );
		}
	}
	else
	{
		if ( m_pImageBasicInfoClub )	m_pImageBasicInfoClub->SetVisibleSingle ( FALSE );
	}

	if (m_pImageMMRBadge)
	{
		m_pImageMMRBadge->Render(pd3dDevice);
	}


	return hr;
}

void CPlayStatsPage::UpdateInfo()
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter )	return;

	WORD wClassIndex = CharClassToIndex( pCharacter->m_emClass );
	WORD wSchoolIndex = pCharacter->m_wSchool;

	m_dwMMR = pCharacter->m_dwMMR; // Make sure pCharacter->m_dwMMR is the correct value

	ResetMMRBadge();

	CString strTemp("");

	if ( m_pTextBasicInformationName )
	{
		m_pTextBasicInformationName->ClearText();
		m_pTextBasicInformationName->SetText( pCharacter->m_szName, NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextBasicInformationSchool )
	{
		m_pTextBasicInformationSchool->ClearText();
		m_pTextBasicInformationSchool->SetText( GLCONST_CHAR::strSCHOOLNAME[wSchoolIndex].c_str(), NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextBasicInformationClass )
	{
		m_pTextBasicInformationClass->ClearText();
		m_pTextBasicInformationClass->SetText( COMMENT::CHARCLASS[wClassIndex].c_str(), NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextBasicInformationGuild )
	{
		m_pTextBasicInformationGuild->ClearText();

		if ( pCharacter->m_sCLUB.IsMember( pCharacter->m_dwCharID ) )
			m_pTextBasicInformationGuild->SetText( pCharacter->m_sCLUB.m_szName, NS_UITEXTCOLOR::GREENYELLOW );
		else
			m_pTextBasicInformationGuild->SetText( "None", NS_UITEXTCOLOR::GREENYELLOW );
	}

	for ( int i=0; i<INFO_SCHOOL_SIZE; ++i )
	{
		if ( m_pImageBasicInfoSchool[i] )
			m_pImageBasicInfoSchool[i]->SetVisibleSingle( FALSE );
	}

	for ( int i=0; i<INFO_CLASS_SIZE; ++i )
	{
		if ( m_pImageBasicInfoClass[i] )
			m_pImageBasicInfoClass[i]->SetVisibleSingle( FALSE );
	}

	if ( m_pImageBasicInfoSchool[wSchoolIndex] )
		m_pImageBasicInfoSchool[wSchoolIndex]->SetVisibleSingle( TRUE );

	if ( m_pImageBasicInfoClass[wClassIndex] )
		m_pImageBasicInfoClass[wClassIndex]->SetVisibleSingle( TRUE );


	/* pk combo, Juver, 2021/02/09 */
	for ( int i=0; i<EMPKCOMBO_COUNT_SIZE; ++i )
	{
		strTemp.Format( "%d", pCharacter->m_sPKComboCount.dwCount[i] );

		if ( m_pTextMultiKillStreak[i] )
		{
			m_pTextMultiKillStreak[i]->ClearText();
			m_pTextMultiKillStreak[i]->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
		}
	}


	/*activity point, Juver, 2017/08/23 */
	if ( m_pTextPlayInfoActivityPoints )
	{
		strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pCharacter->m_dwActivityPoint, 3, "," );
		m_pTextPlayInfoActivityPoints->ClearText();
		m_pTextPlayInfoActivityPoints->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}

	/*contribution point, Juver, 2017/08/23 */
	if ( m_pTextPlayInfoContributionPoints )
	{
		strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pCharacter->m_dwContributionPoint, 3, "," );
		m_pTextPlayInfoContributionPoints->ClearText();
		m_pTextPlayInfoContributionPoints->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}


	/* play time system, Juver, 2021/01/26 */
	if ( m_pTextPlayInfoPlayTime )
	{
		m_pTextPlayInfoPlayTime->ClearText();

		strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pCharacter->m_llPlayTime, 3, "," );
		m_pTextPlayInfoPlayTime->AddText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );

		__time64_t ttime = pCharacter->m_llPlayTime * 60; //play time is minutes
		CTimeSpan cTimeSpan( ttime );
		strTemp.Format ( "Days:%I64d Hours:%d Minutes:%d", cTimeSpan.GetDays(), cTimeSpan.GetHours(), cTimeSpan.GetMinutes() );
		m_pTextPlayInfoPlayTime->AddText ( strTemp, NS_UITEXTCOLOR::DARKORANGE );
	}

	/* play time system, Juver, 2021/01/26 */
	if ( m_pTextPlayInfoPlayPoints )
	{
		strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pCharacter->m_llPlayPoint, 3, "," );
		m_pTextPlayInfoPlayPoints->ClearText();
		m_pTextPlayInfoPlayPoints->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}

	const nscombat_record::SDATA& sCombatRecord = pCharacter->m_sCombatRecord;

	if ( m_pTextCombatRecordTotalKills )
	{
		m_pTextCombatRecordTotalKills->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwPVPKills );
		m_pTextCombatRecordTotalKills->SetOneLineText( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextCombatRecordTotalDeaths )
	{
		m_pTextCombatRecordTotalDeaths->ClearText();
		strTemp.Format( "D:%u", sCombatRecord.dwPVPDeaths );
		m_pTextCombatRecordTotalDeaths->SetOneLineText( strTemp, NS_UITEXTCOLOR::RED );
	}

	if ( m_pTextCombatRecordTotalResu )
	{
		m_pTextCombatRecordTotalResu->ClearText();
		strTemp.Format( "R:%u", sCombatRecord.dwPVPResu );
		m_pTextCombatRecordTotalResu->SetOneLineText( strTemp, NS_UITEXTCOLOR::GOLDENROD );
	}

	if ( m_pTextCombatRecordTyrannyKills )
	{
		m_pTextCombatRecordTyrannyKills->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwTyrannyKills );
		m_pTextCombatRecordTyrannyKills->SetOneLineText( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextCombatRecordTyrannyDeaths )
	{
		m_pTextCombatRecordTyrannyDeaths->ClearText();
		strTemp.Format( "D:%u", sCombatRecord.dwTyrannyDeaths );
		m_pTextCombatRecordTyrannyDeaths->SetOneLineText( strTemp, NS_UITEXTCOLOR::RED );
	}

	if ( m_pTextCombatRecordTyrannyResu )
	{
		m_pTextCombatRecordTyrannyResu->ClearText();
		strTemp.Format( "R:%u", sCombatRecord.dwTyrannyResu );
		m_pTextCombatRecordTyrannyResu->SetOneLineText( strTemp, NS_UITEXTCOLOR::GOLDENROD );
	}

	if ( m_pTextCombatRecordClubWarsKills )
	{
		m_pTextCombatRecordClubWarsKills->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwClubWarKills );
		m_pTextCombatRecordClubWarsKills->SetOneLineText( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextCombatRecordClubWarsDeaths )
	{
		m_pTextCombatRecordClubWarsDeaths->ClearText();
		strTemp.Format( "D:%u", sCombatRecord.dwClubWarDeaths );
		m_pTextCombatRecordClubWarsDeaths->SetOneLineText( strTemp, NS_UITEXTCOLOR::RED );
	}

	if ( m_pTextCombatRecordClubWarsResu )
	{
		m_pTextCombatRecordClubWarsResu->ClearText();
		strTemp.Format( "R:%u", sCombatRecord.dwClubWarResu );
		m_pTextCombatRecordClubWarsResu->SetOneLineText( strTemp, NS_UITEXTCOLOR::GOLDENROD );
	}

	if ( m_pTextCombatRecordCaptureTheFlagKills )
	{
		m_pTextCombatRecordCaptureTheFlagKills->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwCaptureTheFlagKills );
		m_pTextCombatRecordCaptureTheFlagKills->SetOneLineText( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextCombatRecordCaptureTheFlagDeaths )
	{
		m_pTextCombatRecordCaptureTheFlagDeaths->ClearText();
		strTemp.Format( "D:%u", sCombatRecord.dwCaptureTheFlagDeaths );
		m_pTextCombatRecordCaptureTheFlagDeaths->SetOneLineText( strTemp, NS_UITEXTCOLOR::RED );
	}

	if ( m_pTextCombatRecordCaptureTheFlagResu )
	{
		m_pTextCombatRecordCaptureTheFlagResu->ClearText();
		strTemp.Format( "R:%u", sCombatRecord.dwCaptureTheFlagResu );
		m_pTextCombatRecordCaptureTheFlagResu->SetOneLineText( strTemp, NS_UITEXTCOLOR::GOLDENROD );
	}

	if ( m_pTextCombatRecordClubDeathMatchKills )
	{
		m_pTextCombatRecordClubDeathMatchKills->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwClubDeathMatchKills );
		m_pTextCombatRecordClubDeathMatchKills->SetOneLineText( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextCombatRecordClubDeathMatchDeaths )
	{
		m_pTextCombatRecordClubDeathMatchDeaths->ClearText();
		strTemp.Format( "D:%u", sCombatRecord.dwClubDeathMatchDeaths );
		m_pTextCombatRecordClubDeathMatchDeaths->SetOneLineText( strTemp, NS_UITEXTCOLOR::RED );
	}

	if ( m_pTextCombatRecordClubDeathMatchResu )
	{
		m_pTextCombatRecordClubDeathMatchResu->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwClubDeathMatchResu );
		m_pTextCombatRecordClubDeathMatchResu->SetOneLineText( strTemp, NS_UITEXTCOLOR::GOLDENROD );
	}

	if ( m_pTextCombatRecordSchoolWarsKills )
	{
		m_pTextCombatRecordSchoolWarsKills->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwSchoolWarsKills );
		m_pTextCombatRecordSchoolWarsKills->SetOneLineText( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextCombatRecordSchoolWarsDeaths )
	{
		m_pTextCombatRecordSchoolWarsDeaths->ClearText();
		strTemp.Format( "D:%u", sCombatRecord.dwSchoolWarsDeaths );
		m_pTextCombatRecordSchoolWarsDeaths->SetOneLineText( strTemp, NS_UITEXTCOLOR::RED );
	}

	if ( m_pTextCombatRecordSchoolWarsResu )
	{
		m_pTextCombatRecordSchoolWarsResu->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwSchoolWarsResu );
		m_pTextCombatRecordSchoolWarsResu->SetOneLineText( strTemp, NS_UITEXTCOLOR::GOLDENROD );
	}

	if ( m_pTextCombatRecordPKMapKills )
	{
		m_pTextCombatRecordPKMapKills->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwPKMapKills );
		m_pTextCombatRecordPKMapKills->SetOneLineText( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextCombatRecordPKMapDeaths )
	{
		m_pTextCombatRecordPKMapDeaths->ClearText();
		strTemp.Format( "D:%u", sCombatRecord.dwPKMapDeaths );
		m_pTextCombatRecordPKMapDeaths->SetOneLineText( strTemp, NS_UITEXTCOLOR::RED );
	}

	if ( m_pTextCombatRecordPKMapResu )
	{
		m_pTextCombatRecordPKMapResu->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwPKMapResu );
		m_pTextCombatRecordPKMapResu->SetOneLineText( strTemp, NS_UITEXTCOLOR::GOLDENROD );
	}

	if ( m_pTextCombatRecordPVEKills )
	{
		m_pTextCombatRecordPVEKills->ClearText();
		strTemp.Format( "K:%u", sCombatRecord.dwPVEKills );
		m_pTextCombatRecordPVEKills->SetOneLineText( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextCombatRecordPVEDeaths )
	{
		m_pTextCombatRecordPVEDeaths->ClearText();
		strTemp.Format( "D:%u", sCombatRecord.dwPVEDeaths );
		m_pTextCombatRecordPVEDeaths->SetOneLineText( strTemp, NS_UITEXTCOLOR::RED );
	}

	if ( m_pTextCombatRecordPVEGoldLoot )
	{
		m_pTextCombatRecordPVEGoldLoot->ClearText();
		strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( sCombatRecord.llPVEMoneyGain, 3, "," );
		m_pTextCombatRecordPVEGoldLoot->AddText ( strTemp, NS_UITEXTCOLOR::GOLDENROD );
	}

	if (m_pTextMMRRankName)
    {
        m_pTextMMRRankName->ClearText();

        int rankIndex = CalculateVal(m_dwMMR);

        // Prevent out-of-bounds access
        if (rankIndex < 0 || rankIndex >= 10) rankIndex = 0;

        // Debugging log
        //printf("MMR Rank Index: %d, Rank Name: %s\n", rankIndex, GLCONST_CHAR::strRank_Name[rankIndex].c_str());

        CString strTemp;
        strTemp.Format("[%s]", GLCONST_CHAR::strRank_Name[rankIndex].c_str());
        m_pTextMMRRankName->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);
    }

    if (m_pTextMMRPoints)
    {
        m_pTextMMRPoints->ClearText();

        CString strTemp;
        strTemp.Format("%d", m_dwMMR); // Correct integer formatting
        m_pTextMMRPoints->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);
    }

	if (m_pMMRProgressBar)
	{
		UpdateMMRProgressBar();
	}

}


//MMR Rank
int CPlayStatsPage::CalculateVal(DWORD dwCalc)
{
    if (!GLCONST_CHAR::dwRank_Point) 
        return 0;

    for (int i = 9; i >= 0; --i)
    {
        if (dwCalc >= GLCONST_CHAR::dwRank_Point[i])
            return i;
    }
    return 0;
}
void CPlayStatsPage::ResetMMRBadge()
{
    int rankIndex = CalculateVal(m_dwMMR);
    if (rankIndex < 0 || rankIndex >= 10) 
        rankIndex = 0;

    for (int i = 0; i < 10; ++i)
    {
        if (m_pImageMMRBadges[i])
        {
            m_pImageMMRBadges[i]->SetVisibleSingle(i == rankIndex);
        }
    }

   // CDebugSet::ToFileWithTime("_MMRBADGE.txt", "[SUCCESS] Displaying Badge: MMR_DISPLAY_%d\n", rankIndex + 1);
}

void CPlayStatsPage::UpdateMMRProgressBar()
{
    if (!m_pMMRProgressBar || !m_pMMRProgressText) return;

    DWORD dwNextMMR = 0;
    DWORD dwPreviousMMR = 0;

    // Identify current and next rank thresholds
    for (int i = 0; i < 10; ++i)
    {
        if (m_dwMMR < GLCONST_CHAR::dwRank_Point[i])
        {
            dwNextMMR = GLCONST_CHAR::dwRank_Point[i];
            dwPreviousMMR = (i == 0) ? 0 : GLCONST_CHAR::dwRank_Point[i - 1];
            break;
        }
    }

    // Calculate progress as a percentage
    DWORD dwMMRInBetween = m_dwMMR - dwPreviousMMR;
    DWORD dwRange = dwNextMMR - dwPreviousMMR;
    float fPercent = (dwRange > 0) ? (float)dwMMRInBetween / dwRange : 0.0f;

    // Display values
    CString strMMRProgress;
    strMMRProgress.Format("%d / %d", dwMMRInBetween, dwRange);
    m_pMMRProgressText->SetOneLineText(strMMRProgress, NS_UITEXTCOLOR::GREENYELLOW);

    m_pMMRProgressBar->SetPercent(fPercent);
}
