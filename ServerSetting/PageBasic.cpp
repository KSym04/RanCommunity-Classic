// PageBasic.cpp : implementation file
//

#include "stdafx.h"
#include "./ServerSetting.h"
#include "./PageBasic.h"
#include "afxdialogex.h"
#include "./ServerSettingTab.h"
#include "../Lib_Network/s_CCfg.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Helper/HLibDataConvert.h"
#include "../Lib_Engine/Hash/MD52.h"
#include "../Lib_Helper/RPCID/RPCID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPageBasic dialog

IMPLEMENT_DYNAMIC(CPageBasic, CPropertyPage)

CPageBasic::CPageBasic(LOGFONT logfont)
	: CPropertyPage(CPageBasic::IDD)
	, m_pFont(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CPageBasic::~CPageBasic()
{
	SAFE_DELETE(m_pFont);
}

void CPageBasic::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageBasic, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_BASIC_SAVE, &CPageBasic::OnBnClickedButtonBasicSave)
	ON_BN_CLICKED(IDC_BUTTON_HASH_BUILD, &CPageBasic::OnBnClickedButtonHashBuild)
END_MESSAGE_MAP()


// CPageBasic message handlers
BOOL CPageBasic::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CServerSettingTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

#ifndef _DEBUG
	SetWin_ShowWindow( this, IDC_BUTTON_HASH_BUILD, FALSE );
#endif 

	ShowData();

	return TRUE;  
}

BOOL CPageBasic::ShowData()
{
	SetWin_Num_int( this, IDC_EDIT_SERVER_VERSION, CCfg::GetInstance()->GetServerVersion() );
	SetWin_Num_int( this, IDC_EDIT_PATCH_VERSION, CCfg::GetInstance()->GetPatchVersion() );
	SetWin_Text( this, IDC_EDIT_SERVER_NAME, CCfg::GetInstance()->GetServerName() );

	SetWin_Num_int( this, IDC_EDIT_SERVER_MAX_CLIENT, CCfg::GetInstance()->GetServerMaxClient() );
	SetWin_Num_int( this, IDC_EDIT_SERVER_MAX_CHANNEL_CLIENT, CCfg::GetInstance()->GetServerChannelMaxClient() );

	SetWin_Text( this, IDC_EDIT_SERVER_IP, CCfg::GetInstance()->GetServerIP() );
	SetWin_Num_int( this, IDC_EDIT_SERVER_SERVICE_PORT, CCfg::GetInstance()->GetServicePort() );
	SetWin_Num_int( this, IDC_EDIT_SERVER_CONTROL_PORT, CCfg::GetInstance()->GetControlPort() );

	SetWin_Text( this, IDC_EDIT_SERVER_SERVICEPORT_LIST, CCfg::GetInstance()->GetAllowPortList() );
	
	SetWin_Num_int( this, IDC_EDIT_SERVER_TYPE, CCfg::GetInstance()->GetServerType() );
	SetWin_Num_int( this, IDC_EDIT_SERVER_GROUP, CCfg::GetInstance()->GetServerGroup() );
	SetWin_Num_int( this, IDC_EDIT_SERVER_FIELD, CCfg::GetInstance()->GetServerField() );
	SetWin_Num_int( this, IDC_EDIT_SERVER_CHANNEL, CCfg::GetInstance()->GetServerChannel() );
	SetWin_Num_int( this, IDC_EDIT_SERVER_CHANNEL_NUMBER, CCfg::GetInstance()->GetServerChannelNumber() );
	SetWin_Num_int( this, IDC_EDIT_SERVER_NUMBER, CCfg::GetInstance()->GetServerNumber() );

	SetWin_Num_int( this, IDC_EDIT_SERVICE_PROVIDER, CCfg::GetInstance()->GetServiceProvider() );
	SetWin_Num_int( this, IDC_EDIT_LANGUAGE_SET, CCfg::GetInstance()->GetLangSet() );
	SetWin_Num_int( this, IDC_EDIT_MAX_THREAD, CCfg::GetInstance()->GetMaxThread() );
	SetWin_Num_int( this, IDC_EDIT_IP_CLIENT_MAX, CCfg::GetInstance()->GetIPClientMax() );

	SetWin_Check( this, IDC_CHECK_PKLESS, CCfg::GetInstance()->IsPKLessServer() );
	SetWin_Check( this, IDC_CHECK_PK, CCfg::GetInstance()->IsPKServer() );
	SetWin_Check( this, IDC_CHECK_ADULT, CCfg::GetInstance()->AdultCheck() );
	SetWin_Check( this, IDC_CHECK_TEST_SERVER, CCfg::GetInstance()->IsTestServer() );
	SetWin_Check( this, IDC_CHECK_HEARTBEAT, CCfg::GetInstance()->HeartBeatCheck() );
	SetWin_Check( this, IDC_CHECK_GAME_GUARD_AUTH, CCfg::GetInstance()->GemeGuardAuth() );
	SetWin_Check( this, IDC_CHECK_USE_EVENT_THREAD, CCfg::GetInstance()->IsUserEventThread() );

	SetWin_Text( this, IDC_EDIT_WHITE_ROCK_SERVER, CCfg::GetInstance()->GetWhiteRockName() );
	SetWin_Num_int( this, IDC_EDIT_WHITE_ROCK_PORT, CCfg::GetInstance()->GetWhiteRockPort() );

	/*ccfg security, Juver, 2018/07/08 */
	SetWin_Text( this, IDC_EDIT_HASH_CPU, CCfg::GetInstance()->getHashCPU() );
	SetWin_Text( this, IDC_EDIT_HASH_LAN, CCfg::GetInstance()->getHashLAN() );
	SetWin_Text( this, IDC_EDIT_HASH_DISK, CCfg::GetInstance()->getHashDISK() );
	SetWin_Text( this, IDC_EDIT_HASH_NAME, CCfg::GetInstance()->getHashPCNAME() );
	SetWin_Text( this, IDC_EDIT_HASH_ALL, CCfg::GetInstance()->getHashCOMBINED() );
	SetWin_Text( this, IDC_EDIT_SERVER_NOTE, CCfg::GetInstance()->getHashINFORMATION() );

	/* proxy ip, Juver, 2021/07/04 */
	SetWin_Check( this, IDC_CHECK_USE_PROXY, CCfg::GetInstance()->getUseProxy() );
	SetWin_Text( this, IDC_EDIT_SERVER_IP_PROXY, CCfg::GetInstance()->getProxyIP() );

	return TRUE;  
}

BOOL CPageBasic::SaveData()
{
	CCfg::GetInstance()->SetServerVersion( GetWin_Num_int( this, IDC_EDIT_SERVER_VERSION ) );
	CCfg::GetInstance()->SetPatchVersion( GetWin_Num_int( this, IDC_EDIT_PATCH_VERSION ) );
	CCfg::GetInstance()->SetServerName( GetWin_Text( this, IDC_EDIT_SERVER_NAME ).GetString() );

	CCfg::GetInstance()->SetServerMaxClient( GetWin_Num_int( this, IDC_EDIT_SERVER_MAX_CLIENT ) );
	CCfg::GetInstance()->SetServerChannelMaxClient( GetWin_Num_int( this, IDC_EDIT_SERVER_MAX_CHANNEL_CLIENT ) );

	CCfg::GetInstance()->SetServerIP( GetWin_Text( this, IDC_EDIT_SERVER_IP ).GetString() );
	CCfg::GetInstance()->SetServicePort( GetWin_Num_int( this, IDC_EDIT_SERVER_SERVICE_PORT ) );
	CCfg::GetInstance()->SetControlPort( GetWin_Num_int( this, IDC_EDIT_SERVER_CONTROL_PORT ) );

	CCfg::GetInstance()->SetAllowPortList( GetWin_Text( this, IDC_EDIT_SERVER_SERVICEPORT_LIST ).GetString() );

	CCfg::GetInstance()->SetServerType( GetWin_Num_int( this, IDC_EDIT_SERVER_TYPE ) );
	CCfg::GetInstance()->SetServerGroup( GetWin_Num_int( this, IDC_EDIT_SERVER_GROUP ) );
	CCfg::GetInstance()->SetServerField( GetWin_Num_int( this, IDC_EDIT_SERVER_FIELD ) );
	CCfg::GetInstance()->SetServerChannel( GetWin_Num_int( this, IDC_EDIT_SERVER_CHANNEL ) );
	CCfg::GetInstance()->SetServerChannelNumber( GetWin_Num_int( this, IDC_EDIT_SERVER_CHANNEL_NUMBER ) );
	CCfg::GetInstance()->SetServerNumber( GetWin_Num_int( this, IDC_EDIT_SERVER_NUMBER ) );

	CCfg::GetInstance()->SetServiceProvider( GetWin_Num_int( this, IDC_EDIT_SERVICE_PROVIDER ) );
	CCfg::GetInstance()->SetLangSet( GetWin_Num_int( this, IDC_EDIT_LANGUAGE_SET ) );
	CCfg::GetInstance()->SetMaxThread( GetWin_Num_int( this, IDC_EDIT_MAX_THREAD ) );
	CCfg::GetInstance()->SetIPClientMax( GetWin_Num_int( this, IDC_EDIT_IP_CLIENT_MAX ) );

	CCfg::GetInstance()->SetPKLessServer( GetWin_Check( this, IDC_CHECK_PKLESS )? true:false );
	CCfg::GetInstance()->SetPKServer( GetWin_Check( this, IDC_CHECK_PK )? true:false );
	CCfg::GetInstance()->SetAdultCheck ( GetWin_Check( this, IDC_CHECK_ADULT )? true:false );
	CCfg::GetInstance()->SetTestServer( GetWin_Check( this, IDC_CHECK_TEST_SERVER )? true:false );
	CCfg::GetInstance()->SetHeartBeatCheck( GetWin_Check( this, IDC_CHECK_HEARTBEAT )? true:false  );
	CCfg::GetInstance()->SetGameGuardAuth( GetWin_Check( this, IDC_CHECK_GAME_GUARD_AUTH )? true:false );
	CCfg::GetInstance()->SetUserEventThread( GetWin_Check( this, IDC_CHECK_USE_EVENT_THREAD )? true:false );

	CCfg::GetInstance()->SetWhiteRockName( GetWin_Text( this, IDC_EDIT_WHITE_ROCK_SERVER ).GetString() );
	CCfg::GetInstance()->SetWhiteRockPort( GetWin_Num_int( this, IDC_EDIT_WHITE_ROCK_PORT ) );

	/*ccfg security, Juver, 2018/07/08 */
	CCfg::GetInstance()->setHashCPU( GetWin_Text( this, IDC_EDIT_HASH_CPU ).GetString() );
	CCfg::GetInstance()->setHashLAN( GetWin_Text( this, IDC_EDIT_HASH_LAN ).GetString() );
	CCfg::GetInstance()->setHashDISK( GetWin_Text( this, IDC_EDIT_HASH_DISK ).GetString() );
	CCfg::GetInstance()->setHashPCNAME( GetWin_Text( this, IDC_EDIT_HASH_NAME ).GetString() );
	CCfg::GetInstance()->setHashCOMBINED( GetWin_Text( this, IDC_EDIT_HASH_ALL ).GetString() );
	CCfg::GetInstance()->setHashINFORMATION( GetWin_Text( this, IDC_EDIT_SERVER_NOTE ).GetString() );


	/* proxy ip, Juver, 2021/07/04 */
	CCfg::GetInstance()->setUseProxy( GetWin_Check( this, IDC_CHECK_USE_PROXY )? true:false ) ;
	CCfg::GetInstance()->setProxyIP( GetWin_Text( this, IDC_EDIT_SERVER_IP_PROXY ).GetString() );

	return TRUE;  
}

void CPageBasic::OnBnClickedButtonBasicSave()
{
	if ( SaveData() )
		CDebugSet::MsgBox( GetSafeHwnd(), "Page Basic Saved!" );
	else
		CDebugSet::MsgBox( GetSafeHwnd(), "Page Basic Save Failed!");
}


void CPageBasic::OnBnClickedButtonHashBuild()
{
//#ifndef _DEBUG
//	CDebugSet::MsgBox( GetSafeHwnd(), "active build type does not allow editing, contact developer" );
//#else
	std::string text = RPCID::getStringCPU();
	std::string hash = md5( text );
	SetWin_Text( this, IDC_EDIT_HASH_CPU, hash.c_str() );

	text = RPCID::getStringLAN();
	hash = md5( text );
	SetWin_Text( this, IDC_EDIT_HASH_LAN, hash.c_str() );

	text = RPCID::getStringDISK();
	hash = md5( text );
	SetWin_Text( this, IDC_EDIT_HASH_DISK, hash.c_str() );

	text = RPCID::getStringPCNAME();
	hash = md5( text );
	SetWin_Text( this, IDC_EDIT_HASH_NAME, hash.c_str() );

	text = RPCID::getStringCombined();
	SetWin_Text( this, IDC_EDIT_SERVER_NOTE, text.c_str() );

	hash = md5( text );
	SetWin_Text( this, IDC_EDIT_HASH_ALL, hash.c_str() );
//#endif //
}
