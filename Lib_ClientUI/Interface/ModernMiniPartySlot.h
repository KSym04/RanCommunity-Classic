#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"
#include "../../Lib_Client/G-Logic/GLParty.h"

class	CBasicTextBox;
class	CBasicProgressBar;
class	CBasicLineBox;

class	CModernMiniPartySlot : public CUIGroup
{
protected:

private:
	CBasicTextBox*		m_pPlayerName;
	CBasicTextBox*		m_pPlayerLevel;
	CBasicTextBox*		m_pPlayerSlotText;
	CUIControl*			m_pSchool[3];
	CUIControl*			m_pSlotMaster;
	CUIControl*			m_pSlotSubMaster;
	CBasicProgressBar*	m_pHP[GLCI_NUM_8CLASS];
	CBasicLineBox*		m_pSelect;

public:
	CModernMiniPartySlot ();
	virtual	~CModernMiniPartySlot ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	void	SetPlayerName ( CString strName )	{ m_strName  = strName; }
	void	SetPlayerLevel ( CString strLevel )	{ m_strLevel  = strLevel; }
	void	SetHP ( float fPercent, int nClassType );
	void	SetNumber ( int nSlotNumber );
	void	SetSchool ( int nSchoolType );
	void	SetSelect( bool b );
	void	SetDie( bool b );
	void	SetMaster( bool bMaster );

private:
	CString	m_strName;
	CString m_strLevel;
	bool	m_bDie;
	bool	m_bSel;

private:
	void	ApplyText();

protected:
	CUIControl*		CreateControl ( const char* szControl );
	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR D3DCOLOR, int nAlign );
};