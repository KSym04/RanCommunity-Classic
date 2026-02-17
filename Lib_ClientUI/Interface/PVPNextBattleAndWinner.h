#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLPVPTyrannyData.h"

class CBasicTextBox;

const DWORD UIMSG_PVPTYRANNY_NEXT_BATTLE_WINNER_LBDUP = UIMSG_USER1;

class CPVPNextBattleAndWinner : public CUIGroup
{
private:
	enum
	{
		TOWER_STATUS_BOX1 = NO_ID + 1,
	};

	enum
	{
		TOWER_STATUS_TYPES = TYRANNY_TOWER_SIZE+1,
	};

public:
	CPVPNextBattleAndWinner ();
	virtual	~CPVPNextBattleAndWinner ();

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
	CBasicTextBox*	m_pTextTower0;
	CBasicTextBox*	m_pTextTower1;
	CBasicTextBox*	m_pTextTower2;

	CBasicTextBox*	m_pTextStatus;
	CBasicTextBox*	m_pTextStatusNextBattle;
	CBasicTextBox*	m_pTextRewardProgress;
	CBasicTextBox*	m_pTextRewardProgressTimer;
	CBasicTextBox*	m_pTextRegistrationProgress;
	CBasicTextBox*	m_pTextRegistrationProgressTimer;
	CBasicTextBox*	m_pTextTimer;
	CBasicTextBox*	m_pTextTimerNextBattle;
	CUIControl*		m_pStatusWinner[TOWER_STATUS_TYPES];

public:
	void	UpdateInfo();

};