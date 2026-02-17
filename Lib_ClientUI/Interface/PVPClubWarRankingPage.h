#ifndef PVPCLUBWARRANKINGPAGE_H__INCLUDED
#define PVPCLUBWARRANKINGPAGE_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLPVPClubWarData.h"

class CBasicTextBox;
class CBasicScrollBarEx;
class CPVPClubWarRankingPageSlot;

class CPVPClubWarRankingPage : public CUIGroup
{
private:
	enum
	{
		PVP_CLUB_WAR_RANKING_PAGE_SCROLLBAR = NO_ID + 1,
	};

	enum
	{
		PVP_CLUB_WAR_RANKING_PAGE_MAX_SLOT = 10,
	};

public:
	CPVPClubWarRankingPage ();
	virtual	~CPVPClubWarRankingPage ();

public:
	CBasicTextBox*	CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign );
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	int	m_nType;
	int	m_nStart;
	int	m_nTotal;

	CBasicTextBox*	m_pTextRank;
	CBasicTextBox*	m_pTextSchool;
	CBasicTextBox*	m_pTextClass;
	CBasicTextBox*	m_pTextGuild;
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextKill;
	CBasicTextBox*	m_pTextDeath;
	CBasicTextBox*	m_pTextResu;
	CBasicTextBox*	m_pTextScore;
	CBasicTextBox*	m_pTextPoint;

	CPVPClubWarRankingPageSlot*	m_pSlotRank[PVP_CLUB_WAR_RANKING_PAGE_MAX_SLOT];
	CPVPClubWarRankingPageSlot*	m_pSlotRankSelf;
	CBasicScrollBarEx*			m_pScrollBar;

	CBasicTextBox*	m_pTextGuildOwner;
	CUIControl*		m_pImageGuildOwner;
	CBasicTextBox*	m_pTextWinnerBonus;

public:
	SPVP_CLUB_WAR_CLIENT_DATA*	m_pData;

	void SetData( SPVP_CLUB_WAR_CLIENT_DATA* pData );
	void RefreshRanking();
	void RenderView();
};

#endif // PVPCLUBWARRANKINGPAGE_H__INCLUDED