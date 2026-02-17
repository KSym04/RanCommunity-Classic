#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"
#include "../../Lib_Client/G-Logic/GLParty.h"

const DWORD UIMSG_MOUSEIN_TOMOVE = UIMSG_USER1;

struct	GLPARTY_CLIENT;
class	CBasicTextBox;
class	CModernPartyStateSlot;
class	CModernPartyStateSkillEffect;

class	CModernPartyState : public CUIGroup
{
protected:
	enum
	{
		PARTY_STATE_MEMBER0 = NO_ID + 1,
		PARTY_STATE_MEMBER1,
		PARTY_STATE_MEMBER2,
		PARTY_STATE_MEMBER3,
		PARTY_STATE_MEMBER4,
		PARTY_STATE_MEMBER5,
		PARTY_STATE_MEMBER6,
		PARTY_STATE_MEMBER7,
	};

public:
	CModernPartyState ();
	virtual	~CModernPartyState ();

public:
	void	CreateSubControl ();
	void	CheckMouseState ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:

protected:

private:
	CModernPartyStateSlot*			m_pSlot[MAXPARTY];
	CModernPartyStateSkillEffect*	m_pSkill[MAXPARTY];

private:
	void	SetSlotMember ( CModernPartyStateSlot* pSlot, CModernPartyStateSkillEffect* pEff, GLPARTY_CLIENT *pMember, BOOL bSameMap );
	BOOL	IsSameMap ( GLPARTY_CLIENT *pSelf, GLPARTY_CLIENT *pMember );
	GLPARTY_CLIENT*		FindSelfClient ();

private:
	BOOL		m_bFirstGap;
	D3DXVECTOR2	m_vGap;
	int			m_PosX;
	int			m_PosY;
	bool		m_bCHECK_MOUSE_STATE;

};