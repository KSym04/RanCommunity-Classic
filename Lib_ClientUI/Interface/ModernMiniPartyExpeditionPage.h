#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLParty.h"

class	CModernMiniPartySlot;
struct	GLPARTY_CLIENT;
class	CPartyBuffInfo;

class	CModernMiniPartyExpeditionPage : public CUIGroup
{
protected:
	enum
	{
		MINIPARTY_SLOT_MEMBER0 = NO_ID + 1,
		MINIPARTY_SLOT_MEMBER1,
		MINIPARTY_SLOT_MEMBER2,
		MINIPARTY_SLOT_MEMBER3,
		MINIPARTY_SLOT_MEMBER4,
		MINIPARTY_SLOT_MEMBER5,
		MINIPARTY_SLOT_MEMBER6,
		MINIPARTY_SLOT_MEMBER7,

		MINIPARTY_BUFF_INFO,
	};

public:
	CModernMiniPartyExpeditionPage ();
	virtual	~CModernMiniPartyExpeditionPage ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:

protected:

private:
	CModernMiniPartySlot*		m_pPartySlot[MAXPARTY];
	CPartyBuffInfo*		m_pPartyBuffInfo;
	bool				m_bMOUSEIN;
	

private:
	GLPARTY_CLIENT*		FindSelfClient ();
	BOOL	IsSameMap ( GLPARTY_CLIENT *pSelf, GLPARTY_CLIENT *pMember );
	void	SetSlotMember ( CModernMiniPartySlot* pPartySlot, GLPARTY_CLIENT *pMember, BOOL bSameMap, const int nPartySlotID );
	void	SetSelect( int nSelect, bool bNone = false );

};