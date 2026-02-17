#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/GLPVPTyrannyData.h"

class	CBasicProgressBar;

class CPVPTyrannyTowerProgress : public CUIGroup
{
private:
	enum
	{
	};

public:
	CPVPTyrannyTowerProgress ();
	virtual	~CPVPTyrannyTowerProgress ();

public:
	void	CreateSubControl ();
	virtual	HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );

	CBasicProgressBar*	m_pTowerStatus[TYRANNY_SCHOOL_SIZE];
	CUIControl*			m_pTowerOwner[TYRANNY_SCHOOL_SIZE];
	CUIControl*			m_pImageClub;

	DWORD				m_dwClubID;
	DWORD				m_dwClubMarkVer;

public:
	void	UpdateInfo( WORD wOwner, float* fDamage, DWORD dwClubID, DWORD dwClubMarkVer );	
}; 