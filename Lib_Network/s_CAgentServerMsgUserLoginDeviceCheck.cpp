#include "stdafx.h"
#include "./s_CAgentServer.h"

#include "./s_NetMsgLobby.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* login device check, Juver, 2020/11/07 */
void CAgentServer::UserLoginDeviceCheckVerifyRequestInit( MSG_LIST* pMsg )
{
	if ( pMsg == NULL) return;	

	//login active
	if (m_pClientManager->IsAccountPassing( pMsg->dwClient ) == true)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserLoginDeviceCheckVerifyRequestInit IsAccountPassing()") );
		return;
	}

	//register active
	if (m_pClientManager->IsRegisterWait( pMsg->dwClient ) == true)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserLoginDeviceCheckVerifyRequestInit IsRegisterWait()") );
		return;
	}

	NET_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_INIT* pNmr = NULL;
	DWORD dwClient   = 0;

	dwClient = pMsg->dwClient;

	pNmr = reinterpret_cast<NET_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_INIT *> (pMsg->Buffer);

	if (sizeof(NET_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_INIT) != pNmr->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserLoginDeviceCheckVerifyRequestInit Wrong Message Size") );
		return;
	}

	int nChannel = pNmr->nChannel;

	if ( nChannel < 0 || nChannel >= m_nServerChannelNumber)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserLoginDeviceCheckVerifyRequestInit Invalid Channel Number (Channel:%d)"), nChannel );
		return;
	}


	m_pClientManager->SetUserLoginDeviceCheckVerifyRequestInit( dwClient, pNmr->bInit );

	//this could happen if player close the login verify page and player is in the process of login verify
	if ( m_pClientManager->GetUserLoginDeviceCheckVerifyRequestWait( dwClient ) )
	{
		m_pClientManager->SetUserLoginDeviceCheckVerifyRequestWait( dwClient, FALSE );
	}

	//CConsoleMessage::GetInstance()->Write( _T("UserLoginDeviceCheckVerifyRequestInit %d %d"), dwClient, pNmr->bInit );
}

/* login device check, Juver, 2020/11/07 */
void CAgentServer::UserLoginDeviceCheckVerifyRequestAction( MSG_LIST* pMsg )
{
	NET_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB nmfb;

	if ( pMsg == NULL) 
	{
		nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_SERVER;
		SendClient(pMsg->dwClient, &nmfb);
		return;	
	}

	if (m_pClientManager->IsAccountPassing( pMsg->dwClient ) == true)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserLoginDeviceCheckVerifyRequestAction IsAccountPassing()") );
		return;
	}

	if (m_pClientManager->IsRegisterWait( pMsg->dwClient ) == true)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserLoginDeviceCheckVerifyRequestAction IsRegisterWait()") );
		return;
	}

	if ( !m_pClientManager->GetUserLoginDeviceCheckVerifyRequestInit( pMsg->dwClient ) )
	{
		nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_NOT_INIT;
		SendClient(pMsg->dwClient, &nmfb);
		return;
	}

	if ( m_pClientManager->GetUserLoginDeviceCheckVerifyRequestWait( pMsg->dwClient ) )
	{
		nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_ON_WAIT;
		SendClient(pMsg->dwClient, &nmfb);
		return;
	}

	NET_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION* pNmr = NULL;

	DWORD dwClient   = 0;

	dwClient = pMsg->dwClient;

	pNmr = reinterpret_cast<NET_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION *> (pMsg->Buffer);

	if (sizeof(NET_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION) != pNmr->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserLoginDeviceCheckVerifyRequestAction Wrong Message Size") );
		nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_SERVER;
		SendClient(dwClient, &nmfb);
		return;
	}

	int nChannel = pNmr->nChannel;
	if ( nChannel < 0 || nChannel >= m_nServerChannelNumber)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserLoginDeviceCheckVerifyRequestAction Invalid Channel Number (Channel:%d)"), nChannel );
		nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_SERVER;
		SendClient(dwClient, &nmfb);
		return;
	}

	m_Tea.decrypt( pNmr->szUser, USR_ID_LENGTH ); 
	m_Tea.decrypt( pNmr->szPass, USR_PASS_LENGTH ); 

	TCHAR szUserid[USR_ID_LENGTH] = {0};
	TCHAR szPass[USR_PASS_LENGTH] = {0};

	StringCchCopy( szUserid, USR_ID_LENGTH, pNmr->szUser );
	StringCchCopy( szPass, USR_PASS_LENGTH, pNmr->szPass );

	if ( (strlen( szUserid ) < USR_INFO_MIN) || (strlen( szUserid ) > USR_ID_LENGTH) )
	{
		nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_LEN_USERID;
		SendClient(dwClient, &nmfb);
		return;
	}

	if ( (strlen( szPass ) < USR_INFO_MIN) || (strlen( szPass ) > USR_PASS_LENGTH) )
	{
		nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_LEN_PASS;
		SendClient(dwClient, &nmfb);
		return;
	}

	{
		CString str = szUserid;
		str.Trim();
		if ( STRUTIL::CheckString( str.GetString() ) )
		{
			nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_INVALID_USERID;
			SendClient(dwClient, &nmfb);
			return;
		}
	}

	{
		CString str = szPass;
		str.Trim();
		if ( STRUTIL::CheckString( str.GetString() ) )
		{
			nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_INVALID_PASS;
			SendClient(dwClient, &nmfb);
			return;
		}
	}

	//PCID
	m_Tea.decrypt( pNmr->szPCID_HWID, PCID_LENGTH );
	m_Tea.decrypt( pNmr->szPCID_MAC, PCID_LENGTH );

	TCHAR szPCID_HWID[PCID_LENGTH] = {0};
	TCHAR szPCID_MAC[PCID_LENGTH] = {0};

	StringCchCopy (szPCID_HWID, PCID_LENGTH, pNmr->szPCID_HWID );
	StringCchCopy (szPCID_MAC, PCID_LENGTH, pNmr->szPCID_MAC );

	if ( (strlen( szPCID_HWID ) <= 0) || (strlen( szPCID_HWID ) >= PCID_LENGTH) )
	{
		nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_LEN_PCID;
		SendClient(dwClient, &nmfb);
		return;
	}

	if ( (strlen( szPCID_MAC ) <= 0) || (strlen( szPCID_MAC ) >= PCID_LENGTH) )
	{
		nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_LEN_PCID;
		SendClient(dwClient, &nmfb);
		return;
	}

	m_pClientManager->SetUserLoginDeviceCheckVerifyRequestWait( dwClient, true );
	m_pClientManager->SetUserID( dwClient, szUserid );

	nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_GOOD_WAITING;
	SendClient(dwClient, &nmfb);

	CAgentUserLoginDeviceCheckVerifyRequest* pAction = new CAgentUserLoginDeviceCheckVerifyRequest( 
		szUserid,
		szPass, 
		m_nServerGroup,
		m_nServerNum,
		0,
		dwClient,
		m_pClientManager->GetClientIP( dwClient ),
		szPCID_HWID,			//PCID
		szPCID_MAC				//PCID
		);
	COdbcManager::GetInstance()->AddUserJob((CDbAction*) pAction);

	//CConsoleMessage::GetInstance()->Write( _T("UserLoginDeviceCheckVerifyRequestAction Client ID:%d User:%s Pass:%s IP:%s pcid HWID:%s pcid MAC:%s" ), 
	//	dwClient, 
	//	szUserid, 
	//	szPass,
	//	m_pClientManager->GetClientIP( dwClient ),
	//	szPCID_HWID, 
	//	szPCID_MAC );
}

/* login device check, Juver, 2020/11/07 */
void CAgentServer::UserLoginDeviceCheckVerifyRequestActionFeedback( NET_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB2* fb2 )
{
	if ( !fb2 )	return;

	NET_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB nmfb;

	DWORD dwCLIENTID = fb2->nClient;
	BOOL bERROR = FALSE;

	if ( m_pClientManager->IsAccountPassing( dwCLIENTID ) == true)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserLoginDeviceCheckVerifyRequestActionFeedback IsAccountPassing()") );
		bERROR = TRUE;
	}

	if (m_pClientManager->IsRegisterWait( dwCLIENTID ) == true)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::UserLoginDeviceCheckVerifyRequestActionFeedback IsRegisterWait()") );
		bERROR = TRUE;
	}

	if ( !m_pClientManager->GetUserLoginDeviceCheckVerifyRequestInit( dwCLIENTID ) )
		bERROR = TRUE;

	if ( !m_pClientManager->GetUserLoginDeviceCheckVerifyRequestWait(dwCLIENTID ) )
		bERROR = TRUE;

	BOOL bOK = ( (m_pClientManager->IsOnline( dwCLIENTID ) == true) && 
		(strcmp( m_pClientManager->GetClientIP( dwCLIENTID ), fb2->szIp ) == 0) && 
		(strcmp( m_pClientManager->GetUserID( dwCLIENTID ),   fb2->szUserid ) == 0) );

	if ( !bOK )
		bERROR = TRUE;

	if( bERROR )
	{
		m_pClientManager->SetUserLoginDeviceCheckVerifyRequestWait( dwCLIENTID, FALSE );
		m_pClientManager->ResetUserID( dwCLIENTID );
		m_pClientManager->SetUserLoginDeviceCheckVerifyRequestInit( dwCLIENTID, FALSE );

		nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_GOOD_ERROR;
		SendClient( dwCLIENTID, &nmfb);
		return;
	}

	switch( fb2->emFB )
	{
	case EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB2_ERROR:
		{	
			m_pClientManager->SetUserLoginDeviceCheckVerifyRequestInit( dwCLIENTID, FALSE );

			nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_GOOD_ERROR;
			SendClient( dwCLIENTID, &nmfb);
		}break;

	case EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB2_SUCCESS:
		{
			m_pClientManager->SetUserLoginDeviceCheckVerifyRequestInit( dwCLIENTID, FALSE );

			nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_GOOD_SUCCESS;
			SendClient( dwCLIENTID, &nmfb);
		}break;

	case EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB2_PASS_INCORRECT:
		{
			nmfb.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_GOOD_INCORRECT_PASS;
			SendClient( dwCLIENTID, &nmfb);
		}break;
	};	

	m_pClientManager->SetUserLoginDeviceCheckVerifyRequestWait( dwCLIENTID, FALSE );
	m_pClientManager->ResetUserID( dwCLIENTID );
}
