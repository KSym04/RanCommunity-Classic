#ifndef PVPCLUBWARRANKINGWINDOW_H__INCLUDED
#define PVPCLUBWARRANKINGWINDOW_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_ClientUI/Interface/UIWindowEx.h"

class CMultiModeButton;
class CPVPClubWarRankingPage;


class	CPVPClubWarRankingWindow : public CUIWindowEx
{
	enum
	{
		PVPCLUB_WAR_RANKING_WINDOW_BUTTON_0 = ET_CONTROL_NEXT,
		PVPCLUB_WAR_RANKING_WINDOW_BUTTON_1,
		PVPCLUB_WAR_RANKING_WINDOW_BUTTON_2,
		PVPCLUB_WAR_RANKING_WINDOW_BUTTON_3,
		PVPCLUB_WAR_RANKING_WINDOW_PAGE_0,
		PVPCLUB_WAR_RANKING_WINDOW_PAGE_1,
		PVPCLUB_WAR_RANKING_WINDOW_PAGE_2,
		PVPCLUB_WAR_RANKING_WINDOW_PAGE_3,
	};

	enum
	{
		PVPCLUB_WAR_RANKING_WINDOW_MAX_PAGE = 4,
	};

public:
	CPVPClubWarRankingWindow();
	virtual ~CPVPClubWarRankingWindow();

public:
	void	CreateSubControl ();

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

	void	SetVisiblePage( int nPage );
	void	SetData();

public:
	int m_nPage;
	CMultiModeButton*			m_pButton[PVPCLUB_WAR_RANKING_WINDOW_MAX_PAGE];
	CPVPClubWarRankingPage*		m_pPage[PVPCLUB_WAR_RANKING_WINDOW_MAX_PAGE];
};

#endif // PVPCLUBWARRANKINGWINDOW_H__INCLUDED
