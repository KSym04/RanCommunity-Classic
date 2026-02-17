#include "stdafx.h"
#include "./s_CDbAction.h"

#include "./s_CDbAction.h"

#include "./s_CAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/* login device check, Juver, 2020/11/07 */
CAgentUserLoginDeviceCheckVerifyRequest::CAgentUserLoginDeviceCheckVerifyRequest(
	const TCHAR* szUser,
	const TCHAR* szPass,
	int nSvrGrp,
	int nSvrNum,
	DWORD dwClient,
	DWORD dwClientNum,
	const TCHAR* szUserIP,
	const TCHAR* szPCID_HWID,	//PCID
	const TCHAR* szPCID_MAC		//PCID
	)
	: m_nSvrGrp( nSvrGrp )
	, m_nSvrNum( nSvrNum )

	//PCID
	, m_strPCID_HWID( szPCID_HWID )
	, m_strPCID_MAC( szPCID_MAC )
{	
	memset(m_szUser, 0, sizeof(char) * USR_ID_LENGTH);
	memset(m_szPass, 0, sizeof(char) * USR_PASS_LENGTH);

	if ( szUser != NULL)	
		StringCchCopy( m_szUser, CHR_ID_LENGTH, szUser );

	if ( szPass != NULL)	
		StringCchCopy( m_szPass, CHR_ID_LENGTH, szPass );

	m_dwClientNum = dwClientNum;
	m_dwClient = dwClient;
	m_strUserIP = szUserIP;
}

/* login device check, Juver, 2020/11/07 */
int CAgentUserLoginDeviceCheckVerifyRequest::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pTemp == NULL) return NET_ERROR;

	int nRetCode = 0;	
	nRetCode = COdbcManager::GetInstance()->UserLoginDeviceCheckVerifyRequest ( m_szUser, m_szPass, m_nSvrGrp, m_nSvrNum, m_strUserIP.GetString(), m_strPCID_HWID.GetString(), m_strPCID_MAC.GetString() );

	NET_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB2 fb2;
	fb2.nClient = m_dwClientNum;

	StringCchCopy( fb2.szIp, MAX_IP_LENGTH+1,	m_strUserIP.GetString());
	StringCchCopy( fb2.szUserid, USR_ID_LENGTH+1, m_szUser );

	//PCID
	StringCchCopy( fb2.szPCID_HWID, PCID_LENGTH, m_strPCID_HWID.GetString() );
	StringCchCopy( fb2.szPCID_MAC, PCID_LENGTH, m_strPCID_MAC.GetString() );

	switch (nRetCode)
	{
	case DB_ERROR: 
		fb2.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB2_ERROR;
		break;

	case 0:
		fb2.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB2_SUCCESS;
		break;

	case 1:
		fb2.emFB = EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB2_PASS_INCORRECT;
		break;
	};

	pTemp->UserLoginDeviceCheckVerifyRequestActionFeedback( &fb2 );

	return NET_OK;
}
