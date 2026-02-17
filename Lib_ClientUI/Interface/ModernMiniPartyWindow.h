#pragma	once

#include "./UIWindowEx.h"
#include "../../Lib_Client/G-Logic/GLParty.h"


class	CModernMiniPartyExpeditionPage;

class	CModernMiniPartyWindow : public CUIWindowEx
{
protected:
	

public:
	CModernMiniPartyWindow ();
	virtual	~CModernMiniPartyWindow ();

public:
	void CreateSubControl ();

private:
	CUIControl*			m_pBackGround;
	CModernMiniPartyExpeditionPage*	m_pPartyPage;

private:
	

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:

};