/*!
 * \file PVPPBGRankings.h
 *
 * \author Juver
 * \date 2023/01/12
 *
 * 
 */

#ifndef PVPPBGRANKINGS_H_INCLUDED__
#define PVPPBGRANKINGS_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./UIWindowEx.h"

class CMultiModeButton;
class CBasicTextButton;
class CPVPPBGRankingPage;
class CBasicTextBox;

class CPVPPBGRankings : public CUIWindowEx
{

	enum
	{
		RANK_BUTTON_ALL = ET_CONTROL_NEXT,

		RANK_PAGE_ALL,

		RANK_BUTTON_CLOSE,
	};

	enum
	{
		RANK_ALL,
		RANK_SIZE
	};

public:
	CPVPPBGRankings ();
	virtual ~CPVPPBGRankings ();

public:
	void	CreateSubControl ();
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
public:
	CMultiModeButton*		m_pButtonRank[RANK_SIZE];
	CPVPPBGRankingPage*		m_pPageRanking[RANK_SIZE];
	CBasicTextButton*		m_pButtonClose;
	CBasicTextBox*			m_pInfoExit;
	float					m_fUpdateTime;

public:
	int m_nPage;

public:
	void	SetVisiblePage( int nPage );
	void	UpdateRankings();
};

#endif // PVPPBGRANKINGS_H_INCLUDED__