///////////////////////////////////////////////////////////////////////////////
// s_NetClientMsgRegister.cpp
//
// class CNetClient
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "s_NetClient.h"
#include "./s_CClientConsoleMsg.h"
#include <stdlib.h>
#include "../Lib_Client/G-Logic/GLContrlMsg.h"
#include "./china_md5.h"
#include <string.h>
#include <wchar.h>

#include "./s_NetMsgLobby.h"

#include "../Lib_Engine/Core/NSRParam.h"

#include "../Lib_Helper/RPCID/RPCID.h"
#include "../Lib_Helper/RPCID/RPCIDSMBIOS.h"
#include "../Lib_Helper/RPCID/RPCIDMAC.h"
#include "../Lib_Engine/Hash/CHash.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int CNetClient::RegisterInit( bool bInit, int nChannel )
{
	if (nChannel < 0 || nChannel >= MAX_CHANNEL_NUMBER) nChannel = 0;

	NET_REGISTER_INIT nmsg;

	nmsg.nChannel  = nChannel;
	nmsg.bInit = bInit;

	return Send((char *) &nmsg);
}

int CNetClient::RegisterAction( 
	const TCHAR* szUserID, 
	const TCHAR* szPass, 
	const TCHAR* szPass2, 
	const TCHAR* szMail, 
	const TCHAR* szFullName, 
	const TCHAR* szSA, 
	DWORD dwReferralUserNum, 
	int nChannel/*=0*/ )
{
	if ( !RPARAM::bRegister )	return 0;

	if (nChannel < 0 || nChannel >= MAX_CHANNEL_NUMBER) nChannel = 0;

	NET_REGISTER_ACTION nmsg;

	nmsg.nChannel  = nChannel;

	if ( RPARAM::bRegisterUseMD5 )
	{
		CHINA_MD5::MD5 md5;
		size_t nLength = 0;
		HRESULT sRet = StringCchLength( szPass, USR_PASS_LENGTH, &nLength );

		if (sRet == STRSAFE_E_INVALID_PARAMETER)
			return NET_ERROR;

		md5.update( reinterpret_cast<const unsigned char*> (szPass), nLength );	
		std::string strPass = md5.asString();
		StringCchCopy( nmsg.szPass,   USR_PASS_LENGTH, strPass.c_str() );
	}
	else
	{
		StringCchCopy( nmsg.szPass, USR_PASS_LENGTH, szPass );
	}

	if ( RPARAM::bRegisterUseMD5 )
	{
		CHINA_MD5::MD5 md5;
		size_t nLength = 0;
		HRESULT sRet = StringCchLength( szPass2, USR_PASS_LENGTH, &nLength );

		if (sRet == STRSAFE_E_INVALID_PARAMETER)
			return NET_ERROR;

		md5.update( reinterpret_cast<const unsigned char*> (szPass2), nLength );	
		std::string strPass = md5.asString();
		StringCchCopy( nmsg.szPass2,   USR_PASS_LENGTH, strPass.c_str() );
	}
	else
	{
		StringCchCopy( nmsg.szPass2, USR_PASS_LENGTH, szPass2 );
	}
	

	StringCchCopy( nmsg.szUser, USR_ID_LENGTH, szUserID ); 
	StringCchCopy( nmsg.szMail, USR_INFOMAIL_LENGTH, szMail ); 
	StringCchCopy( nmsg.szFullName, USR_INFOFULLNAME_LENGTH, szFullName ); 
	StringCchCopy( nmsg.szSA, USR_PASS_LENGTH, szSA ); 
	
	m_Tea.encrypt( nmsg.szUser, USR_ID_LENGTH ); 
	m_Tea.encrypt( nmsg.szMail, USR_INFOMAIL_LENGTH ); 
	m_Tea.encrypt( nmsg.szFullName, USR_INFOFULLNAME_LENGTH ); 
	m_Tea.encrypt( nmsg.szSA, USR_PASS_LENGTH ); 

	nmsg.dwReferralUserNum = dwReferralUserNum;

	{
		//PCID
		SPCID_SMBIOS sPCID_SMBIOS;
		RPCID::getSMBIOS( sPCID_SMBIOS );
		::StringCchCopy(nmsg.szPCID_HWID, PCID_LENGTH, RPCID::getHashSMBIOS(sPCID_SMBIOS).c_str() );
		::StringCchCopy(nmsg.szPCID_MAC, PCID_LENGTH, RPCID::getHashWindowsMAC().c_str() );
		m_Tea.encrypt( nmsg.szPCID_HWID, PCID_LENGTH );
		m_Tea.encrypt( nmsg.szPCID_MAC, PCID_LENGTH );
	}

	return Send((char *) &nmsg);
}