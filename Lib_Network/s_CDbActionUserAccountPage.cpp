#include "stdafx.h"
#include "./s_CDbAction.h"

#include "./s_CAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*register page, Juver, 2017/11/18 */
CAgentUserAccountPageLoginDeviceCheckSet::CAgentUserAccountPageLoginDeviceCheckSet(
	DWORD dwUserNum,
	const TCHAR* szUser,
	const TCHAR* szPass,
	bool bUserDeviceCheck,
	const TCHAR* szUserIP,
	int nSvrGrp,
	int nSvrNum,
	DWORD dwClient,
	DWORD dwClientNum )
	: m_dwUserNum(dwUserNum)
	, m_bUserDeviceCheck( bUserDeviceCheck )
	, m_nSvrGrp(nSvrGrp)
	, m_nSvrNum(nSvrNum)
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


/* login device check, Juver, 2020/11/06 */
int CAgentUserAccountPageLoginDeviceCheckSet::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pTemp == NULL) return NET_ERROR;

	int nRetCode = 0;	
	nRetCode = COdbcManager::GetInstance()->UserAccountPageLoginDeviceCheckSet ( 
	m_dwUserNum,
	m_szUser, 
	m_szPass, 
	m_bUserDeviceCheck,
	m_nSvrGrp, 
	m_nSvrNum );

	NET_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB2 fb2;
	fb2.dwUserNum = m_dwUserNum;
	fb2.dwClient = m_dwClientNum;
	StringCchCopy( fb2.szUserid, USR_ID_LENGTH+1, m_szUser );
	StringCchCopy( fb2.szIp, MAX_IP_LENGTH+1,	m_strUserIP.GetString());

	fb2.bUserLoginDeviceCheck = m_bUserDeviceCheck;

	switch (nRetCode)
	{
	case DB_ERROR : 
		{
			fb2.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_ERROR;
		}break;

	case 0:
		{
			fb2.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_DONE;
		}break;

	case 1:
		{
			fb2.emFB = EMUSER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB_NOT_FOUND;
		}break;
	};

	pTemp->UserAccountPageLoginDeviceCheckSetFeedback( &fb2 );

	return NET_OK;
}
