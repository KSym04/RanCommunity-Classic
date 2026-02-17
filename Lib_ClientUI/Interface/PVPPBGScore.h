#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class CBasicTextBox;
class CBasicLineBoxEx;

const DWORD UIMSG_PVP_PBG_SCORE_WINDOW_LBDUP = UIMSG_USER1;

class CPVPPBGScore : public CUIGroup
{
private:
	enum
	{
		PVP_PBG_SCORE_BG = NO_ID + 1,
	};

public:
	CPVPPBGScore ();
	virtual	~CPVPPBGScore ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

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

public:
	CBasicLineBoxEx*	m_pLineBoxBG;

	CBasicTextBox*		m_pTextName;
	CBasicTextBox*		m_pTextKills;
	CBasicTextBox*		m_pTextDeaths;

	CBasicTextBox*		m_pTextTime;

public:
	void	UpdateInfo();

};