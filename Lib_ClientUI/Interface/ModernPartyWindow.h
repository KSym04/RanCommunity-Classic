
#pragma	once

#include "./UIWindowEx.h"
#include "../../Lib_Client/G-Logic/GLParty.h"

#include "./ModernPartyWindowPageParty.h"

struct	GLPARTY_CLIENT;
class CMultiModeButton;
class CModernPartyWindowPageParty;

class	CModernPartyWindow : public CUIWindowEx
{
protected:
	enum
	{
		PARTY_TAB = ET_CONTROL_NEXT,
		PAGE_PARTY,
		HELP_BUTTON
	};

public:
	CModernPartyWindow ();
	virtual	~CModernPartyWindow ();

public:
    void	CreateSubControl ();
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );

private:
	CUIControl*						m_pBackGround;
	CMultiModeButton*				m_pPARTYTAB;
	CModernPartyWindowPageParty*	m_pPageParty;
	CBasicButton*					m_pHelp;

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	SetPartyInfo ( DWORD dwPartyID, DWORD dwMasterID )	{	m_pPageParty->SetPartyInfo( dwPartyID, dwMasterID );	}
	DWORD	GetPartyID ()	{	return m_pPageParty->GetPartyID();		}
	DWORD	GetMasterID()	{	return m_pPageParty->GetMasterID();	}

};