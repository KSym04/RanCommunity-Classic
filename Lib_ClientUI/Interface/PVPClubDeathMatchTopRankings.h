/*!
 * \file PVPSchoolWarsTopRankings.h
 *
 * \author Juver
 * \date January 2018
 *
 * 
 */

#pragma	once

#include "./UIWindowEx.h"

#include "../../Lib_Client/G-Logic/PVPClubDeathMatchDefine.h"

class CPVPClubDeathMatchTopRankingsSlot;
class CBasicTextButton;
class CBasicTextBox;

class	CPVPClubDeathMatchTopRankings : public CUIWindowEx
{
	enum
	{
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_BUTTON_CLOSE = ET_CONTROL_NEXT,
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_0,
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_1,
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_2,
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_3,
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_4,
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_5,
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_6,
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_7,
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_8,
		PVP_CLUB_DEATH_MATCH_TOP_RANKINGS_SLOT_9,
	};

public:
	CPVPClubDeathMatchTopRankings ();
	virtual ~CPVPClubDeathMatchTopRankings ();

public:
	void	CreateSubControl ();
	CBasicTextBox*	CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign );

public:
	CBasicTextBox*		m_pRankText;
	CBasicTextBox*		m_pClubText;
	CBasicTextBox*		m_pPointText;

	CPVPClubDeathMatchTopRankingsSlot*	m_pSlot[PVP_CLUB_DEATH_MATCH_RANK_TOP];

	CBasicTextButton*	m_pButtonClose;

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

public:
	void	SetVisiblePage( int nPage );
	void	UpdateRankings();
};