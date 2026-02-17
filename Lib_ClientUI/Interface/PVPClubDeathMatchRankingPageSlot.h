#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/PVPClubDeathMatchData.h"
#include "../../Lib_Client/G-Logic/PVPClubDeathMatchDefine.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"

class CBasicTextBox;
class CBasicButton;

class CPVPClubDeathMatchRankingPageSlot : public CUIGroup
{
private:
	enum
	{
		PVP_CLUB_DEATH_MATCH_RANKING_PAGE_SLOT_BUTTON_INFO = NO_ID + 1,
	};

public:
	CPVPClubDeathMatchRankingPageSlot ();
	virtual	~CPVPClubDeathMatchRankingPageSlot ();

public:
	void	CreateSubControl( BOOL bSelf );

public:
	CBasicTextBox*	m_pTextRank;
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextMaster;
	CBasicTextBox*	m_pTextPoint;
	CBasicTextBox*	m_pTextKill;
	CBasicTextBox*	m_pTextDeath;
	CBasicTextBox*	m_pTextResu;

	CUIControl*		m_pImageClub;
	CBasicButton*	m_pInfoButton;

	DWORD			m_dwClubID;
	DWORD			m_dwClubMarkVer;

public:
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

public:
	BOOL	m_bSelf;
	void	ResetData();
	void	SetData( PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK* pData );

};