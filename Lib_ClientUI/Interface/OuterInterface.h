#pragma	once

#include "../../Lib_Engine/GUInterface/UIMan.h"

#include "./OuterInterfaceGuid.h"
#include "./OuterInterfaceModalCallerID.h"


class CModalWindow;
class CSelectServerPage;
class CLoginPage;
class CPassKeyboard;
class CSelectCharacterPage;
class CWaitDialogue;
class DxLobyStage;
class CSecPassSetPage;
class CSecPassCheckPage;
class CMessageWindow;
class CCharacterCreatePage;
class CCharacterCreateInfo;
class CCharacterCreateRotate;
class CLobbyTitle;
class CRegisterPage;				/*register page, Juver, 2017/11/17 */
class CUserAccountPage;				/* user account page, Juver, 2020/11/05 */
class CLoginDeviceCheckVerifyPage;
/*dmk14 ingame web*/
class CForgotPassPage;
class CForgotPassChangePage;
class CSelectCharacterChangePass;
class CSelectCharacterChangePin;
class CSelectCharacterChangeMail;
class CSelectCharacterResetPin;
class CSelectCharacterTopUp;
class CGameTimeConvertUI;

class CAnnouncementWindow; /*XINCODE Lobby Announcement*/

class COuterInterface : public CUIMan
{
	static	const	float	fLIST_LINE_INTERVAL;

private:
	BOOL	m_bOnTheInterface;
	bool	m_bBlockProgramFound;
	float	m_fWaitTime;
	BOOL	m_bUseWaitTime;
	USHORT	m_uChaRemain;			// 생성 가능한 캐릭터 수
	INT		m_nChaRemainExtremeM;	// 생성 가능한 극강부 남자 캐릭터 수
	INT		m_nChaRemainExtremeW;	// 생성 가능한 극강부 여자 캐릭터 수
	int		m_nServerGroup;
	int		m_nServerChannel;
	bool	m_bCHANNEL;
	BOOL	m_bLoginCancel;

	LONG	m_lResolutionBack;

	UIGUID	m_uidMoalCallWindow;
	CString	m_strDaumGID;

	/* login device check, Juver, 2020/11/05 */
	bool	m_bUserLoginDeviceCheck;
	
	/*dmk14 ingame web*/
	DWORD	m_dwGameTime;
	DWORD	m_dwPPoints;
	CString m_strEmail;

private:
	CModalWindow*				m_pModalWindow;
	CSelectServerPage*			m_pSelectServerPage;
	CLoginPage*					m_pLoginPage;
	CSecPassSetPage*			m_pSecPassSetPage;
	CSecPassCheckPage*			m_pSecPassCheckPage;
	CMessageWindow*				m_pMessageWindow;
	CPassKeyboard*				m_pPassKeyboard;
	CSelectCharacterPage*		m_pSelectCharacterPage;
	CAnnouncementWindow*		m_pAnnouncementWindow; /*XINCODE Lobby Announcement*/
	CUIControl*					m_pFullScreenButton;
	CUIControl*					m_pFullScreenButtonDummy;
	CUIControl*					m_pUpImage;
	CUIControl*					m_pDownImage;
	CUIControl*					m_pMessageWindowDummy;
	CUIControl*					m_pLoginMark;
	CCharacterCreatePage*		m_pCharacterCreatePage;
	CCharacterCreateInfo*		m_pCharacterCreateInfo;
	CCharacterCreateRotate*		m_pCharacterCreateRotate;
	CLobbyTitle*				m_pLobbyTitle;
	CRegisterPage*				m_pRegisterPage;			/*register page, Juver, 2017/11/17 */
	CUserAccountPage*			m_pUserAccountPage;			/* user account page, Juver, 2020/11/05 */
	CLoginDeviceCheckVerifyPage*	m_pLoginDeviceCheckVerifyPage;		/* login device check, Juver, 2020/11/06 */
	
	/*dmk14 ingame web*/
	CForgotPassPage*			m_pForgotPassPage;
	CForgotPassChangePage*		m_pForgotPassChangePage;
	CSelectCharacterChangePass*			m_pSelectCharacterChangePass;
	CSelectCharacterChangePin*			m_pSelectCharacterChangePin;
	CSelectCharacterChangeMail*			m_pSelectCharacterChangeMail;
	CSelectCharacterResetPin*			m_pSelectCharacterResetPin;
	CSelectCharacterTopUp*				m_pSelectCharacterTopUp;
	CGameTimeConvertUI*					m_pGameTimeConvertUI;

	CWaitDialogue*				m_pWaitDisplay;
	DxLobyStage*				m_pDxLobyStage;

	void ResetControl();

public:
	COuterInterface ();
	virtual	~COuterInterface ();

public:
	UIGUID	GetRenderStage ();

public:
	virtual HRESULT OneTimeSceneInit();
	virtual	HRESULT FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT FinalCleanup();
	virtual HRESULT DeleteDeviceObjects();

public:
	void SetOnTheInterface ()							{ m_bOnTheInterface = TRUE; }
	BOOL IsOnTheInterface ()							{ return m_bOnTheInterface; }
	void ResetOnTheInterface ()							{ m_bOnTheInterface = FALSE; }

	void	START_WAIT_TIME ( float fTime );
	float	GET_WAIT_TIME()								{ return m_fWaitTime; }

	void SetCancelToLogin()								{ m_bLoginCancel = TRUE; }
	void ResetCancelToLogin()							{ m_bLoginCancel = FALSE; }
	BOOL IsLoginCancel()								{ return m_bLoginCancel; }

	void SetModalCallWindowID( UIGUID uID )				{ m_uidMoalCallWindow = uID; }
	UIGUID GetModalCallWindowID()						{ return m_uidMoalCallWindow; }

private:
	void	UPDATE_WAIT_TIME ( float fElapsedTime );
	void	ModalMsgProcess ( UIGUID nCallerID, DWORD dwMsg );
	void	CloseAllWindow ();

public:
	void MsgProcess ( LPVOID pBuffer );

	void SetDaumGID( const CString & strDaumGID )	{ m_strDaumGID = strDaumGID; }
	const TCHAR * GetDaumGID() const				{ return m_strDaumGID.GetString(); }

	void SetCharRemain( const USHORT uChaRemain )		{ m_uChaRemain = uChaRemain; }
	const USHORT GetCharRemain() const					{ return m_uChaRemain; }

	void SetCharRemainExM( const INT nChaRemain )		{ m_nChaRemainExtremeM = nChaRemain; }
	const INT GetCharRemainExM() const					{ return m_nChaRemainExtremeM; }

	void SetCharRemainExW( const INT nChaRemain )		{ m_nChaRemainExtremeW = nChaRemain; }
	const INT GetCharRemainExW() const					{ return m_nChaRemainExtremeW; }

	BOOL IsCreateExtream()								{ return (m_nChaRemainExtremeM>0 || m_nChaRemainExtremeW>0); }

	/* login device check, Juver, 2020/11/05 */
	void SetUserLoginDeviceCheck( const bool bSet )		{ m_bUserLoginDeviceCheck = bSet; }
	const bool GetUserLoginDeviceCheck() const			{ return m_bUserLoginDeviceCheck; }
	
	/*dmk14 ingame web*/
	void SetGameTime( const DWORD dwTime )				{ m_dwGameTime = dwTime; }
	const DWORD GetGameTime()							{ return m_dwGameTime; }

	void SetPremiumPoint( const DWORD dwPoint )			{ m_dwPPoints = dwPoint; }
	const DWORD GetPremiumPoint()						{ return m_dwPPoints; }

	void SetEmail( CString strEmail );
	CString GetEmail()									{ return m_strEmail.GetString(); }

	// 중국 MMOSPEED
	void RunMMOSpeed();

	const char* MakeString ( const char* szFormat, ... );

	void SetBlockProgramFound ( bool bFOUND )			{ m_bBlockProgramFound = bFOUND; }
	bool IsBlockProgramFound ()							{ return m_bBlockProgramFound; }

	void WAITSERVER_DIALOGUE_OPEN ( const CString& strMessage, const int nAction, const float fTimer );
	void WAITSERVER_DIALOGUE_CLOSE();

	void ToSelectServerPage ( UIGUID cID );
	void ToLoginPage ( UIGUID cID );
	void ToSecPassPage ( UIGUID cID, int nCheckFlag );
	void ToChinaAreaPage ( UIGUID cID );
	void ToSelectCharacterPage ( UIGUID cID );
	void ToCreateCharacterPage ( UIGUID cID );
	void ToRegisterPage( UIGUID cID );					/*register page, Juver, 2017/11/17 */
	void ToUserAccountPage( UIGUID cID );				/* user account page, Juver, 2020/11/05 */
	void ToLoginDeviceCheckVerifyPage( UIGUID cID );	/* login device check, Juver, 2020/11/06 */
	
	/*dmk14 ingame web*/
	void ToForgotPassPage( UIGUID cID );
	void ToForgotPassChangePage( UIGUID cID );
	void ToSelectCharacterChangePass( UIGUID cID );
	void ToSelectCharacterChangePin( UIGUID cID );
	void ToSelectCharacterChangeMail( UIGUID cID );
	void ToSelectCharacterResetPin( UIGUID cID );
	void ToSelectCharacterTopUp( UIGUID cID );
	void ToSelectCharacterGameTimeConvertUI( UIGUID cID );

	void OpenMessageWindow ( UIGUID cID, BOOL bOpen );

	void DeleteCharacter ();
	
	bool IsCHANNEL()									{ return m_bCHANNEL; }

	CModalWindow*			GetModalWindow ()			{ return m_pModalWindow; }
	CSelectServerPage*		GetSelectServerPage ()		{ return m_pSelectServerPage; }
	CLoginPage*				GetLoginPage ()				{ return m_pLoginPage; }
	CSecPassSetPage*		GetSecPassSetPage ()		{ return m_pSecPassSetPage;	}
	CSecPassCheckPage*		GetSecPassCheckPage ()		{ return m_pSecPassCheckPage;	}
	CPassKeyboard*			GetPassKeyboard()			{ return m_pPassKeyboard; }
	CSelectCharacterPage*	GetSelectCharacterPage ()	{ return m_pSelectCharacterPage; }
	CUIControl*				GetUpImage()				{ return m_pUpImage; }
	CUIControl*				GetDownImage()				{ return m_pDownImage; }
	CCharacterCreatePage*	GetCharacterCreatePage()	{ return m_pCharacterCreatePage; }
	CCharacterCreateInfo*	GetCharacterCreateInfo()	{ return m_pCharacterCreateInfo; }
	CCharacterCreateRotate*	GetCharacterCreateRotate()	{ return m_pCharacterCreateRotate;	}
	CLobbyTitle*			GetLobbyTitle()				{ return m_pLobbyTitle;	}
	CRegisterPage*			GetRegisterPage()			{ return m_pRegisterPage; }			/*register page, Juver, 2017/11/17 */
	CUserAccountPage*		GetUserAccountPage()		{ return m_pUserAccountPage; }		/* user account page, Juver, 2020/11/05 */
	CLoginDeviceCheckVerifyPage*	GetLoginDeviceCheckVerifyPage()	{ return m_pLoginDeviceCheckVerifyPage; }	/* login device check, Juver, 2020/11/06 */
	/*dmk14 ingame web*/
	CForgotPassPage*		GetForgotPassPage()			{ return m_pForgotPassPage; }
	CForgotPassChangePage*	GetForgotPassChangePage()	{ return m_pForgotPassChangePage; }
	CSelectCharacterChangePass*			GetSelectCharacterChangePass()			{ return m_pSelectCharacterChangePass; }
	CSelectCharacterChangePin*			GetSelectCharacterChangePin()			{ return m_pSelectCharacterChangePin; }
	CSelectCharacterChangeMail*			GetSelectCharacterChangeMail()			{ return m_pSelectCharacterChangeMail; }
	CSelectCharacterResetPin*			GetSelectCharacterResetPin()			{ return m_pSelectCharacterResetPin; }
	CSelectCharacterTopUp*				GetSelectCharacterTopUp()				{ return m_pSelectCharacterTopUp; }
	CGameTimeConvertUI*					GetGameTimeConvertUI()					{ return m_pGameTimeConvertUI; }

	void SetConnectServerInfo ( const int nServerGroup, const int nServerChannel );
	void GetConnectServerInfo ( int& nServerGroup, int& nServerChannel );

	void SetLobyStage( DxLobyStage * pLobyStage )	{ GASSERT( pLobyStage ); m_pDxLobyStage = pLobyStage; }

	void SetResolutionBack( long lResolution )	{ m_lResolutionBack = lResolution; }
	long GetResolutionBack()						{ return m_lResolutionBack; }

	void CreateCharacterSetUseCancel( BOOL bSet );
	void SetLobbyTitle( int nCID );

	/* login device check, Juver, 2020/11/06 */
	void UserAccountPageLoginDeviceCheckSet();

private:
	BOOL LoadClientVersion ( int& nPatchVer, int& nGameVer );

public:
	static COuterInterface& GetInstance();
};