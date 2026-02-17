#include "stdafx.h"

//#include "./DbExecuter.h"
#include "./s_CFieldServer.h"
#include "./s_CIPFilter.h"

#include "../Lib_Engine/Hash/CHash.h"
#include "../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFieldServer::CFieldServer( HWND hWnd, HWND hEditBox, HWND hEditBoxInfo, HWND hEditBoxStats ) 
	: CServer( hWnd, hEditBox, hEditBoxInfo, hEditBoxStats )
	, m_dwFieldServerID(0)
	, m_pClientManager(NULL)
	, m_dwAgentSlot(0)
	, m_bReservationServerStop(false)
	, m_fReservationtime(0.0f)
	, m_dwHeartBeatStart1(0)
	, m_dwHeartBeatStart2(0)	
	, m_dwPeakPC(0)
	, m_dwPeakInstance(0)
{
	/*client hash check, EJCode, 2018/11/26 */
	memset(m_client_hash1, 0, sizeof(char) * CLIENT_HASH_SIZE);
	memset(m_client_hash2, 0, sizeof(char) * CLIENT_HASH_SIZE);

#ifdef USE_MSHIELD
	memset(m_szMShieldDLLHash1, 0, sizeof(char) * CLIENT_HASH_SIZE);
	memset(m_szMShieldDLLHash2, 0, sizeof(char) * CLIENT_HASH_SIZE);
#endif
}

CFieldServer::~CFieldServer()
{
	SAFE_DELETE(m_pClientManager);
}

int CFieldServer::StartClientManager()
{
	///////////////////////////////////////////////////////////////////////////
	// 클라이언트 정보저장 클래스 생성
	SAFE_DELETE(m_pClientManager);
	m_nMaxClient = m_nMaxClient * 2;
	//m_pClientManager = new CClientField(m_nMaxClient, m_hIOServer);
	m_pClientManager = new CClientField(m_nMaxClient, m_pSendIOCP, m_pRecvIOCP, m_hIOServer);
	if (!m_pClientManager)
	{
		m_bIsRunning = false;
		return NET_ERROR;
	}
	m_pClientManager->SetAgentIP ( CCfg::GetInstance()->GetAgentIP() );
	return NET_OK;
}

int CFieldServer::StartDbManager()
{	
	int nRetCode;
	///////////////////////////////////////////////////////////////////////////
	// Create DB manager and Open DB	
	CDbExecuter::GetInstance()->SetServer((CServer*) this);
	CLogDbExecuter::GetInstance()->SetServer((CServer*) this);
	CUserDbExecuter::GetInstance()->SetServer((CServer*) this);
	
	COdbcManager::GetInstance()->SetServiceProvider(m_nServiceProvider);

	nRetCode = COdbcManager::GetInstance()->OpenUserDB(CCfg::GetInstance()->GetUserOdbcName(),
													   CCfg::GetInstance()->GetUserOdbcUser(),
													   CCfg::GetInstance()->GetUserOdbcPass(),
													   CCfg::GetInstance()->GetUserOdbcDatabase(),
													   CCfg::GetInstance()->GetUserOdbcPoolSize(),
													   CCfg::GetInstance()->GetUserOdbcResponseTime());
	if (nRetCode == DB_ERROR)
	{
		//DB_Shutdown();
		CConsoleMessage::GetInstance()->WriteDatabase(_T("ERROR:User DB Open Error"));
		m_bIsRunning = false;
		return NET_ERROR;
	}
	else
	{
		CConsoleMessage::GetInstance()->WriteDatabase(_T("User DB Open OK"));
	}
	
	nRetCode = COdbcManager::GetInstance()->OpenGameDB(CCfg::GetInstance()->GetGameOdbcName(),
													   CCfg::GetInstance()->GetGameOdbcUser(),
													   CCfg::GetInstance()->GetGameOdbcPass(),
													   CCfg::GetInstance()->GetGameOdbcDatabase(),
													   CCfg::GetInstance()->GetGameOdbcPoolSize(),
													   CCfg::GetInstance()->GetGameOdbcResponseTime());

	if (nRetCode == DB_ERROR)
	{
		//DB_Shutdown();
		CConsoleMessage::GetInstance()->WriteDatabase(_T("ERROR:Game DB Open Error"));
		m_bIsRunning = false;
		return NET_ERROR;
	}
	else
	{
		CConsoleMessage::GetInstance()->WriteDatabase(_T("Game DB Open OK"));
	}

	nRetCode = COdbcManager::GetInstance()->OpenLogDB(CCfg::GetInstance()->GetLogOdbcName(),
													  CCfg::GetInstance()->GetLogOdbcUser(),
													  CCfg::GetInstance()->GetLogOdbcPass(),
													  CCfg::GetInstance()->GetLogOdbcDatabase(),
													  CCfg::GetInstance()->GetLogOdbcPoolSize(),
													  CCfg::GetInstance()->GetLogOdbcResponseTime());

	if (nRetCode == DB_ERROR)
	{
		//DB_Shutdown();
		CConsoleMessage::GetInstance()->WriteDatabase(_T("ERROR:Log DB Open Error"));
		m_bIsRunning = false;
		return NET_ERROR;
	}
	else
	{
		CConsoleMessage::GetInstance()->WriteDatabase(_T("Log DB Open OK"));
	}

	// Shop DB
	nRetCode = COdbcManager::GetInstance()->OpenShopDB(CCfg::GetInstance()->GetShopOdbcName(),
													   CCfg::GetInstance()->GetShopOdbcUser(),
													   CCfg::GetInstance()->GetShopOdbcPass(),
													   CCfg::GetInstance()->GetShopOdbcDatabase(),
													   CCfg::GetInstance()->GetShopOdbcPoolSize(),
													   CCfg::GetInstance()->GetShopOdbcResponseTime());

	if (nRetCode == DB_ERROR)
	{
		//DB_Shutdown();
		CConsoleMessage::GetInstance()->Write(_T("ERROR:Shop DB Open Error"));
		m_bIsRunning = false;
		return NET_ERROR;
	}
	else
	{
		CConsoleMessage::GetInstance()->Write(_T("Shop DB Open OK"));
	}

	// Terra User Database (Malaysia)
	if (m_nServiceProvider == SP_TERRA || m_nServiceProvider == SP_MALAYSIA )
	{
		nRetCode = COdbcManager::GetInstance()->OpenTerraDB(CCfg::GetInstance()->GetTerraOdbcName(),
													        CCfg::GetInstance()->GetTerraOdbcUser(),
													        CCfg::GetInstance()->GetTerraOdbcPass(),
													        CCfg::GetInstance()->GetTerraOdbcDatabase(),
													        CCfg::GetInstance()->GetTerraOdbcPoolSize(),
													        CCfg::GetInstance()->GetTerraOdbcResponseTime());

		if (nRetCode == DB_ERROR)
		{
			//DB_Shutdown();
			CConsoleMessage::GetInstance()->Write(_T("ERROR:Terra DB Open Error"));
			m_bIsRunning = false;
			return NET_ERROR;
		}
		else
		{
			CConsoleMessage::GetInstance()->Write(_T("Terra DB Open OK"));
		}
	}	

	// Gsp User Database
	if (m_nServiceProvider == SP_GLOBAL)
	{
		nRetCode = COdbcManager::GetInstance()->OpenGspDB(
													CCfg::GetInstance()->GetGspOdbcName(),
													CCfg::GetInstance()->GetGspOdbcUser(),
													CCfg::GetInstance()->GetGspOdbcPass(),
													CCfg::GetInstance()->GetGspOdbcDatabase(),
													CCfg::GetInstance()->GetGspOdbcPoolSize(),
													CCfg::GetInstance()->GetGspOdbcResponseTime() );

		if (nRetCode == DB_ERROR)
		{
			//DB_Shutdown();
			CConsoleMessage::GetInstance()->Write(_T("ERROR:Gsp DB Open Error"));
			m_bIsRunning = false;
			return NET_ERROR;
		}
		else
		{
			CConsoleMessage::GetInstance()->Write(_T("Gsp DB Open OK"));
		}
	}

	// Thailand User Database (Thailand)
	if (m_nServiceProvider == SP_THAILAND)
	{
		nRetCode = COdbcManager::GetInstance()->OpenThaiDB(CCfg::GetInstance()->GetTerraOdbcName(),
													        CCfg::GetInstance()->GetTerraOdbcUser(),
													        CCfg::GetInstance()->GetTerraOdbcPass(),
													        CCfg::GetInstance()->GetTerraOdbcDatabase(),
													        CCfg::GetInstance()->GetTerraOdbcPoolSize(),
													        CCfg::GetInstance()->GetTerraOdbcResponseTime());

		if (nRetCode == DB_ERROR)
		{
			//DB_Shutdown();
			CConsoleMessage::GetInstance()->WriteDatabase( _T("ERROR:Thai DB Open Error") );
			m_bIsRunning = false;
			return NET_ERROR;
		}
		else
		{
			CConsoleMessage::GetInstance()->WriteDatabase( _T("Thai DB Open OK") );
		}
	}

	if (StartDatabaseThread()     == NET_ERROR)	return NET_ERROR;
	if (StartLogDatabaseThread()  == NET_ERROR) return NET_ERROR;
	if (StartUserDatabaseThread() == NET_ERROR) return NET_ERROR;

	return NET_OK;
}

int CFieldServer::Start()
{
	int nRetCode = 0;
	HRESULT hr;
	m_bIsRunning = true;
	if (StartCfg()			== NET_ERROR) return NET_CFG_LOADERROR;
	
	/*ccfg security, Juver, 2018/07/08 */
	if (CheckRPCIDHash() == NET_ERROR ) 
	{
		m_bIsRunning = FALSE;
		CConsoleMessage::GetInstance()->Write("ERROR:PCID Hash Failed");	
		return NET_CFG_LOADERROR;	
	}

	if ( !isRPCIDHashValid() ) return NET_ERROR;	

	if (StartIOCPList()		== NET_ERROR) return NET_ERROR;    	
	if (StartMsgManager()	== NET_ERROR) return NET_ERROR;
	if (StartIOCP()			== NET_ERROR) return NET_ERROR;
	if (StartClientManager()== NET_ERROR) return NET_ERROR;	
	if (StartDbManager()    == NET_ERROR) return NET_ERROR;
	if (StartWorkThread()	== NET_ERROR) return NET_ERROR;

	// nProtect GameGuard dll 을 초기화 한다.
	DWORD dwGGErrCode = ERROR_SUCCESS;
	dwGGErrCode = InitGameguardAuth ("", 1000, FALSE, 0x03);
	if (dwGGErrCode != ERROR_SUCCESS)
	{	
		m_bGameGuardInit = false;
		CConsoleMessage::GetInstance()->Write(
			                                  _T("nProtect GameGuard server DLL load failed"));
	}
	else
	{
		m_bGameGuardInit = true;		
		CConsoleMessage::GetInstance()->Write(
			                                  _T("nProtect GameGuard server DLL load success"));
		m_bGameGuardAuth = CCfg::GetInstance()->GemeGuardAuth();
		if (m_bGameGuardAuth == true)
		{
			CConsoleMessage::GetInstance()->Write( _T("nProtect GameGuard Auth ON") );
		}
		else
		{
			CConsoleMessage::GetInstance()->Write(
		                                          _T("nProtect GameGuard Auth OFF"));
		}
	}
	
	//	Note : 가이아 서버 생성.
	m_dwFieldServerID = CCfg::GetInstance()->GetServerField();
	DWORD dwMaxClient = CCfg::GetInstance()->GetMaxClient();
	int nChannel = CCfg::GetInstance()->GetServerChannel();
	bool bPKMODE = CCfg::GetInstance()->IsPKServer() == true;
	SERVER_UTIL::CSystemInfo SysInfo;

	hr = DxFieldInstance::Create(SysInfo.GetAppPath(),
		                         this,
								 CConsoleMessage::GetInstance(),
								 COdbcManager::GetInstance(),
								 m_nServiceProvider,
								 NULL,
								 m_dwFieldServerID,
								 dwMaxClient,
								 bPKMODE,
								 nChannel,
								 m_bPKLess,
								 m_dwLangSet );
	if ( FAILED(hr) )
	{
		CConsoleMessage::GetInstance()->Write( _T("DxFieldInstance::Create hr failed") );
		m_bIsRunning = false;
		return NET_ERROR;
	}

	if (StartUpdateThread() == NET_ERROR) return NET_ERROR;	
	if (StartListenThread() == NET_ERROR) return NET_ERROR;		
	
	// Connect Session Server
	nRetCode = SessionConnect(CCfg::GetInstance()->GetSessionServerIP(), CCfg::GetInstance()->GetSessionServerPort());
	if (nRetCode != NET_OK)
	{	
		m_bIsRunning = false;
		return NET_ERROR;
	}

	// Send Server info to Session Server
	SessionSndSvrInfo();
	SessionSndSvrInfo();

	CConsoleMessage::GetInstance()->Write( _T("NET_MSG_BASE %d"), NET_MSG_BASE );
	
	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, _T("================================"));
	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, _T("Server Start OK"));
	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, _T("================================"));

	m_nStatus = S_SERVER_RUNING;

	// 알고 있는 주소에 자신의 주소, Session 서버의 주소, Agent 서버의 주소를 등록한다.
	//SERVER_UTIL::CAddressChecker::GetInstance()->addKnownAddress(CCfg::GetInstance()->GetServerIP());
	//SERVER_UTIL::CAddressChecker::GetInstance()->addKnownAddress(CCfg::GetInstance()->GetSessionServerIP());
	//SERVER_UTIL::CAddressChecker::GetInstance()->addKnownAddress(CCfg::GetInstance()->GetAgentIP());
	/*dmk14 whitelist*/
	m_IPFilter.SetServer( this );
	
	m_IPFilter.ListLoad();
	m_IPFilter.AddIPKnown( CCfg::GetInstance()->GetServerIP() );
	m_IPFilter.AddIPKnown( CCfg::GetInstance()->GetSessionServerIP() );
	m_IPFilter.AddIPKnown( CCfg::GetInstance()->GetAgentIP() );

	const TCHAR* szServerName = CCfg::GetInstance()->GetServerName();
	if ( szServerName )
	{
		char szTempChar[64];
		sprintf( szTempChar, "%s_%d", szServerName, m_dwFieldServerID );
		SetWindowText( m_hWnd, szTempChar );
	}
	else
	{
		char szTempChar[64];
		GetWindowText( m_hWnd, szTempChar, 64 );
		sprintf( szTempChar, "%s_%d", szTempChar, m_dwFieldServerID );
		SetWindowText( m_hWnd, szTempChar );
	}
	
	//iplimit
	/*if ( m_pClientManager )
	{
		m_pClientManager->IPClear();
		m_pClientManager->IPSetClientMax( CCfg::GetInstance()->GetIPClientMax() );
		m_pClientManager->IPAddExcluded( CCfg::GetInstance()->GetServerIP() );
		m_pClientManager->IPAddExcluded( CCfg::GetInstance()->GetSessionServerIP() );
		m_pClientManager->IPAddExcluded( CCfg::GetInstance()->GetAgentIP() );
	}*/

	m_dwPeakPC = 0;
	m_dwPeakInstance = 0;

	/*client hash check, EJCode, 2018/11/26 */
	client_check_get_client_hash( SysInfo.GetAppPath().GetString() );
	client_check_get_mshield_hash( SysInfo.GetAppPath().GetString() );

	return NET_OK;
}

int CFieldServer::FinalStop()
{
	// Stop All Thread and exit
	DWORD dwExitCode = 0;

    CloseAllClient();

	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, "== Please wait until server final stop");

	LockOn();
	m_bIsRunning = false;
	LockOff();

	if( m_bUseEventThread )	
	{
		::SetEvent( m_hUpdateQuitEvent );

		::WaitForSingleObject( m_hUpdateThread, INFINITE );
		SAFE_CLOSE_HANDLE( m_hUpdateQuitEvent );
	}else{
		while ( m_hUpdateThread )	
		{
			Sleep( 0 );
		}
	}


	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, "== Update Thread stopped.");

	//CDbExecuter::GetInstance()->endThread();
	//CLogDbExecuter::GetInstance()->endThread();
	//CUserDbExecuter::GetInstance()->endThread();

	StopListenThread();
	StopIOCPList();
	StopWorkThread();
	StopIOCP();	

	// 클라이언트 저장 클래스 종료
	SAFE_DELETE(m_pClientManager);
	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, "== Client Manager closed");
	// IOCP 종료
	SAFE_DELETE(m_pSendIOCP);
	SAFE_DELETE(m_pRecvIOCP);
	// Message Queue 종료
	SAFE_DELETE(m_pRecvMsgManager);
	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, "== Message Manager closed");
	// DB 연결종료
	COdbcManager::GetInstance()->ReleaseInstance();
	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, "== Database Manager closed");
	// CFG class 종료
	CCfg::GetInstance()->ReleaseInstance();

	// nProtect GameGuard 종료
	if (m_bGameGuardInit == true)
	{
		CleanupGameguardAuth();		
	}

	// Put message to console
	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, ("================================"));	
	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, "Server Stop OK");	
	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, ("================================"));

	m_nStatus = S_SERVER_STOP;

	return NET_OK;
}

int CFieldServer::Stop()
{
	// 서버의 정지를 예약한다.
	m_bReservationServerStop = true;
	CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, "== Please wait until server stop");
	CloseAllClient();
	//GLMSG::SNET_REQ_SERVERTEST InnerMsg;
	//InsertMsg ( 0, (char*) &InnerMsg );

	return NET_OK;
}

int	CFieldServer::Pause()
{
	::closesocket(m_sServer);
	m_sServer = INVALID_SOCKET;
	CloseAllClient();
	m_nStatus = S_SERVER_PAUSE;
	// Put message to console
	CConsoleMessage::GetInstance()->Write( ("================================"));	
	CConsoleMessage::GetInstance()->Write( "Server Pause OK");	
	CConsoleMessage::GetInstance()->Write( ("================================"));

	return NET_OK;	
}

int CFieldServer::Resume()
{
	if (StartListenThread() == NET_OK)
	{
		m_nStatus = S_SERVER_RUNING;
		// Put message to console
		CConsoleMessage::GetInstance()->Write( ("================================"));	
		CConsoleMessage::GetInstance()->Write( "Server Resume OK");	
		CConsoleMessage::GetInstance()->Write( ("================================"));

		return NET_OK;
	}
	else
	{
		// Put message to console
		CConsoleMessage::GetInstance()->Write( ("================================"));	
		CConsoleMessage::GetInstance()->Write( "Server Resume Failed");	
		CConsoleMessage::GetInstance()->Write( ("================================"));

		return NET_ERROR;
	}
}

int	CFieldServer::ReStart()
{
	Stop();
	Start();
	return NET_OK;
}

// close all client connections
void CFieldServer::CloseAllClient()
{	
	if (m_pClientManager)
	{
		for (DWORD i=0; i < (DWORD) m_pClientManager->GetMaxClient(); ++i)
		{
			if (m_pClientManager->IsOnline(i))
				CloseClient(i);
		}		
	}
}


namespace PROFILE
{
	void DumpOutputToNon ();
};

void CFieldServer::ResetGaeaID ( DWORD dwClient )
{
	m_pClientManager->SetGaeaID ( dwClient, GAEAID_NULL );
}

/*
// 사용자를 로그아웃 시킨다.
bool CFieldServer::CloseClient(
	DWORD dwClient )
{	
	LockOn();

	if ( dwClient<0 ) return false;

	int   nUserNum = 0;	
	DWORD dwGaeaID = 0;
	bool bReturn = false;

	// 서버간 통신종료
	if ( dwClient < NET_RESERVED_SLOT )
	{
		if (dwClient == m_dwSession && m_bIsRunning == true)
		{
			// Session 서버 연결 끊김.
			if (true == m_pClientManager->CloseClient( m_dwSession ) )
			{
				// 연결이 끊겼을 경우 바로 다시 연결 시도 하지 않는다. Heart Beat에서 다시 체크하기 때문이다.
				SessionConnectSndSvrInfo();

				CConsoleMessage::GetInstance()->Write(_T("================================"));
				CConsoleMessage::GetInstance()->Write(_T("Session Server Connection Failed"));
				CConsoleMessage::GetInstance()->Write(_T("================================"));

				bReturn = true;
			}
		}
	}
	else // 일반 사용자 종료
	{	
		DWORD dwCloseSlot = dwClient;

		if (dwCloseSlot != 0)
		{
			nUserNum = m_pClientManager->GetUserNum(dwCloseSlot);
			dwGaeaID = m_pClientManager->GetGaeaID(dwCloseSlot);

			if ( true == m_pClientManager->CloseClient(dwCloseSlot) )
			{
				bReturn = true;

				if (nUserNum > 0 &&	dwGaeaID != GAEAID_NULL)
				{
					CConsoleMessage::GetInstance()->Write(
						LOG_CONSOLE, 
						_T("(CID %d, GID %d) Drop & Save Character"),
						dwCloseSlot,
						dwGaeaID );
					// 가이아서버 캐릭터 드랍 예약
					GLGaeaServer::GetInstance().ReserveDropOutPC(dwGaeaID);
				}

				// 현재 서버상태 전송
				SessionSndSvrCurState();
			}
		}
		else
		{
			if( true == m_pClientManager->CloseClient(dwClient) )
			{
				bReturn = true;

				// 현재 서버상태 전송
				SessionSndSvrCurState();
			}
		}
	}

	LockOff();

	return bReturn;
}
*/

// 사용자를 로그아웃 시킨다.
void CFieldServer::CloseClient(
							   DWORD dwClient )
{	
	LockOn();
	{
		if ((m_pClientManager->IsOnline(dwClient) == false) || (dwClient<0))
		{
			LockOff();
			return;
		}

		int   nUserNum = 0;	
		DWORD dwGaeaID = 0;

		// 서버간 통신종료
		if ((dwClient < NET_RESERVED_SLOT) &&  (dwClient >= 0))
		{
			CConsoleMessage::GetInstance()->Write( _T("Session Server connection closed %d"), dwClient );

			if (dwClient == NET_RESERVED_SESSION && m_bIsRunning == true)
			{
				// Session 서버 연결 끊김.
				m_pClientManager->CloseClient( NET_RESERVED_SESSION );
				
				// 연결이 끊겼을 경우 바로 다시 연결 시도 하지 않는다. Heart Beat에서 다시 체크하기 때문이다.
				//SessionConnectSndSvrInfo();

				CConsoleMessage::GetInstance()->Write(_T("================================"));
				CConsoleMessage::GetInstance()->Write(_T("Session Server Connection Failed"));
				CConsoleMessage::GetInstance()->Write(_T("================================"));
				LockOff();
				return;
			}
		}
		else // 일반 사용자 종료
		{	
			DWORD dwFieldAgent  = 0;
			DWORD dwFieldClient = 0;

			if (m_pClientManager->IsClientSlot(dwClient))
			{
				dwFieldAgent  = m_pClientManager->GetSlotFieldAgent(dwClient);				
				// 등록된 IP 리스트를 제거한다.
				// SERVER_UTIL::CAddressChecker::GetInstance()->Delete(m_pClientManager->GetClientIP(dwClient));
			}
			else
			{
				dwFieldAgent  = dwClient;				
			}

			if (dwFieldAgent != 0)
			{
				nUserNum = m_pClientManager->GetUserNum(dwFieldAgent);

				/*dmk14 offline vend new code*/
				bool bOffVend = false;
				if (nUserNum > 0)
				{	
					// CConsoleMessage::GetInstance()->Write(_T("%d User connection closed"), nUserNum);
					dwGaeaID = m_pClientManager->GetGaeaID(dwFieldAgent);
					if (dwGaeaID != GAEAID_NULL)
					{
						PGLCHAR pGLChar = GLGaeaServer::GetInstance().GetChar( dwGaeaID );
						if( pGLChar )	bOffVend = pGLChar->m_sPMarket.IsPremiumMarket();
						if( !bOffVend ) 
						{
							CConsoleMessage::GetInstance()->Write(LOG_CONSOLE, 
								_T("(CID %d, GID %d) Drop & Save Character"),
								dwFieldAgent,
								dwGaeaID);
							// 가이아서버 캐릭터 드랍 예약
							GLGaeaServer::GetInstance().ReserveDropOutPC(dwGaeaID);
						}
					}/*else{						
						CConsoleMessage::GetInstance()->WriteConsole( _T("ERROR:%d Invalid Gaea ID"), dwClient );
					}*/
				}

				if( !bOffVend )
					m_pClientManager->CloseClient(dwFieldAgent);
			}
			else
			{
				m_pClientManager->CloseClient(dwClient);
			}

			// 현재 서버상태 전송
			SessionSndSvrCurState();
		}
	}

	LockOff();
}

void CFieldServer::Send(DWORD dwClient, LPPER_IO_OPERATION_DATA PerIoData, DWORD dwSize)
{
	CConsoleMessage::GetInstance()->Write( _T("ERROR:Illegal Function call (Send)(%d)"), dwClient);
}

void CFieldServer::SendAllClient(LPVOID pBuffer)
{
    if (pBuffer == NULL) return;

	DWORD dwMaxClient;
	DWORD dwStartClient;

	dwStartClient = NET_RESERVED_SLOT;
	dwMaxClient = (DWORD) m_nMaxClient;
	/* disable to les flood on field */
	//CConsoleMessage::GetInstance()->Write(_T("Sending message to all users"));

	for (dwStartClient; dwStartClient < dwMaxClient; ++dwStartClient)
	{
		// 게임중이면 메시지를 보낸다
		if (m_pClientManager->IsGaming(dwStartClient))		
			SendClient(dwStartClient, pBuffer);
	}
}

int CFieldServer::SendClient( DWORD dwClient, LPVOID pBuffer )
{
	//offline vend
	DWORD dwGaeaID = m_pClientManager->GetGaeaID(dwClient);
	PGLCHAR pGLChar = GLGaeaServer::GetInstance().GetChar( dwGaeaID );
	bool bOffVend = false;
	if( pGLChar )	bOffVend = pGLChar->m_sPMarket.IsPremiumMarket();
	if( bOffVend )	return NET_OK;

    if ( (pBuffer == NULL) || (!m_pClientManager->IsOnline(dwClient)) ) 
	{		
		return NET_ERROR;
	}
	else
	{
		return m_pClientManager->SendClient(dwClient, pBuffer);	
	}
}

int CFieldServer::SendAgent( DWORD dwClient, LPVOID pBuffer )
{
	//offline vend
	DWORD dwGaeaID = m_pClientManager->GetGaeaID(dwClient);
	PGLCHAR pGLChar = GLGaeaServer::GetInstance().GetChar( dwGaeaID );
	bool bOffVend = false;
	if( pGLChar )	bOffVend = pGLChar->m_sPMarket.IsPremiumMarket();
	if( bOffVend )	return NET_OK;
	
	if ( (pBuffer == NULL) || (!m_pClientManager->IsOnline(dwClient)) ) 
	{
		return NET_ERROR;
	}
	else
	{
		return m_pClientManager->SendAgent( dwClient, pBuffer );
	}
}

int CFieldServer::SendAgent( LPVOID pBuffer )
{	
    if ( pBuffer == NULL )
	{
		return NET_ERROR;
	}
	else
	{
		return m_pClientManager->SendAgent(m_dwAgentSlot, pBuffer);
	}
}

int CFieldServer::CheckTracingUser( const char *pUserAccount, const int nUserNum )
{

	int i;
	for( i = 0; i < m_vecTracingData.size(); i++ )
	{
		STracingData data = m_vecTracingData[i];
		if( strcmpi( data.strAccount.c_str(), pUserAccount ) == 0 ||
			data.dwUserNum  == nUserNum )
		{
			return i;
		}
	}

	return -1;
}

void CFieldServer::RecvMsgProcess()
{
	MSG_LIST* pMsg = NULL;	

	// Flip Messge Queue
	m_pRecvMsgManager->MsgQueueFlip();

	// Get Messages and Processing message
	pMsg = m_pRecvMsgManager->GetMsg();
	while (pMsg != NULL)
	{
		// 메시지 처리
		MsgProcess(pMsg);					
		pMsg = m_pRecvMsgManager->GetMsg(); // 다음메시지 가져오기
	}
	
}

void CFieldServer::FieldSrvHeartBeatCheck( DWORD dwUdpateTime )
{
	DWORD dwCurrentTime = timeGetTime();

	if ( (dwCurrentTime-m_dwHeartBeatTime) >= dwUdpateTime )
	{	
		// 잘못된 접속 종료
		// 접속만 하고 더이상 메시지를 보내지 않아서 GAEAID_NULL 인 상태
		for ( DWORD dwCheck=NET_RESERVED_SLOT; dwCheck<(DWORD) m_nMaxClient; ++dwCheck )
		{
			if ((m_pClientManager->GetSlotType(dwCheck) == NET_SLOT_CLIENT) && 
				(m_pClientManager->GetGaeaID(dwCheck)   == GAEAID_NULL))
			{
				//CConsoleMessage::GetInstance()->Write(_T("INFO:Client GaeaID NULL Client %d InOnline %d"), 
				//	dwCheck, m_pClientManager->IsOnline(dwCheck));
				CloseClient(dwCheck);
			}
		}

		/////////////////////////////////////////////////////////////////////////////
		// Session Server State Check
		if ((dwCurrentTime - m_dwHeartBeatStart1) > SESSION_CHECK_TIME)
		{
			if( !m_pClientManager->IsOnline( NET_RESERVED_SESSION ) )
			{
				SessionConnectSndSvrInfo();
			}
			m_dwHeartBeatStart1 = timeGetTime();
		}

		// HeartBeat Check
		//if ( (dwCurrentTime - m_dwHeartBeatStart2) > HEARTBEAT_TIME )
		//{
		//	if (FALSE) // Session Send heartbeat
		//	{
		//		if (true == m_pClientManager->IsOnline( NET_RESERVED_SESSION ) )
		//		{
		//			m_pClientManager->ResetHeartBeat( NET_RESERVED_SESSION );
		//			NET_HEARTBEAT_SERVER_REQ HeartbeatServerReq;
		//			SendClient( NET_RESERVED_SESSION, &HeartbeatServerReq );
		//		}
		//		else
		//		{
		//			CConsoleMessage::GetInstance()->Write(_T("================================"));
		//			CConsoleMessage::GetInstance()->Write(_T("Session Server Connection Failed"));
		//			CConsoleMessage::GetInstance()->Write(_T("Reconnecting Session Server."));
		//			CConsoleMessage::GetInstance()->Write(_T("================================"));

		//			// SessionConnectSndSvrInfo();
		//		}
		//	}
		//	else // Session Check heartbeat
		//	{
		//		if (true == m_pClientManager->IsOnline( NET_RESERVED_SESSION ) &&
		//			false == m_pClientManager->CheckHeartBeat( NET_RESERVED_SESSION ) )
		//		{
		//			CloseClient( NET_RESERVED_SESSION );
		//		}
		//	}
		//}
		m_dwHeartBeatTime = dwCurrentTime;
	}
}

void CFieldServer::PrintDebugMsg( DWORD dwUdpateTime )
{
	char szLog[C_BUFFER_SIZE+1] = {0};
	// 프레임 측정
	DWORD dwCurrentTime = timeGetTime();
	if ( (dwCurrentTime-m_dwFrameTime) >= dwUdpateTime )
	{		


		// 프로파일링 
		DWORD dwProfileStart = timeGetTime();
		PROFILE_DUMPOUTPUT_FILE();
		DWORD dwProfileEnd = timeGetTime() - dwProfileStart;

		// 처리시간이 500msec 이상이라면 기록한다.
		if (dwProfileEnd > 500)
		{
			CConsoleMessage::GetInstance()->Write(
				_T("INFO:PROFILE_DUMPOUTPUT_FILE %d msec"), 
				dwProfileEnd );
		}

		// DB 연결유지
		// COdbcManager::GetInstance()->CheckConnection();

		// 업데이트 정보 출력
		float fFrame = ( (float) ( (m_dwFrameCount * dwUdpateTime) / (float) (timeGetTime() - m_dwFrameTime) ) );
		m_dwFrameCount = 0;

		DWORD dwCurPC = GLGaeaServer::GetInstance().GetNumPC();
		if ( dwCurPC > m_dwPeakPC )
			m_dwPeakPC = dwCurPC;
		
		DWORD dwNumInstance = GLGaeaServer::GetInstance().GetInstantMapNum();
		DWORD dwNumInstance2 = GLGaeaServer::GetInstance().GetInstantMapNum2();
		DWORD dwNumInstanceStuck = GLGaeaServer::GetInstance().GetInstantMapStuckNum();
		if ( dwNumInstance > m_dwPeakInstance )
			m_dwPeakInstance = dwNumInstance;

		CDebugSet::ToLogFile(
			_T("**************************************************"));

		StringCchPrintf(
			szLog,
			C_BUFFER_SIZE+1,
			_T("Update %d FPS Avg %d msec Character %d Max %d"), 
			(int) (fFrame/(dwUdpateTime/1000)), 
			(int) (dwUdpateTime/fFrame),
			(int) dwCurPC, (int)m_dwPeakPC );				
		CConsoleMessage::GetInstance()->WriteInfo( szLog );
		CDebugSet::ToLogFile( szLog );

		// 패킷 처리량 디스플레이
		StringCchPrintf(
			szLog,
			C_BUFFER_SIZE+1,
			_T("In Packet Count %d/sec Traffic %d(bytes)/sec"),
			(int) (m_pClientManager->getInPacketCount() / (dwUdpateTime/1000)),
			(int) (m_pClientManager->getInPacketSize() / (dwUdpateTime/1000)) );
		CConsoleMessage::GetInstance()->WriteInfo( szLog );
		CDebugSet::ToLogFile( szLog );

		StringCchPrintf(
			szLog,
			C_BUFFER_SIZE+1,
			_T("Out Packet Count %d/sec Traffic %d(bytes)/sec"),
			(int) (m_pClientManager->getOutPacketCount() / (dwUdpateTime/1000)),
			(int) (m_pClientManager->getOutPacketSize() / (dwUdpateTime/1000)) );
		CConsoleMessage::GetInstance()->WriteInfo( szLog );
		CDebugSet::ToLogFile( szLog );

		StringCchPrintf(
			szLog,
			C_BUFFER_SIZE+1,
			_T("Compress Count %d/sec Traffic %d(bytes)/sec"),
			(int) (m_pClientManager->getOutCompressCount() / (dwUdpateTime/1000)),
			(int) (m_pClientManager->getOutCompressSize() / (dwUdpateTime/1000)) );
		CConsoleMessage::GetInstance()->WriteInfo( szLog );
		CDebugSet::ToLogFile( szLog );
		StringCchPrintf(
			szLog,
			C_BUFFER_SIZE+1,
			_T("Msg Queue Size %d Send %d/%d Recv %d/%d"), 
			m_pRecvMsgManager->m_pMsgBack->GetCount(),
			m_pSendIOCP->GetUseCount(),
			m_pSendIOCP->GetUnUseCount(),
			m_pRecvIOCP->GetUseCount(),
			m_pRecvIOCP->GetUnUseCount() );
		CConsoleMessage::GetInstance()->WriteInfo( szLog );
		CDebugSet::ToLogFile( szLog );

		//stats line
		CConsoleMessage::GetInstance()->WriteStats( _T("IPFilter IPKnown :%d KnownRef:%I64u"), m_IPFilter.GetIPKnownNum(), m_IPFilter.GetRefKnown() );
		CConsoleMessage::GetInstance()->WriteStats( _T("IPFilter IPBlock :%d BlockRef:%I64u"), m_IPFilter.GetIPBlockNum(), m_IPFilter.GetRefBlocked() );

		//iplimit
		/*CConsoleMessage::GetInstance()->WriteStats( _T("IPLIMIT IP:%u Client:%u Excluded:%u Denied:%I64u"),
			m_pClientManager->IPGetCount(), m_pClientManager->IPGetTotalClient(), 
			m_pClientManager->IPGetExcludedCount(), m_pClientManager->IPGetDenyNum());*/

		CConsoleMessage::GetInstance()->WriteStats( _T("INSTANCE %u %u Max:%u Stuck:%u"),  
			dwNumInstance, dwNumInstance2, m_dwPeakInstance, dwNumInstanceStuck );

		StringCchPrintf( szLog, C_BUFFER_SIZE+1, _T("IPFILTER IPKnown :%u KnownRef:%I64u"), m_IPFilter.GetIPKnownNum(), m_IPFilter.GetRefKnown() );
		CDebugSet::ToLogFile( szLog );

		StringCchPrintf( szLog, C_BUFFER_SIZE+1, _T("IPFILTER IPBlock :%u BlockRef:%I64u"), m_IPFilter.GetIPBlockNum(), m_IPFilter.GetRefBlocked() );
		CDebugSet::ToLogFile( szLog );

		//iplimit
		/*StringCchPrintf( szLog, C_BUFFER_SIZE+1, _T("IPLIMIT IP:%u Client:%u Excluded:%u Denied:%I64u"),
			m_pClientManager->IPGetCount(), m_pClientManager->IPGetTotalClient(), 
			m_pClientManager->IPGetExcludedCount(), m_pClientManager->IPGetDenyNum());
		CDebugSet::ToLogFile( szLog );*/

		StringCchPrintf( szLog, C_BUFFER_SIZE+1, _T("INSTANCE %u %u Max:%u Stuck:%u"), 
			dwNumInstance, dwNumInstance2, m_dwPeakInstance, dwNumInstanceStuck );
		CDebugSet::ToLogFile( szLog );
		


		CConsoleMessage::GetInstance()->WriteStats( _T("..") );
		CConsoleMessage::GetInstance()->WriteStats( _T(".") );

		m_pClientManager->resetPacketCount();

		m_dwFrameTime = dwCurrentTime;
	} 
	else
	{
		DWORD dwProfileDumpStart = timeGetTime();
		g_profile_1.DumpOutputToNon();
		DWORD dwProfileDumpEnd = timeGetTime() - dwProfileDumpStart;
		if (dwProfileDumpEnd > 500)
			CConsoleMessage::GetInstance()->Write(
			_T("INFO:PROFILE::DumpOutputToNon() %d msec"),
			dwProfileDumpEnd );
	}
}

void CFieldServer::EndUpdateThread()
{
	TRACE( "[UpdateProc End]\n" );
	CConsoleMessage::GetInstance()->Write( _T("= [UpdateProc End] - Begin") );
	{
		//	Note : 모든 활동 PC,NPC를 종료시킨다.
		GLGaeaServer::GetInstance().ClearDropObj();
		GLGaeaServer::GetInstance().ClearReservedDropOutPC();


		//	Note : 클럽배틀 정보를 저장한다.
		GLGaeaServer::GetInstance().SaveClubBattle();

		// DB 작업이 끝날때 까지 기다린다.
		// Last db action
		CDbExecuter::GetInstance()->ExecuteJobLast();
		CLogDbExecuter::GetInstance()->ExecuteJobLast();
		CUserDbExecuter::GetInstance()->ExecuteJobLast();
		CConsoleMessage::GetInstance()->Write("= Last Database Job Executed");

		//	Note : 가이아 서버 중단뒤 메모리 해제.
		DxFieldInstance::CleanUp();
	}
	CConsoleMessage::GetInstance()->Write("= [UpdateProc End] - End");
	TRACE ( "[Gaea Server CleanUp]\n" );

	//	업데이트 스래드 사라짐.
	CloseHandle( m_hUpdateThread );
	m_hUpdateThread = NULL;
	m_bUpdateEnd	= true;
}

void CFieldServer::IPFilterBlockAdd( std::string strIP )
{
	if ( strIP.size() <= 0 )
	{
		CConsoleMessage::GetInstance()->Write("Invalid IP");
		return;
	}

	if ( m_IPFilter.IsBlocked( strIP.c_str() ) )
	{
		CConsoleMessage::GetInstance()->Write("%s IP Already in Blocked List", strIP.c_str() );
		return;
	}

	NET_IPFILTER_SERVER_TO_SESSION_BLOCK_ADD netMsg;
	StringCchCopy( netMsg.szIP, MAX_IP_LENGTH+1, strIP.c_str() );
	SendSession( &netMsg );
}

void CFieldServer::IPFilterBlockDel( std::string strIP )
{
	if ( strIP.size() <= 0 )
	{
		CConsoleMessage::GetInstance()->Write("Invalid IP");
		return;
	}

	if ( !m_IPFilter.IsBlocked( strIP.c_str() ) )
	{
		CConsoleMessage::GetInstance()->Write("%s IP Not Exist in Blocked List", strIP.c_str() );
		return;
	}

	NET_IPFILTER_SERVER_TO_SESSION_BLOCK_DEL netMsg;
	StringCchCopy( netMsg.szIP, MAX_IP_LENGTH+1, strIP.c_str() );
	SendSession( &netMsg );
}

void CFieldServer::IPFilterKnownAdd( std::string strIP )
{
	if ( strIP.size() <= 0 )
	{
		CConsoleMessage::GetInstance()->Write("Invalid IP");
		return;
	}

	if ( m_IPFilter.IsIPKnown( strIP.c_str() ) )
	{
		CConsoleMessage::GetInstance()->Write("%s IP Already in Known List", strIP.c_str() );
		return;
	}

	NET_IPFILTER_SERVER_TO_SESSION_KNOWN_ADD netMsg;
	StringCchCopy( netMsg.szIP, MAX_IP_LENGTH+1, strIP.c_str() );
	SendSession( &netMsg );
}

void CFieldServer::IPFilterKnownDel( std::string strIP )
{
	if ( strIP.size() <= 0 )
	{
		CConsoleMessage::GetInstance()->Write("Invalid IP");
		return;
	}

	if ( !m_IPFilter.IsIPKnown( strIP.c_str() ) )
	{
		CConsoleMessage::GetInstance()->Write("%s IP Not Exist in Known List", strIP.c_str() );
		return;
	}

	NET_IPFILTER_SERVER_TO_SESSION_KNOWN_DEL netMsg;
	StringCchCopy( netMsg.szIP, MAX_IP_LENGTH+1, strIP.c_str() );
	SendSession( &netMsg );
}


void CFieldServer::IPFilterSessionBlockAdd( std::string strIP )
{
	if ( strIP.size() <= 0 )
	{
		CConsoleMessage::GetInstance()->Write("IPFilterSessionBlockAdd Invalid IP");
		return;
	}

	BOOL bDone = m_IPFilter.AddIPBlock( strIP );
	if ( bDone )
		CConsoleMessage::GetInstance()->Write("IPFilter added to block IP:%s", strIP.c_str() );
	else
		CConsoleMessage::GetInstance()->Write("IPFilter failed to add block IP:%s", strIP.c_str() );

	IPFilterCleanIP( strIP );
}

void CFieldServer::IPFilterSessionBlockDel( std::string strIP )
{
	if ( strIP.size() <= 0 )
	{
		CConsoleMessage::GetInstance()->Write("IPFilterSessionBlockDel Invalid IP");
		return;
	}

	BOOL bDone = m_IPFilter.RemoveIPBlock( strIP );
	if ( bDone )
		CConsoleMessage::GetInstance()->Write("IPFilter removed block IP:%s", strIP.c_str() );
	else
		CConsoleMessage::GetInstance()->Write("IPFilter failed to remove block IP:%s", strIP.c_str() );
}

void CFieldServer::IPFilterSessionKnownAdd( std::string strIP )
{
	if ( strIP.size() <= 0 )
	{
		CConsoleMessage::GetInstance()->Write("IPFilterSessionKnownAdd Invalid IP");
		return;
	}

	BOOL bDone = m_IPFilter.AddIPKnown( strIP );
	if ( bDone )
		CConsoleMessage::GetInstance()->Write("IPFilter added to known IP:%s", strIP.c_str() );
	else
		CConsoleMessage::GetInstance()->Write("IPFilter failed to add known IP:%s", strIP.c_str() );
}

void CFieldServer::IPFilterSessionKnownDel( std::string strIP )
{
	if ( strIP.size() <= 0 )
	{
		CConsoleMessage::GetInstance()->Write("IPFilterSessionKnownDel Invalid IP");
		return;
	}

	BOOL bDone = m_IPFilter.RemoveIPKnown( strIP );
	if ( bDone )
		CConsoleMessage::GetInstance()->Write("IPFilter removed known IP:%s", strIP.c_str() );
	else
		CConsoleMessage::GetInstance()->Write("IPFilter failed to remove known IP:%s", strIP.c_str() );
}

void CFieldServer::IPFilterSave()
{
	NET_IPFILTER_SERVER_TO_SESSION_LISTSAVE netMsg;
	SendSession( &netMsg );
}

void CFieldServer::IPFilterCleanIP( std::string strIP )
{
	if ( !m_pClientManager )	return;
	if ( !strIP.size() )		return;

	for( int i=NET_RESERVED_SLOT; i<m_nMaxClient; ++i )
	{
		if( !m_pClientManager->IsOnline(i) )	continue;
		if( m_pClientManager->GetSlotType(i) != NET_SLOT_CLIENT )	continue;

		std::string strClientIP = m_pClientManager->GetClientIP( i );
		if ( !strClientIP.size() )	continue;

		if ( strClientIP.compare( strIP ) == 0)
		{
			CConsoleMessage::GetInstance()->Write(_T("IPFilter Drop Client:%d IP:%s"), i, strIP.c_str());
			CloseClient( i );
		}
	}
}

/*client hash check, EJCode, 2018/11/26 */
void CFieldServer::client_check_get_client_hash( std::string app_path )
{
	if ( !RPARAM::client_hash_check_use )
	{
		CConsoleMessage::GetInstance()->WriteDatabase( _T("Client Hash Check Disabled") );
		return;
	}

	HASH::CHash g_hashObj_1;
	g_hashObj_1.SetHashAlgorithm( HASH::MD5 );
	g_hashObj_1.SetHashOperation( HASH::FILE_HASH );
	g_hashObj_1.SetHashFormat( HASH::UPPERCASE_NOSPACES );

	std::string client_file_1 = app_path; 
	client_file_1 += "\\";
	client_file_1 += RPARAM::client_hash_check_game_file_1;

	g_hashObj_1.SetHashFile( client_file_1.c_str() );

	StringCchCopy( m_client_hash1, CLIENT_HASH_SIZE, g_hashObj_1.DoHash().GetString() );

	CConsoleMessage::GetInstance()->WriteDatabase( _T("Client: %s Hash: %s"), client_file_1.c_str(), m_client_hash1 );
	//CDebugSet::ToFileWithTime( "_client_hash_check.txt", "field read file: %s hash: %s", client_file_1.c_str(), m_client_hash1 );

	HASH::CHash g_hashObj_2;
	g_hashObj_2.SetHashAlgorithm( HASH::MD5 );
	g_hashObj_2.SetHashOperation( HASH::FILE_HASH );
	g_hashObj_2.SetHashFormat( HASH::UPPERCASE_NOSPACES );

	std::string client_file_2 = app_path; 
	client_file_2 += "\\";
	client_file_2 += RPARAM::client_hash_check_game_file_2;

	g_hashObj_2.SetHashFile( client_file_2.c_str() );

	StringCchCopy( m_client_hash2, CLIENT_HASH_SIZE, g_hashObj_2.DoHash().GetString() );

	CConsoleMessage::GetInstance()->WriteDatabase( _T("Client: %s Hash: %s"), client_file_2.c_str(), m_client_hash2 );
	//CDebugSet::ToFileWithTime( "_client_hash_check.txt", "field read file: %s hash: %s", client_file_2.c_str(), m_client_hash2 );
}

void CFieldServer::client_check_get_mshield_hash( std::string app_path )
{
#ifdef USE_MSHIELD
	HASH::CHash g_hashObj_1;
	g_hashObj_1.SetHashAlgorithm( HASH::MD5 );
	g_hashObj_1.SetHashOperation( HASH::FILE_HASH );
	g_hashObj_1.SetHashFormat( HASH::UPPERCASE_NOSPACES );

	std::string mshield_file_1 = app_path; 
	mshield_file_1 += "\\";
	mshield_file_1 += RPARAM::strMShieldDLL1;

	g_hashObj_1.SetHashFile( mshield_file_1.c_str() );

	StringCchCopy( m_szMShieldDLLHash1, CLIENT_HASH_SIZE, g_hashObj_1.DoHash().GetString() );

	CConsoleMessage::GetInstance()->WriteDatabase( _T("MShield1: %s Hash: %s"), mshield_file_1.c_str(), m_szMShieldDLLHash1 );
	//CDebugSet::ToFileWithTime( "_mshield_hash_check.txt", "field read MShield1 file: %s hash: %s", mshield_file_1.c_str(), m_szMShieldDLLHash1 );

	HASH::CHash g_hashObj_2;
	g_hashObj_2.SetHashAlgorithm( HASH::MD5 );
	g_hashObj_2.SetHashOperation( HASH::FILE_HASH );
	g_hashObj_2.SetHashFormat( HASH::UPPERCASE_NOSPACES );

	std::string mshield_file_2 = app_path; 
	mshield_file_2 += "\\";
	mshield_file_2 += RPARAM::strMShieldDLL2;

	g_hashObj_2.SetHashFile( mshield_file_2.c_str() );

	StringCchCopy( m_szMShieldDLLHash2, CLIENT_HASH_SIZE, g_hashObj_2.DoHash().GetString() );

	CConsoleMessage::GetInstance()->WriteDatabase( _T("MShield2: %s Hash: %s"), mshield_file_2.c_str(), m_szMShieldDLLHash2 );
	//CDebugSet::ToFileWithTime( "_mshield_hash_check.txt", "field read MShield2 file: %s hash: %s", mshield_file_2.c_str(), m_szMShieldDLLHash2 );
#endif
}

void CFieldServer::client_check_rehash()
{
	SERVER_UTIL::CSystemInfo SysInfo;
	std::string strAppPath = SysInfo.GetAppPath().GetString();

	//reset
	memset(m_client_hash1, 0, sizeof(char) * CLIENT_HASH_SIZE);
	memset(m_client_hash2, 0, sizeof(char) * CLIENT_HASH_SIZE);

#ifdef USE_MSHIELD
	memset(m_szMShieldDLLHash1, 0, sizeof(char) * CLIENT_HASH_SIZE);
	memset(m_szMShieldDLLHash2, 0, sizeof(char) * CLIENT_HASH_SIZE);
#endif

	client_check_get_client_hash( strAppPath );

#ifdef USE_MSHIELD
	client_check_get_mshield_hash( strAppPath );
#endif
}

//! -----------------------------------------------------------------------
//! I/O operation memory
//int CFieldServer::StopIOCPList()
//{
//	/*
//	DWORD dwCount = 0;
//	
//	dwCount = m_pClientManager->getSendUseCount();
//
//	for (DWORD i=0; i<dwCount; ++i)
//	{
//		::PostQueuedCompletionStatus(
//			m_hIOServer,
//			0,
//			m_dwCompKey,
//			NULL );
//	}
//	Sleep( 1000 );
//
//	nCount = m_pSendIOCP->GetUseCount();
//	for (int i=0; i<nCount; i++) 
//	{
//		::PostQueuedCompletionStatus(m_hIOServer,
//							0,
//							m_dwCompKey, 
//							NULL);
//	}
//	Sleep( 1000 );
//	*/
//
//	return NET_OK;
//}

/*
PER_IO_OPERATION_DATA* CFieldServer::getSendIO(
	DWORD dwClient )
{
	return m_pClientManager->getSendIO( dwClient );
}

PER_IO_OPERATION_DATA* CFieldServer::getRecvIO(
	DWORD dwClient )
{
	return m_pClientManager->getRecvIO( dwClient );
}

void CFieldServer::releaseSendIO(
	DWORD dwClient,
	PER_IO_OPERATION_DATA* pData )
{
	m_pClientManager->releaseSendIO( dwClient, pData );
}

void CFieldServer::releaseRecvIO(
	DWORD dwClient,
	PER_IO_OPERATION_DATA* pData )
{
	m_pClientManager->releaseRecvIO( dwClient, pData );
}

void CFieldServer::releaseIO(
	DWORD dwClient,
	PER_IO_OPERATION_DATA* pData )
{
	if (NULL != pData)
	{
		if (NET_SEND_POSTED == pData->OperationType)
		{
			releaseSendIO( dwClient, pData );
		}
		else
		{
			releaseRecvIO( dwClient, pData );
		}
	}
}
*/