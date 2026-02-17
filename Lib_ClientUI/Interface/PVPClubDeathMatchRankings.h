/*!
 * \file PVPSchoolWarsRankings.h
 *
 * \author Juver
 * \date January 2018
 *
 * 
 */
#pragma	once

#include "./UIWindowEx.h"

class CMultiModeButton;
class CBasicTextButton;
class CPVPClubDeathMatchRankingPage;
class CBasicTextBox;

class	CPVPClubDeathMatchRankings : public CUIWindowEx
{

	enum
	{
		PVP_CLUB_DEATH_MATCH_RANK_BUTTON_ALL = ET_CONTROL_NEXT,

		PVP_CLUB_DEATH_MATCH_RANK_PAGE_ALL,

		PVP_CLUB_DEATH_MATCH_RANK_BUTTON_CLOSE,
	};

	enum
	{
		PVP_CLUB_DEATH_MATCH_RANK_ALL = 0,
		PVP_CLUB_DEATH_MATCH_RANK_SIZE,
	};

public:
	CPVPClubDeathMatchRankings ();
	virtual ~CPVPClubDeathMatchRankings ();

public:
	void	CreateSubControl ();
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
public:
	CMultiModeButton*		m_pButtonRank[PVP_CLUB_DEATH_MATCH_RANK_SIZE];
	CPVPClubDeathMatchRankingPage*	m_pPageRanking[PVP_CLUB_DEATH_MATCH_RANK_SIZE];

	CBasicTextButton*		m_pButtonClose;
	CBasicTextBox*			m_pInfoExit;
	float					m_fUpdateTime;

public:
	int m_nPage;

public:
	void	SetVisiblePage( int nPage );
	void	UpdateRankings();
};