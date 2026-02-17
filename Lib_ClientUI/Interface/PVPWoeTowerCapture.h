#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "../Lib_Client/G-Logic/GLPVPWoeData.h"

class CBasicTextBox;
class CPVPWoeTowerCaptureGuild;
class GLCLUB;
const DWORD UIMSG_PVPWOE_TOWER_CAPTURE_LBDUP = UIMSG_USER1;

class CPVPWoeTowerCapture : public CUIGroup
{
private:
	enum
	{
		WOE_TOWER_CAPTURE_BG = NO_ID + 1
	};

	
public:
	CPVPWoeTowerCapture();
	virtual	~CPVPWoeTowerCapture();

private:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	virtual HRESULT InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);
	virtual HRESULT Render(LPDIRECT3DDEVICEQ pd3dDevice);

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
	
	CBasicTextBox*	m_pGuildName;
	CBasicTextBox* m_pHolderName;
	CBasicTextBox*	m_pTextTimer;

	CUIControl* m_pBackGround;
	CUIControl* m_pClubDefault;
	CUIControl* m_pClubMark;

	DWORD		m_dwClubID;
	DWORD		m_dwEmperiumMarkVer;

	std::string m_strGuildName;
	
	GLCLUB* pclub;

public:
	void UpdateInfo();

};