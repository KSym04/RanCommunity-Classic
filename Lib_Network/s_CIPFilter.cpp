#include "StdAfx.h"
#include "./s_CIPFilter.h"
#include "./s_CSystemInfo.h"

#include "s_CServer.h"
#include "s_CSessionServer.h"
#include "s_CLoginServer.h"
#include "s_CFieldServer.h"
#include "s_CAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CIPFilter* CIPFilter::SelfInstance = NULL;

CIPFilter::CIPFilter()
	: m_llBlockRef( 0 )
	, m_llKnownRef( 0 )
	/*dmk14 whitelist*/
	, m_llWhiteListRef(0)
{
	m_setIPBlock.clear();
	m_setIPKnown.clear();
	/*dmk14 whitelist*/
	m_setWhiteListIP.clear();

	InitializeCriticalSection(&m_CSPCLock);
}

CIPFilter::~CIPFilter()
{
	m_setIPBlock.clear();
	m_setIPKnown.clear();
	/*dmk14 whitelist*/
	m_setWhiteListIP.clear();

	DeleteCriticalSection(&m_CSPCLock);
}

CIPFilter* CIPFilter::GetInstance()
{
	if (SelfInstance == NULL)
		SelfInstance = new CIPFilter();

	return SelfInstance;
}

void CIPFilter::SetServer( CServer* _pServer )
{
	m_pServer = NULL;

	if ( _pServer )
		m_pServer = _pServer;
}

BOOL CIPFilter::IsIPBlocked( SOCKET _sSocket )
{
	sockaddr_in	sAddrIn;
	int nSize = sizeof(sockaddr_in);
	char szIp[IP_FILTER_MAX_IP_LENGTH+1] = {0};

	::getpeername(_sSocket, (sockaddr *) &sAddrIn, &nSize);
	::StringCchCopy(szIp, IP_FILTER_MAX_IP_LENGTH+1, ::inet_ntoa(sAddrIn.sin_addr));

	return IsIPBlocked( std::string(szIp) );
}

BOOL CIPFilter::IsIPBlocked( std::string _strIP )
{
	if ( _strIP.size() <= 0 )	return FALSE;

	if ( IsIPKnown( _strIP ) == TRUE )	return FALSE;

	return IsBlocked( _strIP );
}

BOOL CIPFilter::IsIPKnown( std::string _strIP )
{
	if ( _strIP.size() <= 0 )	return FALSE;

	EnterCriticalSection(&m_CSPCLock);

	if ( m_setIPKnown.empty() )
	{
		LeaveCriticalSection(&m_CSPCLock);
		return FALSE;
	}

	SET_IP_ITER iter = m_setIPKnown.find( _strIP );
	if ( iter != m_setIPKnown.end() )	
	{
		m_llKnownRef ++;
		LeaveCriticalSection(&m_CSPCLock);
		return TRUE;
	}

	LeaveCriticalSection(&m_CSPCLock);
	return FALSE;
}

BOOL CIPFilter::IsBlocked( std::string _strIP )
{
	if ( _strIP.size() <= 0 )	return FALSE;

	EnterCriticalSection(&m_CSPCLock);

	if ( m_setIPBlock.empty() )
	{
		LeaveCriticalSection(&m_CSPCLock);
		return FALSE;
	}

	SET_IP_ITER iter = m_setIPBlock.find( _strIP );
	if ( iter != m_setIPBlock.end() )	
	{
		m_llBlockRef ++;
		LeaveCriticalSection(&m_CSPCLock);
		return TRUE;
	}

	LeaveCriticalSection(&m_CSPCLock);
	return FALSE;
}

BOOL CIPFilter::AddIPBlock( std::string _strIP )
{
	if ( _strIP.size() <= 0 )	return FALSE;

	EnterCriticalSection(&m_CSPCLock);

	SET_IP_ITER iter = m_setIPBlock.find( _strIP );
	if ( iter != m_setIPBlock.end() )	
	{
		LeaveCriticalSection(&m_CSPCLock);
		return FALSE;
	}

	m_setIPBlock.insert( _strIP );
	LeaveCriticalSection(&m_CSPCLock);

	return TRUE;
}

BOOL CIPFilter::AddIPKnown( std::string _strIP )
{
	if ( _strIP.size() <= 0 )	return FALSE;

	EnterCriticalSection(&m_CSPCLock);

	SET_IP_ITER iter = m_setIPKnown.find( _strIP );
	if ( iter != m_setIPKnown.end() )	
	{
		LeaveCriticalSection(&m_CSPCLock);
		return FALSE;
	}

	m_setIPKnown.insert( _strIP );
	LeaveCriticalSection(&m_CSPCLock);

	return TRUE;
}

BOOL CIPFilter::RemoveIPBlock( std::string _strIP )
{
	if ( _strIP.size() <= 0 )	FALSE;

	EnterCriticalSection(&m_CSPCLock);

	SET_IP_ITER iter = m_setIPBlock.find( _strIP );
	if ( iter != m_setIPBlock.end() )	
	{
		m_setIPBlock.erase( iter );
		LeaveCriticalSection(&m_CSPCLock);
		return TRUE;
	}

	LeaveCriticalSection(&m_CSPCLock);
	return FALSE;
}

BOOL CIPFilter::RemoveIPKnown( std::string _strIP )
{
	if ( _strIP.size() <= 0 )	return FALSE;

	EnterCriticalSection(&m_CSPCLock);

	SET_IP_ITER iter = m_setIPKnown.find( _strIP );
	if ( iter != m_setIPKnown.end() )	
	{
		m_setIPKnown.erase( iter );
		LeaveCriticalSection(&m_CSPCLock);
		return TRUE;
	}

	LeaveCriticalSection(&m_CSPCLock);
	return FALSE;
}

void CIPFilter::ListLoad()
{
	m_setIPBlock.clear();
	m_setIPKnown.clear();
	/*dmk14 whitelist*/
	m_setWhiteListIP.clear();

	SERVER_UTIL::CSystemInfo SysInfo;
	CString strAppPath;	

	strAppPath = SysInfo.GetAppPath();
	strAppPath += _T("\\cfg\\");
	strAppPath += "IPFilter.cfg";

	FILE *oFile = NULL;

	if ((oFile = ::_tfopen(strAppPath.GetString(), _T("r"))) == NULL)	return;

	TCHAR line[300];

	while (::_fgetts(line, 200, oFile))
	{
		TCHAR seps[]   = _T(" ,\t\n");

		TCHAR* token = _tcstok(line, seps);
		while (token != NULL)
		{
			if (_tcscmp(token, _T("//")) == 0) 
			{	
				break;
			}
			else if (_tcscmp(token, "allow") == 0)
			{
				token = ::_tcstok(NULL, seps );
				if (token)
				{
					AddIPKnown( token );
				}else break;
			}
			else if (_tcscmp(token, "block") == 0)
			{
				token = ::_tcstok(NULL, seps );
				if (token)
				{
					AddIPBlock( token );
				}else break;
			}
			/*dmk14 whitelist*/
			else if (_tcscmp(token, "whitelist") == 0)
			{
				token = ::_tcstok(NULL, seps );
				if (token)
				{
					AddWhiteList( token );
				}else break;
			}

			token = ::_tcstok( NULL, seps );
		}
	}

	::fclose( oFile );
}

void CIPFilter::ListSave()
{
	SERVER_UTIL::CSystemInfo SysInfo;
	CString strAppPath;	

	strAppPath = SysInfo.GetAppPath();
	strAppPath += _T("\\cfg\\");
	strAppPath += "IPFilter.cfg";

	FILE *oFile = NULL;

	if ((oFile = ::_tfopen(strAppPath.GetString(), _T("w"))) == NULL)	return;

	CString strText;
	SET_IP_ITER iter;

	for ( iter = m_setIPKnown.begin(); iter != m_setIPKnown.end(); iter++ )
	{
		CString strFormat;
		strFormat.Format( "allow %s", std::string( *iter ).c_str() );
		strText += strFormat.GetString();
		strText += "\n";
	}

	for ( iter = m_setIPBlock.begin(); iter != m_setIPBlock.end(); iter++ )
	{
		CString strFormat;
		strFormat.Format( "block %s", std::string( *iter ).c_str() );
		strText += strFormat.GetString();
		strText += "\n";
	}

	/*dmk14 whitelist*/
	for ( iter = m_setWhiteListIP.begin(); iter != m_setWhiteListIP.end(); iter++ )
	{
		CString strFormat;
		strFormat.Format( "whitelist %s", std::string( *iter ).c_str() );
		strText += strFormat.GetString();
		strText += "\n";
	}

	if (strText.GetLength())
		::fwrite ( strText.GetBuffer(), strText.GetLength(), 1, oFile );

	::fclose( oFile );
}

/*dmk14 whitelist*/
BOOL CIPFilter::IsWhiteList( SOCKET _sSocket )
{
	sockaddr_in	sAddrIn;
	int nSize = sizeof(sockaddr_in);
	char szIp[MAX_IP_LENGTH+1] = {0};

	::getpeername(_sSocket, (sockaddr *) &sAddrIn, &nSize);
	::StringCchCopy(szIp, MAX_IP_LENGTH+1, ::inet_ntoa(sAddrIn.sin_addr));

	return IsWhiteList( std::string(szIp) );
}

BOOL CIPFilter::IsWhiteList( std::string _strIP )
{
	if ( !IsIpAddress( _strIP.c_str() ) )				return FALSE;
	if ( _strIP.size() <= 0 )							return FALSE;
	if ( _strIP == CCfg::GetInstance()->GetServerIP() ) return TRUE;

	EnterCriticalSection(&m_CSPCLock);

	SET_IP_ITER iter = m_setWhiteListIP.find( _strIP );
	if ( iter != m_setWhiteListIP.end() )	
	{
		m_llWhiteListRef ++;
		LeaveCriticalSection(&m_CSPCLock);
		return TRUE;
	}

	LeaveCriticalSection(&m_CSPCLock);
	return FALSE;
}

void CIPFilter::AddWhiteList( std::string _strIP, bool _bBroadcast /*= false*/ )
{
	if ( _strIP.size() <= 0 || !IsIpAddress( _strIP.c_str() ) )	
		return;

	EnterCriticalSection(&m_CSPCLock);

	SET_IP_ITER iter = m_setWhiteListIP.find( _strIP );
	if ( iter == m_setWhiteListIP.end() )	
	{
		m_setWhiteListIP.insert( _strIP );
	}

	if ( m_pServer && _bBroadcast )
	{
		int nServerType = m_pServer->GetTypeServer();

		NET_IPFILTER_WHITELIST_ADD_SESSION netMsg;
		StringCchCopy( netMsg.szIP, MAX_IP_LENGTH+1, _strIP.c_str() );
		netMsg.nServerType = nServerType;

		if( nServerType ==  NET_SERVER_LOGIN )
		{
			CLoginServer* pServer = (CLoginServer*)m_pServer;
			if ( pServer  )
				pServer->SendSession( &netMsg );
		}
		else if( nServerType == NET_SERVER_SESSION )
		{
			CSessionServer* pServer = (CSessionServer*)m_pServer;
			if ( pServer  )
				pServer->MsgServerBroadCastIPFilterWhiteListAddSelf( _strIP );
		}
		else if( nServerType == NET_SERVER_FIELD )
		{
			CFieldServer* pServer = (CFieldServer*)m_pServer;
			if ( pServer  )
				pServer->SendSession( &netMsg );
		}
		else if( nServerType == NET_SERVER_AGENT )
		{
			CAgentServer* pServer = (CAgentServer*)m_pServer;
			if ( pServer  )
				pServer->SendSession( &netMsg );
		}
	}

	LeaveCriticalSection(&m_CSPCLock);
}

void CIPFilter::RemoveWhiteList( std::string _strIP, bool _bBroadcast /*= false*/ )
{
	if ( _strIP.size() <= 0 || !IsIpAddress( _strIP.c_str() ) )		
		return;

	EnterCriticalSection(&m_CSPCLock);

	SET_IP_ITER iter = m_setWhiteListIP.find( _strIP );
	if ( iter != m_setWhiteListIP.end() )	
	{
		m_setWhiteListIP.erase( iter );

		char cWhiteList[125];
		sprintf(cWhiteList,"netsh advfirewall firewall delete rule name=\"%s\"", _strIP.c_str() );
		WinExec(cWhiteList, SW_HIDE);
	}

	LeaveCriticalSection(&m_CSPCLock);
}

bool CIPFilter::IsIpAddress(const TCHAR* ipAddress) {
	// Variables to hold parsed IP components
	int a, b, c, d;
	// Attempt to parse the IP address components
	if (_stscanf(ipAddress, _T("%d.%d.%d.%d"), &a, &b, &c, &d) == 4) {
		// Check if each component is within the valid range
		if (a >= 0 && a <= 255 &&
			b >= 0 && b <= 255 &&
			c >= 0 && c <= 255 &&
			d >= 0 && d <= 255) {
				return true; // Valid IP address
			}
	}
	return false; // Not a valid IP address
}