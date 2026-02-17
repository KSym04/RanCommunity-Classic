#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class CBasicTextBox;
class CBasicLineBoxEx;

const DWORD UIMSG_PVP_CLUB_DEATH_MATCH_SCORE_WINDOW_LBDUP = UIMSG_USER1;

class CPVPClubDeathMatchScore : public CUIGroup
{
private:
	enum
	{
		PVP_CLUB_DEATH_MATCH_SCORE_BG = NO_ID + 1,
	};

public:
	CPVPClubDeathMatchScore ();
	virtual	~CPVPClubDeathMatchScore ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	void	CheckMouseState();
	void	CreateSubControl ();

private:
	BOOL		m_bCHECK_MOUSE_STATE;
	BOOL		m_bFirstGap;
	D3DXVECTOR2	m_vGap;
	int			m_nPosX;
	int			m_nPosY;
	float		m_fUpdateTime;
	DWORD		m_dwLastTopClub;

public:
	CBasicLineBoxEx*	m_pLineBoxBG;

	CBasicTextBox*		m_pTextFirstRank;
	CBasicTextBox*		m_pTextFirstName;
	CBasicTextBox*		m_pTextFirstScore;
	CUIControl*			m_pFirstClubImage;

	CBasicTextBox*		m_pTextMyRank;
	CBasicTextBox*		m_pTextMyName;
	CBasicTextBox*		m_pTextMyScore;
	CUIControl*			m_pMyClubImage;

	CBasicTextBox*		m_pTextState;
	CBasicTextBox*		m_pTextTime;


public:
	void	UpdateInfo();

};