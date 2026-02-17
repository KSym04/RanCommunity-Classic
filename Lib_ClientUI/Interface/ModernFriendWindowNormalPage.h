#pragma	once

#include "./UIWindowEx.h"
#include "../../Lib_Client/G-Logic/GLFriendClient.h"

class	CBasicTextBoxEx;
class	CBasicScrollBarEx;
class	CMultiModeButton;
class	CUIEditBox;

class	CModernFriendWindowNormalPage : public CUIGroup
{
private:
static	const int nLIMIT_CHAR;

private:
	enum
	{
		FRIEND_LIST_TEXTBOX = NO_ID + 1,
		FRIEND_LIST_SCROLLBAR,
		FRIEND_LIST_ADDBUTTON,
		FRIEND_LIST_DELBUTTON,
		FRIEND_LIST_BLOCKBUTTON,
		FRIEND_LIST_MOVEBUTTON,
		FRIEND_LIST_PARTYBUTTON,
		FRIEND_LIST_WHISPERBUTTON,
		FRIEND_EDITBOX
	};

	enum
	{
		ONLINE = 0,
		OFFLINE
	};

public:
	CModernFriendWindowNormalPage ();
	virtual ~CModernFriendWindowNormalPage ();

public:
	void	CreateSubControl ();
	CMultiModeButton* CreateButton( char* szControl, const char* szText, const UIGUID& cID = NO_ID );

public:
	void	LoadFriendList ();
	void	EDIT_END ();
	void	EDIT_BEGIN ( const CString& strName = "" );

public:
	void	ADD_FRIEND ( const CString& strName );
	void	DEL_FRIEND ( const CString& strName );
	void	ADD_BLOCK ( const CString& strName );
	void	MOVETO_FRIEND ( const CString& strName );
	const CString& GET_FRIEND_NAME () const						{ return m_strSelectName; }

private:
	BOOL	GetFriend ( const int nCharID, CString & strName );

public:
	void	ADD_FRIEND_NAME_TO_EDITBOX ( const CString& strName );

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

private:
	void	LoadFriend ( const SFRIEND& sFriend );

private:
	int		m_nSelectIndex;
	CString	m_strSelectName;

private:
	CBasicTextBoxEx*	m_pListText;
	CBasicScrollBarEx*	m_pListScrollBar;

	CMultiModeButton*	m_pADD;
	CMultiModeButton*	m_pDEL;
	CMultiModeButton*	m_pBLOCK;
	CMultiModeButton*	m_pMOVE;
	CMultiModeButton*	m_pPARTY;
	CMultiModeButton*	m_pWHISPER;

	CUIEditBox*			m_pEditBox;

	BOOL				m_bINIT;

	SFRIEND				m_sFriend;

protected:
	BOOL SetFriendInfo( const int nCharID );

public:
	SFRIEND & GetFriendInfo() { return m_sFriend; }
};
