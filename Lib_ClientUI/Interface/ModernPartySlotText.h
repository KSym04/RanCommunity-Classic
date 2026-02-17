#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"

class	CBasicTextBox;

#define MODERN_PARTY_SLOT_TEXT_UPDATE_TIME 0.2f

class	CModernPartySlotText : public CUIGroup
{
protected:


public:
	CModernPartySlotText ();
	virtual	~CModernPartySlotText ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	void	SetSchool( int nSchool );
	void	SetPlayerName ( CString strName );
	void	SetPlayerPos ( CString strPos );
	void	SetMapName ( CString strMapName );
	void	SetLevel( CString strLevel );
	void	ApplyText();
	void	RefreshText( bool bSel );
	void	Reset();
	void	SetDie( bool bDIE );

protected:
	CUIControl*		CreateControl ( const char* szControl );

private:
	float	m_fUpdateTime;

	CString m_strName;
	CString m_strPos;
	CString m_strMap;
	CString m_strLevel;
	bool	m_bDie;
	bool	m_bSel;

private:
	CUIControl*		m_pSchool[3];
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextLevel;
	CBasicTextBox*	m_pTextPos;
	CBasicTextBox*	m_pTextMap;
};