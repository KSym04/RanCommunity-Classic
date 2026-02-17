#include "stdafx.h"
#include "./s_NetClient.h"

#include "./s_NetMsgLobby.h"

#include "../Lib_Engine/Core/NSRParam.h"
#include "./china_md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* user account page, Juver, 2020/11/05 */
int CNetClient::UserAccountPageInit( bool bInit, int nChannel )
{
	if (nChannel < 0 || nChannel >= MAX_CHANNEL_NUMBER) nChannel = 0;

	NET_USER_ACCOUNT_PAGE_INIT nmsg;

	nmsg.nChannel  = nChannel;
	nmsg.bInit = bInit;

	return Send((char *) &nmsg);
}

/* login device check, Juver, 2020/11/06 */
int CNetClient::UserAccountPageLoginDeviceCheckSet( const TCHAR* szPassword, bool bSet )
{
	if (szPassword == NULL) return NET_ERROR;

	NET_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET netMsg;
	netMsg.bSet = bSet;

	if ( RPARAM::bRegisterUseMD5 )
	{
		CHINA_MD5::MD5 md5;
		size_t nLength = 0;
		HRESULT sRet = StringCchLength( szPassword, USR_PASS_LENGTH, &nLength );

		if (sRet == STRSAFE_E_INVALID_PARAMETER)
			return NET_ERROR;

		md5.update( reinterpret_cast<const unsigned char*> (szPassword), nLength );
		std::string strPass = md5.asString();

		StringCchCopy( netMsg.szPassword, USR_PASS_LENGTH, strPass.c_str() );
	}
	else
	{
		StringCchCopy( netMsg.szPassword, USR_PASS_LENGTH, szPassword );
	}

	m_Tea.encrypt( netMsg.szPassword, USR_PASS_LENGTH );

	return Send( (char *) &netMsg );
}