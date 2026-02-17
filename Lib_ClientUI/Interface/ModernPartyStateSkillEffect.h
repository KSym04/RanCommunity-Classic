#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"
#include "../../Lib_Client/G-Logic/GLCharData.h"

struct	GLPARTY_CLIENT;

class	CModernPartyStateSkillEffect : public CUIGroup
{
protected:
	enum
	{
		PARTY_STATE_SKILLEFFECT_DUMMY_BASE = NO_ID + 1,
		PARTY_STATE_SKILLEFFECT_UNIT = 1000,
		PARTY_STATE_SKILLEFFECT_UNIT_END = PARTY_STATE_SKILLEFFECT_UNIT + 10000,
	};


public:
	CModernPartyStateSkillEffect ();
	virtual	~CModernPartyStateSkillEffect ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	void	SetPartyID ( const int nPartyID );

protected:

private:
	typedef	std::pair<DWORD,UIGUID>			SKILLTIME_PAIR;
	typedef	std::list<SKILLTIME_PAIR>		SKILLTIME_DISPLAY_LIST;
	typedef	SKILLTIME_DISPLAY_LIST::iterator	SKILLTIME_DISPLAY_LIST_ITER;

	SKILLTIME_DISPLAY_LIST	m_SkillPlayList;
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	UIGUID	m_NEWID;
	int		m_nPartyID;

private:
	CUIControl*		m_pSkillUnitDummy[SKILLFACT_SIZE];

private:
	UIGUID	GET_EXIST_PLAY_SKILL_CONTROL ( const DWORD& dwSkillID );
	bool	GET_PLAY_SKILLFACT ( const DWORD& dwSkillID, SSKILLFACT& sSKILLFACT );
	bool	NEW_PLAY_SKILL ( const DWORD& dwSkillID, const WORD wLEVEL );
	bool	GET_SKILL_LIFE_TIME ( const DWORD& dwSkillID, const WORD wLEVEL, float& fLifeTime );
	GLPARTY_CLIENT*		FindSelfClient ();
};