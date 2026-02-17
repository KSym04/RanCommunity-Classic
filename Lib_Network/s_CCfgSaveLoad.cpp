#include "stdafx.h"
#include "./s_CCfg.h"
#include "./s_CSystemInfo.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Common/SUBPATH.h"
#include "../Lib_Engine/Hash/MD52.h"

#include "VirtualizerSDK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char* CCfg::SERVER_CFG_FILEHEAD = "SERVER_CFG";


int CCfg::Import( const TCHAR* filepath )
{
	CString strAppPath = filepath;

	FILE *oFile = NULL;
	TCHAR line[300];

	// Open for read 
	if ((oFile = ::_tfopen(strAppPath.GetString(), _T("r"))) == NULL)
	{
		return -1;
	}	

	// Read a line and process
	while (::_fgetts(line, 200, oFile))
	{
		Process(line);
	}
	
	// Close file
	if (::fclose( oFile ))
	{
		return -1;
	}

	return 0;
}

BOOL CCfg::Save_File ( const char* filename )
{
	StringCchCopy( m_szFileName, MAX_PATH, filename );

	char path[MAX_PATH] = {0};
	StringCchCopy ( path, MAX_PATH, RPATH::getServerConfigPath() );
	StringCchCat ( path, MAX_PATH, m_szFileName );

	CSerialFile serial_file;
	if ( serial_file.OpenFile ( FOT_WRITE, path ) )	
	{
		serial_file << (DWORD)VERSION;

		serial_file.BeginBlock();
		{
			serial_file.SetEncodeType( EMRCRYPT_BYTE_TYPE_2021_SERVER_CFG_V1 );

			Save( serial_file );
		}
		serial_file.EndBlock();
		serial_file.CloseFile();

		return TRUE;
	}

	return FALSE;
}

BOOL CCfg::Load_File( const char* filename )
{
	StringCchCopy( m_szFileName, MAX_PATH, filename );

	char path[MAX_PATH] = {0};
	StringCchCopy ( path, MAX_PATH, RPATH::getServerConfigPath() );
	StringCchCat ( path, MAX_PATH, m_szFileName );

	CSerialFile serial_file;
	if ( serial_file.OpenFile ( FOT_READ, path ) )	
	{
		DWORD ver, size;
		serial_file >> ver;
		serial_file >> size;

		serial_file.SetEncodeType( EMRCRYPT_BYTE_TYPE_2021_SERVER_CFG_V1 );

		if ( ver == VERSION )
		{
			Load_0007( serial_file );
		}
		else
		{
			CDebugSet::ErrorVersion( "CCfg::LoadFile", serial_file.GetFileName(), ver );
			DWORD offset = serial_file.GetfTell();
			serial_file.SetOffSet( offset+size );

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}


BOOL CCfg::Load_File_Tool( const char* filename )
{
	StringCchCopy( m_szFileName, MAX_PATH, filename );

	char path[MAX_PATH] = {0};
	StringCchCopy ( path, MAX_PATH, RPATH::getServerConfigPath() );
	StringCchCat ( path, MAX_PATH, m_szFileName );

	CSerialFile serial_file;
	if ( serial_file.OpenFile ( FOT_READ, path ) )	
	{
		DWORD ver, size;
		serial_file >> ver;
		serial_file >> size;

		serial_file.SetEncodeType( EMRCRYPT_BYTE_TYPE_SERVER_CFG_VER_1 );

		if ( ver >= VERSION_ENCODE2 )
			serial_file.SetEncodeType( EMRCRYPT_BYTE_TYPE_SERVER_CFG_VER_2 );

		if ( ver >= VERSION_ENCODE_2021_SERVER_CFG_V1 )
			serial_file.SetEncodeType( EMRCRYPT_BYTE_TYPE_2021_SERVER_CFG_V1 );

		if ( ver == VERSION )
		{
			Load_0007( serial_file );
		}
		else if ( ver == 0x0006 )
		{
			Load_0006( serial_file );
		}
		else if ( ver == 0x0005 )
		{
			Load_0005( serial_file );
		}
		else if ( ver == 0x0004 )
		{
			Load_0004( serial_file );
		}
		else if ( ver == 0x0003 )
		{
			Load_0003( serial_file );
		}
		else if ( ver == 0x0002 )
		{
			Load_0002( serial_file );
		}
		else if ( ver == 0x0001 )
		{
			Load_0001( serial_file );
		}
		else
		{
			CDebugSet::ErrorVersion( "CCfg::Load_File_Tool", serial_file.GetFileName(), ver );
			DWORD offset = serial_file.GetfTell();
			serial_file.SetOffSet( offset+size );

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

void CCfg::Save( CSerialFile& SFile )
{
	SFile << m_nServerVersion;						// 서버 버전, 클라이언트 버전과 동일
	SFile << m_nPatchVersion;						// 패치 프로그램 버전

	SFile.WriteBuffer( m_szServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 서버명	
	SFile.WriteBuffer( m_szServerIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// 서버 IP, 프로그램에서 자동으로 결정될수 있다
	SFile.WriteBuffer( m_szAgentIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// Agent 서버 IP

	/*dmk14 whitelist*/
	SFile.WriteBuffer( m_szAllowPortList, sizeof( TCHAR ) * ( MAX_PORT_LENGTH+1 ) );
	
	/* proxy ip, Juver, 2021/07/04 */
	SFile << m_bUseProxyIp;	
	SFile.WriteBuffer( m_szProxyIp, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );


	SFile << m_nPortService;							// 서비스용 포트
	SFile << m_nPortControl;							// 컨트롤용 포트

	SFile << m_nServerType;							// 서버 유형
	SFile << m_nServerGroup;							// 서버 그룹
	SFile << m_nServerNumber;						// 서버 번호
	SFile << m_nServerField;							// 서버 필드 번호
	SFile << m_nServerChannel;                       // 서버 채널 번호
	SFile << m_nServerChannelNumber;                 // Agent 서버가 가진 채널 갯수

	SFile << m_nMaxFieldNumber; // 한 채널이 가지는 필드서버 갯수.

	SFile << m_nServiceProvider;						// 서비스 제공 회사유형

	SFile << m_nServerMaxClient;						// 최대 접속 가능 클라이언트 수
	SFile << m_nMaxChannelUser;                      // 채널당 최대 접속 가능 클라이언트 수
	SFile << m_bUseEventThread;						// 이벤트 쓰레드 사용 여부
	SFile << m_bHeartBeat;							// HeartBeat 를 사용해서 클라이언트 접속끊김을 조사
	SFile << m_nMaxThread;							// 최대 Work Thread 갯수.
	SFile << m_bTestServer;							// 테스트 서버 유/무
	SFile << m_bAdultCheck;							// 성인체크
	SFile << m_bPK;									// true:PK 서버, false:Non PK 서버
	SFile << m_bPkLess;                              /// 선도클럽, PK, 학원간 PK 등 모든 적대행위 Disable
	SFile << m_bGameGuardAuth;                       // nProtect GameGuard 인증 사용여부
	SFile << m_nLangSet;								// Language Set

	SFile.WriteBuffer( m_szWhiteRockServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 일본 WhiteRock 서버명
	SFile << m_nWhiteRockServerPort;								// 일본 WhiteRock 서버사용 Port

	//iplimit
	SFile << m_wIPClientMax;

	/*ccfg security, Juver, 2018/07/08 */
	SFile.WriteBuffer( m_szHashCPU, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.WriteBuffer( m_szHashLAN, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.WriteBuffer( m_szHashDISK, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.WriteBuffer( m_szHashPCNAME, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );

	///////////////////////////////////////////////////////////////////////////
	// database setting value 	

	///////////////////////////////////////////////////////////////////////////
	// ODBC
	// User database setting value
	SFile.WriteBuffer( m_szUserOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.WriteBuffer( m_szUserOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.WriteBuffer( m_szUserOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );    // Password
	SFile.WriteBuffer( m_szUserOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile << m_nUserOdbcPoolSize;					    // Database pool size
	SFile << m_nUserOdbcResponseTime;				    // Query response time (sec)

	// Game database setting value 
	SFile.WriteBuffer( m_szGameOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.WriteBuffer( m_szGameOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.WriteBuffer( m_szGameOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.WriteBuffer( m_szGameOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile << m_nGameOdbcPoolSize;					    // Database pool size
	SFile << m_nGameOdbcResponseTime;				    // Query response time (sec)

	// Log database setting value 
	SFile.WriteBuffer( m_szLogOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.WriteBuffer( m_szLogOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	   // User account
	SFile.WriteBuffer( m_szLogOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	   // Password
	SFile.WriteBuffer( m_szLogOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	   // Database name
	SFile << m_nLogOdbcPoolSize;					       // Database pool size
	SFile << m_nLogOdbcResponseTime;				       // Query response time (sec)

	// Shop database setting value 
	SFile.WriteBuffer( m_szShopOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.WriteBuffer( m_szShopOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.WriteBuffer( m_szShopOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.WriteBuffer( m_szShopOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile << m_nShopOdbcPoolSize;					    // Database pool size
	SFile << m_nShopOdbcResponseTime;				    // Query response time (sec)

	// Terra database setting value 
	SFile.WriteBuffer( m_szTerraOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.WriteBuffer( m_szTerraOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.WriteBuffer( m_szTerraOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.WriteBuffer( m_szTerraOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile << m_nTerraOdbcPoolSize;					     // Database pool size
	SFile << m_nTerraOdbcResponseTime;				     // Query response time (sec)

	// GSP database setting value 
	SFile.WriteBuffer( m_szGspOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.WriteBuffer( m_szGspOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.WriteBuffer( m_szGspOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.WriteBuffer( m_szGspOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile <<  m_nGspOdbcPoolSize;					     // Database pool size
	SFile << m_nGspOdbcResponseTime;				     // Query response time (sec)

	// Korea database setting value 
	SFile.WriteBuffer( m_szKorOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.WriteBuffer( m_szKorOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.WriteBuffer( m_szKorOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.WriteBuffer( m_szKorOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile << m_nKorOdbcPoolSize;					     // Database pool size
	SFile << m_nKorOdbcResponseTime;				     // Query response time (sec)

	// Malaysia database setting value
	SFile.WriteBuffer( m_szMyOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );	 // Database server name
	SFile.WriteBuffer( m_szMyOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.WriteBuffer( m_szMyOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.WriteBuffer( m_szMyOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );		 // Database name
	SFile << m_nMyOdbcPoolSize;					     // Database pool size
	SFile << m_nMyOdbcResponseTime;				     // Query response time (sec)

	///////////////////////////////////////////////////////////////////////////

	/*ccfg security, Juver, 2018/07/08 */
	SFile.WriteBuffer( m_szHashCOMBINED, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.WriteBuffer( m_szHashInformation, sizeof( TCHAR ) * ( CCFG_NOTE_LENGTH ) );

	SFile.WriteBuffer( &m_sLOGINServer, sizeof( G_SERVER_INFO ) );
	SFile.WriteBuffer( &m_sSESSIONServer, sizeof( G_SERVER_INFO ) );
	SFile.WriteBuffer( m_sFIELDServer, sizeof( F_SERVER_INFO ) * ( MAX_CHANNEL_NUMBER * FIELDSERVER_MAX ) );	// Field Server Information
	SFile.WriteBuffer( m_sChannelInfo, sizeof( CHANNEL_INFO ) * ( MAX_CHANNEL_NUMBER ) );
}
/*dmk14 whitelist*/
void CCfg::Load_0007( CSerialFile& SFile )
{
	VIRTUALIZER_FISH_WHITE_START 

	SFile >> m_nServerVersion;						// 서버 버전, 클라이언트 버전과 동일
	SFile >> m_nPatchVersion;						// 패치 프로그램 버전

	SFile.ReadBuffer( m_szServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 서버명	
	SFile.ReadBuffer( m_szServerIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// 서버 IP, 프로그램에서 자동으로 결정될수 있다
	SFile.ReadBuffer( m_szAgentIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// Agent 서버 IP
	SFile.ReadBuffer( m_szAllowPortList, sizeof( TCHAR ) * ( MAX_PORT_LENGTH+1 ) );

	/* proxy ip, Juver, 2021/07/04 */
	SFile >> m_bUseProxyIp;		
	SFile.ReadBuffer( m_szProxyIp, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );

	SFile >> m_nPortService;							// 서비스용 포트
	SFile >> m_nPortControl;							// 컨트롤용 포트

	SFile >> m_nServerType;							// 서버 유형
	SFile >> m_nServerGroup;							// 서버 그룹
	SFile >> m_nServerNumber;						// 서버 번호
	SFile >> m_nServerField;							// 서버 필드 번호
	SFile >> m_nServerChannel;                       // 서버 채널 번호
	SFile >> m_nServerChannelNumber;                 // Agent 서버가 가진 채널 갯수

	SFile >> m_nMaxFieldNumber; // 한 채널이 가지는 필드서버 갯수.

	SFile >> m_nServiceProvider;						// 서비스 제공 회사유형

	SFile >> m_nServerMaxClient;						// 최대 접속 가능 클라이언트 수
	SFile >> m_nMaxChannelUser;                      // 채널당 최대 접속 가능 클라이언트 수
	SFile >> m_bUseEventThread;						// 이벤트 쓰레드 사용 여부
	SFile >> m_bHeartBeat;							// HeartBeat 를 사용해서 클라이언트 접속끊김을 조사
	SFile >> m_nMaxThread;							// 최대 Work Thread 갯수.
	SFile >> m_bTestServer;							// 테스트 서버 유/무
	SFile >> m_bAdultCheck;							// 성인체크
	SFile >> m_bPK;									// true:PK 서버, false:Non PK 서버
	SFile >> m_bPkLess;                              /// 선도클럽, PK, 학원간 PK 등 모든 적대행위 Disable
	SFile >> m_bGameGuardAuth;                       // nProtect GameGuard 인증 사용여부
	SFile >> m_nLangSet;								// Language Set

	SFile.ReadBuffer( m_szWhiteRockServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 일본 WhiteRock 서버명
	SFile >> m_nWhiteRockServerPort;								// 일본 WhiteRock 서버사용 Port

	//iplimit
	SFile >> m_wIPClientMax;

	/*ccfg security, Juver, 2018/07/08 */
	SFile.ReadBuffer( m_szHashCPU, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashLAN, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashDISK, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashPCNAME, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );

	///////////////////////////////////////////////////////////////////////////
	// database setting value 	

	///////////////////////////////////////////////////////////////////////////
	// ODBC
	// User database setting value
	SFile.ReadBuffer( m_szUserOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szUserOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szUserOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );    // Password
	SFile.ReadBuffer( m_szUserOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nUserOdbcPoolSize;					    // Database pool size
	SFile >> m_nUserOdbcResponseTime;				    // Query response time (sec)

	// Game database setting value 
	SFile.ReadBuffer( m_szGameOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGameOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szGameOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szGameOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nGameOdbcPoolSize;					    // Database pool size
	SFile >> m_nGameOdbcResponseTime;				    // Query response time (sec)

	// Log database setting value 
	SFile.ReadBuffer( m_szLogOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szLogOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	   // User account
	SFile.ReadBuffer( m_szLogOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	   // Password
	SFile.ReadBuffer( m_szLogOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	   // Database name
	SFile >> m_nLogOdbcPoolSize;					       // Database pool size
	SFile >> m_nLogOdbcResponseTime;				       // Query response time (sec)

	// Shop database setting value 
	SFile.ReadBuffer( m_szShopOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szShopOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szShopOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szShopOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nShopOdbcPoolSize;					    // Database pool size
	SFile >> m_nShopOdbcResponseTime;				    // Query response time (sec)

	// Terra database setting value 
	SFile.ReadBuffer( m_szTerraOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szTerraOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szTerraOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szTerraOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nTerraOdbcPoolSize;					     // Database pool size
	SFile >> m_nTerraOdbcResponseTime;				     // Query response time (sec)

	// GSP database setting value 
	SFile.ReadBuffer( m_szGspOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGspOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szGspOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szGspOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >>  m_nGspOdbcPoolSize;					     // Database pool size
	SFile >> m_nGspOdbcResponseTime;				     // Query response time (sec)

	// Korea database setting value 
	SFile.ReadBuffer( m_szKorOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szKorOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szKorOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szKorOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nKorOdbcPoolSize;					     // Database pool size
	SFile >> m_nKorOdbcResponseTime;				     // Query response time (sec)

	// Malaysia database setting value
	SFile.ReadBuffer( m_szMyOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );	 // Database server name
	SFile.ReadBuffer( m_szMyOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szMyOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szMyOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );		 // Database name
	SFile >> m_nMyOdbcPoolSize;					     // Database pool size
	SFile >> m_nMyOdbcResponseTime;				     // Query response time (sec)

	/*ccfg security, Juver, 2018/07/08 */
	SFile.ReadBuffer( m_szHashCOMBINED, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashInformation, sizeof( TCHAR ) * ( CCFG_NOTE_LENGTH ) );

	SFile.ReadBuffer( &m_sLOGINServer, sizeof( G_SERVER_INFO ) );
	SFile.ReadBuffer( &m_sSESSIONServer, sizeof( G_SERVER_INFO ) );
	SFile.ReadBuffer( m_sFIELDServer, sizeof( F_SERVER_INFO ) * ( MAX_CHANNEL_NUMBER * FIELDSERVER_MAX ) );	// Field Server Information
	SFile.ReadBuffer( m_sChannelInfo, sizeof( CHANNEL_INFO ) * ( MAX_CHANNEL_NUMBER ) );

	VIRTUALIZER_FISH_WHITE_END

}

void CCfg::Load_0006( CSerialFile& SFile )
{
	VIRTUALIZER_FISH_WHITE_START 

	SFile >> m_nServerVersion;						// 서버 버전, 클라이언트 버전과 동일
	SFile >> m_nPatchVersion;						// 패치 프로그램 버전

	SFile.ReadBuffer( m_szServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 서버명	
	SFile.ReadBuffer( m_szServerIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// 서버 IP, 프로그램에서 자동으로 결정될수 있다
	SFile.ReadBuffer( m_szAgentIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// Agent 서버 IP

	/* proxy ip, Juver, 2021/07/04 */
	SFile >> m_bUseProxyIp;		
	SFile.ReadBuffer( m_szProxyIp, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );

	SFile >> m_nPortService;							// 서비스용 포트
	SFile >> m_nPortControl;							// 컨트롤용 포트

	SFile >> m_nServerType;							// 서버 유형
	SFile >> m_nServerGroup;							// 서버 그룹
	SFile >> m_nServerNumber;						// 서버 번호
	SFile >> m_nServerField;							// 서버 필드 번호
	SFile >> m_nServerChannel;                       // 서버 채널 번호
	SFile >> m_nServerChannelNumber;                 // Agent 서버가 가진 채널 갯수

	SFile >> m_nMaxFieldNumber; // 한 채널이 가지는 필드서버 갯수.

	SFile >> m_nServiceProvider;						// 서비스 제공 회사유형

	SFile >> m_nServerMaxClient;						// 최대 접속 가능 클라이언트 수
	SFile >> m_nMaxChannelUser;                      // 채널당 최대 접속 가능 클라이언트 수
	SFile >> m_bUseEventThread;						// 이벤트 쓰레드 사용 여부
	SFile >> m_bHeartBeat;							// HeartBeat 를 사용해서 클라이언트 접속끊김을 조사
	SFile >> m_nMaxThread;							// 최대 Work Thread 갯수.
	SFile >> m_bTestServer;							// 테스트 서버 유/무
	SFile >> m_bAdultCheck;							// 성인체크
	SFile >> m_bPK;									// true:PK 서버, false:Non PK 서버
	SFile >> m_bPkLess;                              /// 선도클럽, PK, 학원간 PK 등 모든 적대행위 Disable
	SFile >> m_bGameGuardAuth;                       // nProtect GameGuard 인증 사용여부
	SFile >> m_nLangSet;								// Language Set

	SFile.ReadBuffer( m_szWhiteRockServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 일본 WhiteRock 서버명
	SFile >> m_nWhiteRockServerPort;								// 일본 WhiteRock 서버사용 Port

	//iplimit
	SFile >> m_wIPClientMax;

	/*ccfg security, Juver, 2018/07/08 */
	SFile.ReadBuffer( m_szHashCPU, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashLAN, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashDISK, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashPCNAME, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );

	///////////////////////////////////////////////////////////////////////////
	// database setting value 	

	///////////////////////////////////////////////////////////////////////////
	// ODBC
	// User database setting value
	SFile.ReadBuffer( m_szUserOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szUserOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szUserOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );    // Password
	SFile.ReadBuffer( m_szUserOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nUserOdbcPoolSize;					    // Database pool size
	SFile >> m_nUserOdbcResponseTime;				    // Query response time (sec)

	// Game database setting value 
	SFile.ReadBuffer( m_szGameOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGameOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szGameOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szGameOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nGameOdbcPoolSize;					    // Database pool size
	SFile >> m_nGameOdbcResponseTime;				    // Query response time (sec)

	// Log database setting value 
	SFile.ReadBuffer( m_szLogOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szLogOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	   // User account
	SFile.ReadBuffer( m_szLogOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	   // Password
	SFile.ReadBuffer( m_szLogOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	   // Database name
	SFile >> m_nLogOdbcPoolSize;					       // Database pool size
	SFile >> m_nLogOdbcResponseTime;				       // Query response time (sec)

	// Shop database setting value 
	SFile.ReadBuffer( m_szShopOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szShopOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szShopOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szShopOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nShopOdbcPoolSize;					    // Database pool size
	SFile >> m_nShopOdbcResponseTime;				    // Query response time (sec)

	// Terra database setting value 
	SFile.ReadBuffer( m_szTerraOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szTerraOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szTerraOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szTerraOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nTerraOdbcPoolSize;					     // Database pool size
	SFile >> m_nTerraOdbcResponseTime;				     // Query response time (sec)

	// GSP database setting value 
	SFile.ReadBuffer( m_szGspOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGspOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szGspOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szGspOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >>  m_nGspOdbcPoolSize;					     // Database pool size
	SFile >> m_nGspOdbcResponseTime;				     // Query response time (sec)

	// Korea database setting value 
	SFile.ReadBuffer( m_szKorOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szKorOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szKorOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szKorOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nKorOdbcPoolSize;					     // Database pool size
	SFile >> m_nKorOdbcResponseTime;				     // Query response time (sec)

	// Malaysia database setting value
	SFile.ReadBuffer( m_szMyOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );	 // Database server name
	SFile.ReadBuffer( m_szMyOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szMyOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szMyOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );		 // Database name
	SFile >> m_nMyOdbcPoolSize;					     // Database pool size
	SFile >> m_nMyOdbcResponseTime;				     // Query response time (sec)

	/*ccfg security, Juver, 2018/07/08 */
	SFile.ReadBuffer( m_szHashCOMBINED, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashInformation, sizeof( TCHAR ) * ( CCFG_NOTE_LENGTH ) );

	SFile.ReadBuffer( &m_sLOGINServer, sizeof( G_SERVER_INFO ) );
	SFile.ReadBuffer( &m_sSESSIONServer, sizeof( G_SERVER_INFO ) );
	SFile.ReadBuffer( m_sFIELDServer, sizeof( F_SERVER_INFO ) * ( MAX_CHANNEL_NUMBER * FIELDSERVER_MAX ) );	// Field Server Information
	SFile.ReadBuffer( m_sChannelInfo, sizeof( CHANNEL_INFO ) * ( MAX_CHANNEL_NUMBER ) );

	VIRTUALIZER_FISH_WHITE_END

}

void CCfg::Load_0005( CSerialFile& SFile )
{
	VIRTUALIZER_FISH_WHITE_START 

	SFile >> m_nServerVersion;						// 서버 버전, 클라이언트 버전과 동일
	SFile >> m_nPatchVersion;						// 패치 프로그램 버전

	SFile.ReadBuffer( m_szServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 서버명	
	SFile.ReadBuffer( m_szServerIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// 서버 IP, 프로그램에서 자동으로 결정될수 있다
	SFile.ReadBuffer( m_szAgentIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// Agent 서버 IP

	/* proxy ip, Juver, 2021/07/04 */
	SFile >> m_bUseProxyIp;		
	SFile.ReadBuffer( m_szProxyIp, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );

	SFile >> m_nPortService;							// 서비스용 포트
	SFile >> m_nPortControl;							// 컨트롤용 포트

	SFile >> m_nServerType;							// 서버 유형
	SFile >> m_nServerGroup;							// 서버 그룹
	SFile >> m_nServerNumber;						// 서버 번호
	SFile >> m_nServerField;							// 서버 필드 번호
	SFile >> m_nServerChannel;                       // 서버 채널 번호
	SFile >> m_nServerChannelNumber;                 // Agent 서버가 가진 채널 갯수

	SFile >> m_nMaxFieldNumber; // 한 채널이 가지는 필드서버 갯수.

	SFile >> m_nServiceProvider;						// 서비스 제공 회사유형

	SFile >> m_nServerMaxClient;						// 최대 접속 가능 클라이언트 수
	SFile >> m_nMaxChannelUser;                      // 채널당 최대 접속 가능 클라이언트 수
	SFile >> m_bUseEventThread;						// 이벤트 쓰레드 사용 여부
	SFile >> m_bHeartBeat;							// HeartBeat 를 사용해서 클라이언트 접속끊김을 조사
	SFile >> m_nMaxThread;							// 최대 Work Thread 갯수.
	SFile >> m_bTestServer;							// 테스트 서버 유/무
	SFile >> m_bAdultCheck;							// 성인체크
	SFile >> m_bPK;									// true:PK 서버, false:Non PK 서버
	SFile >> m_bPkLess;                              /// 선도클럽, PK, 학원간 PK 등 모든 적대행위 Disable
	SFile >> m_bGameGuardAuth;                       // nProtect GameGuard 인증 사용여부
	SFile >> m_nLangSet;								// Language Set

	SFile.ReadBuffer( m_szWhiteRockServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 일본 WhiteRock 서버명
	SFile >> m_nWhiteRockServerPort;								// 일본 WhiteRock 서버사용 Port

	//iplimit
	SFile >> m_wIPClientMax;

	/*ccfg security, Juver, 2018/07/08 */
	SFile.ReadBuffer( m_szHashCPU, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashLAN, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashDISK, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashPCNAME, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );

	///////////////////////////////////////////////////////////////////////////
	// database setting value 	

	///////////////////////////////////////////////////////////////////////////
	// ODBC
	// User database setting value
	SFile.ReadBuffer( m_szUserOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szUserOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szUserOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );    // Password
	SFile.ReadBuffer( m_szUserOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nUserOdbcPoolSize;					    // Database pool size
	SFile >> m_nUserOdbcResponseTime;				    // Query response time (sec)

	// Game database setting value 
	SFile.ReadBuffer( m_szGameOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGameOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szGameOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szGameOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nGameOdbcPoolSize;					    // Database pool size
	SFile >> m_nGameOdbcResponseTime;				    // Query response time (sec)

	// Log database setting value 
	SFile.ReadBuffer( m_szLogOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szLogOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	   // User account
	SFile.ReadBuffer( m_szLogOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	   // Password
	SFile.ReadBuffer( m_szLogOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	   // Database name
	SFile >> m_nLogOdbcPoolSize;					       // Database pool size
	SFile >> m_nLogOdbcResponseTime;				       // Query response time (sec)

	// Shop database setting value 
	SFile.ReadBuffer( m_szShopOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szShopOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szShopOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szShopOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nShopOdbcPoolSize;					    // Database pool size
	SFile >> m_nShopOdbcResponseTime;				    // Query response time (sec)

	// Terra database setting value 
	SFile.ReadBuffer( m_szTerraOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szTerraOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szTerraOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szTerraOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nTerraOdbcPoolSize;					     // Database pool size
	SFile >> m_nTerraOdbcResponseTime;				     // Query response time (sec)

	// GSP database setting value 
	SFile.ReadBuffer( m_szGspOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGspOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szGspOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szGspOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >>  m_nGspOdbcPoolSize;					     // Database pool size
	SFile >> m_nGspOdbcResponseTime;				     // Query response time (sec)

	// Korea database setting value 
	SFile.ReadBuffer( m_szKorOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szKorOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szKorOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szKorOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nKorOdbcPoolSize;					     // Database pool size
	SFile >> m_nKorOdbcResponseTime;				     // Query response time (sec)

	// Malaysia database setting value
	SFile.ReadBuffer( m_szMyOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );	 // Database server name
	SFile.ReadBuffer( m_szMyOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szMyOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szMyOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );		 // Database name
	SFile >> m_nMyOdbcPoolSize;					     // Database pool size
	SFile >> m_nMyOdbcResponseTime;				     // Query response time (sec)

	/*ccfg security, Juver, 2018/07/08 */
	SFile.ReadBuffer( m_szHashCOMBINED, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashInformation, sizeof( TCHAR ) * ( CCFG_NOTE_LENGTH ) );

	SFile.ReadBuffer( &m_sLOGINServer, sizeof( G_SERVER_INFO ) );
	SFile.ReadBuffer( &m_sSESSIONServer, sizeof( G_SERVER_INFO ) );

	F_SERVER_INFO_100 sFIELDServer[MAX_CHANNEL_NUMBER][FIELDSERVER_MAX];	
	SFile.ReadBuffer( sFIELDServer, sizeof( F_SERVER_INFO_100 ) * ( MAX_CHANNEL_NUMBER * FIELDSERVER_MAX ) );	// Field Server Information

	for ( int i=0; i<MAX_CHANNEL_NUMBER; ++i )
	{
		for( int k=0; k<FIELDSERVER_MAX; ++k )
		{
			m_sFIELDServer[i][k].Assign( sFIELDServer[i][k] );
		}
	}

	SFile.ReadBuffer( m_sChannelInfo, sizeof( CHANNEL_INFO ) * ( MAX_CHANNEL_NUMBER ) );

	VIRTUALIZER_FISH_WHITE_END

}

void CCfg::Load_0004( CSerialFile& SFile )
{
VIRTUALIZER_FISH_WHITE_START 

	SFile >> m_nServerVersion;						// 서버 버전, 클라이언트 버전과 동일
	SFile >> m_nPatchVersion;						// 패치 프로그램 버전

	SFile.ReadBuffer( m_szServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 서버명	
	SFile.ReadBuffer( m_szServerIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// 서버 IP, 프로그램에서 자동으로 결정될수 있다
	SFile.ReadBuffer( m_szAgentIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// Agent 서버 IP

	SFile >> m_nPortService;							// 서비스용 포트
	SFile >> m_nPortControl;							// 컨트롤용 포트

	SFile >> m_nServerType;							// 서버 유형
	SFile >> m_nServerGroup;							// 서버 그룹
	SFile >> m_nServerNumber;						// 서버 번호
	SFile >> m_nServerField;							// 서버 필드 번호
	SFile >> m_nServerChannel;                       // 서버 채널 번호
	SFile >> m_nServerChannelNumber;                 // Agent 서버가 가진 채널 갯수

	SFile >> m_nMaxFieldNumber; // 한 채널이 가지는 필드서버 갯수.

	SFile >> m_nServiceProvider;						// 서비스 제공 회사유형

	SFile >> m_nServerMaxClient;						// 최대 접속 가능 클라이언트 수
	SFile >> m_nMaxChannelUser;                      // 채널당 최대 접속 가능 클라이언트 수
	SFile >> m_bUseEventThread;						// 이벤트 쓰레드 사용 여부
	SFile >> m_bHeartBeat;							// HeartBeat 를 사용해서 클라이언트 접속끊김을 조사
	SFile >> m_nMaxThread;							// 최대 Work Thread 갯수.
	SFile >> m_bTestServer;							// 테스트 서버 유/무
	SFile >> m_bAdultCheck;							// 성인체크
	SFile >> m_bPK;									// true:PK 서버, false:Non PK 서버
	SFile >> m_bPkLess;                              /// 선도클럽, PK, 학원간 PK 등 모든 적대행위 Disable
	SFile >> m_bGameGuardAuth;                       // nProtect GameGuard 인증 사용여부
	SFile >> m_nLangSet;								// Language Set

	SFile.ReadBuffer( m_szWhiteRockServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 일본 WhiteRock 서버명
	SFile >> m_nWhiteRockServerPort;								// 일본 WhiteRock 서버사용 Port

	//iplimit
	SFile >> m_wIPClientMax;

	/*ccfg security, Juver, 2018/07/08 */
	SFile.ReadBuffer( m_szHashCPU, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashLAN, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashDISK, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashPCNAME, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );

	///////////////////////////////////////////////////////////////////////////
	// database setting value 	

	///////////////////////////////////////////////////////////////////////////
	// ODBC
	// User database setting value
	SFile.ReadBuffer( m_szUserOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szUserOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szUserOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );    // Password
	SFile.ReadBuffer( m_szUserOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nUserOdbcPoolSize;					    // Database pool size
	SFile >> m_nUserOdbcResponseTime;				    // Query response time (sec)

	// Game database setting value 
	SFile.ReadBuffer( m_szGameOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGameOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szGameOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szGameOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nGameOdbcPoolSize;					    // Database pool size
	SFile >> m_nGameOdbcResponseTime;				    // Query response time (sec)

	// Log database setting value 
	SFile.ReadBuffer( m_szLogOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szLogOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	   // User account
	SFile.ReadBuffer( m_szLogOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	   // Password
	SFile.ReadBuffer( m_szLogOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	   // Database name
	SFile >> m_nLogOdbcPoolSize;					       // Database pool size
	SFile >> m_nLogOdbcResponseTime;				       // Query response time (sec)

	// Shop database setting value 
	SFile.ReadBuffer( m_szShopOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szShopOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szShopOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szShopOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nShopOdbcPoolSize;					    // Database pool size
	SFile >> m_nShopOdbcResponseTime;				    // Query response time (sec)

	// Terra database setting value 
	SFile.ReadBuffer( m_szTerraOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szTerraOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szTerraOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szTerraOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nTerraOdbcPoolSize;					     // Database pool size
	SFile >> m_nTerraOdbcResponseTime;				     // Query response time (sec)

	// GSP database setting value 
	SFile.ReadBuffer( m_szGspOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGspOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szGspOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szGspOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >>  m_nGspOdbcPoolSize;					     // Database pool size
	SFile >> m_nGspOdbcResponseTime;				     // Query response time (sec)

	// Korea database setting value 
	SFile.ReadBuffer( m_szKorOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szKorOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szKorOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szKorOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nKorOdbcPoolSize;					     // Database pool size
	SFile >> m_nKorOdbcResponseTime;				     // Query response time (sec)

	// Malaysia database setting value
	SFile.ReadBuffer( m_szMyOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );	 // Database server name
	SFile.ReadBuffer( m_szMyOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szMyOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szMyOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );		 // Database name
	SFile >> m_nMyOdbcPoolSize;					     // Database pool size
	SFile >> m_nMyOdbcResponseTime;				     // Query response time (sec)

	/*ccfg security, Juver, 2018/07/08 */
	SFile.ReadBuffer( m_szHashCOMBINED, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashInformation, sizeof( TCHAR ) * ( CCFG_NOTE_LENGTH ) );

	SFile.ReadBuffer( &m_sLOGINServer, sizeof( G_SERVER_INFO ) );
	SFile.ReadBuffer( &m_sSESSIONServer, sizeof( G_SERVER_INFO ) );
	
	F_SERVER_INFO_100 sFIELDServer[MAX_CHANNEL_NUMBER][FIELDSERVER_MAX];	
	SFile.ReadBuffer( sFIELDServer, sizeof( F_SERVER_INFO_100 ) * ( MAX_CHANNEL_NUMBER * FIELDSERVER_MAX ) );	// Field Server Information

	for ( int i=0; i<MAX_CHANNEL_NUMBER; ++i )
	{
		for( int k=0; k<FIELDSERVER_MAX; ++k )
		{
			m_sFIELDServer[i][k].Assign( sFIELDServer[i][k] );
		}
	}

	SFile.ReadBuffer( m_sChannelInfo, sizeof( CHANNEL_INFO ) * ( MAX_CHANNEL_NUMBER ) );

VIRTUALIZER_FISH_WHITE_END

}


void CCfg::Load_0003( CSerialFile& SFile )
{
	/*ccfg security, Juver, 2018/07/08 */
	SFile.ReadBuffer( m_szHashCOMBINED, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashInformation, sizeof( TCHAR ) * ( CCFG_NOTE_LENGTH ) );
	SFile.ReadBuffer( m_szHashCPU, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashLAN, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashDISK, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashPCNAME, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	
	SFile >> m_nServerVersion;						// 서버 버전, 클라이언트 버전과 동일
	SFile >> m_nPatchVersion;						// 패치 프로그램 버전

	SFile.ReadBuffer( m_szServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 서버명	
	SFile.ReadBuffer( m_szServerIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// 서버 IP, 프로그램에서 자동으로 결정될수 있다
	SFile.ReadBuffer( m_szAgentIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// Agent 서버 IP

	SFile >> m_nPortService;							// 서비스용 포트
	SFile >> m_nPortControl;							// 컨트롤용 포트

	SFile >> m_nServerType;							// 서버 유형
	SFile >> m_nServerGroup;							// 서버 그룹
	SFile >> m_nServerNumber;						// 서버 번호
	SFile >> m_nServerField;							// 서버 필드 번호
	SFile >> m_nServerChannel;                       // 서버 채널 번호
	SFile >> m_nServerChannelNumber;                 // Agent 서버가 가진 채널 갯수

	SFile >> m_nMaxFieldNumber; // 한 채널이 가지는 필드서버 갯수.

	SFile >> m_nServiceProvider;						// 서비스 제공 회사유형

	SFile >> m_nServerMaxClient;						// 최대 접속 가능 클라이언트 수
	SFile >> m_nMaxChannelUser;                      // 채널당 최대 접속 가능 클라이언트 수
	SFile >> m_bUseEventThread;						// 이벤트 쓰레드 사용 여부
	SFile >> m_bHeartBeat;							// HeartBeat 를 사용해서 클라이언트 접속끊김을 조사
	SFile >> m_nMaxThread;							// 최대 Work Thread 갯수.
	SFile >> m_bTestServer;							// 테스트 서버 유/무
	SFile >> m_bAdultCheck;							// 성인체크
	SFile >> m_bPK;									// true:PK 서버, false:Non PK 서버
	SFile >> m_bPkLess;                              /// 선도클럽, PK, 학원간 PK 등 모든 적대행위 Disable
	SFile >> m_bGameGuardAuth;                       // nProtect GameGuard 인증 사용여부
	SFile >> m_nLangSet;								// Language Set

	SFile.ReadBuffer( m_szWhiteRockServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 일본 WhiteRock 서버명
	SFile >> m_nWhiteRockServerPort;								// 일본 WhiteRock 서버사용 Port

	//iplimit
	SFile >> m_wIPClientMax;

	///////////////////////////////////////////////////////////////////////////
	// database setting value 	

	///////////////////////////////////////////////////////////////////////////
	// ODBC
	// User database setting value
	SFile.ReadBuffer( m_szUserOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szUserOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szUserOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );    // Password
	SFile.ReadBuffer( m_szUserOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nUserOdbcPoolSize;					    // Database pool size
	SFile >> m_nUserOdbcResponseTime;				    // Query response time (sec)

	// Game database setting value 
	SFile.ReadBuffer( m_szGameOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGameOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szGameOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szGameOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nGameOdbcPoolSize;					    // Database pool size
	SFile >> m_nGameOdbcResponseTime;				    // Query response time (sec)

	// Log database setting value 
	SFile.ReadBuffer( m_szLogOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szLogOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	   // User account
	SFile.ReadBuffer( m_szLogOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	   // Password
	SFile.ReadBuffer( m_szLogOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	   // Database name
	SFile >> m_nLogOdbcPoolSize;					       // Database pool size
	SFile >> m_nLogOdbcResponseTime;				       // Query response time (sec)

	// Shop database setting value 
	SFile.ReadBuffer( m_szShopOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szShopOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szShopOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szShopOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nShopOdbcPoolSize;					    // Database pool size
	SFile >> m_nShopOdbcResponseTime;				    // Query response time (sec)

	// Terra database setting value 
	SFile.ReadBuffer( m_szTerraOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szTerraOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szTerraOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szTerraOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nTerraOdbcPoolSize;					     // Database pool size
	SFile >> m_nTerraOdbcResponseTime;				     // Query response time (sec)

	// GSP database setting value 
	SFile.ReadBuffer( m_szGspOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGspOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szGspOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szGspOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >>  m_nGspOdbcPoolSize;					     // Database pool size
	SFile >> m_nGspOdbcResponseTime;				     // Query response time (sec)

	// Korea database setting value 
	SFile.ReadBuffer( m_szKorOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szKorOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szKorOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szKorOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nKorOdbcPoolSize;					     // Database pool size
	SFile >> m_nKorOdbcResponseTime;				     // Query response time (sec)

	// Malaysia database setting value
	SFile.ReadBuffer( m_szMyOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );	 // Database server name
	SFile.ReadBuffer( m_szMyOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szMyOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szMyOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );		 // Database name
	SFile >> m_nMyOdbcPoolSize;					     // Database pool size
	SFile >> m_nMyOdbcResponseTime;				     // Query response time (sec)


	SFile.ReadBuffer( &m_sLOGINServer, sizeof( G_SERVER_INFO ) );
	SFile.ReadBuffer( &m_sSESSIONServer, sizeof( G_SERVER_INFO ) );
	
	F_SERVER_INFO_100 sFIELDServer[MAX_CHANNEL_NUMBER][FIELDSERVER_MAX];	
	SFile.ReadBuffer( sFIELDServer, sizeof( F_SERVER_INFO_100 ) * ( MAX_CHANNEL_NUMBER * FIELDSERVER_MAX ) );	// Field Server Information

	for ( int i=0; i<MAX_CHANNEL_NUMBER; ++i )
	{
		for( int k=0; k<FIELDSERVER_MAX; ++k )
		{
			m_sFIELDServer[i][k].Assign( sFIELDServer[i][k] );
		}
	}

	SFile.ReadBuffer( m_sChannelInfo, sizeof( CHANNEL_INFO ) * ( MAX_CHANNEL_NUMBER ) );
}

void CCfg::Load_0002( CSerialFile& SFile )
{
	SFile >> m_nServerVersion;						// 서버 버전, 클라이언트 버전과 동일
	SFile >> m_nPatchVersion;						// 패치 프로그램 버전

	SFile.ReadBuffer( m_szServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 서버명	
	SFile.ReadBuffer( m_szServerIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// 서버 IP, 프로그램에서 자동으로 결정될수 있다
	SFile.ReadBuffer( m_szAgentIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// Agent 서버 IP

	SFile >> m_nPortService;							// 서비스용 포트
	SFile >> m_nPortControl;							// 컨트롤용 포트

	SFile >> m_nServerType;							// 서버 유형
	SFile >> m_nServerGroup;							// 서버 그룹
	SFile >> m_nServerNumber;						// 서버 번호
	SFile >> m_nServerField;							// 서버 필드 번호
	SFile >> m_nServerChannel;                       // 서버 채널 번호
	SFile >> m_nServerChannelNumber;                 // Agent 서버가 가진 채널 갯수

	SFile >> m_nMaxFieldNumber; // 한 채널이 가지는 필드서버 갯수.

	SFile >> m_nServiceProvider;						// 서비스 제공 회사유형

	SFile >> m_nServerMaxClient;						// 최대 접속 가능 클라이언트 수
	SFile >> m_nMaxChannelUser;                      // 채널당 최대 접속 가능 클라이언트 수
	SFile >> m_bUseEventThread;						// 이벤트 쓰레드 사용 여부
	SFile >> m_bHeartBeat;							// HeartBeat 를 사용해서 클라이언트 접속끊김을 조사
	SFile >> m_nMaxThread;							// 최대 Work Thread 갯수.
	SFile >> m_bTestServer;							// 테스트 서버 유/무
	SFile >> m_bAdultCheck;							// 성인체크
	SFile >> m_bPK;									// true:PK 서버, false:Non PK 서버
	SFile >> m_bPkLess;                              /// 선도클럽, PK, 학원간 PK 등 모든 적대행위 Disable
	SFile >> m_bGameGuardAuth;                       // nProtect GameGuard 인증 사용여부
	SFile >> m_nLangSet;								// Language Set

	SFile.ReadBuffer( m_szWhiteRockServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 일본 WhiteRock 서버명
	SFile >> m_nWhiteRockServerPort;								// 일본 WhiteRock 서버사용 Port

	//iplimit
	SFile >> m_wIPClientMax;

	///////////////////////////////////////////////////////////////////////////
	// database setting value 	

	///////////////////////////////////////////////////////////////////////////
	// ODBC
	// User database setting value
	SFile.ReadBuffer( m_szUserOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szUserOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szUserOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );    // Password
	SFile.ReadBuffer( m_szUserOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nUserOdbcPoolSize;					    // Database pool size
	SFile >> m_nUserOdbcResponseTime;				    // Query response time (sec)

	// Game database setting value 
	SFile.ReadBuffer( m_szGameOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGameOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szGameOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szGameOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nGameOdbcPoolSize;					    // Database pool size
	SFile >> m_nGameOdbcResponseTime;				    // Query response time (sec)

	// Log database setting value 
	SFile.ReadBuffer( m_szLogOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szLogOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	   // User account
	SFile.ReadBuffer( m_szLogOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	   // Password
	SFile.ReadBuffer( m_szLogOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	   // Database name
	SFile >> m_nLogOdbcPoolSize;					       // Database pool size
	SFile >> m_nLogOdbcResponseTime;				       // Query response time (sec)

	// Shop database setting value 
	SFile.ReadBuffer( m_szShopOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szShopOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szShopOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szShopOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nShopOdbcPoolSize;					    // Database pool size
	SFile >> m_nShopOdbcResponseTime;				    // Query response time (sec)

	// Terra database setting value 
	SFile.ReadBuffer( m_szTerraOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szTerraOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szTerraOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szTerraOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nTerraOdbcPoolSize;					     // Database pool size
	SFile >> m_nTerraOdbcResponseTime;				     // Query response time (sec)

	// GSP database setting value 
	SFile.ReadBuffer( m_szGspOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGspOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szGspOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szGspOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >>  m_nGspOdbcPoolSize;					     // Database pool size
	SFile >> m_nGspOdbcResponseTime;				     // Query response time (sec)

	// Korea database setting value 
	SFile.ReadBuffer( m_szKorOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szKorOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szKorOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szKorOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nKorOdbcPoolSize;					     // Database pool size
	SFile >> m_nKorOdbcResponseTime;				     // Query response time (sec)

	// Malaysia database setting value
	SFile.ReadBuffer( m_szMyOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );	 // Database server name
	SFile.ReadBuffer( m_szMyOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szMyOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szMyOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );		 // Database name
	SFile >> m_nMyOdbcPoolSize;					     // Database pool size
	SFile >> m_nMyOdbcResponseTime;				     // Query response time (sec)


	SFile.ReadBuffer( &m_sLOGINServer, sizeof( G_SERVER_INFO ) );
	SFile.ReadBuffer( &m_sSESSIONServer, sizeof( G_SERVER_INFO ) );

	F_SERVER_INFO_100 sFIELDServer[MAX_CHANNEL_NUMBER][FIELDSERVER_MAX];	
	SFile.ReadBuffer( sFIELDServer, sizeof( F_SERVER_INFO_100 ) * ( MAX_CHANNEL_NUMBER * FIELDSERVER_MAX ) );	// Field Server Information

	for ( int i=0; i<MAX_CHANNEL_NUMBER; ++i )
	{
		for( int k=0; k<FIELDSERVER_MAX; ++k )
		{
			m_sFIELDServer[i][k].Assign( sFIELDServer[i][k] );
		}
	}

	SFile.ReadBuffer( m_sChannelInfo, sizeof( CHANNEL_INFO ) * ( MAX_CHANNEL_NUMBER ) );

	/*ccfg security, Juver, 2018/07/08 */
	SFile.ReadBuffer( m_szHashCPU, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashLAN, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashDISK, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashPCNAME, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashCOMBINED, sizeof( TCHAR ) * ( CCFG_HASH_LENGTH ) );
	SFile.ReadBuffer( m_szHashInformation, sizeof( TCHAR ) * ( CCFG_NOTE_LENGTH ) );
}

void CCfg::Load_0001( CSerialFile& SFile )
{
	SFile >> m_nServerVersion;						// 서버 버전, 클라이언트 버전과 동일
	SFile >> m_nPatchVersion;						// 패치 프로그램 버전

	SFile.ReadBuffer( m_szServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 서버명	
	SFile.ReadBuffer( m_szServerIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// 서버 IP, 프로그램에서 자동으로 결정될수 있다
	SFile.ReadBuffer( m_szAgentIP, sizeof( TCHAR ) * ( MAX_IP_LENGTH+1 ) );				// Agent 서버 IP

	SFile >> m_nPortService;							// 서비스용 포트
	SFile >> m_nPortControl;							// 컨트롤용 포트

	SFile >> m_nServerType;							// 서버 유형
	SFile >> m_nServerGroup;							// 서버 그룹
	SFile >> m_nServerNumber;						// 서버 번호
	SFile >> m_nServerField;							// 서버 필드 번호
	SFile >> m_nServerChannel;                       // 서버 채널 번호
	SFile >> m_nServerChannelNumber;                 // Agent 서버가 가진 채널 갯수

	SFile >> m_nMaxFieldNumber; // 한 채널이 가지는 필드서버 갯수.

	SFile >> m_nServiceProvider;						// 서비스 제공 회사유형

	SFile >> m_nServerMaxClient;						// 최대 접속 가능 클라이언트 수
	SFile >> m_nMaxChannelUser;                      // 채널당 최대 접속 가능 클라이언트 수
	SFile >> m_bUseEventThread;						// 이벤트 쓰레드 사용 여부
	SFile >> m_bHeartBeat;							// HeartBeat 를 사용해서 클라이언트 접속끊김을 조사
	SFile >> m_nMaxThread;							// 최대 Work Thread 갯수.
	SFile >> m_bTestServer;							// 테스트 서버 유/무
	SFile >> m_bAdultCheck;							// 성인체크
	SFile >> m_bPK;									// true:PK 서버, false:Non PK 서버
	SFile >> m_bPkLess;                              /// 선도클럽, PK, 학원간 PK 등 모든 적대행위 Disable
	SFile >> m_bGameGuardAuth;                       // nProtect GameGuard 인증 사용여부
	SFile >> m_nLangSet;								// Language Set

	SFile.ReadBuffer( m_szWhiteRockServerName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );		// 일본 WhiteRock 서버명
	SFile >> m_nWhiteRockServerPort;								// 일본 WhiteRock 서버사용 Port

	//iplimit
	SFile >> m_wIPClientMax;

	///////////////////////////////////////////////////////////////////////////
	// database setting value 	

	///////////////////////////////////////////////////////////////////////////
	// ODBC
	// User database setting value
	SFile.ReadBuffer( m_szUserOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szUserOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szUserOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );    // Password
	SFile.ReadBuffer( m_szUserOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nUserOdbcPoolSize;					    // Database pool size
	SFile >> m_nUserOdbcResponseTime;				    // Query response time (sec)

	// Game database setting value 
	SFile.ReadBuffer( m_szGameOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGameOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szGameOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szGameOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nGameOdbcPoolSize;					    // Database pool size
	SFile >> m_nGameOdbcResponseTime;				    // Query response time (sec)

	// Log database setting value 
	SFile.ReadBuffer( m_szLogOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szLogOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	   // User account
	SFile.ReadBuffer( m_szLogOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	   // Password
	SFile.ReadBuffer( m_szLogOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	   // Database name
	SFile >> m_nLogOdbcPoolSize;					       // Database pool size
	SFile >> m_nLogOdbcResponseTime;				       // Query response time (sec)

	// Shop database setting value 
	SFile.ReadBuffer( m_szShopOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szShopOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	    // User account
	SFile.ReadBuffer( m_szShopOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	// Password
	SFile.ReadBuffer( m_szShopOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	    // Database name
	SFile >> m_nShopOdbcPoolSize;					    // Database pool size
	SFile >> m_nShopOdbcResponseTime;				    // Query response time (sec)

	// Terra database setting value 
	SFile.ReadBuffer( m_szTerraOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szTerraOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szTerraOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szTerraOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nTerraOdbcPoolSize;					     // Database pool size
	SFile >> m_nTerraOdbcResponseTime;				     // Query response time (sec)

	// GSP database setting value 
	SFile.ReadBuffer( m_szGspOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szGspOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szGspOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szGspOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >>  m_nGspOdbcPoolSize;					     // Database pool size
	SFile >> m_nGspOdbcResponseTime;				     // Query response time (sec)

	// Korea database setting value 
	SFile.ReadBuffer( m_szKorOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) ); // Database server name
	SFile.ReadBuffer( m_szKorOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szKorOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szKorOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );	 // Database name
	SFile >> m_nKorOdbcPoolSize;					     // Database pool size
	SFile >> m_nKorOdbcResponseTime;				     // Query response time (sec)

	// Malaysia database setting value
	SFile.ReadBuffer( m_szMyOdbcName, sizeof( TCHAR ) * ( DB_SVR_NAME_LENGTH+1 ) );	 // Database server name
	SFile.ReadBuffer( m_szMyOdbcUser, sizeof( TCHAR ) * ( USR_ID_LENGTH+1 ) );	     // User account
	SFile.ReadBuffer( m_szMyOdbcPass, sizeof( TCHAR ) * ( USR_PASS_LENGTH+1 ) );	 // Password
	SFile.ReadBuffer( m_szMyOdbcDatabase, sizeof( TCHAR ) * ( DB_NAME_LENGTH+1 ) );		 // Database name
	SFile >> m_nMyOdbcPoolSize;					     // Database pool size
	SFile >> m_nMyOdbcResponseTime;				     // Query response time (sec)


	SFile.ReadBuffer( &m_sLOGINServer, sizeof( G_SERVER_INFO ) );
	SFile.ReadBuffer( &m_sSESSIONServer, sizeof( G_SERVER_INFO ) );
	
	F_SERVER_INFO_100 sFIELDServer[MAX_CHANNEL_NUMBER][FIELDSERVER_MAX];	
	SFile.ReadBuffer( sFIELDServer, sizeof( F_SERVER_INFO_100 ) * ( MAX_CHANNEL_NUMBER * FIELDSERVER_MAX ) );	// Field Server Information

	for ( int i=0; i<MAX_CHANNEL_NUMBER; ++i )
	{
		for( int k=0; k<FIELDSERVER_MAX; ++k )
		{
			m_sFIELDServer[i][k].Assign( sFIELDServer[i][k] );
		}
	}
	
	SFile.ReadBuffer( m_sChannelInfo, sizeof( CHANNEL_INFO ) * ( MAX_CHANNEL_NUMBER ) );
}


