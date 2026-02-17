#pragma	once

#include "./UIWindowEx.h"
#include "./FriendWindowDef.h"

class	CModernFriendWindowNormalPage;
class	CModernFriendWindowBlockPage;
class	CMultiModeButton;
struct	SFRIEND;

class	CModernFriendWindow : public CUIWindowEx
{
	enum
	{
		FRIEND_NORMAL_TAB_BUTTON = ET_CONTROL_NEXT,
		FRIEND_BLOCK_TAB_BUTTON,

		FRIEND_NORMAL_PAGE,
		FRIEND_BLOCK_PAGE,
	};

public:
	CModernFriendWindow ();
	virtual ~CModernFriendWindow ();

public:
	void	CreateSubControl ();

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	LoadFriendList ();

	void	ADD_FRIEND_NAME_TO_EDITBOX ( const CString& strName );
	void	ADD_FRIEND ( const CString& strName );

	const CString& GET_FRIEND_NAME() const;
	const CString& GET_BLOCK_NAME() const;

public:
	void	EDIT_END ();

private:
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );

private:
	void	ChangePage ( const UIGUID& cSetVisiblePage );

public:
	virtual	void	TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

private:
	CMultiModeButton*	m_pNORMAL_TAB_BUTTON;
	CMultiModeButton*	m_pBLOCK_TAB_BUTTON;

	CModernFriendWindowBlockPage*		m_pBlockPage;
	CModernFriendWindowNormalPage*		m_pNormalPage;

public:
	SFRIEND & GetFriendSMSInfo();
};