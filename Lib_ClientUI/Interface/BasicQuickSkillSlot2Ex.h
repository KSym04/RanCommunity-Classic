/*!
 * \file SkillTrayTabEx.h
 *
 * \author Juver
 * \date August 2018
 *
 * modern skill tray
 */

#pragma	once

#include "./BasicQuickSkillSlot2.h"
#include "./SkillTrayTab.h"

class	CBasicTextBox;

class	CBasicQuickSkillSlot2Ex : public CBasicQuickSkillSlot2
{
private:
static	const	int		nNOT_INIT;

public:
	CBasicQuickSkillSlot2Ex ();
	virtual	~CBasicQuickSkillSlot2Ex ();

public:
	virtual	void	CreateSubControl ( INT nTabIndex );
	void	CreateNumberText ( CD3DFontPar* pFont8, int nNumber );

	INT GetTabIndex()			{ return m_nTabIndex; }
	void	SetShotcutText( CString strTemp );

public:
	virtual void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );	

private:
	int				m_nNumber;
	CBasicTextBox*	m_pNumberText;
	INT				m_nTabIndex;
};