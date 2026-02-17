#include "stdafx.h"

#include "./s_CDbAction.h"
#include "./s_CSessionServer.h"
#include "./s_CFieldServer.h"
#include "./s_CAgentServer.h"

#include "../Lib_Client/G-Logic/GLCharData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*register page, Juver, 2017/11/18 */
CAgentUserRegister::CAgentUserRegister(
	const TCHAR* szUser,
	const TCHAR* szPass,
	const TCHAR* szPass2,
	const TCHAR* szSA,
	const TCHAR* szMail,	
	const TCHAR* szFullName,
	const TCHAR* szUserIP,
	int nSvrGrp,
	int nSvrNum,
	DWORD dwClient,
	DWORD dwClientNum,
									   
	const TCHAR* szPCID_HWID,
	const TCHAR* szPCID_MAC,
	BOOL bRegisterPCIDCheck,
	BOOL bRegisterEmailCheck,

	DWORD dwReferralUserNum,
	EMREFERRAL_REGISTER_TYPE emRegisterReferral )
	: m_nSvrGrp( nSvrGrp )
	, m_nSvrNum( nSvrNum )

	//PCID
	, m_strPCID_HWID( szPCID_HWID )
	, m_strPCID_MAC( szPCID_MAC )
	, m_bRegisterPCIDCheck( bRegisterPCIDCheck )
	, m_bRegisterEmailCheck( bRegisterEmailCheck )

	, m_emRegisterReferral(emRegisterReferral)
	, m_dwReferralUserNum(dwReferralUserNum)
{	
	memset(m_szUser, 0, sizeof(char) * USR_ID_LENGTH);
	memset(m_szPass, 0, sizeof(char) * USR_PASS_LENGTH);
	memset(m_szPass2, 0, sizeof(char) * USR_PASS_LENGTH);
	memset(m_szSA, 0, sizeof(char) * USR_PASS_LENGTH);
	memset(m_szMail, 0, sizeof(char) * USR_INFOMAIL_LENGTH);
	memset(m_szFullName, 0, sizeof(char) * USR_INFOFULLNAME_LENGTH);

	if ( szUser != NULL)	
		StringCchCopy( m_szUser, CHR_ID_LENGTH, szUser );

	if ( szPass != NULL)	
		StringCchCopy( m_szPass, CHR_ID_LENGTH, szPass );

	if ( szPass2 != NULL)	
		StringCchCopy( m_szPass2, CHR_ID_LENGTH, szPass2 );

	if ( szSA != NULL)	
		StringCchCopy( m_szSA, CHR_ID_LENGTH, szSA );

	if ( szMail != NULL)	
		StringCchCopy( m_szMail, USR_INFOMAIL_LENGTH, szMail );

	if ( szFullName != NULL)	
		StringCchCopy( m_szFullName, USR_INFOFULLNAME_LENGTH, szFullName );

	m_dwClientNum = dwClientNum;
	m_dwClient = dwClient;
	m_strUserIP = szUserIP;
}

int CAgentUserRegister::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pTemp == NULL) return NET_ERROR;

	int nRetCode = 0;	
	int nResult = COdbcManager::GetInstance()->UserRegister ( 
		m_szUser, 
		m_szPass, 
		m_szPass2, 
		m_szSA, 
		m_szMail, 
		m_szFullName, 
		m_nSvrGrp, 
		m_nSvrNum,

		m_strPCID_HWID.GetString(),	//PCID
		m_strPCID_MAC.GetString(),	//PCID
		m_bRegisterPCIDCheck,		//PCID 
		m_bRegisterEmailCheck,		//PCID

		m_dwReferralUserNum,
		m_emRegisterReferral
		);

	NET_REGISTER_ACTION_FB2 fb2;
	fb2.nClient = m_dwClientNum;
	
	StringCchCopy( fb2.szIp, MAX_IP_LENGTH+1,	m_strUserIP.GetString());
	StringCchCopy( fb2.szUserid, USR_ID_LENGTH+1, m_szUser );

	//PCID
	StringCchCopy( fb2.szPCID_HWID, PCID_LENGTH, m_strPCID_HWID.GetString() );
	StringCchCopy( fb2.szPCID_MAC, PCID_LENGTH, m_strPCID_MAC.GetString() );

	switch (nResult)
	{
	case DB_ERROR : 
		{
			fb2.emFB = EMREGISTER_FB2_ERROR;
		}break;
	case 0:
		{
			fb2.emFB = EMREGISTER_FB2_OK;
		}break;
	case 1:
		{
			fb2.emFB = EMREGISTER_FB2_USER_USED;
		}break;
	case 2:
		{
			fb2.emFB = EMREGISTER_FB2_EMAIL_USED;
		}break;

	case 100:
		{
			fb2.emFB = EMREGISTER_FB2_PCID_BLOCKED;
		}break;

	case 200:
		{
			fb2.emFB = EMREGISTER_FB2_REFERRAL_INVALID;
		}break;
	};
	
	pTemp->RegisterFeedback( &fb2 );

	return NET_OK;
}

