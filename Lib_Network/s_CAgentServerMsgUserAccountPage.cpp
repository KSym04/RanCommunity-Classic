#include "stdafx.h"

#include "./s_CAgentServer.h"

#include "../Lib_Network/s_NetMsgLobby.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* user account page, Juver, 2020/11/05 */
void CAgentServer::UserAccountPageInit( MSG_LIST* pMsg )
{
	if ( pMsg == NULL) return;	

	//check if user has actually logged in
	if (m_pClientManager->IsAccountPass( pMsg->dwClient ) == false)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserAccountPageInit IsAccountPass() == false") );
		CloseClient( pMsg->dwClient );
		return;
	}

	DWORD dwClient = pMsg->dwClient;
	NET_USER_ACCOUNT_PAGE_INIT* pNetMsgClient = reinterpret_cast<NET_USER_ACCOUNT_PAGE_INIT *> (pMsg->Buffer);

	if (sizeof(NET_USER_ACCOUNT_PAGE_INIT) != pNetMsgClient->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserAccountPageInit Wrong Message Size") );
		return;
	}

	int nChannel = pNetMsgClient->nChannel;

	if ( nChannel < 0 || nChannel >= m_nServerChannelNumber)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserAccountPageInit Invalid Channel Number (Channel:%d)"), nChannel );
		return;
	}


	m_pClientManager->SetUserAccountPageInit( dwClient, pNetMsgClient->bInit );

	if ( m_pClientManager->GetUserAccountPageWait( dwClient ) )
	{
		m_pClientManager->SetUserAccountPageWait( dwClient, false );
	}

	//CConsoleMessage::GetInstance()->Write( _T("SetUserAccountPageInit Client:%d User:[%d]%s Init:%d"), 
	//	dwClient, 
	//	m_pClientManager->GetUserNum(dwClient),
	//	m_pClientManager->GetUserID(dwClient), 
	//	m_pClientManager->GetUserAccountPageInit(dwClient) );
}

/* login device check, Juver, 2020/11/06 */
void CAgentServer::UserAccountPageLoginDeviceCheckSet( MSG_LIST* pMsg )
{
	if (pMsg == NULL) return;

	//user not logged in
	if (m_pClientManager->IsAccountPass( pMsg->dwClient ) == false)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserAccountPageLoginDeviceCheckSet IsAccountPass() == false") );
		CloseClient( pMsg->dwClient );
		return;
	}

	//user account page check
	if (m_pClientManager->GetUserAccountPageInit( pMsg->dwClient ) == false )
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserAccountPageLoginDeviceCheckSet GetUserAccountPageInit() == false") );
		return;
	}
	
	//user account page transaction on progress
	if (m_pClientManager->GetUserAccountPageWait( pMsg->dwClient ) == true )
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserAccountPageLoginDeviceCheckSet GetUserAccountPageWait()") );
		return;
	}


	NET_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET* pClientNetMsg = reinterpret_cast<NET_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET*> (pMsg->Buffer);

	if (sizeof(NET_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET) != pClientNetMsg->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserAccountPageLoginDeviceCheckSet Wrong Message Size") );
		return;
	}

	DWORD dwClient = pMsg->dwClient;

	NET_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB nmfb;

	int nUserNum = m_pClientManager->GetUserNum( dwClient );
	if ( nUserNum <= 0 )
	{
		nmfb.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_ERROR_SERVER;
		SendClient(dwClient, &nmfb);
		return;
	}

	m_Tea.decrypt( pClientNetMsg->szPassword, USR_PASS_LENGTH );

	TCHAR szPassword[USR_PASS_LENGTH] = {0};
	StringCchCopy( szPassword, USR_PASS_LENGTH, pClientNetMsg->szPassword );

	if ( (strlen( szPassword ) < USR_INFO_MIN) || (strlen( szPassword ) > USR_PASS_LENGTH) )
	{
		nmfb.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_PASS_SIZE;
		SendClient(dwClient, &nmfb);
		return;
	}

	CString strCheck = szPassword;
	strCheck.Trim();
	if ( STRUTIL::CheckString( strCheck.GetString() ) )
	{
		nmfb.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_PASS_STRING;
		SendClient(dwClient, &nmfb);
		return;
	}

	bool bUserLoginDeviceCheck = m_pClientManager->GetUserLoginDeviceCheck( dwClient );
	if ( pClientNetMsg->bSet )
	{
		if ( bUserLoginDeviceCheck )
		{
			nmfb.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_ALREADY_ENABLED;
			SendClient(dwClient, &nmfb);
			return;
		}
	}
	else
	{
		if ( !bUserLoginDeviceCheck )
		{
			nmfb.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_ALREADY_DISABLED;
			SendClient(dwClient, &nmfb);
			return;
		}
	}

	m_pClientManager->SetUserAccountPageWait( dwClient, true );

	nmfb.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_GOOD_WAIT;
	SendClient(dwClient, &nmfb);

	bool bUserLoginDeviceCheckChange = !bUserLoginDeviceCheck;
	CAgentUserAccountPageLoginDeviceCheckSet* pAction = new CAgentUserAccountPageLoginDeviceCheckSet( 
		m_pClientManager->GetUserNum(dwClient),
		m_pClientManager->GetUserID(dwClient),
		szPassword, 
		bUserLoginDeviceCheckChange,
		m_pClientManager->GetClientIP( dwClient ),
		m_nServerGroup,
		m_nServerNum,
		0,
		dwClient );
	COdbcManager::GetInstance()->AddUserJob((CDbAction*) pAction);

	//CConsoleMessage::GetInstance()->Write( _T("UserAccountPageLoginDeviceCheckSet Client:%d User:[%d]%s set:%d pass:%s"), 
	//	dwClient, 
	//	m_pClientManager->GetUserNum(dwClient),
	//	m_pClientManager->GetUserID(dwClient), 
	//	pClientNetMsg->bSet,
	//	szPassword );
}

/* login device check, Juver, 2020/11/06 */
void CAgentServer::UserAccountPageLoginDeviceCheckSetFeedback( NET_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB2* fb2 )
{
	if ( !fb2 ) return;

	DWORD dwClientID = fb2->dwClient;

	BOOL bERROR = FALSE;

	//user not logged in
	if (m_pClientManager->IsAccountPass( dwClientID ) == false)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserAccountPageLoginDeviceCheckSetFeedback IsAccountPass() == false") );
		bERROR = TRUE;
	}

	//user account page check
	if (m_pClientManager->GetUserAccountPageInit( dwClientID ) == false )
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserAccountPageLoginDeviceCheckSetFeedback GetUserAccountPageInit() == false") );
		bERROR = TRUE;
	}

	//user account page transaction on progress
	if (m_pClientManager->GetUserAccountPageWait(dwClientID ) == false )
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserAccountPageLoginDeviceCheckSetFeedback GetUserAccountPageWait() == false") );
		bERROR = TRUE;
	}

	BOOL bOK = ( (m_pClientManager->IsOnline( dwClientID ) == true) && 
		(strcmp( m_pClientManager->GetClientIP( dwClientID ), fb2->szIp ) == 0) && 
		(strcmp( m_pClientManager->GetUserID( dwClientID ), fb2->szUserid ) == 0) && 
		( m_pClientManager->GetUserNum( dwClientID ) == fb2->dwUserNum ) );

	if ( !bOK )
		bERROR = TRUE;

	NET_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB nmfb;

	if( bERROR )
	{
		m_pClientManager->SetUserAccountPageWait( dwClientID, FALSE );
		m_pClientManager->SetUserAccountPageInit( dwClientID, FALSE );
		nmfb.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_GOOD_ERROR;
		SendClient( dwClientID, &nmfb);
		return;
	}

	switch( fb2->emFB )
	{
	case EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_ERROR:
		{	
			m_pClientManager->SetUserAccountPageWait( dwClientID, FALSE );
			m_pClientManager->SetUserAccountPageInit( dwClientID, FALSE );
			nmfb.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_GOOD_ERROR;
			SendClient( dwClientID, &nmfb);
		}break;

	case EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_DONE:
		{
			m_pClientManager->SetUserLoginDeviceCheck( dwClientID, fb2->bUserLoginDeviceCheck );
			nmfb.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_GOOD_DONE;
			nmfb.bUserLoginDeviceCheck = m_pClientManager->GetUserLoginDeviceCheck( dwClientID );

			SendClient( dwClientID, &nmfb);
		}break;

	case EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_NOT_FOUND:
		{
			nmfb.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_GOOD_NOT_FOUND;
			SendClient( dwClientID, &nmfb);
		}break;
	};	

	m_pClientManager->SetUserAccountPageWait( dwClientID, FALSE );
}