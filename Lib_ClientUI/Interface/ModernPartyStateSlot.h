#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"

const DWORD UIMSG_MOUSEIN_ICON = UIMSG_USER2;

class	CModernPartyStateInfo;
class	CModernPartyStateClassIcon;

class	CModernPartyStateSlot : public CUIGroup
{
protected:
	enum
	{
		PARTY_STATE_SLOT_ICON = NO_ID + 1,
	};


public:
	CModernPartyStateSlot ();
	virtual	~CModernPartyStateSlot ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	SetClass ( int nClassType, BOOL bSameMap );
	void	SetDie( bool bDIE );
	void	SetSchool ( int nSchoolType );
	void	SetPlayerName ( CString strName );
	void	SetHP ( float fPercent, int nClassType );
	void	SetHP ( CString str );
	void	SetMaster( bool bMaster );

private:
	CModernPartyStateInfo*		m_pInfo;
	CModernPartyStateClassIcon*	m_pIcon;
};