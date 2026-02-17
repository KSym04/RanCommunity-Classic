#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/GLPersonalLock.h"

const DWORD UIMSG_PERSONAL_LOCK_LBDUP = UIMSG_USER1;

class	CPersonalLockImage;
class	CItemShopButton;

/* party finder, Juver, 2019/12/28 */
class	CPartyFinderIcon;

class CPersonalLockInfo : public CUIGroup
{
private:
	enum
	{
		IMAGE_DUMMY = NO_ID + 1,
		IMAGE_00,
		IMAGE_01,
		IMAGE_02,

		/* party finder, Juver, 2019/12/28 */
		PARTY_FINDER_ICON,	
		//ITEMSHOP_BUTTON, // Item Mall - JX
	};

	enum
	{
		IMAGE_SIZE = 3,
	};

public:
	CPersonalLockInfo ();
	virtual	~CPersonalLockInfo ();

public:
	void	CreateSubControl ();

public:
	void	CheckMouseState ();

	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CUIControl*			m_pImageBase;
	CUIControl*			m_pImageDummy;

	CPersonalLockImage* m_pImage[IMAGE_SIZE];

	/* party finder, Juver, 2019/12/28 */
	CPartyFinderIcon*	m_pPartyFinderIcon;
	CItemShopButton*	m_pItemShopButton; // Item Mall - JX

private:
	BOOL		m_bFirstGap;
	D3DXVECTOR2	m_vGap;

	int			m_PosX;
	int			m_PosY;

	bool		m_bCHECK_MOUSE_STATE;

public:
	void		CheckLockStatus();
	EMPERSONAL_LOCK ImageToEnum( int nIndex );
};