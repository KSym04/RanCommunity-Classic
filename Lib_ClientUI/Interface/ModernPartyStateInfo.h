#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"

class	CBasicTextBox;
class	CBasicProgressBar;

class	CModernPartyStateInfo : public CUIGroup
{
protected:


public:
	CModernPartyStateInfo ();
	virtual	~CModernPartyStateInfo ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	SetSchool ( int nSchoolType );
	void	SetPlayerName ( CString strName );
	void	ApplyText();
	void	SetDie( bool bDIE );
	void	SetHP ( float fPercent, int nClassType );
	void	SetHP ( CString str );

protected:
	CUIControl*		CreateControl ( const char* szControl );

private:
	CBasicProgressBar*	m_pHP[GLCI_NUM_8CLASS];
	CUIControl*		m_pSchool[3];
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextHP;

private:
	bool	m_bDie;
	CString m_strName;
	CString m_strHP;

};