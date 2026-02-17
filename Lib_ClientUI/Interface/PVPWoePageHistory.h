#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "../Lib_Client/G-Logic/GLPVPWoeData.h"

class CBasicTextBox;
class CBasicLineBox;
class CD3DFontPar;
class CBasicButton;
class CPVPWoePageHistoryBuff;

class CPVPWoePageHistory : public CUIGroup
{
private:
	enum
	{
		WOE_PAGE_INFO_HISTORY_BUTTON_RANKING = NO_ID + 1,
		WOE_PAGE_INFO_HISTORY_BUFF,
	};

public:
	CPVPWoePageHistory ();
	virtual	~CPVPWoePageHistory ();

public:
	void	CreateSubControl ();
	CBasicTextBox*	CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign );
	CUIControl*		CreateUIControl( const char* szControl );

public:
	CBasicTextBox*		m_pTextHistoryTitleStatic;
	CBasicTextBox*		m_pTextHistoryDurationStatic;
	CBasicTextBox*		m_pTextHistoryNameStatic;
	CBasicTextBox*		m_pTextHistoryBadgeStatic;
	CBasicTextBox*		m_pTextHistoryLeaderStatic;

	CBasicTextBox*		m_pTextHistoryTimeText;
	CBasicTextBox*		m_pTextHistoryNameText;
	CBasicTextBox*		m_pTextHistoryLeaderText;

	CUIControl*			m_pClubMark;

	std::string m_strName;
	std::string m_strLeader;
	DWORD		m_dwClubID;
	DWORD		m_dwMarkVer;

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

	float	m_fUpdateTime;

	void	ShowInfo();

private:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	virtual HRESULT InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);
	virtual HRESULT Render(LPDIRECT3DDEVICEQ pd3dDevice);
};