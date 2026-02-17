///////////////////////////////////////////////////////////////////////////////
// s_CCfg.h
//
// class CCfg
//
// * History
// 2002.05.30 jgkim Create
//
// Copyright 2002-2003 (c) Mincoms. All rights reserved.                 
// 
// * Note
// Config file load class head file
//
///////////////////////////////////////////////////////////////////////////////

#ifndef S_CCFG_H_
#define S_CCFG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./s_NetGlobal.h"
#include "../Lib_Engine/Common/SerialFile.h"

class CCfg
{
private:
	CCfg();
	CCfg(const TCHAR* filename);
	virtual ~CCfg();
	static CCfg* SelfInstance;

public:
	static CCfg* GetInstance();
	static void	ReleaseInstance();

protected:
	///////////////////////////////////////////////////////////////////////////
	// Server Setting Values
	int		m_nServerVersion;						// 서버 버전, 클라이언트 버전과 동일
	int		m_nPatchVersion;						// 패치 프로그램 버전
	
	TCHAR	m_szServerName[DB_SVR_NAME_LENGTH+1];		// 서버명	
	TCHAR	m_szServerIP[MAX_IP_LENGTH+1];				// 서버 IP, 프로그램에서 자동으로 결정될수 있다
	TCHAR	m_szAgentIP [MAX_IP_LENGTH+1];				// Agent 서버 IP
	/*dmk14 whitelist*/
	TCHAR	m_szAllowPortList[MAX_PORT_LENGTH+1];

	int		m_nPortService;							// 서비스용 포트
	int		m_nPortControl;							// 컨트롤용 포트
	
	int		m_nServerType;							// 서버 유형
	int		m_nServerGroup;							// 서버 그룹
	int		m_nServerNumber;						// 서버 번호
	int		m_nServerField;							// 서버 필드 번호
	int     m_nServerChannel;                       // 서버 채널 번호
	int     m_nServerChannelNumber;                 // Agent 서버가 가진 채널 갯수

	int     m_nMaxFieldNumber; // 한 채널이 가지는 필드서버 갯수.

	int		m_nServiceProvider;						// 서비스 제공 회사유형
	
	int		m_nServerMaxClient;						// 최대 접속 가능 클라이언트 수
	int     m_nMaxChannelUser;                      // 채널당 최대 접속 가능 클라이언트 수
	bool	m_bUseEventThread;						// 이벤트 쓰레드 사용 여부
	bool    m_bHeartBeat;							// HeartBeat 를 사용해서 클라이언트 접속끊김을 조사
	int		m_nMaxThread;							// 최대 Work Thread 갯수.
	bool    m_bTestServer;							// 테스트 서버 유/무
	bool    m_bAdultCheck;							// 성인체크
	bool	m_bPK;									// true:PK 서버, false:Non PK 서버
	bool    m_bPkLess;                              /// 선도클럽, PK, 학원간 PK 등 모든 적대행위 Disable
	bool    m_bGameGuardAuth;                       // nProtect GameGuard 인증 사용여부
	int		m_nLangSet;								// Language Set

	TCHAR	m_szWhiteRockServerName[DB_SVR_NAME_LENGTH+1];		// 일본 WhiteRock 서버명
	int		m_nWhiteRockServerPort;								// 일본 WhiteRock 서버사용 Port

	//iplimit
	WORD	m_wIPClientMax;

	std::vector<CString> m_vecAllowIPType1;
	std::vector<CString> m_vecAllowIPType2;
	std::vector<CString> m_vecAllowIPType3;
	bool m_bAllowFileLoad;

	/*ccfg security, Juver, 2018/07/08 */
	TCHAR	m_szHashCPU[CCFG_HASH_LENGTH];
	TCHAR	m_szHashLAN[CCFG_HASH_LENGTH];
	TCHAR	m_szHashDISK[CCFG_HASH_LENGTH];
	TCHAR	m_szHashPCNAME[CCFG_HASH_LENGTH];
	TCHAR	m_szHashCOMBINED[CCFG_HASH_LENGTH];
	TCHAR	m_szHashInformation[CCFG_NOTE_LENGTH];

	///////////////////////////////////////////////////////////////////////////
	// database setting value 	

	///////////////////////////////////////////////////////////////////////////
	// ODBC
	// User database setting value
	TCHAR	m_szUserOdbcName    [DB_SVR_NAME_LENGTH+1]; // Database server name
	TCHAR	m_szUserOdbcUser    [USR_ID_LENGTH+1];	    // User account
	TCHAR	m_szUserOdbcPass    [USR_PASS_LENGTH+1];    // Password
	TCHAR	m_szUserOdbcDatabase[DB_NAME_LENGTH+1];	    // Database name
	int		m_nUserOdbcPoolSize;					    // Database pool size
	int		m_nUserOdbcResponseTime;				    // Query response time (sec)

	// Game database setting value 
	TCHAR	m_szGameOdbcName    [DB_SVR_NAME_LENGTH+1]; // Database server name
	TCHAR	m_szGameOdbcUser    [USR_ID_LENGTH+1];	    // User account
	TCHAR	m_szGameOdbcPass    [USR_PASS_LENGTH+1];	// Password
	TCHAR	m_szGameOdbcDatabase[DB_NAME_LENGTH+1];	    // Database name
	int		m_nGameOdbcPoolSize;					    // Database pool size
	int		m_nGameOdbcResponseTime;				    // Query response time (sec)

	// Log database setting value 
	TCHAR	m_szLogOdbcName    [DB_SVR_NAME_LENGTH+1]; // Database server name
	TCHAR	m_szLogOdbcUser    [USR_ID_LENGTH+1];	   // User account
	TCHAR	m_szLogOdbcPass    [USR_PASS_LENGTH+1];	   // Password
	TCHAR	m_szLogOdbcDatabase[DB_NAME_LENGTH+1];	   // Database name
	int		m_nLogOdbcPoolSize;					       // Database pool size
	int		m_nLogOdbcResponseTime;				       // Query response time (sec)

	// Shop database setting value 
	TCHAR	m_szShopOdbcName    [DB_SVR_NAME_LENGTH+1]; // Database server name
	TCHAR	m_szShopOdbcUser    [USR_ID_LENGTH+1];	    // User account
	TCHAR	m_szShopOdbcPass    [USR_PASS_LENGTH+1];	// Password
	TCHAR	m_szShopOdbcDatabase[DB_NAME_LENGTH+1];	    // Database name
	int		m_nShopOdbcPoolSize;					    // Database pool size
	int		m_nShopOdbcResponseTime;				    // Query response time (sec)

	// Terra database setting value 
	TCHAR	m_szTerraOdbcName    [DB_SVR_NAME_LENGTH+1]; // Database server name
	TCHAR	m_szTerraOdbcUser    [USR_ID_LENGTH+1];	     // User account
	TCHAR	m_szTerraOdbcPass    [USR_PASS_LENGTH+1];	 // Password
	TCHAR	m_szTerraOdbcDatabase[DB_NAME_LENGTH+1];	 // Database name
	int		m_nTerraOdbcPoolSize;					     // Database pool size
	int		m_nTerraOdbcResponseTime;				     // Query response time (sec)

	// GSP database setting value 
	TCHAR m_szGspOdbcName    [DB_SVR_NAME_LENGTH+1]; // Database server name
	TCHAR m_szGspOdbcUser    [USR_ID_LENGTH+1];	     // User account
	TCHAR m_szGspOdbcPass    [USR_PASS_LENGTH+1];	 // Password
	TCHAR m_szGspOdbcDatabase[DB_NAME_LENGTH+1];	 // Database name
	int	  m_nGspOdbcPoolSize;					     // Database pool size
	int	  m_nGspOdbcResponseTime;				     // Query response time (sec)

	// Korea database setting value 
	TCHAR m_szKorOdbcName    [DB_SVR_NAME_LENGTH+1]; // Database server name
	TCHAR m_szKorOdbcUser    [USR_ID_LENGTH+1];	     // User account
	TCHAR m_szKorOdbcPass    [USR_PASS_LENGTH+1];	 // Password
	TCHAR m_szKorOdbcDatabase[DB_NAME_LENGTH+1];	 // Database name
	int	  m_nKorOdbcPoolSize;					     // Database pool size
	int	  m_nKorOdbcResponseTime;				     // Query response time (sec)

	// Malaysia database setting value
	TCHAR m_szMyOdbcName    [DB_SVR_NAME_LENGTH+1];	 // Database server name
	TCHAR m_szMyOdbcUser    [USR_ID_LENGTH+1];	     // User account
	TCHAR m_szMyOdbcPass    [USR_PASS_LENGTH+1];	 // Password
	TCHAR m_szMyOdbcDatabase[DB_NAME_LENGTH+1];		 // Database name
	int	  m_nMyOdbcPoolSize;					     // Database pool size
	int	  m_nMyOdbcResponseTime;				     // Query response time (sec)

	///////////////////////////////////////////////////////////////////////////

	G_SERVER_INFO m_sLOGINServer;
	G_SERVER_INFO m_sSESSIONServer;
	F_SERVER_INFO m_sFIELDServer[MAX_CHANNEL_NUMBER][FIELDSERVER_MAX];	// Field Server Information
	CHANNEL_INFO  m_sChannelInfo[MAX_CHANNEL_NUMBER];

	/* proxy ip, Juver, 2021/07/04 */
	bool		m_bUseProxyIp;
	TCHAR		m_szProxyIp[MAX_IP_LENGTH+1];

public:
	// long				ConvertStrToAddr(const TCHAR* szAddr);
	void				SetDefault(void);
	int					Process(TCHAR* szLine);
	int					Load(const TCHAR* filename);

	F_SERVER_INFO*		GetFieldServer(int nServerID, int nServerChannel=0);
	F_SERVER_INFO*		GetFieldServerArray()		{	return &m_sFIELDServer[0][0]; }

	TCHAR*				GetSessionServerIP(void);
	int					GetSessionServerPort(void);
	
	const TCHAR*        GetProxyIp(const TCHAR* szDefaultVal);
	const TCHAR*        GetProxyIpField(const TCHAR* szDefaultVal, const TCHAR* szFieldProxy);

	int					GetServerGroup(void);	
	void				SetServerGroup(int n);

	int					GetServerNumber(void);
	void				SetServerNumber(int n);

	int					GetServerField(void);
	void				SetServerField(int n);

	int					GetServerType(void);
	void				SetServerType(int n);
    
	int                 GetServerChannel(void);
	void                SetServerChannel(int n);

	int                 GetServerChannelNumber(void);
	void                SetServerChannelNumber(int n);

	int                 GetServerChannelMaxClient(void);
	void                SetServerChannelMaxClient(int n);

	int					GetServerMaxClient(void);
	void				SetServerMaxClient(int n);	

	int					GetServiceProvider(void);
	void				SetServiceProvider(int n);
	
	int					GetServerVersion(void);
	void				SetServerVersion(int nVer);

	int					GetPatchVersion(void);
	void				SetPatchVersion(int nVer);

	int					GetMaxFieldNumber(void);
	void				SetMaxFieldNumber(int n);
	
	const TCHAR*		GetServerIP(void);
	void				SetServerIP(const TCHAR* s);

	int					GetServicePort();
	void				SetServicePort(int n);

	TCHAR*				GetAgentIP(void);
	void				SetAgentIP(const TCHAR* s);

	int					GetControlPort(void);
	void				SetControlPort(int n);

	int					GetMaxClient(void)	{ return GetServerMaxClient(); }

	int					GetMaxThread(void);
	void				SetMaxThread(int n);

	bool				HeartBeatCheck(void);
	void				SetHeartBeatCheck(bool b);

	bool                GemeGuardAuth(void);
	void				SetGameGuardAuth(bool b);

	bool				AdultCheck(void);
	void				SetAdultCheck(bool b);

	bool				IsUserEventThread(void);
	void				SetUserEventThread(bool b);

	bool				IsTestServer(void);
	void				SetTestServer(bool b);

	bool				IsPKServer(void);          // 해당 서버가 PK 서버인지 조사한다. Field
	void				SetPKServer(bool b); 

	bool                IsPkChannel(int nChannel); // 해당채널이 PK 채널인지 조사한다. Agent
	void				SetPkChannel(int nChannel, bool bpk );

	//! 선도클럽, PK, 학원간 PK 등 모든 적대행위 Disable
	bool				IsPKLessServer(void);
	void				SetPKLessServer(bool b);
	/*dmk14 whitelist*/
	const TCHAR*		GetAllowPortList(void);
	void				SetAllowPortList(const TCHAR* s);
	
	//-- 일본 WhiteRock System 설정 변수-------------------------------------//
	TCHAR*				GetWhiteRockName(void);
	void				SetWhiteRockName(const TCHAR* s);

	int					GetWhiteRockPort(void);
	void				SetWhiteRockPort(int n);

	int					GetLangSet(void);
	void				SetLangSet(int n);

	int					GetAllowIPType1Size( void );
	int					GetAllowIPType2Size( void );
	int					GetAllowIPType3Size( void );
	void				GetAllowIPType1( int nCount, CString& strAllowIP );
	void				GetAllowIPType2( int nCount, CString& strAllowIP );
	void				GetAllowIPType3( int nCount, CString& strAllowIP );
	bool				GetAllowFileLoad( void );
	void				SetAllowFileLoad( bool bAllowFileLoad );

	///////////////////////////////////////////////////////////////////////////
	// ODBC	
	TCHAR*				GetUserOdbcName(void);
	TCHAR*				GetUserOdbcUser(void);
	TCHAR*				GetUserOdbcPass(void);
	TCHAR*				GetUserOdbcDatabase(void);
	int					GetUserOdbcResponseTime(void);
	int					GetUserOdbcPoolSize(void);	

	void				SetUserOdbcName(const TCHAR* s);
	void				SetUserOdbcUser(const TCHAR* s);
	void				SetUserOdbcPass(const TCHAR* s);
	void				SetUserOdbcDatabase(const TCHAR* s);
	void				SetUserOdbcResponseTime(int n);
	void				SetUserOdbcPoolSize(int n);	
	
	TCHAR*				GetGameOdbcName(void);
	TCHAR*				GetGameOdbcUser(void);
	TCHAR*				GetGameOdbcPass(void);
	TCHAR*				GetGameOdbcDatabase(void);
	int					GetGameOdbcResponseTime(void);
	int					GetGameOdbcPoolSize(void);

	void				SetGameOdbcName(const TCHAR* s);
	void				SetGameOdbcUser(const TCHAR* s);
	void				SetGameOdbcPass(const TCHAR* s);
	void				SetGameOdbcDatabase(const TCHAR* s);
	void				SetGameOdbcResponseTime(int n);
	void				SetGameOdbcPoolSize(int n);

	TCHAR*				GetLogOdbcName(void);
	TCHAR*				GetLogOdbcUser(void);
	TCHAR*				GetLogOdbcPass(void);
	TCHAR*				GetLogOdbcDatabase(void);
	int					GetLogOdbcResponseTime(void);
	int					GetLogOdbcPoolSize(void);

	void				SetLogOdbcName(const TCHAR* s);
	void				SetLogOdbcUser(const TCHAR* s);
	void				SetLogOdbcPass(const TCHAR* s);
	void				SetLogOdbcDatabase(const TCHAR* s);
	void				SetLogOdbcResponseTime(int n);
	void				SetLogOdbcPoolSize(int n);

	TCHAR*				GetShopOdbcName(void);
	TCHAR*				GetShopOdbcUser(void);
	TCHAR*				GetShopOdbcPass(void);
	TCHAR*				GetShopOdbcDatabase(void);
	int					GetShopOdbcResponseTime(void);
	int					GetShopOdbcPoolSize(void);

	void				SetShopOdbcName(const TCHAR* s);
	void				SetShopOdbcUser(const TCHAR* s);
	void				SetShopOdbcPass(const TCHAR* s);
	void				SetShopOdbcDatabase(const TCHAR* s);
	void				SetShopOdbcResponseTime(int n);
	void				SetShopOdbcPoolSize(int n);

	TCHAR*				GetTerraOdbcName(void);
	TCHAR*				GetTerraOdbcUser(void);
	TCHAR*				GetTerraOdbcPass(void);
	TCHAR*				GetTerraOdbcDatabase(void);
	int					GetTerraOdbcResponseTime(void);
	int					GetTerraOdbcPoolSize(void);

	void				SetTerraOdbcName(const TCHAR* s);
	void				SetTerraOdbcUser(const TCHAR* s);
	void				SetTerraOdbcPass(const TCHAR* s);
	void				SetTerraOdbcDatabase(const TCHAR* s);
	void				SetTerraOdbcResponseTime(int n);
	void				SetTerraOdbcPoolSize(int n);

	TCHAR*				GetGspOdbcName(void);
	TCHAR*				GetGspOdbcUser(void);
	TCHAR*				GetGspOdbcPass(void);
	TCHAR*				GetGspOdbcDatabase(void);
	int					GetGspOdbcResponseTime(void);
	int					GetGspOdbcPoolSize(void);

	void				SetGspOdbcName(const TCHAR* s);
	void				SetGspOdbcUser(const TCHAR* s);
	void				SetGspOdbcPass(const TCHAR* s);
	void				SetGspOdbcDatabase(const TCHAR* s);
	void				SetGspOdbcResponseTime(int n);
	void				SetGspOdbcPoolSize(int n);

	TCHAR*				GetKorOdbcName(void);
	TCHAR*				GetKorOdbcUser(void);
	TCHAR*				GetKorOdbcPass(void);
	TCHAR*				GetKorOdbcDatabase(void);
	int					GetKorOdbcResponseTime(void);
	int					GetKorOdbcPoolSize(void);

	void				SetKorOdbcName(const TCHAR* s);
	void				SetKorOdbcUser(const TCHAR* s);
	void				SetKorOdbcPass(const TCHAR* s);
	void				SetKorOdbcDatabase(const TCHAR* s);
	void				SetKorOdbcResponseTime(int n);
	void				SetKorOdbcPoolSize(int n);

	// 말레이시아 PC방 이벤트
	TCHAR*				GetMyOdbcName(void);
	TCHAR*				GetMyOdbcUser(void);
	TCHAR*				GetMyOdbcPass(void);
	TCHAR*				GetMyOdbcDatabase(void);
	int					GetMyOdbcResponseTime(void);
	int					GetMyOdbcPoolSize(void);

	void				SetMyOdbcName(const TCHAR* s);
	void				SetMyOdbcUser(const TCHAR* s);
	void				SetMyOdbcPass(const TCHAR* s);
	void				SetMyOdbcDatabase(const TCHAR* s);
	void				SetMyOdbcResponseTime(int n);
	void				SetMyOdbcPoolSize(int n);

	//iplimit
	WORD				GetIPClientMax();
	void				SetIPClientMax(WORD w);

	const TCHAR*		GetServerName(void);
	void				SetServerName(const TCHAR* s);


	G_SERVER_INFO*		GetSessionServer()	{ return &m_sSESSIONServer; }
	G_SERVER_INFO*		GetLoginServer()	{ return &m_sLOGINServer; }

	/*ccfg security, Juver, 2018/07/08 */
	TCHAR*				getHashCPU(void);
	TCHAR*				getHashLAN(void);
	TCHAR*				getHashDISK(void);
	TCHAR*				getHashPCNAME(void);
	TCHAR*				getHashCOMBINED(void);
	void				setHashCPU(const TCHAR* s);
	void				setHashLAN(const TCHAR* s);
	void				setHashDISK(const TCHAR* s);
	void				setHashPCNAME(const TCHAR* s);
	void				setHashCOMBINED(const TCHAR* s);
	TCHAR*				getHashINFORMATION(void);
	void				setHashINFORMATION(const TCHAR* s);

	/* proxy ip, Juver, 2021/07/04 */
	bool				getUseProxy(void);
	void				setUseProxy( bool bUse );

	const TCHAR*		getProxyIP(void);
	void				setProxyIP( const TCHAR* szIP );

public:
	enum
	{
		VERSION				= 0x0007,	

		VERSION_ENCODE_2021_SERVER_CFG_V1	= 0x0004,

		VERSION_ENCODE2		= 0x0003,
		VERSION_ENCODE		= 0x0001,
	};

	/*server setting, Juver, 2018/06/17 */
	static const char*	SERVER_CFG_FILEHEAD;

	char				m_szFileName[MAX_PATH];

	const char* GetFileName()			{ return m_szFileName; }

	int Import( const TCHAR* filepath );
	BOOL Save_File( const char* filename );
	BOOL Load_File( const char* filename );
	BOOL Load_File_Tool( const char* filename );

	void Save( CSerialFile& SFile );
	void Load_0001( CSerialFile& SFile );
	void Load_0002( CSerialFile& SFile );
	void Load_0003( CSerialFile& SFile );
	void Load_0004( CSerialFile& SFile );
	void Load_0005( CSerialFile& SFile );
	void Load_0006( CSerialFile& SFile );
	/*dmk14 whitelist*/
	void Load_0007( CSerialFile& SFile );
};

#endif // S_CCFG_H_
