#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/PVPClubDeathMatchData.h"
#include "../../Lib_Client/G-Logic/PVPClubDeathMatchDefine.h"

class CBasicTextBox;

class CPVPClubDeathMatchTopRankingsSlot : public CUIGroup
{
private:
	enum
	{
	};

public:
	CPVPClubDeathMatchTopRankingsSlot ();
	virtual	~CPVPClubDeathMatchTopRankingsSlot ();

public:
	void	CreateSubControl ();

public:
	CBasicTextBox*	m_pTextRank;
	CBasicTextBox*	m_pTextName;
	CUIControl*		m_pImageClub;
	CBasicTextBox*	m_pTextPoint;

	DWORD			m_dwClubID;
	DWORD			m_dwClubMarkVer;

public:
	void	ResetData();
	void	SetData( PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK* pData );
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );
};